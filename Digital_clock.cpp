// digital clock using ATmega32,BCD decoder,RTC and 7 segment display

#include <avr/io.h>
#include <util/delay.h>
uint8_t ss,mm,hh;


void start_comm(void)
{
	TWBR=72; // set bit rate
	TWSR=(0<<TWPS0)|(0<<TWPS1);  // prescaling 1
	TWCR=0x00;
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));// status is updated by hardware
	while((TWSR & 0xF8)!=0x08);
	
}

void mr_mode()
{
	TWDR=0xD1;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	while((TWSR & 0xF8)!=0x40);
}

void mt_mode()
{   TWDR=0xD0;
	TWCR=(1<< TWINT) |(1<<TWEN);
	while(!(TWCR & (1 << TWINT)));
	while((TWSR & 0xF8)!=0x18);
}

void data_transmision(uint8_t data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	while((TWSR & 0xF8)!=0x28);
}

void stop_comm()
{  // PORTA=0xFF;
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN); //this sends the stop condition
	// PORTA=0xFF;
	//	while (!(TWCR & (1<<TWSTO)));//wait till stop is transmitted
	// P    ORTA=0xFF;
}

uint8_t data_reception()
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);//data byte will be received & ack will be send
	while (!(TWCR & (1<<TWINT)));
	while((TWSR & 0xF8)!=0x50);
	return(TWDR);
}
uint8_t nack()
{
	TWCR=(1<<TWINT)|(1<<TWEN);//data byte will be received & ack will be send
	while (!(TWCR & (1<<TWINT)));
	
	while((TWSR & 0xF8)!=0x58);
	
	return(TWDR);
	
}

void set_time(uint8_t ss,uint8_t mm,uint8_t hh)
{
	start_comm();
	mt_mode();
	data_transmision(0x00);
	data_transmision(ss);
	data_transmision(mm);
	data_transmision(hh);
	stop_comm();
}

void get_time(void)
{   start_comm();
	mt_mode();
	data_transmision(0x00);
	stop_comm();
	start_comm();
	mr_mode();
	ss=data_reception();
	mm=data_reception();
	hh=nack();
	stop_comm();
	
	
}

int main(void)
{
	DDRA=0xFF;
	DDRB=0xFF;
	PORTA=0xF0;
	set_time(0x00,0x00,0x00); // intial time
	while(1)
	{
		get_time(); // fetching time in hr and sec
		// displaying time using BCD decoder on 7 segment display 
		PORTA &=~(1<<PA0);
		PORTB=ss;
		_delay_us(5000);
		PORTA|=1<<PA0;
		_delay_us(5000);
		PORTA &=~(1<<PA1);
		PORTB=ss>>4;
		_delay_us(5000);
		PORTA|=1<<PA1;
		_delay_us(5000);
		PORTA &=~(1<<PA2);
		PORTB=hh;
		_delay_us(5000);
		PORTA|=1<<PA2;
		_delay_us(5000);
		PORTA &=~(1<<PA3);
		PORTB=hh>>4;
		_delay_us(5000);
		PORTA|=1<<PA3;
		_delay_us(5000);
	}
}
