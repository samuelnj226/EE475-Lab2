/**
  EUSART2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart2.h

  @Summary
    This is the generated header file for the EUSART2 driver using Foundation Services Library

  @Description
    This header file provides APIs for driver for EUSART2.
    Generation Information :
        Product Revision  :  Foundation Services Library - 0.1.23
        Device            :  PIC18F25K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
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

#ifndef _EUSART2_H
#define _EUSART2_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: Macro Declarations
*/

/**
  Section: Data Type Definitions
*/

/**
 Section: Global variables
 */


/**
  Section: EUSART2 APIs
*/

void (*EUSART2_TxDefaultInterruptHandler)(void);
void (*EUSART2_RxDefaultInterruptHandler)(void);

/**
 * @brief Initialization routine that takes inputs from the EUSART2 GUI.
 *
 */
void EUSART2_Initialize(void);

/**
 * @brief Check if the EUSART2 transmitter is empty
 *
 * @return The number of available bytes that EUSART2 has remaining in its transmit buffer.
 * @retval 0 the EUSART2 transmitter is not empty
 * @retval 1+ the EUSART2 transmitter has available space for writing
 */
uint8_t EUSART2_is_tx_ready(void);

/**
 * @brief Check if the EUSART2 receiver is not empty
 *
 * @return The number of bytes EUSART2 has available for reading.
 * @retval 0 the EUSART2 receiver is empty
 * @retval 1+ the EUSART2 receiver has data available to read
 */
uint8_t EUSART2_is_rx_ready(void);

/**
 * @brief Check if EUSART2 data is transmitted
 *
 * @return Receiver ready status
 * @retval false  Data is not completely shifted out of the shift register
 * @retval true   Data completely shifted out if the USART shift register
 */
bool EUSART2_is_tx_done(void);

/**
 * @brief Read one character from EUSART2
 *
 * @return Data read from the EUSART2 module
 */
uint8_t EUSART2_Read(void);

/**
 * @brief Write one character on EUSART2
 *
 * @param[in] data The character to write to the EUSART2 bus
 *
 * @return Nothing
 */
void EUSART2_Write(uint8_t txData);

/**
 * @brief Maintains the driver's transmitter state machine and implements its ISR.
 *
 * @return Nothing
 */
void EUSART2_Transmit_ISR(void);

/**
 * @brief Maintains the driver's receiver state machine and implements its ISR
 *
 * @return Nothing
 */
void EUSART2_Receive_ISR(void);

/**
 * @brief sets a custom function to be called at the end of the transmit ISR
 * 
 * @param handler
 */
void EUSART2_SetTxInterruptHandler(void (* handler) (void));

/**
 * @brief sets a custom function to be called at the end of the receive ISR
 * 
 * @param handler
 */
void EUSART2_SetRxInterruptHandler(void (* handler) (void));

//Sam code
typedef enum {
    RECEIVINGFEEDBACK,
    RECEIVEDFEEDBACK,
    OFFEEDBACK,
    PROCESSEDFEEDBACK
} stateFeedBack;

stateFeedBack receiveStateFeedBack;


//sam modified

#define EUSART2_TX_BUFFER_SIZE 8
#define EUSART2_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
*/

volatile uint8_t eusart2TxHead = 0;
volatile uint8_t eusart2TxTail = 0;
volatile uint8_t eusart2TxBuffer[EUSART2_TX_BUFFER_SIZE];
volatile uint8_t eusart2TxBufferRemaining;

volatile uint8_t eusart2RxHead = 0;
volatile uint8_t eusart2RxTail = 0;
volatile uint8_t eusart2RxBuffer[EUSART2_RX_BUFFER_SIZE];
volatile uint8_t eusart2RxCount;

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  // _EUSART2_H
/**
 End of File
*/
