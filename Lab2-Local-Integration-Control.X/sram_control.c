
#include <stddef.h>
#include <xc.h>
#include "sram_control.h"
#include "pic18.h" //for delay


//initialize SRAM control pins
void SRAM_CONTROL_INITIALIZE(void){
    //Turn off Analog functionality of all pins
    //Treat signals as digital
    ANSELA = 0x00;
    ANSELB = 0x00;
    
    //set output of pin bank B other than highest pin
    TRISB = 0x80;
    
    //set input of pin bank C
    TRISA = 0xFF;
    
    //control pins set
    sram_writeEnable = 1;
    sram_outputEnable = 1;
}


uint8_t sram_read(uint8_t address){
    uint8_t data;
    //set input on pins for microcontroller
    TRISA = 0xFF;

    //put write enable high and set address
    sram_outputEnable = 0;
    sram_writeEnable = 1;
    sram_address = (sram_address & 0xE0) | (address & 0x1F);
    
    __delay_us(1);
    
    //read value of data
    data = sram_data;
    
    //reset stuff
    sram_writeEnable = 1;
    sram_outputEnable = 1;
    
    return data;
}


void sram_write(uint8_t address, uint8_t data){
    //put write enable low and set address
    sram_outputEnable = 1;
    sram_writeEnable = 0;
    sram_address = (sram_address & 0xE0) | (address & 0x1F);
    
    //set output on pins for microcontroller
    TRISA = 0x00;
   
    //set data
    sram_data = data; 
    
    __delay_us(1);
    
    //strobe write enable
    sram_writeEnable = 1;
    __delay_us(1);

    //return to tri-state
    sram_outputEnable = 1;
    TRISA = 0xFF;
}







