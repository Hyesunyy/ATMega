/*
 * Test05-FND.c
 *
 * Created: 2024-10-16 오후 3:12:31
 * Author : user
 */ 
#define F_CPU 16000000L
#define CDDR      DDRB // 4개의 sement를 control하는 포트 (B)
#define IDDR      DDRA //image 생성 하는 포트 (A)
#define CPORT      PORTB 
#define IPORT      PORTA 
#define _BV(bit) (1<<(bit))


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char img[]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67};
char data[5]; //int[]여도 무관 

int digit(int num) // num 변수를 1000,100,10,1의 자릿수로 분해하여 data[]배열에 저장
{	//1234
	if(num>9999)return 0; //인수 유효성 Check
	int n1 =  num%10; //123.4 ->[4]
	int n2 = (num/10)%10; //123.4->123->12.3->[3]
	int n3 = (num/100)%10; //12.34->12->1.2->[2]
	int n4 = num/1000; //1.234->[1]
	data[3]=n1; //일
	data[2]=n2; //십
	data[1]=n3; //백
	data[0]=n4; //천
	return 1;
}

int main(void)
{   
   StandBy();
   CDDR |= 0x0f; //하위4bit(0123) 출력 설정
   IDDR |= 0xff; // 모든bit에 대하여 출력 설정
   
   CPORT |= 0x0f; // 4개의 segment에 동일한 값을 인가,모두 선택
   IPORT &= ~0xff; // [8. 8. 8. 8.] 점등, 해당 포트로 0값을 내보내야함 
   
  
	while(1)
	{	
		 for(int num=0;num<10000;num++)
		 {	
			 digit(num);
			 
			for(int i=0;i<4;i++) //segment 선택
			{
				CPORT = 1<<i;//_BV(i); //, 0001->0010->0100->1000
				/*if((i==0) &&(data[i]==0))
				{
					IPORT = 0xff;
					if((i==1)&&(data[i]==0))
					{
						IPORT = 0xff;
						if((i==2)&&(data[i]==0))
						{IPORT = 0xff;}
					}
				}
				else*/
				{IPORT = ~img[data[i]]; //[8.] 출력
				_delay_ms(5);}   //잔상 효과 이용, 4자리 유지
				
			}
		 }_delay_ms(50); 	 
	}
/* while (1) 
    {  
      for(int n=0;n<=9999;n++)
      {
         data[0]=n/1000;
         data[1]=(n-data[0]*1000)/100;
         data[2]=(n-data[0]*1000-data[1]*100)/10;
         data[3]=(n-data[0]*1000-data[1]*100-data[2]*10);
 
         for(int i=0;i<4;i++) //segment 선택
         {
            CPORT = 1<<i;//_BV(i); //, 0001->0010->0100->1000
            IPORT = ~img[data[i]]; //[8.] 출력
            _delay_ms(5);   //잔상 효과 이용, 4자리 유지
         }
      } _delay_ms(50);  
   }*/
}

