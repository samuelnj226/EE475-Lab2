/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <xc.h>
#include "../../sram_control.h"
//
#include "i2c_slave.h"
#include "../i2c2_driver.h"
//

#define I2C1_SLAVE_ADDRESS 0x50 
#define I2C1_SLAVE_MASK    0xFE

/**
 Section: Global Variables
 */

volatile uint8_t i2c_slave_writeData = 0x55;

/**
 Section: Local Functions
 */

void i2c_slave_open(void) {
    //ADDED CODE
    mssp2_enableIRQ(); //enable interrupts, should make this work.
    
    i2c_slave_setIsrHandler(i2c_slave_ISR);
    i2c2_driver_initSlaveHardware();
    i2c2_driver_setAddr(I2C1_SLAVE_ADDRESS << 1);
    i2c2_driver_setMask(I2C1_SLAVE_MASK);
    i2c2_driver_setBusCollisionISR(i2c_slave_BusCollisionISR);
    i2c_slave_setWriteIntHandler(i2c_slave_DefWrInterruptHandler);
    //adjusted to PWM handler (custom code)
    i2c_slave_setReadIntHandler(i2c_slave_PWMRdInterruptHandler);
    //ADDED CODE
    //adjusted to PWM handler (custom code)
    i2c_slave_setAddrIntHandler(i2c_slave_PWMAddrInterruptHandler);

}

void i2c_slave_close(void) {
    i2c2_driver_close();
}

void i2c_slave_ISR(void) {
    PIR3bits.SSP2IF = 0; // clear the slave interrupt flag
    i2c1Data = i2c2_driver_getRXData(); // read SSPBUF to clear BF
    if (0 == i2c2_driver_isRead()) {
        if (1 == i2c2_driver_isData()) {
            i2c_slave_RdCallBack();
        } else {
            i2c_slave_AddrCallBack();
        }
    } else {
        i2c_slave_WrCallBack();
    }

    i2c2_driver_releaseClock();
}

void i2c_slave_BusCollisionISR(void) {
    
}

uint8_t i2c_slave_read(void) {
    return i2c2_driver_getRXData();
}

void i2c_slave_write(uint8_t data) {
    i2c2_driver_TXData(data);
}

void i2c_slave_enable(void) {
    i2c2_driver_initSlaveHardware();
}

void i2c_slave_sendAck(void) {
    i2c2_driver_sendACK();
}

void i2c_slave_sendNack(void) {
    i2c2_driver_sendNACK();
}

// Read Event Interrupt Handlers
void i2c_slave_RdCallBack(void) {
    // Add your custom callback code here
    if (i2c_slave_RdInterruptHandler) {
        i2c_slave_RdInterruptHandler();
    }
}

void i2c_slave_setIsrHandler(void (*handler)(void)) {
    i2c2_driver_setI2cISR(handler);
}

void i2c_slave_setBusCollisionISR(void (*handler)(void)) {
    i2c2_driver_setBusCollisionISR(handler);
}

void i2c_slave_setReadIntHandler(void (*handler)(void)) {
    i2c_slave_RdInterruptHandler = handler;
}

void i2c_slave_DefRdInterruptHandler(void) {
    i2c1Data = i2c2_driver_getRXData();
}

// Write Event Interrupt Handlers
void i2c_slave_WrCallBack(void) {
    // Add your custom callback code here
    if (i2c_slave_WrInterruptHandler) {
        i2c_slave_WrInterruptHandler();
    }
}

void i2c_slave_setWriteIntHandler(void (*handler)(void)) {
    i2c_slave_WrInterruptHandler = handler;
}

void i2c_slave_DefWrInterruptHandler(void) {
    i2c2_driver_TXData(i2c_slave_writeData);
}

// Address Event Interrupt Handlers
void i2c_slave_AddrCallBack(void) {
    // Add your custom callback code here
    if (i2c_slave_AddrInterruptHandler) {
        i2c_slave_AddrInterruptHandler();
    }
}

void i2c_slave_setAddrIntHandler(void (*handler)(void)) {
    i2c_slave_AddrInterruptHandler = handler;
}

void i2c_slave_DefAddrInterruptHandler(void) {
    *sAddr = i2c2_driver_getAddr();
}


//CUSTOM CODE BY SAM modified

// interrupt called when slave is written to
void i2c_slave_PWMRdInterruptHandler(void) {
    i2c1Data = i2c2_driver_getRXData();
    switch(pwmCommand.state) {
        case FIRSTBYTE:
            //change state
            pwmCommand.state = SECONDBYTE;

            //take upper two bits for command type
            pwmCommand.commandType = (i2c1Data >> 6) & 0x3U;
            //take lower two bits for upper parameter
            pwmCommand.parameter = ((uint16_t) (i2c1Data & 0x3U)) << 8;
            
            //write to SRAM
            sram_write(0, pwmCommand.commandType);
            sram_write(1, (pwmCommand.parameter >> 8) & 0xFF);
            
            break;
            
        case SECONDBYTE:
            //take byte and store in lower 8 bits of parameter
            pwmCommand.parameter |= i2c1Data;
            //command received, not processed
            pwmCommand.state = RECEIVED;
            
            //write to SRAM
            sram_write(2, pwmCommand.parameter & 0xFF);
            
            //set parameters to 0 just to check
            pwmCommand.commandType = 0;
            pwmCommand.parameter = 0;
            
            break;
            
        default:
            //nothing happens if more bytes are received here
            break;
    }
}

// interrupt called when slave receives and recognizes its address
void i2c_slave_PWMAddrInterruptHandler(void) {
    // reset pwmCommand State so it's a new command
    pwmCommand.state = FIRSTBYTE;
}

