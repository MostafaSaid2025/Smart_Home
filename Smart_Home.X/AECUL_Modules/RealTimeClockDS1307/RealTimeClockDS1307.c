#include "RealTimeClockDS1307.h"

static RealTimeClockDS1307_T RealTimeClockDS1307;
/* "_Year" - "_Month" - "_Day" */


RealTimeClockDS1307_T RealTimeClockDS1307_Get_Date_Time(void){
    RealTimeClockDS1307._Seconds = I2C_Read1ByteRegister(0xD0, 0x00);
    RealTimeClockDS1307._Minutes = I2C_Read1ByteRegister(0xD0, 0x01);
    RealTimeClockDS1307._Hours   = I2C_Read1ByteRegister(0xD0, 0x02);
    RealTimeClockDS1307._Day     = I2C_Read1ByteRegister(0xD0, 0x04);
    RealTimeClockDS1307._Month   = I2C_Read1ByteRegister(0xD0, 0x05);
    RealTimeClockDS1307._Year    = I2C_Read1ByteRegister(0xD0, 0x06);
    
    return RealTimeClockDS1307;
}

void Print_RealTimeClockDS1307(void)
{
    char timeString[9];  // "hh:mm:ss"
    char dateString[11]; // "dd-mm-yyyy"
    
    //lcd_4bit_send_command(_LCD_CLEAR);
    // Construct the time string
    timeString[0] = (RealTimeClockDS1307._Hours >> 4) + '0';
    timeString[1] = (RealTimeClockDS1307._Hours & 0x0F) + '0';
    timeString[2] = ':';
    timeString[3] = (RealTimeClockDS1307._Minutes >> 4) + '0';
    timeString[4] = (RealTimeClockDS1307._Minutes & 0x0F) + '0';
    timeString[5] = ':';
    timeString[6] = (RealTimeClockDS1307._Seconds >> 4) + '0';
    timeString[7] = (RealTimeClockDS1307._Seconds & 0x0F) + '0';
    timeString[8] = '\0'; // Null-terminate the string
    
    // Construct the date string
    dateString[0] = (RealTimeClockDS1307._Day >> 4) + '0';
    dateString[1] = (RealTimeClockDS1307._Day & 0x0F) + '0';
    dateString[2] = '-';
    dateString[3] = (RealTimeClockDS1307._Month >> 4) + '0';
    dateString[4] = (RealTimeClockDS1307._Month & 0x0F) + '0';
    dateString[5] = '-';
    dateString[8] = (RealTimeClockDS1307._Year >> 4) + '0';
    dateString[9] = (RealTimeClockDS1307._Year & 0x0F) + '0';
    dateString[6] = '2'; // Assuming you are displaying years in the format "20xx"
    dateString[7] = '0';
    dateString[10] = '\0'; // Null-terminate the string
    
    // Display the time string on the first line
    lcd_4bit_set_cursor(1, 1); // Set cursor to the first line, first column
    lcd_4bit_send_string("Time: ");
    lcd_4bit_send_string(timeString);
    
    // Display the date string on the second line
     lcd_4bit_set_cursor(2, 1); // Set cursor to the second line, first column
    lcd_4bit_send_string("Date: ");
    lcd_4bit_send_string(dateString);
}
