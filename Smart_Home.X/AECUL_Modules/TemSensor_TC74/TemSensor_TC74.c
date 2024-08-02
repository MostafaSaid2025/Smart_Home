#include "TemSensor_TC74.h"
#include "../LCD/LCD.h"



uint8_t TempSensor_TC74_Read_Temp(i2c_address_t TC74_Address)
{
    uint8_t TC74_TempValue = 0;
    TC74_TempValue = I2C_Read1ByteRegister(TC74_Address, 0x00);
    
    return TC74_TempValue;
}

void print_temperature_value(uint8_t temperature)
{
    uint8_t tempString[9]; // "Temp: xxC"
    

    // Convert temperature to string format
    tempString[0] = 'T';
    tempString[1] = 'e';
    tempString[2] = 'm';
    tempString[3] = 'p';
    tempString[4] = ':';
    tempString[5] = ' ';
    tempString[6] = (temperature / 10) + '0'; // Tens place
    tempString[7] = (temperature % 10) + '0'; // Ones place
    tempString[8] = 'C'; // Celsius
    tempString[9] = '\0'; // Null-terminate the string

    // Display the temperature string on the third line (assuming a 4-line LCD)
    lcd_4bit_set_cursor(3, 1); // Set cursor to the third line, first column
    lcd_4bit_send_string(tempString);
}