#include <avr/io.h>
#include <avr/interrupt.h>

 #define A (1<<0)
 #define B (1<<2)
 #define C (1<<3)
 #define D (1<<4)
 #define T (1<<1)

 #define delay() for(t=0; t<300; t++) asm("nop")

const uint8_t tabulka[] = {
	T | A,          // 1
	T | B,          // 2
	T | A | B,      // 3
	T | B | C,      // 4
	T | A | B | C,  // 5
	T | B | C | D,  // 6
	T | A | B,      // 
	T | A | C,      // rotace
	T | A | D       // 
};

ISR(INT0_vect)
{
	uint8_t i, cislo;
	uint16_t t;

	cli();
	PORTB = T;
	while (!(PINB & T))
	{
		PORTB  = tabulka[6];
		delay();
		PORTB  = tabulka[7];
		delay();
		PORTB  = tabulka[8];
		delay();
	}
	cislo = tabulka[TCNT0];
	for (i=0; i<3; i++)
	{
		PORTB = T;
		delay();
		PORTB = cislo;
		delay();		
	}	
	sei();
}

int main(void)
{    
	DDRB = A | B | C | D;
	PORTB = T;	
	
	TCCR0A = 1<<WGM01;  // nastav citac co CTC modu	
	OCR0A = 5;          // nastav porovnavaci registr	
	TCNT0 = 0x00;       // vynuluj citaci registr	
	TCCR0B = 1<<CS00;   // nastav frekvenci citani clk/1024	
	MCUCR = 0x00;       // nastav preruseni na sestupnou hranu	
	GIMSK = 1<<INT0;    // povol preruseni od INT0	
	sei();              // globalni povoleni preruseni

	while (1) 
	{
		asm("nop");
	}
}

