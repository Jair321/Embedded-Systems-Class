/*----------------------------------------------------------------------------*/
// Author: Jair Cordova
// File Name: Lab 3
// Purpose:These functions are to output a digit, hex value, or string to a display. 
//         There is also a conversion function from 16 bits to 4 hex digitals.
//         There is also code to support these functions like SetUp and PrintError.
//         The LCD_init(), LCD_send_data(), and LCD_command functons have been produced by Larry Aamodt
/*----------------------------------------------------------------------------*/

# include <MKL25Z4.h>
#include <string.h>
#include <stdio.h>
void LCD_init(void);
void LCD_send_data(uint32_t data);
void SetUp(void);													// Sets up to print (Remember: make sure you add at the beginning of code if you want to print)
void LCD_command(uint32_t command);       // use to send LCD commands
void PrintError(void);										// use to print error
void PrintDigit (uint32_t number);				// use print ASCII 0 to 9
void PrintHex(uint32_t number);						// use to print Hex
void PrintBinaryToHex(uint32_t Binary);   // use to print from Binary to hex (Remember: use 0b for binary and 0x for Hex ex. PrintBinaryToHex(0x1111);)
void PrintChar (char input[]);						// use to print a list of characters (ex. PrintChar("Hello");)
	
int main (void){
	SetUp();
	PrintChar("Hello");
	LCD_command(0xC0);
	PrintChar("World!");
	
	
}
void PrintChar (char input[]){
	int i;
	for (i = 0; i < strlen(input); i++ ){
		LCD_send_data(input[i]);
	}
}

void PrintBinaryToHex(uint32_t Binary){

	uint32_t Binary1 = Binary;
	uint32_t Binary2 = Binary;
	uint32_t Binary3 = Binary;
	uint32_t Binary4 = Binary;
	LCD_send_data(48);
	LCD_send_data(120);
	
	Binary1 &= 0xF000;
	Binary1 >>= 12;
	PrintHex(Binary1);
	
	Binary2 &= 0x0F00;
	Binary2 >>= 8;
	PrintHex(Binary2);
	
	Binary3 &= 0x00F0;
	Binary3 >>= 4;
	PrintHex(Binary3);
	
	Binary4 &= 0x000F;
	PrintHex(Binary4);
	
}


void PrintHex(uint32_t number){
	if (number > 9 && number < 16){
		number +=55;
		LCD_send_data(number);
		
	} else if (number < 10){
		PrintDigit(number);
	}
	else{
		PrintError();
	}
}


void PrintDigit (uint32_t number){
	
	if (number < 10){
		number +=48;
		LCD_send_data(number);
	} 
	else{
		PrintError();
	}
}

void SetUp(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	LCD_init();                   // initialize LCD display
	LCD_command(0x01);            // clear screen
	PTC -> PSOR = (0x2);
}

void PrintError(void){
		LCD_send_data(69);
		LCD_send_data(82);
		LCD_send_data(82);
		LCD_send_data(79);
		LCD_send_data(82);
}
