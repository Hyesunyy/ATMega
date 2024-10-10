/**
 * \file
 */
#define F_CPU 16000000L
#include <asf.h>
#include <avr/io.h>
#define sw1 PORTG0 // (1<< 0)1을 0번 shift 0001
#define sw2 PORTG1 // 1번 shift 0010
#define sw3 PORTG2 
#define LED1 PORTG4
#define _BV(bit) (1<<(bit))


int main (void)
{	
	DDRG = 0x10; // G port 하위 4bit:입력,상위 1bit :출력
/*
DDRG |= 0x10; // xxx1 xxxx
DDRG &= 0xF8; // xxxx x000 ==> xxxx x111(!연산 ~), 아래와 같이 작성 가능
DDRG &= ~0x07; //하위 3bit 입력
*/

	PORTG = _BV(LED1);// 시작되면 LED on, PORTG=(1<<LED1),PORTG = 0x10; 과 같음, 1을 4번 shift 1 0000
	while(1)
	{
		if((PING & _BV(sw1))== 0) //Reg단위로 작성(eg. PING0 ==0 : x), PING & 0x01 /PING & (1<<sw1) 와 같은 표현
		{
			PORTG &= ~_BV(LED1); //LED off
			break;
		}
	}

	
	while(1)
	{
		
		
		
	}
	
	
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   7 