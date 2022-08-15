/*
 * source.c
 *
 * Created: 12/13/2021 8:34:23 PM
 * Author : win_7
 */ 

#include <avr/io.h>

#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "rprintf.h"
#include "global.h"
#include "lcd.h"
#include "keypad.h"
//#define F_CPU 1000000

uint8_t tempLength=8;
uint8_t TempPW[8]={1,1,1,1,1,1,1,1};
	
#define passwordLength 8
uint8_t password[8]={1,1,1,1,1,1,1,1};

// Returns true if arr1[0..n-1] and arr2[0..m-1]
// contain same elements.
uint8_t comparePW(uint8_t arr1[], uint8_t arr2[], uint8_t m, uint8_t n)
{
	// If lengths of array are not equal means
	// array are not equal
	if (n != m)
	return 0;
	
	// Linearly compare elements
	for (int i = 0; i < 8; i++){
	if (arr1[i] != arr2[i]){
	return 0;
	}
	}
	
	// If all elements were same.
	return 1;
}

void showPW(uint8_t array[]){
	for (uint8_t i=0; i<8; i++)
	{
		lcdGotoXY(i,1);
		rprintf("%d",array[i]);
	}

}

uint8_t getPW(const char textShowUp[16], uint8_t tempArray[])
{
	
	//35 ==> '#'
	//42 ==> '*'

	uint8_t a=0;
	uint8_t geta;
	lcdGotoXY(0,0);
	rprintfStr(textShowUp);
	
	while(1){
		if (geta=Key_Scan())
		{
			if (geta==35) {
			while (!Key_Released());	
			return a;
			}
			if (geta==42) {
				a--;
				lcdGotoXY(a,1);
				rprintf(" ");
				while (!Key_Released());
				continue;
			}
			lcdGotoXY(a,1);
			tempArray[a]=geta;
			rprintf("%d",tempArray[a]);
			while (!Key_Released());
			_delay_ms(200);
			lcdGotoXY(a,1);
			rprintf("*");
			a++;
		}
	} 
	
}

void login(){
	
	uint8_t PWL;
	while (1)
	{
		tempLength=getPW("Enter PW  OK=>#",TempPW);
		PWL=comparePW( password, TempPW, passwordLength, tempLength);
		if (PWL==FALSE)
		{
			lcdClear();
			lcdGotoXY(1,0);
			rprintf("wrong password");
			_delay_ms(1500);
		}
		else{
			lcdClear();
			lcdGotoXY(3,0);
			rprintf("succsesful");
			_delay_ms(1500);
			lcdClear();
			break;
		}
	}
	
}



//========================================================================================= 
int main(void)
{
	DDRD=0x8f;
	sbi(DDRD,7);
	cbi(PORTD,7);
	lcdInit();
	rprintfInit(lcdDataWrite);
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

	login();
	
//	----------------------------------------------------------------------
	int Temp;
	while(1){
		lcdGotoXY(0,0);
		Temp=Key_Scan();
		rprintf("%d",Temp);	
	}
}
