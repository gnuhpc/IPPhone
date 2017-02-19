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
void delay_1us(void) //1us��ʱ���� 
{ 
asm("nop"); 
} 

void delay_nus(unsigned int n) //N us��ʱ���� 
{ 
unsigned int i=0; 
for (i=0;i<n;i++) 
delay_1us(); 
} 

void delay_1ms(void) //1ms��ʱ���� 
{ 
unsigned int i; 
for (i=0;i<1140;i++); 
} 

void delay_nms(unsigned int n) //N ms��ʱ���� 
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
  

/* �˺���ÿ��ִ�ж�����һ��ɨ���źţ���λ����һʱ��ֻ��һλΪ '0'�����������ϣ�ͬʱ��ȡ�����ϵĵ�ƽֵ���ж�ɨ���ź�Ϊ '0'�������Ƿ��м����¡�����˺�����ִ��4�κ󣬼�����һ��ɨ���û�з��ְ��������£������16������ֵ��0��15�����16����ָʾ��Чֵ�������⣬��LastNum���ϴζ����İ���ֵ��CurrentNum�汾�ζ����İ���ֵ��������ζ����İ���ֵͬ�������16 �����仰˵�������û���һ��������ã���ֻ����һ����Чֵ������һ�ΰ�����*/

/*ע���˺���һ�����ڶ�ʱ���ж��л���ѭ���У������������һ����ʱ�䣨10msΪ�ˣ���ִ��һ�Σ������𵽼���ɨ������á�*/

uint8_t Keyboard(void)   //����ɨ�躯��������һ����ֵ��16����Чֵ��
{
   static uint8_t ScanCode,TempNum, LastNum,CurrentNum,Times;
   
   switch(ScanCode)   // ɨ���ź�
   	{
   	  case 0xF7:ScanCode=0xFB; break;   //ɨ���źţ�0111��1011
      case 0xFB:ScanCode=0xFD; break;  // 1011��1101
      case 0xFD:ScanCode=0xFE; break;  // 1101��1110
      case 0xFE:ScanCode=0xF7; break;  // 1110��0111
      default: ScanCode=0xF7;break;
   	}

   /*��main()�������轫DDRC���0x0F����PC7~PC4Ϊ���룬PC3~PC0Ϊ�����
    ����PORTC=ScanCode��ζ�ţ���PC7~PC4��Ϊ���������ܵ����룬
    ��PC3~PC0���ɨ���źš�*/
   PORTA=ScanCode;

   switch(PINA)  // ������е�ƽֵ�밴��ֵ�Ĺ�ϵ
  	{ 
/*TempNum��¼�����ļ�ֵ�����ڴ˼�ֵ�辭��һ������
��������ʱ��ֵ��TempNum����Times���Լ�¼û�ж�����Ч��ֵ�Ĵ�����
��������ﵽ4�Σ���ɨ��һ�ֺ�û�з�����Ч��ֵ����
�ͱ�ʾû�м����¡�*/   	  
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
