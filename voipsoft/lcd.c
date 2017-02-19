#include "type.h"
#include "sys.h"


unsigned char keydata[16]=
{ 
	'0','1','2','3',
	'4','5','6','7',
	'8','9','*','#',
	'a','X','Z','z',
};

void Delay(uint32_t time)
{
 while(time>0)
    {time--;}
}
void delay_1us(void) //1us延时函数 
{ 
asm("nop"); 
} 

void delay_nus(unsigned int n) //N us延时函数 
{ 
unsigned int i=0; 
for (i=0;i<n;i++) 
delay_1us(); 
} 

void delay_1ms(void) //1ms延时函数 
{ 
unsigned int i; 
for (i=0;i<1140;i++); 
} 

void delay_nms(unsigned int n) //N ms延时函数 
{ 
unsigned int i=0; 
for (i=0;i<n;i++) 
delay_1ms(); 
}

void En_Toggle(void)
{
 delaytda;
 sbi(LCDPORT,3);
 delayte;
 cbi(LCDPORT,3);
 delayte;
}

void nibble_mode(void)
{
LCDPORT=0x39;
LCDDDR=0xff;
En_Toggle();
delaytfun;
En_Toggle();
delaytfun;
En_Toggle();
delaytfun;

LCDPORT=0x21;
En_Toggle();
delaytfun;
}

void Write_Command(uint8_t Command)
{
uint8_t Commandh,Commandl;
Commandh=Command&0xf0;
Commandl=Command&0x0f;
LCDDDR=0xff;
LCDPORT=(Commandh|0x01);
En_Toggle();
LCDPORT=((Commandl<<4)|0x01);
En_Toggle();
delaytfun;
}

void Write_Position(uint8_t row,uint8_t colum)
 {
  uint8_t p;
  if(row==1)
    {
	p=0x80+colum-1;
	Write_Command(p);
	}
  else
  {p=0xc0+colum-1;
  Write_Command(p);
  }
 }
 
 void write_data(uint8_t Data)
{
  LCDDDR=0xff;
  LCDPORT=((Data&0xf0)|0x05);
  En_Toggle();
  LCDPORT=((Data&0x0f)<<4|0x05);
  En_Toggle();
  delaytfun;
}

void Write_String(uint8_t *s)
 {
   for(;*s!='\0';s++)write_data(*s);
 }
 
 
 void clear_screen(void)
 {
 Write_Command(0x01);
 Delay(10000);
 }
 
 void Initialize_LCD(void)
 {
  nibble_mode();
  Write_Command(0X28);
  clear_screen();
  Write_Command(0X06);
  Write_Command(0X0c);
  }
  

/* 此函数每次执行都产生一个扫描信号（四位，任一时刻只有一位为 '0'）加于列线上，同时读取行线上的电平值以判断扫描信号为 '0'的列上是否有键按下。如果此函数被执行4次后，即经过一轮扫描后，没有发现按键被按下，则输出16（键盘值从0到15，因此16用以指示无效值）。另外，用LastNum存上次读到的按键值，CurrentNum存本次读到的按键值，如果两次读到的按键值同，亦输出16 。换句话说，无论用户按一个键按多久，都只返回一个有效值，当成一次按键。*/

/*注：此函数一般用于定时器中断中或主循环中，即本函数需隔一定的时间（10ms为宜）被执行一次，才能起到键盘扫描的作用。*/

uint8_t Keyboard(void)   //键盘扫描函数，返回一个键值或16（无效值）
{
   static uint8_t ScanCode,TempNum, LastNum,CurrentNum,Times;
   
   switch(ScanCode)   // 扫描信号
   	{
   	  case 0xF7:ScanCode=0xFB; break;   //扫描信号：0111→1011
      case 0xFB:ScanCode=0xFD; break;  // 1011→1101
      case 0xFD:ScanCode=0xFE; break;  // 1101→1110
      case 0xFE:ScanCode=0xF7; break;  // 1110→0111
      default: ScanCode=0xF7;break;
   	}

   /*在main()函数里需将DDRC设成0x0F，即PC7~PC4为输入，PC3~PC0为输出。
    下面PORTC=ScanCode意味着，将PC7~PC4设为带上拉功能的输入，
    而PC3~PC0输出扫描信号。*/
   PORTA=ScanCode;

   switch(PINA)  // 解读行列电平值与按键值的关系
  	{ 
/*TempNum记录读到的键值，由于此键值需经进一步处理，
所以是暂时数值（TempNum）。Times用以记录没有读到有效键值的次数，
如果次数达到4次（即扫描一轮后没有发现有效键值），
就表示没有键按下。*/   	  
	  case 0x77:TempNum=1;Times=0;break; //
      case 0xEE:TempNum=15;Times=0;break; //11101110
	  case 0xED:TempNum=14;Times=0;break; //11101101
	  case 0xEB:TempNum=13;Times=0;break; //11101010
	  case 0x7E:TempNum=10;Times=0;break; 
	  case 0xDE:TempNum=11;Times=0;break;
	  case 0xDD:TempNum=9;Times=0;break;
	  case 0xDB:TempNum=6;Times=0;break;
	  case 0xD7:TempNum=3;Times=0;break;
	  case 0xBE:TempNum=0;Times=0;break;
	  case 0xBB:TempNum=5;Times=0;break;
	  case 0xB7:TempNum=2;Times=0;break;
	  case 0xE7:TempNum=12;Times=0;break;
	  case 0x7D:TempNum=7;Times=0;break;
	  case 0x7B:TempNum=4;Times=0;break;
	  case 0xBD:TempNum=8;Times=0;break;
      default:
			   Times++;
			   if(Times >= 4)
			   {
				Times = 0;
				TempNum=16;
			   }

	}
	LastNum = CurrentNum;
	CurrentNum = TempNum;
	if(CurrentNum == LastNum)
		return 16;
	else
		return CurrentNum;

}
