#include "../pins.h"


void PIN_MANAGER_Initialize(void)
{
   /**
    LATx registers
    */
    LATA = 0x0;
    LATB = 0x0;
    LATC = 0x0;
    LATD = 0x0;
    LATE = 0x0;
    /**
    ODx registers
    */

    /**
    TRISx registers
    */
    TRISA = 0xFE;
    TRISB = 0xFC;
    TRISC = 0xBF;
    TRISD = 0xFF;
    TRISE = 0x7;

    /**
    ANSELx registers
    */
    /*ANSELH = 0x1F;

    /**
    WPUx registers
    */
    //WPUB = 0xFF;


    /**
    SLRCONx registers
    */

    /**
    INLVLx registers
    */

   /**
    RxyI2C | RxyFEAT registers   
    */
    /**
    PPS registers
    */

   /**
    IOCx registers 
    */
    IOCB = 0x0;


    // Enable INTCONbits.RBIE interrupt 
    INTCONbits.RBIE = 1; 
}
  
void PIN_MANAGER_IOC(void)
{
}
/**
 End of File
*/