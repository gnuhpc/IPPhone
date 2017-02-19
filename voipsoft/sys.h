#include <inttypes.h>
#include "type.h"

// Lcd.c
#define LCDPORT   PORTC
#define LCDDDR    DDRC
#define KEYDDR    DDRA
#define KEYPORT   PORTA
#define delaytda  Delay(10)
#define delayte   Delay(20)
#define delaytfun Delay(1000)


#define PARITY_NO	0x00
#define STOP_1 0x00
#define DATA_8 0x06

#define SYSTEM_CLOCK 1000000L
#define BAUD_RATE 19200

#define UART_RX_BUFFER_SIZE 128
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE-1)
#if (UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
#error RX buffer size is not a power of 2
#endif

#define UART_TX_BUFFER_SIZE 128
#define UART_TX_BUFFER_MASK (UART_TX_BUFFER_SIZE-1)
#if (UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK)
#error TX buffer size is not a power of 2
#endif

volatile static unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
volatile static unsigned char UART_RxHead;
volatile static unsigned char UART_RxTail;

volatile static unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
volatile static unsigned char UART_TxHead;
volatile static unsigned char UART_TxTail;

void Delay(unsigned long int time);

/*lcd/key*/
void En_Toggle(void);
void nibble_mode(void);
void Write_Command(uint8_t Command);
void Write_Position(uint8_t row,uint8_t colum);
void write_data(uint8_t Data);
void Write_String(uint8_t *s);
void clear_screen(void);
void Initialize_LCD(void);
void Lcd_reinit(void);
void Lcd_close(void);

uint8_t Keyboard(void);

// String.c
void memcpy_c2d(PDCHAR pDst, PCCHAR pSrc, UCHAR iCount);
UCHAR memcmp_c2d(PDCHAR pDst, PCCHAR pSrc, UCHAR iCount);
void memset_d(PDCHAR pDst, UCHAR iVal, UCHAR iCount);

// UART.c
void UART_Init(void);
void UART_Run(void);
void UART_SendChar(UCHAR data);

// UI.c
void UI_Init(void);
void UI_HandleKey(UCHAR iKey);
void UI_HandleSerialChar(UCHAR iVal);
