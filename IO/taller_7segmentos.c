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
        PD4 - PD7 salidas selectoras de dígito (activos en '0')
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
*/

/******* Definiciones *******/
#define SW1 PC0
#define SW8 PC1

#define BT1 PC2
#define BT8 PC3


/******* Prototipos *******/
void primer_switch_activo(void);
void segundo_switch_activo(void);
void ningun_switch_activo(void);
void conteo_ascendente(void);
void conteo_descendente(void);
void desplegar_numero(void);

/******* Variable global *******/
// 16 bits -> 0 -  65535
uint16_t conteo = 0;


int main(void){

    /******* Variables que se utilizarán *******/

    // 8 bits -> 0 - 255
    uint8_t espejo = 0;

    /******* Inicialización GPIOs *******/
    DDRC = (0 << SW1) | (0 << SW8) | (0 << BT1) | (0 << BT8);  // Pines como entrada
    // Pull up es para dar estado definitivo en '1'
        0X0F
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
    uint8_t espejo = 0;
    switch (espejo){
    case 0b11111001:

        while(espejo == 0b11111001);
            conteo_ascendente();
            desplegar_numero();
        break;
    case 0b11110101:
        while(espejo == 0b11110101);
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