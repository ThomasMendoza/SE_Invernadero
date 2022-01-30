#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>

#include "ADC.h"
#include "UART.h"
#define DHT11_PIN 6
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;
float volt;

void Request()				/* Microcontroller send start pulse/request */
{	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
}

void Response()	{		/* receive response from DHT11 */
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

uint8_t Receive_data()			/* receive data */
{	int q;
	for (q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	} return c;
}

void Enviar(){
   		Request();		/* send start pulse */
		Response();		/* receive response */
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */
 
	       volt  = 1023- ADC_GetData(0);
	       
		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum) //revisar si los datos enviados son los correctos
		{serial_print_str("ERROR");
		}
		
		else{	
		     serial_print_str( flot2str(I_RH));	  	
		   serial_print_str(",");
		   _delay_ms(10);
		   serial_print_str( flot2str(I_Temp));
		   serial_print_str(",");
		   _delay_ms(10);
		   serial_println_str( flot2str(volt));
		   	_delay_ms(200);
		}
	     }
	     
int main(void)
{	
	 serial_begin();
	_delay_ms(10);
	ADC_init();
	_delay_ms(10);
	
    while(1)
	{	
	 Enviar();	
	_delay_ms(2000);
	}	
}
