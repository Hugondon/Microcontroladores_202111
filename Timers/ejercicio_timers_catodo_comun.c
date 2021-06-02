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
const uint8_t RGB_DUTY_CYCLE_LUT [] = {255, 252, 249, 247, 244, 242, 239, 237, 234, 232, 229, 226, 224,
	221, 219, 216, 214, 211, 209, 206, 204, 201, 198, 196, 193, 191, 188, 186, 183, 181, 178, 175, 173, 170,
	168, 165, 163, 160, 158, 155, 153, 150, 147, 145, 142, 140, 137, 135, 132, 130, 127, 124, 122, 119, 117,
	114, 112, 109, 107, 104, 102, 99, 96, 94, 91, 89, 86, 84, 81, 79, 76, 73, 71, 68, 66, 63, 61, 58, 56, 53,
51, 48, 45, 43, 40, 38, 35, 33, 30, 28, 25, 22, 20, 17, 15, 12, 10, 7, 5, 2, 0};

const uint16_t GREEN_DUTY_CYCLE_LUT [] = {781, 773, 765, 757, 749, 741, 734, 726, 718, 710, 702, 695,
	687, 679, 671, 663, 656, 648, 640, 632, 624, 616, 609, 601, 593, 585, 577, 570, 562, 554, 546, 538, 531,
	523, 515, 507, 499, 492, 484, 476, 468, 460, 452, 445, 437, 429, 421, 413, 406, 398, 390, 382, 374, 367,
	359, 351, 343, 335, 328, 320, 312, 304, 296, 288, 281, 273, 265, 257, 249, 242, 234, 226, 218, 210, 203,
	195, 187, 179, 171, 164, 156, 148, 140, 132, 124, 117, 109, 101, 93, 85, 78, 70, 62, 54, 46, 39, 31, 23,
15, 7, 0};

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