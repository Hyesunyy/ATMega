/*
 * StopWatch.c
 *
 * Created: 2024-10-17 오전 10:16:12
 * Author : user
 */ 

#define F_CPU 16000000L
#define CDDR      DDRB // 4개의 segment를 control하는 포트(B)
#define IDDR      DDRA //image 생성 하는 포트(A)
#define CPORT      PORTB
#define IPORT      PORTA
#define _BV(bit) (1<<(bit))


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char img[]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67};
unsigned char Dot[]={0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0xa7, 0xff, 0xe7};
char data[5];
volatile int num = 0;
volatile int cnt=0;


int mode =0; //0:Ready, 1:up_count, 2:Stop
int digit(int n) // num 변수를 1000,100,10,1의 자릿수로 분해하여 data[]배열에 저장
{	//1234
	if(n > 9999)return 0; //인수 유효성 Check
	
	 data[3] =  n%10; //123.4 ->[4]뒤2
	 data[2] = (n/10)%10; //123.4->123->12.3->[3]뒤1
	 data[1] = (n/100)%10; //12.34->12->1.2->[2]앞2
	 data[0] = n/1000; //1.234->[1] 앞1
	return 1;
}






//stop_sw1 

ISR(INT0_vect)
{
	mode++;
	if(mode>2)mode=0; //0->1->2->0	
	
}

ISR(TIMER2_OVF_vect) //FND refresh
{
	
	 for(int i = 0; i < 4; i++) //segment 선택
	 {
		 if(i == 1)
			{	
			 CPORT = 1 << i;
			 IPORT = ~Dot[data[i]];
			}
		 else
			 {
			CPORT = 1<<i; //_BV(i);
			IPORT = ~img[data[i]]; //image 출력 
			 } 
		_delay_ms(4); 
	}
}

ISR(TIMER0_COMP_vect) //FND refresh
{
	cnt++;
	if(cnt>7)
	{	
		cnt=0;
		num++;
		digit(num);
	}
	TCNT0=0;
	
	
}


int main(void)
{
	CDDR |= 0x0f; //하위4bit(0123) 출력 설정(segment 선택부, control)
	IDDR |= 0xff; // 모든bit에 대하여 출력 설정 ( led 선택부, image)

	//CPORT = 0x0f; //_BV(i);
	//IPORT = 0xff;
	//StandBy();
	//IPORT = ~0xff;

	//timer2 : FND refresh
	TIMSK |= ((1 << TOIE2) | (1 << OCIE0)); // x1xx xxxx, timer2의  ovf bit
	TCCR2 |=((1 << CS20) | (1 <<CS22)); //분주비 101: 1024//0x05
	
	//Timer0 : 100ms stop-watch
	TCCR0 |= ((1 << CS00) | (1 <<CS01)| (1 <<CS02)); //분주비 1024 / 0111, timer 0
	OCR0 = 22; //1초 만들기 위해 조절 , Tcnt=0; 필요/ 1/16M*22*1024*7=1 
	
  	
    EIMSK |= 0x01; 
    EICRA |= 0x03; 
	
	sei();

    while (1) 
    {
		switch(mode)
		{
			case 0://ready/ 0000
				num=0;
				digit(num);
				break;
				
			case 1: // up
				//cnt++;
				break;
				
			case 2: //stop
				cnt=0;
				break;
				
			default:
				break;
		}
		
	
	}
}

