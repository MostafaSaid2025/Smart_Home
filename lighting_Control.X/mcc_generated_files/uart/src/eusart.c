/**
  Section: Included Files
*/
#include "../eusart.h"

/**
  Section: Macro Declarations
*/

#define EUSART_TX_BUFFER_SIZE (8U) //buffer size should be 2^n
#define EUSART_TX_BUFFER_MASK (EUSART_TX_BUFFER_SIZE - 1U) 

#define EUSART_RX_BUFFER_SIZE (8U) //buffer size should be 2^n
#define EUSART_RX_BUFFER_MASK (EUSART_RX_BUFFER_SIZE - 1U)

/**
  Section: Driver Interface
 */

const uart_drv_interface_t UART1 = {
    .Initialize = &EUSART_Initialize,
    .Deinitialize = &EUSART_Deinitialize,
    .Read = &EUSART_Read,
    .Write = &EUSART_Write,
    .IsRxReady = &EUSART_IsRxReady,
    .IsTxReady = &EUSART_IsTxReady,
    .IsTxDone = &EUSART_IsTxDone,
    .TransmitEnable = &EUSART_TransmitEnable,
    .TransmitDisable = &EUSART_TransmitDisable,
    .AutoBaudSet = &EUSART_AutoBaudSet,
    .AutoBaudQuery = &EUSART_AutoBaudQuery,
    .BRGCountSet = NULL,
    .BRGCountGet = NULL,
    .BaudRateSet = NULL,
    .BaudRateGet = NULL,
    .AutoBaudEventEnableGet = NULL,
    .ErrorGet = &EUSART_ErrorGet,
    .TxCompleteCallbackRegister = &EUSART_TxCompleteCallbackRegister,
    .RxCompleteCallbackRegister = &EUSART_RxCompleteCallbackRegister,
    .TxCollisionCallbackRegister = NULL,
    .FramingErrorCallbackRegister = &EUSART_FramingErrorCallbackRegister,
    .OverrunErrorCallbackRegister = &EUSART_OverrunErrorCallbackRegister,
    .ParityErrorCallbackRegister = NULL,
    .EventCallbackRegister = NULL,
};

/**
  Section: EUSART variables
*/
static volatile uint8_t eusartTxHead = 0;
static volatile uint8_t eusartTxTail = 0;
static volatile uint8_t eusartTxBuffer[EUSART_TX_BUFFER_SIZE];
static volatile uint8_t eusartTxBufferRemaining;

static volatile uint8_t eusartRxHead = 0;
static volatile uint8_t eusartRxTail = 0;
static volatile uint8_t eusartRxBuffer[EUSART_RX_BUFFER_SIZE];
static volatile eusart_status_t eusartRxStatusBuffer[EUSART_RX_BUFFER_SIZE];
static volatile uint8_t eusartRxCount;

static volatile eusart_status_t eusartRxLastError;

/**
  Section: EUSART APIs
*/

void (*EUSART_TxInterruptHandler)(void);
static void (*EUSART_TxCompleteInterruptHandler)(void) = NULL;

void (*EUSART_RxInterruptHandler)(void);
static void (*EUSART_RxCompleteInterruptHandler)(void) = LED_Automatic_Control;

static void (*EUSART_FramingErrorHandler)(void) = NULL;
static void (*EUSART_OverrunErrorHandler)(void) = NULL;

static void EUSART_DefaultFramingErrorCallback(void);
static void EUSART_DefaultOverrunErrorCallback(void);

void EUSART_TransmitISR (void);
void EUSART_ReceiveISR(void);


/**
  Section: EUSART  APIs
*/

void EUSART_Initialize(void)
{
    PIE1bits.RC1IE = 0;   
     EUSART_RxInterruptHandler = EUSART_ReceiveISR;   

    PIE1bits.TX1IE = 0; 
    EUSART_TxInterruptHandler = EUSART_TransmitISR; 

    // Set the EUSART module to the options selected in the user interface.

    //ABDEN disabled; WUE disabled; BRG16 16bit_generator; ABDOVF no_overflow; CKTXP async_noninverted_sync_fallingedge; RXDTP not_inverted; 
    BAUDCON = 0x48; 
    //ADDEN disabled; CREN enabled; SREN disabled; RX9 8-bit; SPEN enabled; 
    RCSTA = 0x90; 
    //TX9D 0x0; BRGH hi_speed; SENDB sync_break_complete; SYNC asynchronous; TXEN enabled; TX9 8-bit; CSRC client_mode; 
    TXSTA = 0x26; 
    //SPBRG 160; 
    SPBRG = 0xA0; 
    //SPBRGH 1; 
    SPBRGH = 0x1; 

    EUSART_FramingErrorCallbackRegister(EUSART_DefaultFramingErrorCallback);
    EUSART_OverrunErrorCallbackRegister(EUSART_DefaultOverrunErrorCallback);
    eusartRxLastError.status = 0;  

    eusartTxHead = 0;
    eusartTxTail = 0;
    eusartTxBufferRemaining = sizeof(eusartTxBuffer);

    eusartRxHead = 0;
    eusartRxTail = 0;
    eusartRxCount = 0;

    PIE1bits.RC1IE = 1; 
}

void EUSART_Deinitialize(void)
{
    PIE1bits.RC1IE = 0;    
    PIE1bits.TX1IE = 0; 
    BAUDCON = 0x00;
    RCSTA = 0x00;
    TXSTA = 0x00;
    SPBRG = 0x00;
    SPBRGH = 0x00;
}

void EUSART_Enable(void)
{
    RCSTAbits.SPEN = 1;

}

void EUSART_Disable(void)
{
    RCSTAbits.SPEN = 0;
}


void EUSART_TransmitEnable(void)
{
    TXSTAbits.TXEN = 1;
}

void EUSART_TransmitDisable(void)
{
    TXSTAbits.TXEN = 0;
}

void EUSART_ReceiveEnable(void)
{
    RCSTAbits.CREN = 1;
}

void EUSART_ReceiveDisable(void)
{
    RCSTAbits.CREN = 0;
}

void EUSART_SendBreakControlEnable(void)
{
    TXSTAbits.SENDB = 1;
}

void EUSART_SendBreakControlDisable(void)
{
    TXSTAbits.SENDB = 0;
}

void EUSART_AutoBaudSet(bool enable)
{
    if(enable)
    {
        BAUDCONbits.ABDEN = 1;
    }
    else
    {
       BAUDCONbits.ABDEN = 0; 
    }
}

bool EUSART_AutoBaudQuery(void)
{
return (bool)(!BAUDCONbits.ABDEN);
}

bool EUSART_IsAutoBaudDetectOverflow(void)
{
    return (bool)BAUDCONbits.ABDOVF; 
}

void EUSART_AutoBaudDetectOverflowReset(void)
{
    BAUDCONbits.ABDOVF = 0; 
}

void EUSART_TransmitInterruptEnable(void)
{
    PIE1bits.TX1IE = 1;

}

void EUSART_TransmitInterruptDisable(void)
{ 
    PIE1bits.TX1IE = 0; 
}

void EUSART_ReceiveInterruptEnable(void)
{
    PIE1bits.RC1IE = 1;
}
void EUSART_ReceiveInterruptDisable(void)
{
    PIE1bits.RC1IE = 0; 
}

bool EUSART_IsRxReady(void)
{
    return (eusartRxCount ? true : false);
}

bool EUSART_IsTxReady(void)
{
    return (eusartTxBufferRemaining ? true : false);
}

bool EUSART_IsTxDone(void)
{
    return TXSTAbits.TRMT;
}

size_t EUSART_ErrorGet(void)
{
    eusartRxLastError.status = eusartRxStatusBuffer[(eusartRxTail + 1U) & EUSART_RX_BUFFER_MASK].status;
    return eusartRxLastError.status;
}

uint8_t EUSART_Read(void)
{
    uint8_t readValue  = 0;
    uint8_t tempRxTail;
    
    readValue = eusartRxBuffer[eusartRxTail];

    tempRxTail = (eusartRxTail + 1U) & EUSART_RX_BUFFER_MASK; // Buffer size of RX should be in the 2^n
    
    eusartRxTail = tempRxTail;

    eusartRxLastError = eusartRxStatusBuffer[eusartRxTail];
    

    PIE1bits.RC1IE = 0; 
    if(0U != eusartRxCount)
    {
        eusartRxCount--;
    }
    PIE1bits.RC1IE = 1;
    return readValue;
}

void EUSART_ReceiveISR(void)
{
    uint8_t regValue;
	uint8_t tempRxHead;

    // use this default receive interrupt handler code
    eusartRxStatusBuffer[eusartRxHead].status = 0;

    if(true == RCSTAbits.OERR)
    {
        eusartRxStatusBuffer[eusartRxHead].oerr = 1;
        if(NULL != EUSART_OverrunErrorHandler)
        {
            EUSART_OverrunErrorHandler();
        }   
    }   
    if(true == RCSTAbits.FERR)
    {
        eusartRxStatusBuffer[eusartRxHead].ferr = 1;
        if(NULL != EUSART_FramingErrorHandler)
        {
            EUSART_FramingErrorHandler();
        }   
    } 
    
    regValue = RCREG;
    
    tempRxHead = (eusartRxHead + 1U) & EUSART_RX_BUFFER_MASK;// Buffer size of RX should be in the 2^n
    if (tempRxHead == eusartRxTail) 
    {
		// ERROR! Receive buffer overflow 
	} 
    else
    {
        eusartRxBuffer[eusartRxHead] = regValue;
		eusartRxHead = tempRxHead;
		eusartRxCount++;
	}   

    if(NULL != EUSART_RxCompleteInterruptHandler)
    {
        (*EUSART_RxCompleteInterruptHandler)();
    } 
}

void EUSART_Write(uint8_t txData)
{
    uint8_t tempTxHead;
    
    if(0 == PIE1bits.TX1IE)
    {
        TXREG = txData;
    }
    else if(0U < eusartTxBufferRemaining) // check if at least one byte place is available in TX buffer
    {
       eusartTxBuffer[eusartTxHead] = txData;
       tempTxHead = (eusartTxHead + 1U) & EUSART_TX_BUFFER_MASK;
       
       eusartTxHead = tempTxHead;
       PIE1bits.TX1IE = 0; //Critical value decrement
       eusartTxBufferRemaining--; // one less byte remaining in TX buffer
    }
    else
    {
        //overflow condition; eusartTxBufferRemaining is 0 means TX buffer is full
    }
    PIE1bits.TX1IE = 1;
}

void EUSART_TransmitISR(void)
{
    uint8_t tempTxTail;
    // use this default transmit interrupt handler code
    if(sizeof(eusartTxBuffer) > eusartTxBufferRemaining) // check if all data is transmitted
    {
       TXREG = eusartTxBuffer[eusartTxTail];
       tempTxTail = (eusartTxTail + 1U) & EUSART_TX_BUFFER_MASK;
       
       eusartTxTail = tempTxTail;
       eusartTxBufferRemaining++; // one byte sent, so 1 more byte place is available in TX buffer
    }
    else
    {
        PIE1bits.TX1IE = 0;
    }

    if(NULL != EUSART_TxCompleteInterruptHandler)
    {
        (*EUSART_TxCompleteInterruptHandler)();
    }
}

static void EUSART_DefaultFramingErrorCallback(void)
{
    
}

static void EUSART_DefaultOverrunErrorCallback(void)
{
    //Continuous Receive must be cleared to clear Overrun Error else Rx will not receive upcoming bytes
    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
}

void EUSART_FramingErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        EUSART_FramingErrorHandler = callbackHandler;
    }
}

void EUSART_OverrunErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        EUSART_OverrunErrorHandler = callbackHandler;
    }    
}

void EUSART_RxCompleteCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
       EUSART_RxCompleteInterruptHandler = callbackHandler; 
    }   
}

void EUSART_TxCompleteCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
       EUSART_TxCompleteInterruptHandler = callbackHandler;
    }   
}

