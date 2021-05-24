#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

/******* Resumen para lector *******/
/*
    Conteo automático con Switch 1
    Conteo manual con Switch 2
PUERTOS:
	Puerto A: PA0 - PA3 conectado a switch. Activos en '0'. 
                    PA0 - Switch 1
                    PA1 - Switch 2
                    PA2 - Push-Button
                    PA3 - Push-Button
	Puerto B: -
	Puerto C: -
	Puerto D:   PD0 - PD3 conectado a selector de dígitos. 
                    PD0 - D1    -- Activos en '0'
                    PD1 - D2
                    PD2 - D3
                    PD3 - D4
                PD4 - PD7 a decodificiador BCD - 7 segmentos
                    PD4 - I1    -- BCD (activo en '1')
                    PD5 - I2
                    PD6 - I3
                    PD7 - I4
*/

#define REFRESH_DELAY_MS    5
#define MIN_COUNT           0
#define MAX_COUNT           9999
    
/******* Prototipos *******/
void desplegar_conteo(uint16_t conteo);
uint16_t conteo_ascendente_automatico(uint16_t cnt);
uint16_t conteo_ascendente_descendente_manual(uint16_t cnt);

int main(void){

    /******* Inicialización GPIOs *******/
	PORTA = 0xFF;       // Activamos pull-ups
    DDRD = 0xFF;        // Declaramos puerto como salida

    /******* Inicialización variables auxiliares*******/
    uint16_t cnt_general = MIN_COUNT;
	
    while (1){
        switch (PINA){
            case 0b11111110: cnt_general = conteo_ascendente_automatico(cnt_general); break;
            case 0b11111101: cnt_general = conteo_ascendente_descendente_manual(cnt_general); break;
            default: desplegar_conteo(cnt_general); break;
        }
    }
	return 0;
}

/******* Funciones *******/
void desplegar_conteo(uint16_t cnt){
	uint8_t millares = cnt/1000;
	uint8_t centenas = (cnt - (millares*1000))/100;
	uint8_t decenas = (cnt - (millares*1000 + centenas*100))/10;
	uint8_t unidades = cnt - (millares*1000 + centenas*100 + decenas*10);
    PORTD = ( millares << 4 ) + 0b00001110;
    _delay_ms(REFRESH_DELAY_MS);
    PORTD = ( centenas << 4 ) + 0b00001101;
    _delay_ms(REFRESH_DELAY_MS);
    PORTD = ( decenas << 4 )+ 0b00001011;
    _delay_ms(REFRESH_DELAY_MS);
    PORTD = ( unidades << 4) + 0b00000111;
    _delay_ms(REFRESH_DELAY_MS);
}
uint16_t conteo_ascendente_automatico(uint16_t cnt){
    cnt = (cnt < MAX_COUNT) ? cnt + 1 : MIN_COUNT;
    desplegar_conteo(cnt);
    return cnt;
}
uint16_t conteo_ascendente_descendente_manual(uint16_t cnt){
    while( (PINA == 0b11111101)) desplegar_conteo(cnt);
        switch (PINA){
            case 0b11111001: 
                while( PINA == 0b11111001 ) desplegar_conteo(cnt);
                cnt = (cnt < MAX_COUNT) ? cnt + 1 : MIN_COUNT;
                desplegar_conteo(cnt);
                break;
            case 0b11110101: 
                while( PINA == 0b11110101 ) desplegar_conteo(cnt);
                cnt = (cnt > MIN_COUNT) ? cnt - 1 : MAX_COUNT;
                desplegar_conteo(cnt);
                break;
            default: desplegar_conteo(cnt); break;
        }
    return cnt;
}