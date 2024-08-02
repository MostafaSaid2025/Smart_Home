#include "LCD.h"
#define _XTAL_FREQ 16000000

static void lcd_send_4bits(uint8_t command);


void lcd_4bit_initialize(void)
{
    lcd_4bit_send_command(_LCD_8BIT_MODE_2_LINE);
    __delay_ms(5);
    lcd_4bit_send_command(_LCD_8BIT_MODE_2_LINE);
    __delay_ms(150);
    lcd_4bit_send_command(_LCD_8BIT_MODE_2_LINE);
    lcd_4bit_send_command( _LCD_CLEAR);
    lcd_4bit_send_command( _LCD_RETURN_HOME);
    lcd_4bit_send_command( _LCD_ENTRY_MODE_INC_SHIFT_OFF);
    lcd_4bit_send_command( _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON);
    lcd_4bit_send_command( _LCD_4BIT_MODE_2_LINE);
    lcd_4bit_send_command(0x80);
    
    
}

void lcd_4bit_send_command(uint8_t command)
{
    LATBbits.LATB0 = 0;
    lcd_send_4bits(command >> 4);
    LATBbits.LATB1 = 1;
    __delay_us(5);
    LATBbits.LATB1 = 0;
    lcd_send_4bits(command);
    LATBbits.LATB1 = 1;
    __delay_us(5);
    LATBbits.LATB1 = 0;
    
}

void lcd_4bit_send_string(uint8_t *str)
{
    LATBbits.LATB0 = 1;
    while(*str)
    {
        lcd_4bit_send_char_data(*str++);
    }
}

void lcd_4bit_send_char_data(uint8_t data)
{
    LATBbits.LATB0 = 1;
    lcd_send_4bits(data >> 4);
    LATBbits.LATB1 = 1;
    __delay_us(5);
    LATBbits.LATB1 = 0;
    lcd_send_4bits(data);
    LATBbits.LATB1 = 1;
    __delay_us(5);
    LATBbits.LATB1 = 0;
    
}

static void lcd_send_4bits(uint8_t command)
{
    LATBbits.LATB2 =  ((command >> 0) & (uint8_t)0x01);
    LATBbits.LATB3 =  ((command >> 1) & (uint8_t)0x01);
    LATBbits.LATB4 =  ((command >> 2) & (uint8_t)0x01);
    LATBbits.LATB5 =  ((command >> 3) & (uint8_t)0x01);

}

void lcd_4bit_set_cursor(uint8_t row , uint8_t column)
{
    switch (row)
         {
            case ROW1:
                lcd_4bit_send_command((0x80 + column));break;
            case ROW2:
                lcd_4bit_send_command((0xC0 + column));break;
            case ROW3:
                lcd_4bit_send_command((0x94 + column));break;
            case ROW4:
                lcd_4bit_send_command((0xD4 + column));break;   
            default: ;    
         }
}