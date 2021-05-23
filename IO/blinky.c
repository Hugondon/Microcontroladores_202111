/*
 * Blinky.c
 *
 * Created: 22/05/2021 05:29:38 p. m.
 * Author : Hugo	
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>


int main(void){

    DDRC = (1 << PC5);
	
    while (1){
		PORTC ^= (1 << PC5);
        _delay_ms(500);
    }
	return 0;
}

