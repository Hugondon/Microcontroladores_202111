/*
 * Blinky.c
 *
 * Created: 22/05/2021 05:29:38 p. m.
 * Author : Hugo	
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL	// 8 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

/*
    Corrimiento
        1. 8 bits de salida.
        2. variable cnt hacia derecha y luego hacia izquierda
    Llegaste a 1, ahora derecha.
    Llegaste a 128, ahora izquierda.
*/


int main(void){
	
    uint8_t cnt = 1;
    bool left_direction_cnt_flag = true;

    DDRC |= (1 << PC0) | (1 << PC1)| (1 << PC2)| (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7);

    while (1){
        PORTC = cnt;
        if(left_direction_cnt_flag){
            if(cnt < 128) cnt *= 2;
            else{
                cnt = 128;
                left_direction_cnt_flag = false;
            }
        }else{
            if(cnt > 1) cnt /= 2;
            else{
                cnt = 1;
                left_direction_cnt_flag = true;
            }
        }
		_delay_ms(100);	
    }
	return 0;
}

