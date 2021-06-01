    #ifndef F_CPU
    #define F_CPU 8000000UL	// 8 MHz
    #endif

    #include <avr/io.h>
    #include <util/delay.h>

    /*
        Output Compare Registers (OCR1A/B)
        Input Capture Register (ICR1)
        Timer/Counter Control Registers (TCCR1A/B)
        Output Compare (OC1A/B)
        BOTTOM = 0x0000
        TOP = highest value in count sequence (OCR1A / ICR1)

        16 bit write. High Byte before Low Byte
        16 bit read. Low byte before High Byte

        if TCNT = OCR1x. Match

        Output Compare Unit A allows it to define Timer/Counter TOP
            - It defines eriod

    */

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

    /******* Variables Globales *******/
    const uint16_t TIMER1_RGB_DUTY_CYCLE_LUT [] = {0, 7, 15, 23, 31, 39, 46, 54, 62, 70, 78, 85, 93, 101, 109, 117, 124,
    132, 140, 148, 156, 164, 171, 179, 187, 195, 203, 210, 218, 226, 234, 242, 249, 257, 265, 273, 281, 288, 296, 304,
    312, 320, 328, 335, 343, 351, 359, 367, 374, 382, 390, 398, 406, 413, 421, 429, 437, 445, 452, 460, 468, 476, 484,
    492, 499, 507, 515, 523, 531, 538, 546, 554, 562, 570, 577, 585, 593, 601, 609, 616, 624, 632, 640, 648, 656, 663,
    671, 679, 687, 695, 702, 710, 718, 726, 734, 741, 749, 757, 765, 773, 781};

    const uint8_t RGB_DUTY_CYCLE_LUT [] = {0, 2, 5, 7, 10, 12, 15, 17, 20, 22, 25, 28, 30, 33, 35, 38, 40, 43, 45, 48, 
    51, 53, 56, 58, 61, 63, 66, 68, 71, 73, 76, 79, 81, 84, 86, 89, 91, 94, 96, 99, 102, 104, 107, 109, 112, 114, 117,
    119, 122, 124, 127, 130, 132, 135, 137, 140, 142, 145, 147, 150, 153, 155, 158, 160, 163, 165, 168, 170, 173, 175,
    178, 181, 183, 186, 188, 191, 193, 196, 198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234,
    237, 239, 242, 244, 247, 249, 252, 255};

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
    void brillo_rgb(uint8_t duty_cycle_rojo, uint8_t duty_cycle_verde, uint8_t duty_cycle_azul){
        OCR0 = RGB_DUTY_CYCLE_LUT[duty_cycle_rojo];
        OCR1BH = HIGH(TIMER1_RGB_DUTY_CYCLE_LUT[duty_cycle_verde]);
        OCR1BL = LOW(TIMER1_RGB_DUTY_CYCLE_LUT[duty_cycle_verde]);
        OCR2 = RGB_DUTY_CYCLE_LUT[duty_cycle_azul];
    }