#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

/******* Resumen para lector *******/
/*
PUERTOS:
	Puerto A: -
	Puerto B: 
        PB3 - PWM Timer 0 (0C0). Rojo
	Puerto C: -
	Puerto D: -
        PD4 - PWM Timer1 (OC1B). Verde
        PD7 - PWM Timer2 (OC2). Azul

TIMERS:
    Timer 0:
    Phase Correct
        WGM00 WGM01 10      (phase correct)
    No Invertido
        COM01 COM00 10      (clear en OC0)
    Prescaler
        CS02 CS01 CS00 100  (clk/256)

    fosc = fclk/(N*510) = 8MHz/(256*510) = 61.27 Hz

    DC = OCR0/255 * 100 
    OCR0 = DC*255/100

    Timer 1: 
        Variar duty cycle
        Phase Correct (necesitamos 0% y 100%)
        
        1) Modo
        MODO 11
        WGM13 WGM12 WGM11 WGM10
        1     0     1     1


        2) ¿Cómo variamos Duty Cycle?
            a) Obtener el tope 
            fg = fosc/(2*N*tope)

            fg = 640 Hz

            640 = 8MHz/(2*32*tope)

            tope = 8MHz/(2*20*32)
            tope = 781
            OCR1A = 781
        
            b) Obtener duty cycle

            DC = OCR1B/TOPE * 100
            OCR1B = DC*TOPE/100 = DC*781/100


        TCCR1A:
            
        No Invertido
            COM1B1 COM1B0 10 (Clear OC1B)
            WGM11 WGM10   11 (Modo 11)

        TCCR1B:

            WGM13 WGM12   10 (Modo 11)
            CS12 CS11 CS10 100

    Timer 2: 
    Timer 0:
    Phase Correct
        WGM20 WGM21 10      (phase correct)
    No Invertido
        COM21 COM20 10      (clear en OC0)
    Prescaler
        CS22 CS21 CS20 110  (clk/256)
    
    fosc = fclk/(N*510) = 8MHz/(256*510) = 61.27 Hz

    DC = OCR2/255 * 100 
    OCR2 = DC*255/100

*/
/******* Pseudo código *******/
/*
    Rojo
    Magenta (rojo + azul)
    Azul
    cian (azul + verde)
verde
amarillo (rojo + verde)
blanco (rojo + verde + azul)

*/
/******* Definiciones *******/
#define HIGH(x) ( (x) >> 8)
#define LOW(x) ( (x) & 0xFF)
#define TOP 781
#define DELAY_MS 500

/******* Prototipos *******/
void brillo_rgb(int duty_cycle_rojo, int duty_cycle_verde, int duty_cycle_azul);

/******* Variables Globales *******/
const uint8_t RGB_DUTY_CYCLE_LUT [] = {0, 2, 5, 7, 10, 12, 15, 17, 20, 22, 25, 28, 30, 33, 35,
	38, 40, 43, 45, 48, 51, 53, 56, 58, 61, 63, 66, 68, 71, 73, 76, 79, 81, 84, 86, 89, 91, 94,
	96, 99, 102, 104, 107, 109, 112, 114, 117, 119, 122, 124, 127, 130, 132, 135, 137, 140, 142,
	145, 147, 150, 153, 155, 158, 160, 163, 165, 168, 170, 173, 175, 178, 181, 183, 186, 188, 191,
	193, 196, 198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 237, 239,
	242, 244, 247, 249, 252, 255};
	
const uint16_t GREEN_DUTY_CYCLE_LUT [] = {0, 7, 15, 23, 31, 39, 46, 54, 62, 70, 78, 85, 93, 101, 109,
	117, 124, 132, 140, 148, 156, 164, 171, 179, 187, 195, 203, 210, 218, 226, 234, 242, 249, 257, 265,
	273, 281, 288, 296, 304, 312, 320, 328, 335, 343, 351, 359, 367, 374, 382, 390, 398, 406, 413, 421,
	429, 437, 445, 452, 460, 468, 476, 484, 492, 499, 507, 515, 523, 531, 538, 546, 554, 562, 570, 577,
	585, 593, 601, 609, 616, 624, 632, 640, 648, 656, 663, 671, 679, 687, 695, 702, 710, 718, 726, 734,
	741, 749, 757, 765, 773, 781};

int main(void){

    /******* Variables que se utilizarán *******/

    /******* Inicialización GPIOs *******/
    DDRB |= (1 << PB3);
    DDRD |= (1 << PD4) | (1 << PD7);
    /******* Inicialización Timers *******/
    TCCR0 |= (1 << WGM00) | (1 << COM01) | (1 << CS02);
    TCCR2 |= (1 << WGM20) | (1 << COM21) | (1 << CS22);

    // Timer 1. Siempre escribir primero la parte alta.
    OCR1AH = HIGH(TOP);
    OCR1AL = LOW(TOP);

    TCCR1A |= (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);
    TCCR1B |= (1 << WGM13) | (1 << CS11);


    /******* Loop *******/
    while (1){
		
        /* PARTE ROJO */
        brillo_rgb(75, 0, 0);
        _delay_ms(DELAY_MS);

        /* PARTE MAGENTA */
        brillo_rgb(50, 0, 50);
        _delay_ms(DELAY_MS);

        /* PARTE AZUL */
        brillo_rgb(0, 0, 75);
        _delay_ms(DELAY_MS);

        /* PARTE CYAN */
        brillo_rgb(0, 50, 50);
        _delay_ms(DELAY_MS);

        /* PARTE VERDE */
        brillo_rgb(0, 75, 0);
        _delay_ms(DELAY_MS);

        /* PARTE AMARILLO */
        brillo_rgb(50, 32, 0);
        _delay_ms(DELAY_MS);

        /* PARTE BLANCO */
        brillo_rgb(61, 81, 61);
        _delay_ms(DELAY_MS);

    }
	return 0;
}

/******* Funciones *******/
void brillo_rgb(int duty_cycle_rojo, int duty_cycle_verde, int duty_cycle_azul){

    OCR0 = RGB_DUTY_CYCLE_LUT[duty_cycle_rojo];
    OCR1B = GREEN_DUTY_CYCLE_LUT[duty_cycle_verde];
    OCR2 = RGB_DUTY_CYCLE_LUT[duty_cycle_azul];

}