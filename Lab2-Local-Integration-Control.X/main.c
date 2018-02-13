//main local node file

#include "mcc_generated_files/mcc.h"
#define _XTAL_FREQ 16000000
#include <p18f25k22.h>
#include <xc.h>
#include <stdio.h>
#include <stdint.h> //for uint8_t datatype
#include <p18cxxx.h>
#include <string.h>
#include <stdlib.h>

#include "sram_control.h"

#define REMOTEADDRESS 0x50



//UART METHODS
void Send_UART(char data){
    EUSART1_Write((uint8_t) data);
}

void printString() {
    char welcome[50] = "\n\rHello. Welcome to EE Hospital \n\r";
    for (int i = 0; i < 50; i++ ) {
        EUSART1_Write(welcome[i]);
        __delay_ms(100);
    }
}

//I2C methods
typedef enum {
    SET,
    INCREMENT,
    DECREMENT,
    ADJUST
} CommandType;

//function to implement i2c data sending for PWM
//inputs:
// commandPWM - specifies type of command to send to remote node
// data - acceptable range 0-639 (decimal) for setting PWM speed
inline bit sendDataToRemoteNode(CommandType commandPWM, uint16_t data) {
    static i2c_error_t status; //status of the bus
    static uint8_t buffer[2]; //buffer to send
    switch(commandPWM) { //upper two bits are command type
        case SET:
            buffer[0] = 0x00;
            break;
        
        case INCREMENT:
            buffer[0] = 0x40;
            break;
            
        case DECREMENT:
            buffer[0] = 0x80;
            break;
        case ADJUST:
            buffer[0] = 0xC0;
            break;
    }
    // send upper two bits of 10 bit pwm data
    buffer[0] |= (data >> 8) & 0x3;
    buffer[1] = data & 0xFF;
    i2c_open(REMOTEADDRESS); //open remote node
    i2c_setBuffer(buffer, 2); //set the buffer
    status = i2c_masterWrite();
    
    if(status == I2C_FAIL) {
        return 0;
    }
    return 1; 
}

/*
//function to implement i2c data sending for PWM
inline bit sendDataToRemoteNode(uint8_t data) {
    static i2c_error_t status; //status of the bus
    static uint8_t buffer[1]; //buffer to send
    buffer[0] = data;
    i2c_open(REMOTEADDRESS); //open remote node
    i2c_setBuffer(buffer, 1); //set the buffer
    status = i2c_masterWrite();
    if(status == I2C_FAIL) {
        return 0;
    }
    return 1; 
}
*/

/*
                         Main application
 */
void main(void)
{
    double speed = 0; //store speed
    bool printError = 0;
    double tempSpeed = 0;

    // Initialize the device
    SYSTEM_Initialize();
    EUSART1_Initialize();
    SRAM_CONTROL_INITIALIZE();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    printString();
    printf("\n\rEnter a command (I: increment, D: decrement, S: set point, T: status, H: help, P: error status) : ");
    
    while (1)
    {
        switch(receiveState) {
            case RECEIVED:     
                for(int i = 0; i < eusart1RxHead; i++) {
                    eusart1RxBuffer[i] = sram_read(i);
                }
                switch(eusart1RxBuffer[0]) {
                    case 'I': printf("Increment speed of motor. "); 
                        sendDataToRemoteNode(INCREMENT,  0);         
                        speed += 0.5;         
                        if (speed > 100) {
                            speed = 100;
                        }
                        tempSpeed = speed;
                        break;

                    case 'D' : printf("Decrement speed of motor. \n\r");
                        sendDataToRemoteNode(DECREMENT, 0);
                        speed -= 0.5;
                        if (speed < 0) {
                            speed = 0;
                        }
                        tempSpeed = speed;
                        break;

                    case 'S' : printf("Specify set point. \n\r"); 
                        char newBuffer[3];
                        int i = 1;
                        while(eusart1RxBuffer[i] != 13) {
                            newBuffer[i - 1] = eusart1RxBuffer[i];   
                            i++;
                        }
                        newBuffer[i -1] = '\0';
                        int set;
                        set = atoi(newBuffer);
                        speed = set;
                        if (set <= 100) {
                            uint16_t parameter = (uint16_t)(set * 6.39);
                            printf("\nThe motor is set to %d%%", set);
                            sendDataToRemoteNode(SET, parameter);
                            tempSpeed = speed;
                        }
                  
                        else {
                            printf("\nThe value entered is too large, reenter command");                                  
                        }
                        break;

                    case 'T':
                        printf("\nThe motor is set to %.1f%%", speed);
                        break;

                    case 'H':
                        printf("\n(I: increment, D: decrement, S: set point, T: status, H: help, P: error status)");
                        break;
                    case 'P':
                        printError = !printError;
                        if (printError) {
                            printf("Enable Error Printing \n");
                        } else {
                            printf("Disable Error Printing \n");
                        }
                        break;
                    default: printf("Unrecognized command. Try again. \n");
                        break;
                }
                eusart1RxHead = 0;
                eusart1RxCount = 0;
                receiveState = RECEIVING;
                printf("\n\rEnter a command: ");
                break;
                
            case OF:
                eusart1RxHead = 0;
                eusart1RxCount = 0;
                receiveState = RECEIVING;
                
                break;
                
            default: //do nothing
                break;
        }

        // DEAL WITH ERRORS
        switch(receiveStateFeedBack) {
            int16_t errorBits;
            int16_t newSpeed;
           // double tempSpeed;
            case RECEIVEDFEEDBACK:
                errorBits = (int16_t) eusart2RxBuffer[0] << 8;
                errorBits |= (int16_t) eusart2RxBuffer[1];
                if ((errorBits / 6.4) >= 5 || (errorBits / 6.4) <= -5) {
                    printf("\n\rError level is 0 and is severe. Adjusting motor speed.");
                    printf("\n\rError is: %0.1f%% ", ((int) errorBits) / 6.4);
                    newSpeed = (int16_t) (tempSpeed * 6.4) - errorBits;
                    printf("\n\rNew speed is %d", newSpeed);
                    if(newSpeed > 639) {
                        sendDataToRemoteNode(ADJUST, (uint16_t) 639);
                        tempSpeed = 100;
                    } else if (newSpeed < 0) {
                        sendDataToRemoteNode(ADJUST, (uint16_t) 0);
                        tempSpeed = 0;
                    } else {
                        sendDataToRemoteNode(ADJUST, (int16_t) newSpeed);
                        tempSpeed = newSpeed / 6.4;
                    }
                }
                else if (((errorBits / 6.4) >= 2 && (errorBits / 6.4) < 5) || ((errorBits / 6.4) <= -2 && (errorBits / 6.4) > -5)) {
                    printf("\n\rError level is 1 and is moderate.");
                    printf("\n\rError is: %0.1f%% ", ((int) errorBits) / 6.39 );
                }
                else if  (((errorBits / 6.4) >= 1 && (errorBits / 6.4) < 2) || ((errorBits / 6.4) <= -1 && (errorBits / 6.4) > -2)) {
                    printf("\n\rError level is 2 and is of concern ");
                    printf("\n\rError is: %0.1f%% ", ((int) errorBits) / 6.39 );
                }
                else if(printError){
                    printf("\n\rError level is fine");
                    printf("\n\rError is: %0.1f%% ", ((int) errorBits) / 6.39 );
                }
                eusart2RxHead = 0;
                eusart2RxCount = 0;
                receiveStateFeedBack = RECEIVINGFEEDBACK;
                break;

            case OFFEEDBACK:
                eusart2RxHead = 0;
                eusart2RxCount = 0;
                receiveStateFeedBack = RECEIVINGFEEDBACK;
                
                break;

            default: //do nothing
                break;
        }
        
    }  

}

/**
 End of File
*/