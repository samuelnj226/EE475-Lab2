/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC18F25K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.10
*/
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
/**
  Section: Included Files
*/
#include "eusart1.h"

#include "../sram_control.h"

/**
  Section: EUSART1 APIs
*/
void EUSART1_Initialize(void)
{
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;
    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; WUE disabled; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON1 = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA1 = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA1 = 0x24;

    // 
    SPBRG1 = 0xA0;

    // 
    SPBRGH1 = 0x01;


    // initializing the driver state
    eusart1TxHead = 0;
    eusart1TxTail = 0;
    eusart1TxBufferRemaining = sizeof(eusart1TxBuffer);

    eusart1RxHead = 0;
    eusart1RxTail = 0;
    eusart1RxCount = 0;

    // enable receive interrupt
    PIE1bits.RC1IE = 1;
}

uint8_t EUSART1_Read(void)
{
    uint8_t readValue  = 0;
    
    while(0 == eusart1RxCount)
    {
    }

    readValue = eusart1RxBuffer[eusart1RxTail++];
    if(sizeof(eusart1RxBuffer) <= eusart1RxTail)
    {
        eusart1RxTail = 0;
    }
    PIE1bits.RC1IE = 0;
    eusart1RxCount--;
    PIE1bits.RC1IE = 1;

    return readValue;
}

void EUSART1_Write(uint8_t txData)
{
    while(0 == eusart1TxBufferRemaining)
    {
    }

    if(0 == PIE1bits.TX1IE)
    {
        TXREG1 = txData;
    }
    else
    {
        PIE1bits.TX1IE = 0;
        eusart1TxBuffer[eusart1TxHead++] = txData;
        if(sizeof(eusart1TxBuffer) <= eusart1TxHead)
        {
            eusart1TxHead = 0;
        }
        eusart1TxBufferRemaining--;
    }
    PIE1bits.TX1IE = 1;
}

char getch(void)
{
    return EUSART1_Read();
}

void putch(char txData)
{
    EUSART1_Write(txData);
}

void EUSART1_Transmit_ISR(void)
{

    // add your EUSART1 interrupt custom code
    if(sizeof(eusart1TxBuffer) > eusart1TxBufferRemaining)
    {
        TXREG1 = eusart1TxBuffer[eusart1TxTail++];
        if(sizeof(eusart1TxBuffer) <= eusart1TxTail)
        {
            eusart1TxTail = 0;
        }
        eusart1TxBufferRemaining++;
    }
    else
    {
        PIE1bits.TX1IE = 0;
    }
}


// OUR CUSTOM INTERRUPT
void EUSART1_Receive_ISR(void)
{
    if(1 == RCSTA1bits.OERR)
    {
        // EUSART1 error - restart

        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }

    if(receiveState == RECEIVING) {
        //eusart1RxBuffer[eusart1RxHead] = RCREG1;
        uint8_t receivedChar = RCREG1;
        sram_write(eusart1RxHead, receivedChar);

        //check for reception of end of line character
        if(receivedChar == 13) { //carriage return character
            receiveState = RECEIVED;
        }
  
        //Overflow check
        if(EUSART1_RX_BUFFER_SIZE <= eusart1RxHead) {
            receiveState = OF; //overflow state
        }
        
        eusart1RxHead++;
        eusart1RxCount++;
    } else if(receiveState == RECEIVED || receiveState == OF) {
        uint8_t temp = RCREG1; //read it in, do nothing
    } else { // state PROCESSED, won't happen
        uint8_t temp = RCREG1; //read it in, do nothing
    }
}    
/**
  End of File
*/
