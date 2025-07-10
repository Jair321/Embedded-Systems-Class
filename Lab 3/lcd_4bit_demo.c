//// *************************************************************************//
////     Example program to place characters on an LCD display                //  
////                                                                          //
////   Filename:  lcd_4bit_demo.c                                             //
////   Author:    L.Aamodt                                                    //
////   Version:   01/27/19 written                                            //
////              01/29/20 added port C clock enable                          //
////   Processor: NXP MKL25Z4                                                 //
////   Compiler:  Keil uVision5                                               //
////   Library:   CMSIS core and device startup                               //
////              also needs lcd_lib_4bit.c                                   //
////   Hardware:  NXP Freedom board connected to a 16x2 LCD display           //
////   Software note:  This program is a "bare metal" application since it    //
////              doesn't use an operating system.                            //
////   Operation: Writes 11 alpha characters (F to P) to LCD display.         //
////              Blinks red LED on for 1 second, off for 1 second            //
//// *************************************************************************//
//# include <MKL25Z4.h>

//#define RED_LED_SHIFT   (18)  // on port B
//#define GREEN_LED_SHIFT (19)  // on port B (not used in this program)
//#define BLUE_LED_SHIFT  (1)   // on port D (not used in this program)

//#define MASK(x) (1UL << (x))

//void LCD_init(void);
//void LCD_command(uint32_t command);       // use to send LCD commands
//void LCD_send_data(uint32_t data);        // use to send one char to screen
//void delayMs(uint32_t n);

//int main(void) {
//	uint32_t j;
//	// Enable clock to Ports B and C
//	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
//	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

//	// Make red RGB LED pin GPIO
//	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
//	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(1);
//	
//	// Set port pin direction to output
//	PTB->PDDR |=MASK(RED_LED_SHIFT);
//	
//	LCD_init();                   // initialize LCD display
//	LCD_command(0x01);            // example command: clear screen
//	
//	for (j=97;j<103;j++) {
//		LCD_send_data(j);     // example data: output lower abcdefg
//        }
//	LCD_command(0xC0);            // move cursor to line 2

//	for (j=72;j<79;j++) {
//		LCD_send_data(j);     // example data: output caps ABCDEFG

//	}

//	while(1) {
//       // alternately turn red LED on/off just to show that CPU is running
//		PTB->PCOR |= MASK(RED_LED_SHIFT);
//		delayMs(1000);
//		PTB->PSOR |= MASK(RED_LED_SHIFT);
//		delayMs(1000);

//	}
//}
