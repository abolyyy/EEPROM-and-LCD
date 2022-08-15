/*
 * keypad.h
 *
 * uint8_t Key_Press(void)    ==> this get key state
 * uint8_t Key_Released(void) ==> this get trustful then the key just pressed one
 * uint8_t Key_Scan(void)	  ==> this is main function and gives a refrence
 *
 * MSB keypad port must be connect to column
 * LSB keypad port must be connect to row and connect to pull-up resistor  
 *
 */ 

//Ascii:
	//* ==> 42
	//# ==> 35

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include <avr/io.h>
#include <util/delay.h>
#include "global.h"

#define PORT_Conf PORTD //port connection name      <---- define before use ----<
#define PIN_Conf  PIND  //pin  connection name		<---- define before use ----<


uint8_t Pad_Number[4][3]={{1 , 2 , 3 },
						 { 4 , 5 , 6 },
						 { 7 , 8 , 9 },
						 {'*', 0 ,'#'}};
uint8_t Key_Press(void){
	uint8_t read_row=0;
	
	PORT_Conf  = 0x70; //1111
	
	if ( read_row= (PIN_Conf & 0x70) != 0x70) //(1101 & 1111)==>(1101) != 1111==>read_row()
	{
		
		_delay_ms(20);
		if (read_row== (PIN_Conf != 0x70))
		{
			return 1;
		}
	}
	return 0;
}
uint8_t Key_Released(void){
	PORT_Conf  = 0x70;
		if (PIN_Conf==0x70)
		{
			return 1;
		}
		return 0;
	
}	
uint8_t Key_Scan(void){
	
 	if (Key_Press())
 	{
		uint8_t row_counter;
		uint8_t key_number=0;		// be able default number on 7_segment

		for (row_counter=0; row_counter<4;row_counter++)
		{
			PORT_Conf  = 0x7f;
			cbi(PORT_Conf,row_counter);
			_delay_ms(3);
			
			if		(! CHECKBIT(PIN_Conf,4))key_number= Pad_Number[row_counter][0];
			else if (! CHECKBIT(PIN_Conf,5))key_number= Pad_Number[row_counter][1];
			else if (! CHECKBIT(PIN_Conf,6))key_number= Pad_Number[row_counter][2];
			//else if (! CHECKBIT(PIN_Conf,7))key_number= Pad_Number[row_counter][3]; //Uncomment if 4 column have 
				
		}
		//if (key_number != '#' || '*'){
			return key_number;
		//}
	}
}

#endif /* KEYPAD_H_ */