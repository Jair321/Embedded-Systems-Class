#include <stdint.h>
#include <MKL25Z4.h>

#define LCD_EN              0x00000002    // PTB1 LCD enable 
#define LCD_RW              0x00000004    // PTB2 LCD read/write
#define LCD_RS              0x00000008    // PTB3 LCD RS
#define LCD_LOW4_MASK       0x0000000F    // low 4 bits of a command
#define LCD_UPPER4_MASK     0x000000F0    // upper 4 bits of a command
#define LCD_MASK            0x0000000F    // PortC bits 0-3
#define LCD_DATA_PINS       0x0000000F    // PortC LCD data pins
#define LCD_CNTRL_PINS      0x0000000E    // PortB LCD control pins

#define clear_screen 0x01
#define cursor_left 0x10
#define cursor_right 0x14
#define cursor_line1 0x80
#define cursor_line2 0xC0

void delayMs(uint32_t n)
{
uint32_t i;
uint32_t j;
for(i=0; i < n; i++)
		for(j=0; j < 3500; j++) {}
}


void pulse_the_LCD_enable(void)
{
	PTB->PSOR = LCD_EN;                     // assert enable
	delayMs(1);
	PTB->PCOR = LCD_EN;                     // de-assert enable
}

void LCD_send_data(uint32_t data)
{
	PTB->PCOR = LCD_RW | LCD_EN;            // clear R/W, RS, and EN
	PTB->PSOR = LCD_RS;                     // set RS high
	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
	PTC->PSOR = (data & LCD_UPPER4_MASK)>>4;   // output upper 4 bits of command
	pulse_the_LCD_enable();
	PTC->PCOR = LCD_MASK;                   // clear output data bits
	PTC->PSOR = (data & LCD_LOW4_MASK);  // output lower 4 bits
	pulse_the_LCD_enable();
}

int main (void){
 LCD_send_data(0);
}