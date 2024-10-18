/*
 * GccApplication1.c
 *
 * Created: 2024-10-18 오후 2:27:05
 * Author : user
 */ 

#define _BV(bit) (1<<(bit))
#define F_CPU	 16000000L
#define CDDR	 DDRB
#define CPORT	 PORTB
#define IDDR	 DDRA
#define IPORT	 PORTA

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

unsigned char img[]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67};
unsigned char Dot[]={0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0xa7, 0xff, 0xe7};
char data[5];
char timedata[5];

int cnt=0, num=0;
int dp=1;
int sec =0, min=0;
volatile int tnum=0, timercnt=0,mode=0;

int timerdisplay_s(int t) //옵션모드로 시간 설정
{
	if(t > 9999)return 0; //인수 유효성 Check
	sec= t%100;
	timedata[2] = (sec/10)%6;
	timedata[3] = sec%10;

	return 1;
}
int timerdisplay_m(int t) //옵션모드로 시간 설정
{
	if(t > 9999)return 0; //인수 유효성 Check
	min = t/100;
	timedata[0] = (min/10)%6;
	timedata[1] = min%10;
	return 1;
}

ISR(INT1_vect) //시간  up
{
	tnum+=60;
}

ISR(INT2_vect) //시간  down
{
	tnum-=60;
}

ISR(TIMER0_OVF_vect) //시간 display
{
	
	for(int j=0;j<4;j++)
	{	for(int k = 0; k < 60 ; k++)
		{
			if(j == dp)// 점을 찍을 자리일 때
			{
				CPORT = 1 << j;
				IPORT = ~(img[timedata[j]]|0x80);
			}
			else
			{
				CPORT = 1<<j; //_BV(i);
				IPORT = ~img[timedata[j]]; //image 출력
			}
			_delay_ms(4);
		}
		
	}
}

ISR(TIMER2_COMP_vect) // 시간 세팅, 1초 ->1분
{	
	timercnt++;
	if(timercnt>7)
	{
		timercnt=0;
		tnum++;
		timerdisplay_s(tnum);
	}
	TCNT3=0;
}

int main(void)
{
    CDDR |= 0x0f;
    IDDR |= 0xff;
	
	TIMSK |= (_BV(TOIE2)|_BV(OCIE0));
	
	TCCR0 |=0x07; //1024
	TCCR2 |=0x05;
	OCR0 =22;
	
	//EIMSK |=0x07;
	//EICRA |=0x03;
	
	sei();
	
	
    while (1) 
    {
    }
}

