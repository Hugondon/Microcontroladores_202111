/*
* INI_LCD.c
*
* Created: 24/05/2021 0:26:10
* Author : efrainmendezflores
*/

#include <avr/io.h>
#define F_CPU 8000000UL
#include "util/delay.h"
#include <stdio.h>


////////////////////////////////////////////////
///////// Definiciones y declaraciones /////////
////////////////////////////////////////////////
// Definiciones de la LCD
#define LCD_PORT   PORTD // Puerto del LCD
#define LCD_RS     0 // "1" Data Input - "0" Command Input
#define LCD_RW     1 // "1" Read - "0" Write
#define LCD_E      2 // Enable
#define LCD_Busy   7 // Busy Flag en DB7

// Definiciones de MACROS
#define CLEAR(PORT,BIT) PORT&=~(1<<BIT)
#define SET(PORT,BIT)   PORT|=(1<<BIT)

unsigned char *DDRxL=(unsigned char *) &LCD_PORT-1;
unsigned char *PINxL=(unsigned char *) &LCD_PORT-2;

// Definiciones de funciones
void LCD_INI(void);
void command(unsigned char data);
void wr_put(unsigned char data);
void LCD_Enable(void);
void wr_comand(unsigned char data);
void LCD_clear(void);
void LCD_Print(unsigned char buffer_LCD[16], int tam);

////////////////////////////////////////////////
///// Funcion de inicializacion de la LCD //////
////////////////////////////////////////////////
void LCD_INI()
{
    *DDRxL=0xFF;
    
    // Reiniciando la LCD
    wr_comand(0xff);
    _delay_ms(20);
    wr_comand(0x03);
    _delay_ms(10);
    wr_comand(0x03);
    _delay_ms(1);
    wr_comand(0x03);
    _delay_ms(1);
    
    wr_comand(0x02); // Seleccion de modo en 4 bits
    _delay_ms(1);
    
    // Configuracion de funciones y modos de operacion
    command(0x28); // Function Set: 4-bit, 2 Line, 5x7 Dots
    command(0x0F); // Display ON - Cursor ON (For cursor OFF 0x0C)
    command(0x01); // Display Clear
    command(0x12); // Entry Mode Set: Automatic Increment - No Display shift.
    command(0x02); // Entry Mode Set
}


////////////////////////////////////////////////
///// Funcion de posicionamiento de la LCD /////
////////////////////////////////////////////////
void LCD_gotoxy(unsigned char Columna,unsigned char Fila)
{
    unsigned char Temp=0;
    if(Columna<0 || Fila<0 || Fila>3)
    return;
    if(Fila==2)
    Temp = 0xC0|(Columna & 0x3F);
    else
    Temp=0x80|(Columna & 0x3F);
    command(Temp);
}

////////////////////////////////////////////////
///////// Funcion de borrado de la LCD /////////
////////////////////////////////////////////////
void LCD_clear( )
{
    command(1);
}

////////////////////////////////////////////////
////// Funcion de volver a home de la LCD //////
////////////////////////////////////////////////
void LCD_home(void)
{
    command(2);
}


////////////////////////////////////////////////
//// Funcion de Comandos simples de la LCD /////
////////////////////////////////////////////////
void wr_comand(unsigned char data)
{
    CLEAR(LCD_PORT,LCD_RS);
    CLEAR(LCD_PORT,LCD_RW);
    LCD_PORT=((LCD_PORT&0x0F)|(data<<4));
    SET(LCD_PORT,LCD_E);
    _delay_ms(1);
    CLEAR(LCD_PORT,LCD_E);
    _delay_ms(1);
}


////////////////////////////////////////////////
/// Funcion de Comandos completos de la LCD ////
////////////////////////////////////////////////
void command(unsigned char data)
{
    CLEAR(LCD_PORT,LCD_RS);
    CLEAR(LCD_PORT,LCD_RW);
    
    SET(LCD_PORT,LCD_E);
    LCD_PORT=((LCD_PORT&0x0F)|(data&0xF0));
    CLEAR(LCD_PORT,LCD_E);
    LCD_Enable();
    
    CLEAR(LCD_PORT,LCD_RS);
    CLEAR(LCD_PORT,LCD_RW);
    
    SET(LCD_PORT,LCD_E);
    LCD_PORT=((LCD_PORT&0x0F)|(data<<4));
    CLEAR(LCD_PORT,LCD_E);
    LCD_Enable();
    
}

////////////////////////////////////////////////
///////// Funcion Para escribir chars //////////
////////////////////////////////////////////////
void wr_put(unsigned char data)
{
    SET(LCD_PORT,LCD_RS);
    CLEAR(LCD_PORT,LCD_RW);
    
    SET(LCD_PORT,LCD_E);
    LCD_PORT=((LCD_PORT&0x0F)|(data&0xF0));
    CLEAR(LCD_PORT,LCD_E);
    LCD_Enable();
    
    SET(LCD_PORT,LCD_E);
    LCD_PORT=((LCD_PORT&0x0F)|(data<<4));
    CLEAR(LCD_PORT,LCD_E);
    LCD_Enable();
}


////////////////////////////////////////////////
////////// Funcion Para Enable con BF //////////
////////////////////////////////////////////////
void LCD_Enable(void)
{
    //The Busy Flag is shown in DB7 when RS = 0 and R/W = 1.
    CLEAR(LCD_PORT,LCD_RS);
    SET(LCD_PORT,LCD_RW);
    
    CLEAR(*DDRxL,LCD_Busy);
    SET(LCD_PORT,LCD_Busy);
    do
    {
        CLEAR(LCD_PORT,LCD_E);
        _delay_ms(1);
        SET(LCD_PORT,LCD_E);
        _delay_ms(1);
    } while (*PINxL&(1<<LCD_Busy));
    SET(*DDRxL,LCD_Busy);
    SET(LCD_PORT,LCD_RS);
    CLEAR(LCD_PORT,LCD_RW);
}

////////////////////////////////////////////////
//// Funcion Para escribir cadenas de chars ////
////////////////////////////////////////////////
void LCD_Print(unsigned char buffer_LCD[16], int tam)
{
    int icont = 0;
    while (tam!=0)
    {
        tam--;
        wr_put(buffer_LCD[icont++]);
    }
}
