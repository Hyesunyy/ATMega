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
char timedata[5];
volatile int tnum,tick,sTick = 0; //sTick: mode=2일 때 버튼이 눌려질 당시의 tick값 저장
volatile int cnt=0;
volatile int mode =0; //0:Ready, 1:up_count, 2:Stop
int dp=1; 
volatile int timercnt=0;

int digit(int n) // num 변수를 1000,100,10,1의 자릿수로 분해하여 data[]배열에 저장
{	//1234
	if(n > 9999)return 0; //인수 유효성 Check
	
	 data[3] =  n%10; //123.4 ->[4]뒤2
	 data[2] = (n/10)%10; //123.4->123->12.3->[3]뒤1
	 data[1] = (n/100)%10; //12.34->12->1.2->[2]앞2
	 data[0] = n/1000; //1.234->[1] 앞1
	return 1;
}

int timerdisplay(int t) //옵션모드로 시간 설정
{
	if(t > 9999)return 0; //인수 유효성 Check
	
	timedata[0] =  (t/600)%6;
	timedata[1] =  (t/60)%10;
	timedata[2] =  (t/6)%6;
	timedata[3] =  t%10;
	return 1;
}

ISR(INT0_vect)
{
	//mode++;
	//if(mode)tick=0;
	//else if(mode==2)sTick = tick;
	//else if(mode>2) {mode = tick = sTick = 0;} //0->1->2->0
	switch(++mode)
	{
		case 0:
			break;
		case 1: //ready
			tick=0;
			break;
		case 2:
			//digit(tick);
			break;
		case 3:
			sTick=tick;
			break;
		default:
			tick=0;
			sTick=0;
			mode=0;
			break;
	}
	
}

//ISR(INT0_vect)
//{
	//mode++;
	//if(mode>2)mode=0;
//}


ISR(TIMER2_OVF_vect) //FND refresh
{
	
	 for(int i = 0; i < 4; i++) //segment 선택
	 {
		 if(i == dp)// 점을 찍을 자리일 때 
			{	
			 CPORT = 1 << i;
			 IPORT = ~(img[data[i]]|0x80);
			 //IPORT = ~Dot[data[i]];
			}
		 else
			 {
			CPORT = 1<<i; //_BV(i);
			IPORT = ~img[data[i]]; //image 출력 
			 } 
		_delay_ms(4); 
	}
}

//ISR(TIMER0_COMP_vect) //up-count
//{
	//cnt++;
	//if(cnt>7)
	//{	
		//cnt=0;
		//num++;
		//digit(num);
	//}
	//TCNT0=0;
//}
ISR(TIMER0_COMP_vect)
{
	tick++;
	TCNT0=0;
}

////////시계//////
ISR(INT1_vect) //시간  up
{
	tnum+=60;
}

ISR(INT2_vect) //시간  down
{
	tnum-=60;
}

ISR(TIMER1_OVF_vect) //시간 display
{
	for(int j=0;j<4;j++)
	{
		if(j == dp)// 점을 찍을 자리일 때
		{
			CPORT = 1 << j;
			IPORT = ~(img[timedata[j]]|0x80);
			//IPORT = ~Dot[data[i]];
		}
		else
		{
			CPORT = 1<<j; //_BV(i);
			IPORT = ~img[timedata[j]]; //image 출력
		}
		_delay_ms(5);
	}
}
ISR(TIMER3_COMPA_vect) // 시간 세팅, 1초 ->1분
{
	timercnt++;
	if(timercnt>420)
	{
		timercnt=0;
		tnum++;
		timerdisplay(tnum);
	}
	TCNT3=0;
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
	//TCCR2 |= 0x04;
	//Timer0 : 100ms stop-watch
	//TCCR0 |= ((1 << CS00) | (1 <<CS01)| (1 <<CS02)); //분주비 1024 / 0111, timer 0
	TCCR0 |=0x04;
	OCR0 = 142; //1초 만들기 위해 조절 , Tcnt=0; 필요/ 1/16M*22*1024*7=1 
	//OCR0 = 84;//실제 10ms동작, 계산값이 정확하지 않음, 계속 동작 시 느려짐, 안 되면 분주비 줄여보기 
  	
    EIMSK |= 0x01; 
    EICRA |= 0x03; 
	
	// 시계모드
	EIMSK |=0x06;
	
	TIMSK  |= ((1 << TOIE1) | (1 << OCIE3A)); //1024
	TCCR1B |= ((1 << CS10)| (1 <<CS12)); //1024
	TCCR3B |= 1 << CS32;//1024
	OCR3A = 22;
	
	sei();

    while (1) 
    {
		switch(mode)
		{
			case 0:
				//timerdisplay(6666);
				timerdisplay(6666);
			
			case 1://ready/ 0000
				//num=0;
				//digit(num);
				 digit(0);
				break;
				
			case 2: // up
				digit(tick);
				break;
				
			case 3: //stop
				//cnt=0;
				digit(sTick);
				break;
				
			default: //시계 
				
				break;
		}
	}
}

