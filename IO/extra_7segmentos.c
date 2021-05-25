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
	Puerto A: -
	Puerto B: -
	Puerto C: PC0 - PC4 conectado a switch. Activos en '0'. 
                    PC0 - Switch 1
                    PC1 - Switch 2
                    PC2 - Push-Button
                    PC3 - Push-Button
                    PC4 - Switch 3
	Puerto D: PD0 - PD3 a decodificiador BCD - 7 segmentos
                    PD0 - I0    -- BCD (activo en '1')
                    PD1 - I1
                    PD2 - I2
                    PD3 - I3
                PD4 - PD7 conectado a selector de dígitos. 
                    PD4 - I1     -- Activos en '0'
                    PD5 - I2
                    PD6 - I3
                    PD7 - I4
    
    DÍGITO 1        DÍGITO 2        DÍGITO 3        DÍGITO 4
    MILLARES        CENTENAS        DECENAS         UNIDADES
*/

/******* Definiciones *******/
#define REFRESH_DELAY_MS    5
#define MIN_COUNT           0
#define MAX_COUNT           256

#define DIGITO1 0b00001110
#define DIGITO2 0b00001101
#define DIGITO3 0b00001011
#define DIGITO4 0b00000111
    
/******* Prototipos *******/
void desplegar_conteo(uint16_t cnt);
uint16_t conteo_ascendente_descendente_manual(uint16_t cnt);
uint16_t conteo_ascendente(uint16_t cnt);
uint16_t conteo_descendente(uint16_t cnt);


int main(void){

    /******* Inicialización GPIOs *******/
	PORTC = 0xFF;       // Activamos pull-ups
    DDRD = 0xFF;        // Declaramos puerto como salida

    /******* Inicialización variables auxiliares*******/
    uint16_t cnt_general = MIN_COUNT;
	
    while (1){
        switch (PINC){
            case 0b11111110: cnt_general = conteo_ascendente(cnt_general); break;
            case 0b11111101: cnt_general = conteo_ascendente_descendente_manual(cnt_general); break;
            case 0b11101111: cnt_general = conteo_descendente(cnt_general); break;
            default: desplegar_conteo(cnt_general); break;
        }
    }
	return 0;
}

/******* Funciones *******/
void desplegar_conteo(uint16_t cnt){
	uint8_t digito_millares = cnt/1000;
	uint8_t digito_centenas = (cnt - (digito_millares*1000))/100;
	uint8_t digito_decenas = (cnt - (digito_millares*1000 + digito_centenas*100))/10;
	uint8_t digito_unidades = cnt - (digito_millares*1000 + digito_centenas*100 + digito_decenas*10);
    PORTD = ( digito_millares << 0 ) + (DIGITO1 << 4);
    _delay_ms(REFRESH_DELAY_MS);
    PORTD = ( digito_centenas << 0 ) + (DIGITO2 << 4);
    _delay_ms(REFRESH_DELAY_MS);
    PORTD = ( digito_decenas << 0 )+ (DIGITO3 << 4);
    _delay_ms(REFRESH_DELAY_MS);
    PORTD = ( digito_unidades << 0) + (DIGITO4 << 4);
    _delay_ms(REFRESH_DELAY_MS);
}
uint16_t conteo_ascendente(uint16_t cnt){
    cnt = (cnt < MAX_COUNT) ? cnt + 1 : MIN_COUNT;
    desplegar_conteo(cnt);
    return cnt;
}
uint16_t conteo_descendente(uint16_t cnt){
    cnt = (cnt > MIN_COUNT) ? cnt - 1 : MAX_COUNT;
    desplegar_conteo(cnt);
    return cnt;
}
uint16_t conteo_ascendente_descendente_manual(uint16_t cnt){
    while( (PINC == 0b11111101)) desplegar_conteo(cnt);
        switch (PINC){
            case 0b11111001: 
                while( PINC == 0b11111001 ) desplegar_conteo(cnt);
                cnt = conteo_ascendente(cnt);
                break;
            case 0b11110101: 
                while( PINC == 0b11110101 ) desplegar_conteo(cnt);
                cnt = conteo_descendente(cnt);
                break;
            default: desplegar_conteo(cnt); break;
        }
    return cnt;
}
