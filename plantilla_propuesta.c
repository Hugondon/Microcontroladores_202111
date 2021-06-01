#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

/******* Resumen para lector *******/
/*
PUERTOS:
	Puerto A: -
	Puerto B: -
	Puerto C: -
	Puerto D: -

TIMERS:
    Timer 0: -
    Timer 1: -
    Timer 2: -
*/
/******* Pseudo código *******/
/*

*/
/******* Definiciones *******/
#define HIGH(x) ( (x) >> 8)
#define LOW(x) ( (x) & 0xFF)
#define CONSTANTE 5

/******* Prototipos *******/
void mi_funcion(void);

/******* Variables Globales *******/

int main(void){

    /******* Variables que se utilizarán *******/

    /******* Inicialización GPIOs *******/

    /******* Inicialización Timers *******/

    /******* Loop *******/
    while (1){

        }
    }
	return 0;
}

/******* Funciones *******/
void mi_funcion(void){

}