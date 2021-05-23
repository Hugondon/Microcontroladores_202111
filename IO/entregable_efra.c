#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

/******* Resumen para lector *******/
/*
PUERTOS:
	Puerto A: PA0 - PA3 conectado a switch. Activos en '0'. 
	Puerto B: -
	Puerto C: PC0 - PC7 conectado a LEDs. Activos en '1'.
	Puerto D: -
*/

/******* Prototipos *******/
void conteo_binario(void);
void running_led_izquierda(void);
void running_led_derecha(void);
void fibonnaci_propuesto(void);

int main(void){

    /******* Inicialización GPIOs *******/
	PORTA = 0xFF;       // Activamos pull-ups
    DDRD = 0xFF;        // Declaramos puerto como salida

    /******* Loop *******/
    while (1){
        switch (PINA){
        case 0b11111110: conteo_binario(); break;
        case 0b11111101: running_led_izquierda(); break;
        case 0b11111011: running_led_derecha(); break;
        case 0b11110111: fibonnaci_propuesto(); break;
        default: PORTD = 0x00; break;
        }
        _delay_ms(200);
    }
	return 0;
}

/******* Funciones *******/

void conteo_binario(void){
    static uint8_t cnt = 0;
    PORTD = cnt;
    cnt++;
}
void running_led_izquierda(void){
    static uint8_t cnt = 1;
    PORTD = cnt;
    // Al hacer 128 * 2, el valor de cnt ser� 0b00000000
    cnt = (cnt != 0 ? cnt*2 : 1);
}
void running_led_derecha(void){
    static uint8_t cnt = 128;
    PORTD = cnt;
    // Al hacer 1 / 2, el valor de cnt ser� 0b00000000
    cnt = (cnt != 0 ? cnt/2 : 128);
}
void fibonnaci_propuesto(void){
    static uint8_t position = 0;
    uint8_t fibonacci_LUT[] = {
        1, 1, 2, 3,
        5, 8, 13,21, 
        34, 55, 89, 144,
        233
    };
    PORTD = fibonacci_LUT[position];
    position = (position < sizeof(fibonacci_LUT)/sizeof(uint8_t) ? position + 1 : 0);
}