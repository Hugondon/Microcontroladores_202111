#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

/******* Resumen para lector *******/
/*
PUERTOS:
	Puerto A: PA0 - PA4 conectado a switch. Activos en '0'. 
            PA0 - Switch 1
            PA1 - Switch 2
            PA2 - Switch 3
            PA3 - Switch 4
            PA4 - Switch 5
	Puerto B: -
	Puerto C: -
	Puerto D: PD0 - PD7 conectado a LEDs. Activos en '1'.
*/

/******* Prototipos *******/
void conteo_binario(void);
void running_led_izquierda(void);
void running_led_derecha(void);
void fibonnaci_propuesto(void);
void running_led_bidireccional(void);


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
        case 0b11101111: running_led_bidireccional(); break;
        default: break;
        }
        _delay_ms(50);
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
    // Al hacer 128 * 2, el valor de cnt será 0b00000000
    cnt = (cnt != 0 ? cnt<<1 : 1);
}
void running_led_derecha(void){
    static uint8_t cnt = 128;
    PORTD = cnt;
    // Al hacer 1 / 2, el valor de cnt será 0b00000000
    cnt = (cnt != 0 ? cnt>>1 : 128);
}
void fibonnaci_propuesto(void){
    static uint8_t position = 0;
    const uint8_t fibonacci_LUT[] = {
        1, 1, 2, 3,
        5, 8, 13,21, 
        34, 55, 89, 144,
        233
    };
    PORTD = fibonacci_LUT[position];
    position = (position < sizeof(fibonacci_LUT)/sizeof(uint8_t) ? position + 1 : 0);
}
void running_led_bidireccional(void){
    static bool left_direction_cnt_flag = true;
    static uint8_t cnt = 1;
	PORTD = cnt;
    if(left_direction_cnt_flag){
        if(cnt < 128) cnt = cnt << 1;
        else{
            cnt = 128;
            left_direction_cnt_flag = false;
        }
    }else{
        if(cnt > 1) cnt = cnt >> 1;
        else{
            cnt = 1;
            left_direction_cnt_flag = true;
        }
    }
}