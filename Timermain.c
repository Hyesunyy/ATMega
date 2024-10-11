/*
 * Test03.c
 *
 * Created: 2024-10-11 오후 2:32:25
 * Author : user
 */ 
#define F_CPU 16000000L
#define _BV(bit) (1<<(bit))
#define LED PORTG1
#define PORT PORTG
#define DDR DDRG

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int cnt=0,cnt1=0, cntEx=5, st=0; //st:상태 표시, 1이면 on 상태

ISR(TIMER0_OVF_vect)
{
	cnt++;
    if(cnt>25) // 100ms
	{
		cnt=0; //500s 지나면 초기화
		cnt1++; // 500ms 될 때까지 증가 
		if(cnt1>=cntEx)  //500ms 주기
		{
			cnt1=0; //500s 지나면 초기화
			if(st)
			{
				 PORT &= ~_BV(LED);  //켜져있으면 끄기
				 st=0; 
			}
			else
			{
				PORT |=_BV(LED);
				st =1;
			}
		}		
	}
}
ISR(INT1_vect) //slower
{
	cntEx++;
}

ISR(INT0_vect) //faster
{
	cntEx--;
	if(cntEx<1) cntEx=1;
}



int main(void)
{	
	DDR |= _BV(LED); //Gport 1번핀에 1(LED출력 설정)

	PORT |= _BV(LED);
	StandBy();
	PORTG &= ~_BV(LED); // sw1 누르면 LED off

	//timer interrupt , 점멸함수 인터럽트용
	TIMSK |= 0x01; //가장 낮은 비트로 설정, TOIE0 , 0 오버플로우 인터럽트를 활성화.
	TCCR0 |= 0x06; //분주비 256 / 0110

	//external interrupt, speed 함수 인터럽트용
	EIMSK |= 0x03;
	EICRA |= 0x0f; //rising edge
	sei();

    while (1) 
    {
	
    }
}

