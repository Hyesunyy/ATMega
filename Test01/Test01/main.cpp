/*
 * Test01.cpp
 *
 * Created: 2024-10-02 오후 12:17:22
 * Author : user
 */ 

#include <avr/io.h> // AVR 기본 헤더 
#include <alloca.h>
//LED port :PG4
int main(void)
{
	DDRG |= 0x10; //1=out , 0= input xxx1 0000
	    while (1) 
    {
		PORTG | = 0x10;
		
		
    }
}

