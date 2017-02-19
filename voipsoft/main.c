#include "sys.h"
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "type.h"
#include "lcd.c"
#include "ui.c"
#include "uart.c"
#include "string.c"

void ControlInitialize(void);
void onekey(char *servernum);
char online='z';
char servernum1[]={'5','9','6','4','6','5','4','3','a'};
char servernum2[]={'5','9','6','4','1','1','2','0','a'};
uint8_t i;

SIGNAL(SIG_OVERFLOW0)
{
	unsigned char number;
	
	uint8_t Keynum=16;
	TCNT0=0xE2;
	
	Keynum=Keyboard(); //读取键盘值
	/*如果键值在0~9之间，就在LCD第1行第1列显示出来。
		(Keynum+0x30)表示取键值的ASCII码。*/
	if (Keynum<10) write_data((Keynum+0x30)); 
	else {
			switch(Keynum)
			{
			case 10:write_data('*');break;
			case 11:write_data('#');break;
			case 12:Lcd_reinit();Write_String("OK");break;
			case 13:Lcd_reinit();Write_String("Redial");break;
			case 14:Lcd_reinit();Write_String("Offline");break;
			case 15:Lcd_reinit();Write_String("Online!");break;
			default: break;
			}
		}
	if(Keynum!=16)
	{
		if(Keynum!=13)
			UI_HandleKey(number=keydata[Keynum]);
		else 
		{
			onekey(servernum1);
		}
	}
}

SIGNAL(SIG_INTERRUPT0)
{
	onekey(servernum1);
}

void onekey(char *servernum)
{
	UI_HandleKey(online);
	delay_nms(10000);
	for(i=0;i<9;i++)
	{
		write_data(servernum[i]);
		UI_HandleKey(servernum[i]);
	}
}

void Lcd_reinit()
{
	clear_screen();
	Write_Position(1,1);
}

void Lcd_close()
{
	clear_screen();
	Write_Position(1,1);
	Write_String("Close!");
}

void ControlInitialize(void)
{
	DDRC=0xFF;
	PORTC=0x00;
	DDRA=0x0F;
	PORTA=0xFF;
	
	PORTD=0x00;
	DDRD=0x00;
	
    EIMSK=0x01;                            // 使用外部中断0（INT0）
    EICRA=0x03;                            // 上升沿中断请求（异步)

    MCUCR=0x00;
	UART_Init();
	
	
	Initialize_LCD();		//LCD初始化
	TCCR0=5;
	TCNT0=0xE2;
	TIMSK=(1<<TOIE0);
	sei();
		
	clear_screen();
	Write_Position(1,1);
	Write_String("Start!");
	Delay(50000);
}


int main(void)
{
	Delay(50000);
	UI_Init();
	ControlInitialize();
	Write_Position(2,1);
	do
	{
		UART_Run();
	} while (1);
}
