#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

/******* Resumen para lector *******/
/*
PUERTOS:
	Puerto A: -
	Puerto B:   PB0 - PB2 sincronización
                    PB0 - RS
                    PB1 - RW
                    PB2 - E
                    PB3 - -
				PB4 - PB7 envío de información
                    PB4 - D0
                    PB5 - D1
                    PB6 - D2
                    PB7 - D3
	Puerto C: -
	Puerto D: -
*/

#define RS  PB0
#define RW  PB1
#define EN  PB2
#define D0  PB4
#define D1  PB5
#define D2  PB6
#define D3  PB7
    
/******* Prototipos *******/
void lcd_cmd(uint8_t cmd);
void lcd_init(void);
void lcd_data(uint8_t data);


int main(void){

    /******* Inicialización GPIOs *******/
    DDRA = 0xFF;        // Puerto como salida
    DDRC = 0xFF;

    /******* Inicialización variables auxiliares*******/
	
    lcd_init();
    lcd_data('H');
    lcd_data('o');
    lcd_data('l');
    lcd_data('a');
    lcd_data(' ');
    lcd_data('P');
    lcd_data('i');
    lcd_data('n');
    lcd_data('k');
    lcd_data('y');

    while (1){
    }
	return 0;
}

/******* Funciones *******/
void lcd_cmd(uint8_t cmd){
    PORTA = cmd;			// Data / Command
	PORTC &= ~(0x01);
	PORTC &= ~(0x02);		// CLR RW
	PORTC |= 0x04;		    // Set enable
    _delay_ms(10);
	PORTC &= ~(0x04);		// Clear enable

}

void lcd_data(uint8_t data){
    PORTA = data;
	PORTC |= (0x01);		// 
	PORTC &= ~(0x02);		// CLR RW
	PORTC |= 0x04;		    // Set enable
    _delay_ms(10);
	PORTC &= ~(0x04);		// Clear enable
}

void lcd_init(void){
    lcd_cmd(0x38);      // 8 bit mode
    lcd_cmd(0x0E);      // enable cursor
    lcd_cmd(0x06);      // cursor increment
    lcd_cmd(0x01);      // clear lcd
    lcd_cmd(0x80);      // row 1, col 1
}