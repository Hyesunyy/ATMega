/*
 * Test06-PWM.c
 *
 * Created: 2024-10-23 오전 9:46:12
 * Author : user
 */ 

#define F_CPU		16000000L
#define _BV(bit)	(1<<(bit))
#define __DELAY_BACKWARD_COMPATIBLE__

#define LED_R		PORTE4
#define LED_B		PORTE5
#define LED_Y		PORTE3
#define DDR		    DDRE
#define PORT		PORTE

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

volatile unsigned int period = 500;
volatile unsigned int DutyRate =50;

int led_y_st = 0;
int cnt = 0, cnt_ref = 1;

ISR(INT0_vect)
{
	//period -= 50;
	//if(period < 0)	period = 500;
	OCR3A += 10000;
}

ISR(INT1_vect) //power control -> LED 밝기 조절 
{
	//DutyRate += 10;//[%]
	//if(DutyRate >90)	DutyRate = 10;
	OCR3B += 20000;
}

ISR(INT2_vect) //power control -> LED 밝기 조절
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
	
}

ISR(TIMER3_COMPC_vect)
{
	
}

int main(void)
{
	//출력 포트
	DDR  |= (1<<LED_R)|(1<<LED_B)|(1<<LED_Y);	//7; 0x07;
	

	
	// 외부 인터럽트
	EIMSK = (1<<INT0)|(1<<INT1)|(1<<INT2);	//0x07;
	EICRA = 0x2A;		//0010 1010 : 10: falling edge, or연산이 아니면 그냥 그대로 작성
	
	
	//타이머 인터럽트
	//TIMSK  = (1<<OCIE0); //0번 타이머 비교일치 인터럽트 활성화
	ETIMSK  = (1<<OCIE3A)|(1<<OCIE3B)|(1<<OCIE3C);
	//TCCR0 |= 0x07; //PSC(분주비) 1024 , 0111 
	//OCR0   = 157; //~10ms
	
	OCR3A = 50000;
	OCR3B = 30000; //15ms 점멸 
	OCR3C = 10000;
	
	//OCR3A = 19000;
	//OCR3B = 10000;
	//OCR3C = 50;

	TCCR3B |= (1<<CS32); //분주비 1024 101
	ICR3 = 60000; //30ms 주기
	//PWM & phase
	TCCR3A |= (1<<WGM31);		//1 1 : past pwm mode
	TCCR3A |= (1<<COM3A1)|(1<<COM3B1)|(1<<COM3C1);			   // 1 0 : 비반전 모드 (normal wave)
	TCCR3B |= (1<<WGM33)|(1<<WGM32);
	
	//인터럽트 활성화 
	sei();
	
    while (1) 
    {
		//duty rate=50%
		//PORT |= ((1<<LED_B)|(1<<LED_R));	 _delay_ms(period * DutyRate / 100);	//(period*(DutyRate/100))<-괄호x		
		//PORT &= ~((1<<LED_B)|(1<<LED_R));	 _delay_ms(period * (100-DutyRate) / 100);	
    }
}

