#ifndef LCD_H
#define	LCD_H


/* Section includes */
#include "string.h"
#include "../../mcc_generated_files/pin_manager.h"


/* Section Macro declarations */

#define _LCD_CLEAR 0x01
#define _LCD_RETURN_HOME 0x02
#define _LCD_ENTRY_MODE_DEC_SHIFT_OFF 0x04
#define _LCD_ENTRY_MODE_DEC_SHIFT_ON  0x05  
#define _LCD_ENTRY_MODE_INC_SHIFT_OFF 0x06  
#define _LCD_ENTRY_MODE_INC_SHIFT_ON  0x07
#define _LCD_CURSOR_MOVE_SHIFT_LEFT   0x10  
#define _LCD_CURSOR_MOVE_SHIFT_RIGHT  0x14
#define _LCD_DISPLAY_SHIFT_LEFT       0x18  
#define _LCD_DISPLAY_SHIFT_RIGHT      0x1C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF    0x0C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON     0x0D
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF     0x0E
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON      0x0F
#define _LCD_DISPLAY_OFF_CURSOR_OFF                 0x08
#define _LCD_8BIT_MODE_2_LINE   0x38
#define _LCD_4BIT_MODE_2_LINE   0x28
#define _LCD_CGRAM_START        0x40
#define _LCD_DDRAM_START        0x80


#define ROW1  1
#define ROW2  2
#define ROW3  3
#define ROW4  4


/* Section Function declarations */

void lcd_4bit_initialize(void);
void lcd_4bit_send_command(uint8_t command);
void lcd_4bit_send_string(uint8_t *str);
void lcd_4bit_send_char_data(uint8_t data);
void lcd_4bit_set_cursor(uint8_t row , uint8_t column);

void convert_uint8_to_string(uint8_t value , uint8_t *str);
void convert_uint16_to_string(uint16_t value , uint8_t *str);
void convert_uint32_to_string(uint32_t value , uint8_t *str);

#endif	/* LCD_H */

