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

#ifndef _I2C_SLAVE_H
#define _I2C_SLAVE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>

/**
  Section: Macro Declarations
 */

#define I2C1_SLAVE_DEFAULT_ADDRESS          0x50

/**
  Section: Variable Definitions
 */

uint8_t *sWrite;
uint8_t *sRead;
uint8_t *sAddr;
uint8_t i2c1Data;

void i2c_slave_open(void);
void i2c_slave_close(void);

void i2c_slave_ISR ( void );

uint8_t i2c_slave_read(void);
void i2c_slave_write(uint8_t data);
void i2c_slave_enable(void);
void i2c_slave_sendAck(void);
void i2c_slave_sendNack(void);

// Read Event Interrupt Handlers
void i2c_slave_RdCallBack(void);
void i2c_slave_setReadIntHandler(void (*handler)(void));
void i2c_slave_DefRdInterruptHandler(void);
void (*i2c_slave_RdInterruptHandler)(void);

// Write Event Interrupt Handlers
void i2c_slave_WrCallBack(void);
void i2c_slave_setWriteIntHandler(void (*handler)(void));
void i2c_slave_DefWrInterruptHandler(void);
void (*i2c_slave_WrInterruptHandler)(void);

// Address Event Interrupt Handlers
void i2c_slave_AddrCallBack(void);
void i2c_slave_setAddrIntHandler(void (*handler)(void));
void i2c_slave_DefAddrInterruptHandler(void);
void (*i2c_slave_AddrInterruptHandler)(void);

//ISR HANDLERS
void i2c_slave_setIsrHandler(void (*handler)(void));
void i2c_slave_setBusCollisionISR(void (*handler)(void));
void i2c_slave_BusCollisionISR(void);

/**
   @Summary
        This varible contains the last data written to the I2C slave
*/

extern volatile uint8_t    i2c_slave_writeData;


//CUSTOM CODE BY SAM

// tell interrupt what state the receive process is in
typedef enum {
    FIRSTBYTE,
    SECONDBYTE,
    RECEIVED,
    EXECUTED
} StateI2C;

typedef enum {
    SET,
    INCREMENT,
    DECREMENT,
    ADJUST
} Command;

//store the two byte command received for setting PWM
typedef struct {
    uint8_t commandType;
    uint16_t parameter;
    StateI2C state;
} commandContainer;

// command storage used by interrupts
volatile commandContainer pwmCommand;



// function declarations
void i2c_slave_PWMRdInterruptHandler(void);
void i2c_slave_PWMAddrInterruptHandler(void);


#endif  // _I2CSLAVE_H
