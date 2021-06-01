#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

/******* Resumen para lector *******/
/*
PUERTOS:
	Puerto A: -
	Puerto B: -
	Puerto C: -
	Puerto D: LCD
    7   6   5   4   2   1   0
    DB7 DB6 DB5 DB4  E   RW  RS

*/
/******* Pseudo código *******/
/*

*/
/******* Definiciones *******/

/******* Prototipos *******/

int main(void){

    /******* Variables que se utilizarán *******/

    /******* Inicialización GPIOs *******/

    LCD_INI();
	unsigned char hola[] = "Hola Mundo";
    LCD_Print(hola, sizeof(hola)/sizeof(unsigned char));

    /******* Loop *******/
    while (1){

        }
	return 0;
}

/******* Funciones *******/
void mi_funcion(void){

}