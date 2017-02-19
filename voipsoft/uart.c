#include "type.h"
#include "sys.h"

#define UART_BUF_LEN	48

volatile DCHAR _iInputHead, _iInputTail;
volatile ICHAR _pInputBuf[UART_BUF_LEN];


void UART_SendChar(UCHAR data)
{
	UDR0	=	data;
	while(!(UCSR0A & (1<<5)));
}

void UART_Init(void)
{

	 UBRR0H=0;
	 UBRR0L=6;
	 UCSR0B=(1<<TXEN);	
	 UCSR0C=DATA_8;
}

void UART_Run(void)
{
	DCHAR iTail;

	iTail = _iInputTail + 1;
	if (iTail == UART_BUF_LEN)
	{
		iTail = 0;
	}

	while (iTail != _iInputHead)
	{
		UI_HandleSerialChar(_pInputBuf[iTail]);

		_iInputTail = iTail;
		iTail ++;
		if (iTail == UART_BUF_LEN)
		{
			iTail = 0;
		}
	}
}

