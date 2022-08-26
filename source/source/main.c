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
uint8_t TempPW[8];
	
#define passwordLength 8
uint8_t EEMEM password_EEPROM[8]={2,2,2,2,2,2,2,2};
uint8_t password[8];

	
#define menuItemNum 4


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
				for (uint8_t i;Key_Scan()==42 && i<100 ;i++)
				{
					_delay_ms(10);
					if (i==99) return;
						
				}
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
	unsigned char Character1[8] = { 0x10, 0x18, 0x1c, 0x1e, 0x1c, 0x18, 0x10, 0x00 };  /* Custom char set for alphanumeric LCD Module */
	unsigned char Character2[8] = { 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char Character3[8] = { 0x00, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00 };

	LCD_Custom_Char(0, Character1);  /* Build Character1 at position 0 */
	LCD_Custom_Char(1, Character2);  /* Build Character2 at position 1 */
	LCD_Custom_Char(2, Character3);  /* Build Character3 at position 2 */
//	----------------------------------------------------------------------
	char menuItemStr[menuItemNum][16]={{"Change PW"},{"Beep Status"},{"Show Current PW"},{"Log out"}};
	int cursurLine;
	uint8_t enter=0;
	uint8_t keyEntered=1;
	uint8_t index;
	uint8_t PW[8]={1,1,1,1,1,1,1,1};
	
	//eeprom_update_block((uint8_t*)PW,(const uint8_t*)password_EEPROM,8);
	eeprom_read_block  ((uint8_t*)password,(const uint8_t*)password_EEPROM,8);
	
	showPW(password);
	_delay_ms(2000);
	
	login();
	sbi(DDRA,3);
	sbi(PORTA,3);
	
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
				
			case 35: 
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
					getPW("enter new PW",password);
					eeprom_update_block((uint8_t*)password,(const uint8_t*)password_EEPROM,8);
					lcdClear();
					lcdGotoXY(0,0);
					rprintfStr("your new PW is :");
					showPW(password);
					_delay_ms(3000);
				break;
				
				case 1:
					lcdClear();
					rprintfStr("this is 1");
				break;
				
				case 2:
					lcdClear();
					lcdGotoXY(0,0);
					rprintfStr("Current PW is:");
					eeprom_update_block((uint8_t*)password,(const uint8_t*)password_EEPROM,8);
					lcdGotoXY(0,1);
					showPW(password);
					while(!(Key_Scan()==42));
				break;
				
				case 3:
					cbi(PORTA,3);
					login();
					sbi(PORTA,3);
					index=0;
					cursurLine=0;
				break;
					
				}
				enter=0;
				keyEntered=1;
		}

	}
	
}
