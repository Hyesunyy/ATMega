/**
 * \file
 */
#define F_CPU 16000000L
#include <asf.h>
#include <avr/io.h>
#define sw1 PORTG0 // (1<< 0)1�� 0�� shift 0001
#define sw2 PORTG1 // 1�� shift 0010
#define sw3 PORTG2 
#define LED1 PORTG4
#define _BV(bit) (1<<(bit))


int main (void)
{	
	DDRG = 0x10; // G port ���� 4bit:�Է�,���� 1bit :���
/*
DDRG |= 0x10; // xxx1 xxxx
DDRG &= 0xF8; // xxxx x000 ==> xxxx x111(!���� ~), �Ʒ��� ���� �ۼ� ����
DDRG &= ~0x07; //���� 3bit �Է�
*/

	PORTG = _BV(LED1);// ���۵Ǹ� LED on, PORTG=(1<<LED1),PORTG = 0x10; �� ����, 1�� 4�� shift 1 0000
	while(1)
	{
		if((PING & _BV(sw1))== 0) //Reg������ �ۼ�(eg. PING0 ==0 : x), PING & 0x01 /PING & (1<<sw1) �� ���� ǥ��
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