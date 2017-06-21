/*
 * Lab3Project.c
 *
 * Author : Oscar Choy
 * 
 * This project uses an ATMega328P board and an external LCD to print out the typed key inputs
 * a user enters to the keyboard/input device. When the user types keys on the input device
 * (typically a keyboard), the external LCD screen displays the char values of what the user typed
 * in. The LCD begins displaying the char values on the LCD's top row. After typing in 16 chars, 
 * the LCD will continue displaying the char values on it's bottom row. After the bottom row has
 * displayed 16 char inputs, the LCD returns to the top row, without erasing any of the previous
 * values on either row. It simply overwrites the values already entered.
 *
 */ 
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>

void UART_Init(unsigned int ubrr);
unsigned char USART_Recieve(void);
void LcdCommandWrite_UpperNibble(unsigned char cm);
void LcdCommandWrite(unsigned char cm);
void LcdDataWrite(unsigned char cm);

char lf = 0x0a;
char cr = 0x0d;
unsigned char input;
int counter = 0;

int main(void)
{
	DDRC |= 1<<DDB0;
	DDRC |= 1<<DDB1;
	DDRC |= 1<<DDB2;
	DDRC |= 1<<DDB3;
	DDRC |= 1<<DDB4;
	DDRC |= 1<<DDB5;
	UART_Init(MYUBRR);
	PORTC |= 1<<PORTC0;
	PORTC |= 1<<PORTC1;
	PORTC |= 1<<PORTC2;
	PORTC |= 1<<PORTC3;
	PORTC |= 1<<PORTC4;
	PORTC |= 1<<PORTC5;
	
	LcdCommandWrite_UpperNibble(0x30);
	_delay_ms(4.1);
	LcdCommandWrite_UpperNibble(0x30);
	_delay_us(100);
	LcdCommandWrite_UpperNibble(0x30);
	LcdCommandWrite_UpperNibble(0x20);
	LcdCommandWrite(0x28);
	LcdCommandWrite(0x08);
	LcdCommandWrite(0x01);
	LcdCommandWrite(0x06);
	LcdCommandWrite(0x0f);
	_delay_ms(120);
	
	//LcdCommandWrite(0x00);
	
	while(1) {
		input = USART_Recieve();
		LcdDataWrite(input);
		counter++;
		_delay_ms(100);
		if (counter == 16) {
			LcdCommandWrite(0xc0);
		} else if (counter == 32) {
			LcdCommandWrite(0x80);
			counter = 0;
		}
	}
	return 0;
}

void UART_Init(unsigned int ubrr) {
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

unsigned char USART_Recieve(void) {
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void LcdCommandWrite_UpperNibble(unsigned char cm) {
	PORTC = (PORTC & 0xf0) | (cm >> 4);
	PORTC &= ~(1<<4);
	PORTC |= 1<<5;
	_delay_ms(1);
	PORTC &= ~(1<<5);
	_delay_ms(1);	
}

void LcdCommandWrite(unsigned char cm) {
	PORTC = (PORTC & 0xf0) | (cm >> 4);
	PORTC &= ~(1<<4);
	PORTC |= 1<<5;
	_delay_ms(1);
	PORTC &= ~(1<<5);
	_delay_ms(1);
	
	PORTC = (PORTC & 0xf0) | (cm & 0x0f);
	PORTC &= ~(1<<4);
	PORTC |= 1<<5;
	_delay_ms(1);
	PORTC &= ~(1<<5);
	_delay_ms(1);
}

void LcdDataWrite(unsigned char cm) {
	PORTC = (PORTC & 0xf0) | (cm >> 4);
	PORTC |= 1<<4;
	PORTC |= 1<<5;
	_delay_ms(1);
	PORTC &= ~(1<<5);
	_delay_ms(1);
	
	PORTC = (PORTC & 0xf0) | (cm & 0x0f);
	PORTC |= 1<<4;
	PORTC |= 1<<5;
	_delay_ms(1);
	PORTC &= ~(1<<5);
	_delay_ms(1);
}