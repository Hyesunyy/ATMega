/*
 * Test01.cpp
 *
 * Created: 2024-10-05 오후 5:26:19
 * Author : hks95
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <alloca.h>
#include <util/delay.h>

// LED 점멸 회로 
// G0 :  program start, G1:점점 빠르게 점멸 , G2 :  점점 느리게 점멸 

 int Check()
 {
	 char v1 = PING & 0x02;
	 char v2 = PING & 0x04;

	 if(v1==0) return 1;
	 if(v2==0) return 2;
	 else return 0;
 }


void delay_control(int d)
{
	for(int i=0;i<d;i++)
	{
		_delay_ms(1);
	}
}

int main(void)
{
   DDRG |= 0x10;       //G port 4번 핀 출력(LED)
   DDRG &= ~(0x01);   //G port 0번 핀 입력(G0)
   DDRG &= ~(0x02);   //G port 1번 핀 입력(G1)
   DDRG &= ~(0x04);   //G port 2번 핀 입력(G2)
   
   int d1=300;
   int d2=300;
   int mode=0; 
   
   while(1)
   {
	   char v = PING &=0x01;
	   if(v==0){ mode=1;break;}
	}   //G0 누르면 프로그램 시작, 눌리기 전까지 대기 
	
   
   if(mode)
   {

       while (1)
	   {	    int c = Check();
		   if(c==1) //G1이 눌린경우 (빠르게 점멸)
		   {
				PORTG |= 0x10; // 4번 핀에 1 입력
				delay_control(d1);
				PORTG &= ~(0x10); //0 입력
				delay_control(d1);
				if(d1>0) d1-=50; // 딜레이 감소 
				//c 값 리셋
			}
				
			
		   
		    if(c==2)// G2가 눌린경우 (느리게 점멸)
			{
			    PORTG |= 0x10; // 4번 핀에 1 입력
			    delay_control(d2);
			    PORTG &= ~(0x10); //0 입력
			    delay_control(d2);
				d2+=50; //딜레이 추가
				c = Check(); //c값 리셋
			}
		   
		}
	}
	 
	return 0;
}

