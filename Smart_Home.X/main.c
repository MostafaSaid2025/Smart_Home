
#include "mcc_generated_files/examples/i2c_master_example.h"

#include "mcc_generated_files/mcc.h"
#include "AECUL_Modules/RealTimeClockDS1307/RealTimeClockDS1307.h"
#include "AECUL_Modules/TemSensor_TC74/TemSensor_TC74.h"

RealTimeClockDS1307_T Real_Time;
uint8_t temp;
bool fan_status1 = 0;
bool fan_status2 = 0;

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    // Enable high priority global interrupts
    INTERRUPT_GlobalInterruptHighEnable();
    // Enable low priority global interrupts.
    INTERRUPT_GlobalInterruptLowEnable();
    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

   
    while (1)
    { 
        Real_Time = RealTimeClockDS1307_Get_Date_Time();
        temp = TempSensor_TC74_Read_Temp(0x9A);
        Print_RealTimeClockDS1307();
        print_temperature_value(temp);
        if ( temp > 26 && fan_status1 == 0)
        {
            I2C_Write1ByteRegister(0x70 ,00 , temp);
            fan_status1 = 1;
            fan_status2 = 0;
        }
        else if ( temp <= 26  && fan_status2 == 0 )
        {
            I2C_Write1ByteRegister(0x70 ,00 , temp);
            fan_status2 = 1;
            fan_status1 = 0;
        }
        __delay_ms(1000);
       
        
    }
}
