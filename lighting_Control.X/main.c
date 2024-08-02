#include "mcc_generated_files/system/system.h"

void LED_Manual_Control(void);
void LED_Automatic_Control(void);


bool led_status = 0;

int main(void)
{
    SYSTEM_Initialize();
    EUSART_ReceiveEnable();
    EUSART_Enable();
    EUSART_ReceiveInterruptEnable();


    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable(); 

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 

    while(1)
    {
       
    }    
}

void LED_Manual_Control(void)
{
    if (led_status == 0)
    {
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        led_status = 1;
    }
    else
    {
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 0;
        led_status = 0;
    }
}

void LED_Automatic_Control(void)
{
    uint8_t auto_status = RCREG;
  
    if (auto_status == 'a')
    {
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
    }
    else if (auto_status == 'b')
    {
       LATBbits.LATB0 = 0;
       LATBbits.LATB1 = 0;
   
    }
    
}