
// Definiciones de funciones para usar en el main

extern void LCD_INI(void);
extern void wr_put(unsigned char data);
extern void LCD_clear(void);
extern void LCD_home(void);
extern void LCD_gotoxy(unsigned char x,unsigned char y);

extern void command(unsigned char data);
extern void LCD_Print(unsigned char buffer_LCD[16], int tam);