//main remote node file

#include "mcc_generated_files/mcc.h"
#include "sram_control.h"

#define _XTAL_FREQ 16000000
#include <p18f25k22.h>
#include <xc.h>
#include <stdio.h>
#include <stdint.h> //for uint8_t datatype
#include <p18cxxx.h>

volatile uint32_t currentPWMspeed;

uint16_t adcValue;
uint16_t count;

#define NUMAVG 5000
#define MAXADC 65208.0

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    // Enable I2C slave behaviour
    // I2C slave with address 0x50 and each time
    // device is written to, data is stored in i2c_slave_writeData
    i2c_slave_open();
    
    
    // setup timer for PWM output
    TMR2_Initialize();
    TMR2_StartTimer();
    
    TMR1_Initialize();
    TMR1_StartTimer();
    
    // setup PWM output
    PWM4_Initialize();

    // setup PWM output to 0
    PWM4_LoadDutyValue(0);
    currentPWMspeed = 0; // has a min of 0, max of 639
    
    //intialize SRAM
    SRAM_CONTROL_INITIALIZE();
    
    //setup modes for ADC capture
    adcValue = 0;
    count = 1;

    while (1)
    {
        //new command, PWM not set yet
        if(pwmCommand.state == RECEIVED) {
            
            //read from SRAM
            uint8_t commandType = sram_read(0);
            uint16_t parameter = sram_read(1) << 8;
            parameter |= sram_read(2);
            
            
            if(commandType == SET) {
                //set pwm speed
                if(parameter > 639) { //outside of range, set to max
                    currentPWMspeed = 639;
                } else {
                    //update current pwm speed
                    currentPWMspeed = parameter;
                }
                PWM4_LoadDutyValue(currentPWMspeed);
            } else if(commandType == INCREMENT) {
                //increment PWM speed
                currentPWMspeed += 3;
                if(currentPWMspeed > 639) {
                    currentPWMspeed = 639;
                }
                PWM4_LoadDutyValue(currentPWMspeed);
            } else if(commandType == DECREMENT) {
                //decrement PWM speed
                currentPWMspeed -= 3;
                if(currentPWMspeed > 639) { // since unsigned, this check is same as <0
                    currentPWMspeed = 0;
                }
                PWM4_LoadDutyValue(currentPWMspeed);
            } else if(commandType == ADJUST) {
                if(parameter > 639) { //outside of range, set to max
                    PWM4_LoadDutyValue(639);
                } else {
                    PWM4_LoadDutyValue(parameter);
                }
            }

            //command has been executed
            pwmCommand.state = EXECUTED;
            
            //reset ADC polling
            adcValue = 0;
            count = 1;
            if(commandType == ADJUST) {
                __delay_ms(100); //wait for Analog signal to settle
            }
        }  

        
        //poll ADC
        //send ADC value back
        if(count % NUMAVG == 0) { // send over to local pic error.
            //Calculate error in 8-bits
            int16_t error = (adcValue / MAXADC) * 639 - currentPWMspeed; //number of bits off
            EUSART2_Write((error >> 8) & 0xFF ); //upper 8
            EUSART2_Write(error & 0xFF); //lower 8
            EUSART2_Write(13);
            adcValue = 0;
        } else if (count % NUMAVG == 1) {
            adcValue = ADC_GetConversion(channel_AN9);
        } else {
            adcValue = adcValue * (((double) (count - 1))/ count) + ADC_GetConversion(channel_AN9) * (((double) 1)/ count);
        }
        count++;

    }
}
/**
 End of File
*/