/* 
 * File:   sram_control.h
 * Author: Sam
 *
 * Created on January 26, 2018, 12:04 PM
 */

#ifndef SRAM_CONTROL_H
#define	SRAM_CONTROL_H

#include <stddef.h>
#include <xc.h>
#include <stdint.h>

/* Pins:
 * RA4-RA0 - 5 bit address
 * RA6 - OE
 * RA7 - WE
 * 
 * RC7-RC0 - 8 bit data
 */

//writeEnable: bit4
#define sram_writeEnable PORTBbits.RB5
//outputEnable: bit5
#define sram_outputEnable PORTBbits.RB6
//address
#define sram_address PORTB
//data 
#define sram_data PORTA

#define _XTAL_FREQ 16000000


void SRAM_CONTROL_INITIALIZE(void);
uint8_t sram_read(uint8_t address);
void sram_write(uint8_t address, uint8_t data);


#endif	/* SRAM_CONTROL_H */

