/*
 * Test06-PWM.c
 *
 * Created: 2024-10-23 오전 9:46:12
 * Author : user
 */ 

#define F_CPU		16000000L //long
#define _BV(bit)	(1<<(bit))
#define __DELAY_BACKWARD_COMPATIBLE__

#define LED_Y		PORTE3
#define LED_B		PORTE5
#define BUZZER		PORTE4

#define DDR		    DDRE
#define PORT		PORTE

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

volatile unsigned int period = 500;
volatile unsigned int DutyRate =50;
int CS = 8; //분주비
int cnt = 0;
int cnt_ref = 1;
int num=0;
enum { DO = 130, Do = 138, RE = 146, Re = 155, MI = 164,FA = 174,Fa = 185,SL = 196,Sl = 207,LA = 220, La= 233, SI = 246}; //음계 [주파수] , 소문자 : #

int song[] = { SL,SL,LA,LA,SL,SL,MI,  SL,SL,MI,MI,RE,  SL,SL,LA,LA,SL,SL,MI,  SL,MI,RE,MI,DO };
//int song[] ={SL,MI,DO,  RE,RE, SL,SL,  SL,SL,MI,DO,  RE,LA,SL  ,LA,LA,SI,  MI,LA,MI,DO,  MI,LA,RE};

int song1[100]; // 해당 음계에 따른 [ICR] 

void conv() //음계에 맞는 ICR 생성하여 song1에 할당
{
	for(int i=0; i<25; i++)
	{
		song1[i] = F_CPU / (long)((long) CS * (long)song[i]*4); // f=clk/(cs*ICR) -> ICR = clk/(cs*f), *4: 옥타브 올려서 소리 식별
	}
}
	
ISR(INT0_vect)
{
	//period -= 50;
	//if(period < 0)	period = 500;
	OCR3A += 10000;
}

ISR(INT1_vect)  
{
	//DutyRate += 10;//[%]
	//if(DutyRate >90)	DutyRate = 10;
	OCR3B += 20000;
}

ISR(INT2_vect) 
{
	//cnt_ref --;
	//if (cnt_ref < 0)	cnt_ref = 50;	
	OCR3C += 30000;
}

ISR(TIMER3_COMPA_vect)
{
	//if(++cnt > cnt_ref )
	//{
		//if(led_y_st) { PORT &= ~(1<<LED_Y);		led_y_st =0; }
		//else		 { PORT |= (1<<LED_Y);		led_y_st =1; }
		//cnt = 0;
	//}
	//
	//TCNT0=0;
}

ISR(TIMER3_COMPB_vect)
{	
	//cnt++;
	//if(cnt > cnt_ref)
	//{
		//cnt=0;
		//ICR3 = song1[num];
		//num++;
		//if(num>23) num = 0;
	//}
}

ISR(TIMER3_COMPC_vect)
{
	
}

int main(void)
{
	//출력 포트
	DDR  |= (1<<BUZZER)|(1<<LED_B)|(1<<LED_Y);	//7; 0x07;
	
	// 외부 인터럽트
	EIMSK = (1<<INT0)|(1<<INT1)|(1<<INT2);	//0x07;
	EICRA = 0x2A;
	
	//타이머 인터럽트
	ETIMSK  = (1<<OCIE3A)|(1<<OCIE3B)|(1<<OCIE3C);
	
	//분주비 & ref
	OCR3A = 157;
	OCR3B = 100; //15ms 점멸   
	OCR3C = 15;
	
	TCCR3B |= (1<<CS31);//|(1<<CS30); //분주비 1024 
	ICR3 = 0; 
	
	//PWM & phase
	TCCR3A |= (1<<WGM31);							 //1 0 : past pwm mode
	TCCR3A |= (1<<COM3A1)|(1<<COM3B1)|(1<<COM3C1);	//normal 
	TCCR3B |= (1<<WGM33)|(1<<WGM32);			   //1 1 : past pwm mode
	
	//인터럽트 활성화 
	sei();
	
	conv();
	
    while (1) 
    {
		cnt++;
		if(cnt > cnt_ref)
		{
			cnt=0;
			ICR3 = song1[num];
			OCR3B = song1[num] / 2;
			_delay_ms(495);
			OCR3B = 1;
			_delay_ms(10);
			num++;
			if(num>23) num = 0;
		}
    }
}

