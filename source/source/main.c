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
	
#define menuItemNum 4
	
#define SEL_arrow   0
#define UP_arrow    1
#define DOWN_arrow  2

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
	lcdClear();
	lcdGotoXY(0,0);
	rprintfStr(textShowUp);
	while (!Key_Released());
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

void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
	unsigned char i;
	if(loc<8)
	{
		lcdControlWrite (0x40 + (loc*8));	/* Command 0x40 and onwards forces the device to point CGRAM address */
		for(i=0;i<8;i++)	/* Write 8 byte for generation of 1 character */
		lcdDataWrite(msg[i]);
	}
}

void CHB(){
	lcdClear();
	rprintfStr(" buzzer  on/off");
	while(1){
		switch(Key_Scan()){
			
			case 1:
				lcdGotoXY(4,1);
				rprintfStr("on ");
			break;
			
			case 3:
			lcdGotoXY(4,1);
			rprintfStr("off");
			break;
			
			case 35:
			lcdClear();
			return;
			
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

	//login();]
	unsigned char Character1[8] = { 0x10, 0x18, 0x1c, 0x1e, 0x1c, 0x18, 0x10, 0x00 };  /* Custom char set for alphanumeric LCD Module */
	unsigned char Character2[8] = { 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char Character3[8] = { 0x00, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00 };

	LCD_Custom_Char(0, Character1);  /* Build Character1 at position 0 */
	LCD_Custom_Char(1, Character2);  /* Build Character2 at position 1 */
	LCD_Custom_Char(2, Character3);  /* Build Character3 at position 2 */
//	----------------------------------------------------------------------
	char menuItemStr[menuItemNum][16]={{"Change PW"},{"Beep Status"},{"WIFI Connection"},{"Progress"}};
	int cursurLine;
	uint8_t enter=0;
	uint8_t keyEntered=1;
	uint8_t index;

	
	while (1)
	{
		
		switch(Key_Scan()){
			
			case 8:	
				if (index<2){
					if (cursurLine==1)
					index ++;
					cursurLine = 1;
					keyEntered=1;
					while (!Key_Released()); 
					}
					break;	
				
			case 2:
				if (index>0){
					if (cursurLine==0) 
					index --;
					cursurLine = 0;
					keyEntered=1;
					while (!Key_Released()); 
					}
					break;
				
			case 5: 
				enter=1; 
				keyEntered=1; 
				while (!Key_Released()); 
				break;
		}
			
		if (keyEntered==1){
				lcdClear();
				
				lcdGotoXY(0,cursurLine);
				rprintfChar(0);
				
				lcdGotoXY(1,0);
				rprintfStr(menuItemStr[index]);
				
				lcdGotoXY(1,1);
				rprintfStr(menuItemStr[index+1]);
				
				keyEntered=0;
		}
		
		if (enter==1){
			switch(cursurLine+index){
				case 0:
					lcdClear();
					rprintfStr("this is 0");
				break;
				
				case 1:
					lcdClear();
					rprintfStr("this is 1");
				break;
				
				case 2:
					lcdClear();
					rprintfStr("WIFI is wrong");
					_delay_ms(2000);
					lcdClear();
				break;
				
				case 3:
					lcdClear();
					rprintfStr("this is 3");
					break;
				}
				enter=0;
				keyEntered=1;
		}

	}
	
/*
	int index;
	lcdClear();
	lcdGotoXY(0,0);
	rprintfChar(0);
	while(1){
		
		lcdGotoXY(1,0);
		rprintfStr("Change PW");
		lcdGotoXY(1,1);
		rprintfStr("BEEP Status");
		
		switch(Key_Scan()){
			
			case 5:
				if (index==0) getPW("enter new PW",password);
				if (index==1) CHB();
				
				lcdClear();
			break;
			
			case 2:
				lcdGotoXY(0,1);
				rprintfChar(' ');
				lcdGotoXY(0,0);
				rprintfChar(0);
				index=0;
			break;
			
			case 8:
				lcdGotoXY(0,1);
				rprintfChar(0);
				lcdGotoXY(0,0);
				rprintfChar(' ');
				index=1;
			break;
		}
		
		
		
	}
*/
}
