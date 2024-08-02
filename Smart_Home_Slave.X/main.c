
#include "mcc_generated_files/mcc.h"

uint8_t RecTC74_A7_TempValue = 0;

static void Custom_I2C_SlaveDefRdInterruptHandler() {
    RecTC74_A7_TempValue = SSPBUF;
    
    if(RecTC74_A7_TempValue > 26){
            DC_MotorPin0_SetHigh();
            DC_MotorPin1_SetLow();
        }
        else{
            DC_MotorPin0_SetLow();
            DC_MotorPin1_SetLow();
        }
}

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
    
    I2C_Open();
    I2C_SlaveSetReadIntHandler(Custom_I2C_SlaveDefRdInterruptHandler);
    
    while (1)
    {
        
    }
}