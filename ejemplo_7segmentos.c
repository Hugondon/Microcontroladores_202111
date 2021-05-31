#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

/*
    1. Repasar binario/hexadecimal
*/

/******* Resumen para lector *******/
/*
PUERTOS:
	Puerto A: -
	Puerto B: -
	Puerto C: 
        ENTRADAS
        PC0 - PC1 switches (activos en '0')
            PC0 - Switch 1
            PC1 - Switch 8
        PC2 - PC3 botones (activos en '0')
            PC2 - Botón 1 (incremento)
            PC3 - Botón 8 (decremento)
	Puerto D: 
        SALIDAS
        PD0 - PD3 salidas BCD para dígito (activos en '1')
        PD0 - I0
        PD1 - I1
        PD2 - I2
        PD3 - I3

        PD4 - PD7 salidas selectoras de dígito (activos en '0')

        PD4 - D0
        PD5 - D1
        PD6 - D2
        PD7 - D3

        0b1110 -> Activar D0
        0b1101 -> Activar D1
        0b1011 -> Activar D2
        0b0111 -> Activar D3
*/
/******* Pseudocódigo *******/
/*
    Inicializaciones (GPIOs)

    loop:
        a) Primer Switch activo (conteo ascendente automático)
        b) Segundo Switch activo. (conteo manual)
            1) No has apretado nada. Desplegar conteo
            2) Presionaste botón para incrementar 
                - INCREMENTAMOS AL DEJAR DE PRESIONAR
            3) Presionaste botón para decrementar
                - DECREMENTAMOS AL DEJAR DE PRESIONAR
        c) Despliegue último conteo

        En funciones para incrementar / decrementar, hay que revisar
        que esté en mínimo o en máximo para resettear
*/

/******* Definiciones *******/
#define SW1 PC0
#define SW8 PC1

#define BT1 PC2
#define BT8 PC3

#define DIGITO1 0b11111110
#define DIGITO2 0b11111101
#define DIGITO3 0b11111011
#define DIGITO4 0b11110111
#define REFRESH_DELAY_MS 5

#define CONTEO_MIN 0
#define CONTEO_MAX 9999

/******* Prototipos *******/
void primer_switch_activo(void);
void segundo_switch_activo(void);
void ningun_switch_activo(void);
void conteo_ascendente(void);
void conteo_descendente(void);
void desplegar_numero(void);

/******* Variable global *******/
// 16 bits -> 0 -  6553
uint16_t conteo = 0;


int main(void){

    /******* Variables que se utilizarán *******/

    // 8 bits -> 0 - 255
    uint8_t espejo = 0;

    /******* Inicialización GPIOs *******/
    DDRC = (0 << SW1) | (0 << SW8) | (0 << BT1) | (0 << BT8);  // Pines como entrada
    // Pull up es para dar estado definitivo en '1'
    PORTC = (1 << SW1) | (1 << SW8) | (1 << BT1) | (1 << BT8);  // Habilitar  pull-ups

    DDRD |= 0xFF;
    
    /******* Loop *******/
    while (1){
        espejo = PINC;
        if(espejo == 0b11111110){

        }else if(espejo == 0b11111101){

        }else{

        }
        switch (espejo){
        case 0b11111110:
            primer_switch_activo();
            break;
        case 0b11111101:
            segundo_switch_activo();
            break;
        default:
            ningun_switch_activo();
            break;
        }


    }
	return 0;
}

/******* Funciones *******/
void primer_switch_activo(void){
    conteo_ascendente();
    desplegar_numero();
}

void segundo_switch_activo(void){
	while(PINC == 0b11111101){
		desplegar_numero();
	}
    switch (PINC){
    case 0b11111001:
        while(PINC == 0b11111001){
			desplegar_numero();
		}
            conteo_ascendente();
            desplegar_numero();
        break;
    case 0b11110101:
        while(PINC == 0b11110101){
	        desplegar_numero();
        }
            conteo_descendente();
            desplegar_numero();
        break;
    default:
        desplegar_numero();
        break;
    }
}
void ningun_switch_activo(void){
    desplegar_numero();
}

void conteo_ascendente(void){
    if(conteo == CONTEO_MAX){
        conteo = CONTEO_MIN;
    }else{
        conteo++;
    }
}
void conteo_descendente(void){
    if(conteo == CONTEO_MIN){
        conteo = CONTEO_MAX;
    }else{
        conteo--;
    }
}
void desplegar_numero(void){
    // 1234 = 1*1000 + 2*100 + 3*10 + 4

    // 1234/1000 = 1
	uint8_t digito_millares = conteo/1000;
    // 1234 - 1000 = 234/100 = 2
	uint8_t digito_centenas = (conteo - (digito_millares*1000))/100;
    // 1234 - 1000 - 200 = 34/10 = 3
	uint8_t digito_decenas = (conteo - (digito_millares*1000 + digito_centenas*100))/10;
    // 1234 - 1000 - 200 - 30 = 4/1 = 4
	uint8_t digito_unidades = conteo - (digito_millares*1000 + digito_centenas*100 + digito_decenas*10);

    // 1
    PORTD = ( digito_millares << 0 ) + (DIGITO1 << 4);
    _delay_ms(REFRESH_DELAY_MS);
    // 2
    PORTD = ( digito_centenas << 0 ) + (DIGITO2 << 4);
    _delay_ms(REFRESH_DELAY_MS);
    // 3
    PORTD = ( digito_decenas << 0 )+ (DIGITO3 << 4);
    _delay_ms(REFRESH_DELAY_MS);
    // 4
    PORTD = ( digito_unidades << 0) + (DIGITO4 << 4);
    _delay_ms(REFRESH_DELAY_MS);
}
