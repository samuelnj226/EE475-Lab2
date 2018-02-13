/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.35
        Device            :  PIC18F25K22
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA4 procedures
#define RA4_SetHigh()    do { LATAbits.LATA4 = 1; } while(0)
#define RA4_SetLow()   do { LATAbits.LATA4 = 0; } while(0)
#define RA4_Toggle()   do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define RA4_GetValue()         PORTAbits.RA4
#define RA4_SetDigitalInput()   do { TRISAbits.TRISA4 = 1; } while(0)
#define RA4_SetDigitalOutput()  do { TRISAbits.TRISA4 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()    do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()   do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()   do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()         PORTBbits.RB0
#define RB0_SetDigitalInput()   do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()  do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()     do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()   do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode() do { ANSELBbits.ANSB0 = 1; } while(0)
#define RB0_SetDigitalMode()do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set SCL2 aliases
#define SCL2_TRIS               TRISBbits.TRISB1
#define SCL2_LAT                LATBbits.LATB1
#define SCL2_PORT               PORTBbits.RB1
#define SCL2_WPU                WPUBbits.WPUB1
#define SCL2_ANS                ANSELBbits.ANSB1
#define SCL2_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define SCL2_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define SCL2_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define SCL2_GetValue()           PORTBbits.RB1
#define SCL2_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define SCL2_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define SCL2_SetPullup()      do { WPUBbits.WPUB1 = 1; } while(0)
#define SCL2_ResetPullup()    do { WPUBbits.WPUB1 = 0; } while(0)
#define SCL2_SetAnalogMode()  do { ANSELBbits.ANSB1 = 1; } while(0)
#define SCL2_SetDigitalMode() do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set SDA2 aliases
#define SDA2_TRIS               TRISBbits.TRISB2
#define SDA2_LAT                LATBbits.LATB2
#define SDA2_PORT               PORTBbits.RB2
#define SDA2_WPU                WPUBbits.WPUB2
#define SDA2_ANS                ANSELBbits.ANSB2
#define SDA2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define SDA2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define SDA2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define SDA2_GetValue()           PORTBbits.RB2
#define SDA2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define SDA2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define SDA2_SetPullup()      do { WPUBbits.WPUB2 = 1; } while(0)
#define SDA2_ResetPullup()    do { WPUBbits.WPUB2 = 0; } while(0)
#define SDA2_SetAnalogMode()  do { ANSELBbits.ANSB2 = 1; } while(0)
#define SDA2_SetDigitalMode() do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set channel_AN9 aliases
#define channel_AN9_TRIS               TRISBbits.TRISB3
#define channel_AN9_LAT                LATBbits.LATB3
#define channel_AN9_PORT               PORTBbits.RB3
#define channel_AN9_WPU                WPUBbits.WPUB3
#define channel_AN9_ANS                ANSELBbits.ANSB3
#define channel_AN9_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define channel_AN9_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define channel_AN9_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define channel_AN9_GetValue()           PORTBbits.RB3
#define channel_AN9_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define channel_AN9_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define channel_AN9_SetPullup()      do { WPUBbits.WPUB3 = 1; } while(0)
#define channel_AN9_ResetPullup()    do { WPUBbits.WPUB3 = 0; } while(0)
#define channel_AN9_SetAnalogMode()  do { ANSELBbits.ANSB3 = 1; } while(0)
#define channel_AN9_SetDigitalMode() do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()    do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()   do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()   do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()         PORTBbits.RB6
#define RB6_SetDigitalInput()   do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()  do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()     do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()   do { WPUBbits.WPUB6 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/