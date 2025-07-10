/*
//		
//		Code written by Jair Cordova with help from N. Smith-Gray, and Larry Aamodt
//    File Name: main_TPM0.c
//    Purpose: This is an example of timer interrupts.
//             The TPM0 interrupt handler toggles the red LED 
//						 The init_switch interrupt handler toggles the Green LED three times if button 1 is pressed and the display with a message when button 2 is pressed
//           	 There is also a lot of supporting code to display something on the screen.
*/

#include <MKL25Z4.h>
#include <string.h>

#define MASK(x) 	(1UL << (x))
#define	LED9			(9)	/* Port C */
#define BLUE_LED (1)		// on port D
#define RED_LED (18)    //on port B
#define GREEN_LED (19)    //on port B
#define SW_POS (1)
#define SW_POS_2 (2)

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




void TPM0_init (void);
void TPM0_IRQHandler(void);
void delayMs(uint32_t n);
void init_switch(void);
void init_switch_LCD(void);
void PORTA_IRQHandler(void);
void pulse_the_LCD_enable(void);
void LCD_send_data(uint32_t data);
void PrintChar (char input[]);	
void LCD_init(void);
void LCD_command(uint32_t command);
void SetUp(void);	

// Shared variables
unsigned count;

int main (void){
	init_switch();
	init_switch_LCD();
	__disable_irq();
	
	/* Setup Clocks */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	SIM->SOPT2 |= 0x1000000;				/* use MCGFLLCLK as timer counter clock */
	
	/* Setup GPIOs */
	PORTB-> PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PTB->PDDR |= MASK(RED_LED);
	
	
	//blue LED
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;          
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);   
	PTD->PDDR |= MASK(BLUE_LED);
	
	/* Turn off the LED */
	PTB->PCOR |= MASK(RED_LED);
	
	/* Setup Interrupts */
	TPM0_init ();

	__enable_irq();
	
	while(1){
		PTD->PSOR |= MASK(BLUE_LED); //Set output register
		delayMs(500);
		PTD->PCOR |= MASK(BLUE_LED);
		delayMs(500);
	}
}

/* Funciton Definitions */
void TPM0_init (void){
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		/* enable clock to TPM0 */
	TPM0->SC = 0;													/* disable timer while configuring */
	TPM0->SC = 0x07;											/* prescaler /128*/ 
	TPM0->MOD = 0xFFFF;										/* max modulo value */
	TPM0->SC |= 0x80;											/* clear TOF */
	TPM0->SC |= 0x40;											/* enable timeout interrupt */
	TPM0->SC |= 0x08;											/* enable timer */
	NVIC_SetPriority(TPM0_IRQn,1);  			/* Set interrupt priority to 3 (function uses NVIC->IP) */
	NVIC_EnableIRQ(TPM0_IRQn);
}


void TPM0_IRQHandler(void){
	PTD->PSOR |= MASK(BLUE_LED);
	PTB->PTOR = MASK(RED_LED);
	TPM0->SC |= 0x80;								/* clear TOF */
}

void init_switch(void) {
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; /* enable clock for port A*/

	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	PORTA->PCR[SW_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	
	/* Set port B switch bit to inputs */
	PTA->PDDR &= ~MASK(SW_POS);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTA_IRQn); 
	NVIC_EnableIRQ(PORTA_IRQn);
}

void PORTA_IRQHandler(void) {
	if(PORTA_ISFR == MASK(SW_POS_2)){
		SetUp();
		PrintChar("Hello");
		LCD_command(0xC0);
		PrintChar("World!");
		delayMs(1500);
		LCD_command(0x01);
	}
	if(PORTA_ISFR == MASK(SW_POS)){
			PTB->PSOR |= MASK(RED_LED);
			PTD->PSOR |= MASK(BLUE_LED);
			PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;          
			PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);   
			PTB->PDDR |= MASK(GREEN_LED);
			PTB->PCOR |= MASK(GREEN_LED); //Set output register
			delayMs(500);
			PTB->PSOR |= MASK(GREEN_LED);
			delayMs(500);
			PTB->PCOR |= MASK(GREEN_LED); //Set output register
			delayMs(500);
			PTB->PSOR |= MASK(GREEN_LED);
			delayMs(500);
			PTB->PCOR |= MASK(GREEN_LED); //Set output register
			delayMs(500);
			PTB->PSOR |= MASK(GREEN_LED);
			delayMs(500);
	}
			// clear pending interrupts
			NVIC_ClearPendingIRQ(PORTA_IRQn);
			if ((PORTA->ISFR & MASK(SW_POS))) {
				count++;
			}
			// clear status flags 
			PORTA->ISFR = 0xffffffff;
	}

void init_switch_LCD(void) {
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; /* enable clock for port A*/

	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	PORTA->PCR[SW_POS_2] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	
	/* Set port B switch bit to inputs */
	PTA->PDDR &= ~MASK(SW_POS_2);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTA_IRQn); 
	NVIC_EnableIRQ(PORTA_IRQn);
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




void PrintChar (char input[]){
	int i;
	for (i = 0; i < strlen(input); i++ ){
		LCD_send_data(input[i]);
	}
}

void delayMs(uint32_t n)
{
uint32_t i;
uint32_t j;
for(i=0; i < n; i++)
		for(j=0; j < 3500; j++) {}
}

void LCD_command(uint32_t command)
{
	PTB->PCOR = LCD_RW | LCD_RS | LCD_EN;   // clear R/W, RS, and EN
	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
	PTC->PSOR = (command & LCD_UPPER4_MASK)>>4; // output upper 4 bits of command
	pulse_the_LCD_enable();
	PTC->PCOR = LCD_MASK;                   // clear output data bits
	PTC->PSOR = (command & LCD_LOW4_MASK);   // output lower 4 bits
	pulse_the_LCD_enable();
	if (command < 4)
		delayMs(3);                           // command 1 and 2 need 1.64ms
	else
		delayMs(1);                           // all others 40us
}

void LCD_init(void)
{
  // Note: you need to turn on Port B and C clocks prior to calling this routine

	uint32_t k;

     // First set up bits in GPIO Port C used by the LCD
	for (k=0; k<4; k++) {                  // make ports GPIO
	PORTC->PCR[k] &= ~PORT_PCR_MUX_MASK;    // 4 LCD data bits
	PORTC->PCR[k] |= PORT_PCR_MUX(1);
	}
	
	for (k=1; k<4; k++) {                   // make ports GPIO
	PORTB->PCR[k] &= ~PORT_PCR_MUX_MASK;    //   LCD control 3-bits 
	PORTB->PCR[k] |= PORT_PCR_MUX(1);	
	}
	
	PTC->PDDR |= LCD_DATA_PINS;             // set ports to output
	PTB->PDDR |= LCD_CNTRL_PINS;

	PTB->PCOR = LCD_RW | LCD_RS | LCD_EN;   // clear R/W, RS, and EN	
		// Now initialize the LCD itself
	delayMs(00);
	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
	PTC->PSOR = (0x3);                   // put a wake-up value on bus
	delayMs(10);
	pulse_the_LCD_enable();
	delayMs(1);
	pulse_the_LCD_enable();
	delayMs(1);
	pulse_the_LCD_enable();
	delayMs(1);
	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
	PTC->PSOR = (0x2);                   // initialize to 4-bit bus mode
	delayMs(10);
	pulse_the_LCD_enable();
	LCD_command(0x28);                      // Set to 4-bit/2-line/5x7pixels
	LCD_command(0x10);                      // 
	LCD_command(0x0F);                      // Display on, cursor on and blink
	LCD_command(0x06);                      //
}

void SetUp(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	LCD_init();                   // initialize LCD display
	LCD_command(0x01);            // clear screen
	PTC -> PSOR = (0x2);
}
