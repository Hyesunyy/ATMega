/*
 * Test07-UART.c
 *
 * Created: 2024-10-25 오전 9:41:39
 * Author : user
 */ 
#define		F_CPU		16000000L	//long
#define		__DELAY_BACKWARD_COMPATIBLE__

#include	<avr/io.h>
#include	<avr/interrupt.h>
#include	<avr/delay.h>

//**[UART Resister setting - (1)] 9600 N 8 1 (Baud rate , No parity , 8bit , Stop : 1 )	
void UART0_init()
{
	//Baud Rate
	UCSR0A |= (1<<U2X0);	//status reg A, 2배속 활성화 비트
	UBRR0H = 0;				//9600 BR의 2배속 값 (상위 4bit)
	UBRR0L = 207;		    //9600 BR의 2배속 값 (하위 8bit)
	
	//Parity
	UCSR0C &= ~ ((1<<UPM01)|(1<<UPM00));	//None parity 0 0 -> 생략 가능
	
	//Data bit -8bit : 0 1 1
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	//Stop Bit
	UCSR0C &= ~(1<<USBS0);
	
	//UART Open : RX / Tx Enable 
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

//버퍼가 비워질 때까지 대기 후 단일 문자 하나를 받아 출력
void UART0_putc(char c)
{
	while(1)
	{
		if(UCSR0A & (1<<UDRE0)) break; // bit check 후 1이면 대기 종료
	}
	UDR0 = c; // send 
}

//문자열 하나를 받아 출력
void UART0_puts(char *str)
{
	
	while(*str) UART0_putc(*str++); //str 의 null에 도달할 때까지 문자열을 한 글자씩 전송
	
}

char buf[256];
char *str = "Button Pushed!!!\r\n";
int n = 0;

ISR(INT0_vect)
{
	sprintf(buf, "Button Pushed %d times.\r\n",n++); //buf 에 문자열 담기
	UART0_puts(buf); //문자열 전송
}


int main(void)
{
    UART0_init();
	UART0_puts("\033[2J");		// scren clear - ESC[2J
	UART0_puts("\033[0;0H");		// move cursor to LeftTop - ESC[0;0H
	
	//외부 인터럽트
	EIMSK |= (1<<INT0);
	EICRA = 0x02; // falling edge 
	
	//활성화
	sei();
	
    while (1) 
    {
		
    }
}

