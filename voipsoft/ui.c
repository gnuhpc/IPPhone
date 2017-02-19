#include "type.h"
#include "sys.h"

#define MODE_ZERO		0
#define MODE_COMMAND	1
#include "type.h"
#include "sys.h"

#define MODE_DATA		2

#define COMMAND_LEN		4
#define MAX_DATA_LEN	24

DCHAR _iMode, _iCommandPos, _iDataPos;
DCHAR _pCommand[COMMAND_LEN];
DCHAR _pData[MAX_DATA_LEN + 1];

unsigned char _pKey[] = "KEY ";

void UI_Init(void)
{
	_iMode = MODE_ZERO;
	_iCommandPos = _iDataPos = 0;
}

BOOLEAN _isprint(UCHAR iVal) 
{
	return (iVal >= 0x20 && iVal < 0x7f) ? TRUE : FALSE;
}

void _Padding(void)
{
	DCHAR i;

	for (i = 0; i < 4; i ++)
	{
		UART_SendChar(0);
	}
}

void _SendCommand(unsigned char* pCommand)
{
	DCHAR i;

	for (i = 0; i < COMMAND_LEN; i ++)
	{
		UART_SendChar(pCommand[i]);
	}
}

void _SendData(PDCHAR pData)
{
	DCHAR i;

	for (i = 0; i < MAX_DATA_LEN; i ++)
	{
		UART_SendChar(pData[i]);
		if (!pData[i])
		{
			break;
		}
	}
}

BOOLEAN _CompareCommand(PCCHAR p)
{
	return memcmp_c2d(_pCommand, p, COMMAND_LEN) ? FALSE : TRUE;
}

void UI_HandleSerialChar(UCHAR iVal)
{
	// Analyze it!
	if (_iMode == MODE_COMMAND)
	{
		if (_iCommandPos < COMMAND_LEN)
		{
			if (_isprint(iVal))
			{
				_pCommand[_iCommandPos] = iVal;
				_iCommandPos ++;
			}
			else
			{
				_iMode = MODE_ZERO;
			}
		}
		else
		{
			if (iVal)
			{
				_iMode = MODE_DATA;
				_pData[0] = iVal;
				_iDataPos = 1;
			}
			else
			{
				_iMode = MODE_ZERO;
			}
		}
	}
	else if (_iMode == MODE_DATA)
	{
		if (iVal)
		{
			if (_iDataPos < MAX_DATA_LEN)
			{
				_pData[_iDataPos] = iVal;
				_iDataPos ++;
			}
		}
		else
		{
			_pData[_iDataPos] = 0;
			_iMode = MODE_ZERO;
		}
	}
	else	// MODE_ZERO
	{
		if (_isprint(iVal))
		{
			_iMode = MODE_COMMAND;
			_pCommand[0] = iVal;
			_iCommandPos = 1;
		}
	}
}

void UI_HandleKey(UCHAR iKey)
{
	DCHAR pBuf[2];

	if (!iKey)	return;

	pBuf[0] = iKey;
	pBuf[1] = 0;

	_SendCommand(_pKey);
	_SendData(pBuf);
	_Padding();
}
