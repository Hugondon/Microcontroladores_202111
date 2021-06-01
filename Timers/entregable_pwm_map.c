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
            PB3 - PWM de Timer0 (ROJO). Activo en '0'
        Puerto C: -
        Puerto D: 
            PD4 - PWM de Timer1 (VERDE). Activo en '0'
            PD7 - PWM de Timer2 (AZUL). Activo en '0'

    TIMERS:
        Timer 0: -
        WGM00 WGM01     ->  10  (Phase Correct)
        COM01 COM00     ->  10  (Clear en OC0) (NO invertido)
        CS02 CS01 CS00  ->  100 (clk/256 -> 8M/256 = 31,250)

        Para Duty Cycle:

            DC_{noInv} = OCR0/255 * 100
            OCR0 = DC*255/100

            fosc = 8M/(256*510) = 61.27 Hz
        Timer 1: 

            1) Elección de Modo
                MODO 11        
                    WGM13   WGM12   WGM11   WGM10
                    1       0       1       1
                
                TOP:    0CR1A (Frecuencia)
                MATCH:  OCR1B (Duty Cycle)
                No Invertido
            
            2) Cálculos.
                a) Frecuencia (20 Hz)
                    20 = 8M/(2*256*TOP) -> TOP = 781
                    OCR1A = 781
                b) Duty Cycle
                    DC = ( OCR1B/781) * 100
                    OCR1B = DC*100/781
        TCCR1A: 
            COM1A1 COM1A0   -> 00
            COM1B1 COM1B0   -> 10

            WGM11 WGM10     -> 11 (Modo 11)

        TCCR1B:
            ICNC1 ICES1     -> 00   (No usamos unidad de captura)
            WGM13 WGM12     -> 10   (Modo 11)
            CS12 CS11 CS10  -> 100  (clk/256)

        Timer 2: -
        WGM20 WGM21     ->  10  (Phase Correct)
        COM21 COM20     ->  10  (Clear en OC0) (NO invertido)
        CS22 CS21 CS20  ->  100 (clk/256 -> 8M/256 = 31,250)

        Para Duty Cycle:

            DC_{noInv} = OCR2/255 * 100
            OCR2 = DC*255/100

            fosc = 8M/(256*510) = 61.27 Hz


    */
    /******* Pseudo código *******/
    /*
        ROJO 
        MAGENTA     (rojo + azul)
        AZUL
        CIAN        (verde + azul)
        VERDE 
        AMARILLO    (rojo + verde)
        BLANCO      (rojo + verde azul)
    */
/******* Definiciones *******/
#define HIGH(x) ( (x) >> 8)
#define LOW(x) ( (x) & 0xFF)
#define DELAY_MS 1000

/******* Prototipos *******/
void brillo_rgb(uint8_t duty_cycle_rojo, uint8_t duty_cycle_verde, uint8_t duty_cycle_azul);
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

int main(void){

	/******* Variables que se utilizarán *******/

	/******* Inicialización GPIOs *******/
	DDRB |= (1 << PB3);
	DDRD |= (1 << PD4) | (1 << PD7);

	/******* Inicialización Timers *******/
	TCCR0 |= (1 << WGM00) | (1 << COM01) | (1 << CS02);
	TCCR2 |= (1 << WGM20) | (1 << COM21) | (1 << CS22);

	OCR1AH = HIGH(781);
	OCR1AL = LOW(781);

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

		for(int i = 0; i < 101; i++){
			brillo_rgb(i,0,0);
			_delay_ms(10);
		}
		for(int i = 0; i < 101; i++){
			brillo_rgb(0,i,0);
			_delay_ms(10);
		}
		for(int i = 0; i < 101; i++){
			brillo_rgb(0,0,i);
			_delay_ms(10);
		}

	}
	return 0;
}

/******* Funciones *******/
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void brillo_rgb(uint8_t duty_cycle_rojo, uint8_t duty_cycle_verde, uint8_t duty_cycle_azul){
    OCR0 = map(duty_cycle_rojo, 0, 100, 0, 255);
    OCR1BH = HIGH(map(duty_cycle_verde, 0, 100, 0, 781));
    OCR1BL = LOW(map(duty_cycle_verde, 0, 100, 0, 781));
    OCR2 = map(duty_cycle_azul, 0, 100, 0, 255);
}
