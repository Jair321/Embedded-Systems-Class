///*
////		
////		Code written by Jair Cordova with help from N. Smith-Gray, Larry Aamodt, and Joseph Nguyen
////    File Name: PIT_Lab.c
////    Purpose: This is code for PIT timers. It is able to display minutes, seconds, and tenths of a seconds
////						 It's basically a stop watch
////             Init_PIT initializes the PIT and PIT_IRQHandler() is the Handler
////						 init_switch() initizes the the button to be used
////						 PORTA_IRQHandler() has the conversions from count_time to time as well as code to known if the button has been pressed once or twice
////           	 There is also a lot of supporting code to display on the screen.
//*/

//#include <MKL25Z4.h>
//#include <string.h>

//#define MASK(x) 	(1UL << (x))
//#define SW_POS (1)
//#define RED_LED (18)    //on port B
//#define GREEN_LED (19)    //on port B
//#define DBG_MAIN_POS (2)

//#define LCD_EN              0x00000002    // PTB1 LCD enable 
//#define LCD_RW              0x00000004    // PTB2 LCD read/write
//#define LCD_RS              0x00000008    // PTB3 LCD RS
//#define LCD_LOW4_MASK       0x0000000F    // low 4 bits of a command
//#define LCD_UPPER4_MASK     0x000000F0    // upper 4 bits of a command
//#define LCD_MASK            0x0000000F    // PortC bits 0-3
//#define LCD_DATA_PINS       0x0000000F    // PortC LCD data pins
//#define LCD_CNTRL_PINS      0x0000000E    // PortB LCD control pins

//#define clear_screen 0x01
//#define cursor_left 0x10
//#define cursor_right 0x14
//#define cursor_line1 0x80
//#define cursor_line2 0xC0


//int DSC = DEFAULT_SYSTEM_CLOCK;
//void Init_PIT(uint32_t period);
//void Start_PIT(void);
//void Stop_PIT(void);
//void PIT_IRQHandler(void);
//void PORTA_IRQHandler(void);
//void init_switch(void);
//void delayMs(uint32_t n);

//void pulse_the_LCD_enable(void);
//void LCD_send_data(uint32_t data);
//void PrintChar (char input[]);	
//void LCD_init(void);
//void LCD_command(uint32_t command);
//void SetUp(void);
//void PrintError(void);					
//void PrintDigit (uint32_t number);

//unsigned count;
//unsigned int token = 0;
//uint32_t count_time;

//int main (void){
//	uint32_t LDV = .1 * (DSC/2);
//	Init_PIT(LDV);
//	init_switch();
//	
//	__disable_irq();
//		/* Setup Clocks */
//	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;


//	/* Setup GPIOs */
//	PORTB-> PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
//	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
//	PTB->PDDR |= MASK(RED_LED);
//	
//	PORTB-> PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
//	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
//	PTB->PDDR |= MASK(GREEN_LED);
//	
//	SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK; /* enable clock for port D */

//	/* Select GPIO for pins connected to debug signals*/
//	PORTD->PCR[DBG_MAIN_POS] |= PORT_PCR_MUX(1);	
//	
//	/* Set bits to outputs */
//	PTD->PDDR |= MASK(DBG_MAIN_POS); 
//	
//	
//	//Turn off LED
//	PTB->PSOR |= MASK(RED_LED);
//	PTB->PSOR |= MASK(GREEN_LED);
//	
//	__enable_irq();

//	while (1){}

//}


//void Init_PIT(uint32_t period) {
//	// Enable clock to PIT module
//	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
//	
//	// Enable module, freeze timers in debug mode
//	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
//	PIT->MCR |= PIT_MCR_FRZ_MASK;

//	// Initialize PIT0 to count down from argument
//	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

//	// No chaining
//	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;

//	// Generate interrupts
//	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
//	
//	/* Enable Interrupts */
//	NVIC_SetPriority(PIT_IRQn, 128); // 0, 64, 128 or 192
//	NVIC_ClearPendingIRQ(PIT_IRQn);
//	NVIC_EnableIRQ(PIT_IRQn);
//}

//void Start_PIT(void) {
//// Enable counter
//	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
//}

//void Stop_PIT(void) {
//// disable counter
//	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
//}

//void PIT_IRQHandler() {
////clear pending IRQ
//	NVIC_ClearPendingIRQ(PIT_IRQn);
//	
//	// check to see which channel triggered interrupt
//	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
//		// clear status flag for timer channel 0
//		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
//		// Do ISR work
//		count_time += 1;
//		
//		PTD->PTOR |= MASK(DBG_MAIN_POS); 
//	
//		
//	}
//}

//void init_switch(void) {
//	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; /* enable clock for port A*/

//	/* Select GPIO and enable pull-up resistors and interrupts 
//		on falling edges for pins connected to switches */
//	PORTA->PCR[SW_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
//	
//	/* Set port A switch bit to inputs */
//	PTA->PDDR &= ~MASK(SW_POS);

//	/* Enable Interrupts */
//	NVIC_SetPriority(PORTA_IRQn, 128); // 0, 64, 128 or 192
//	NVIC_ClearPendingIRQ(PORTA_IRQn); 
//	NVIC_EnableIRQ(PORTA_IRQn);
//}

//void PORTA_IRQHandler(void) {
//	if(token == 0){
//		Start_PIT();
//		
//		PTB->PCOR |= MASK(GREEN_LED);
//		delayMs(500);
//		PTB->PSOR |= MASK(GREEN_LED);
//		
//		token = 1;

//	} else{
//		Stop_PIT();
//		PTB->PCOR |= MASK(RED_LED);
//		delayMs(500);
//		PTB->PSOR |= MASK(RED_LED);

//		SetUp();
//		
//		
//		uint32_t Time_Array[6];
//		uint32_t temp;
//		uint32_t temp1;	
//		
//		temp = count_time % 10;
//		Time_Array[5] = temp;
//		count_time -= temp;
//		count_time /= 10;
//	
//		temp = count_time%60;
//		temp1 = temp%10;
//		Time_Array[4] = temp1;
//		temp -= temp1;
//		temp = temp/10;
//		Time_Array[3] = temp;
//		
//		temp = count_time/60;
//		temp1 = temp%10;
//		Time_Array[2] = temp1;
//		temp -= temp1;
//		temp = temp/10;
//		Time_Array[1] = temp;
//		
//		
//		PrintDigit(Time_Array[1]);
//		PrintDigit(Time_Array[2]);
//		PrintChar(":");
//		PrintDigit(Time_Array[3]);
//		PrintDigit(Time_Array[4]);
//		PrintChar(":");
//		PrintDigit(Time_Array[5]);

//		count_time = 0;
//		
//		token = 0;
//	}
//	
//	// clear pending interrupts
//	NVIC_ClearPendingIRQ(PORTA_IRQn);
//	if ((PORTA->ISFR & MASK(SW_POS))) {
//		count++;
//	}
//	
//	// clear status flags 
//	PORTA->ISFR = 0xffffffff;
//}




//void delayMs(uint32_t n)
//{
//uint32_t i;
//uint32_t j;
//for(i=0; i < n; i++)
//		for(j=0; j < 3500; j++) {}
//}


//void pulse_the_LCD_enable(void)
//{
//	PTB->PSOR = LCD_EN;                     // assert enable
//	delayMs(1);
//	PTB->PCOR = LCD_EN;                     // de-assert enable
//}

//void LCD_send_data(uint32_t data)
//{
//	PTB->PCOR = LCD_RW | LCD_EN;            // clear R/W, RS, and EN
//	PTB->PSOR = LCD_RS;                     // set RS high
//	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
//	PTC->PSOR = (data & LCD_UPPER4_MASK)>>4;   // output upper 4 bits of command
//	pulse_the_LCD_enable();
//	PTC->PCOR = LCD_MASK;                   // clear output data bits
//	PTC->PSOR = (data & LCD_LOW4_MASK);  // output lower 4 bits
//	pulse_the_LCD_enable();
//}




//void PrintChar (char input[]){
//	int i;
//	for (i = 0; i < strlen(input); i++ ){
//		LCD_send_data(input[i]);
//	}
//}

//void LCD_command(uint32_t command)
//{
//	PTB->PCOR = LCD_RW | LCD_RS | LCD_EN;   // clear R/W, RS, and EN
//	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
//	PTC->PSOR = (command & LCD_UPPER4_MASK)>>4; // output upper 4 bits of command
//	pulse_the_LCD_enable();
//	PTC->PCOR = LCD_MASK;                   // clear output data bits
//	PTC->PSOR = (command & LCD_LOW4_MASK);   // output lower 4 bits
//	pulse_the_LCD_enable();
//	if (command < 4)
//		delayMs(3);                           // command 1 and 2 need 1.64ms
//	else
//		delayMs(1);                           // all others 40us
//}

//void LCD_init(void)
//{
//  // Note: you need to turn on Port B and C clocks prior to calling this routine

//	uint32_t k;

//     // First set up bits in GPIO Port C used by the LCD
//	for (k=0; k<4; k++) {                  // make ports GPIO
//	PORTC->PCR[k] &= ~PORT_PCR_MUX_MASK;    // 4 LCD data bits
//	PORTC->PCR[k] |= PORT_PCR_MUX(1);
//	}
//	
//	for (k=1; k<4; k++) {                   // make ports GPIO
//	PORTB->PCR[k] &= ~PORT_PCR_MUX_MASK;    //   LCD control 3-bits 
//	PORTB->PCR[k] |= PORT_PCR_MUX(1);	
//	}
//	
//	PTC->PDDR |= LCD_DATA_PINS;             // set ports to output
//	PTB->PDDR |= LCD_CNTRL_PINS;

//	PTB->PCOR = LCD_RW | LCD_RS | LCD_EN;   // clear R/W, RS, and EN	
//		// Now initialize the LCD itself
//	delayMs(00);
//	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
//	PTC->PSOR = (0x3);                   // put a wake-up value on bus
//	delayMs(10);
//	pulse_the_LCD_enable();
//	delayMs(1);
//	pulse_the_LCD_enable();
//	delayMs(1);
//	pulse_the_LCD_enable();
//	delayMs(1);
//	PTC->PCOR = LCD_MASK;                   // clear output data bits to 0
//	PTC->PSOR = (0x2);                   // initialize to 4-bit bus mode
//	delayMs(10);
//	pulse_the_LCD_enable();
//	LCD_command(0x28);                      // Set to 4-bit/2-line/5x7pixels
//	LCD_command(0x10);                      // 
//	LCD_command(0x0F);                      // Display on, cursor on and blink
//	LCD_command(0x06);                      //
//}

//void SetUp(void){
//	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
//	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
//	LCD_init();                   // initialize LCD display
//	LCD_command(0x01);            // clear screen
//	PTC -> PSOR = (0x2);
//}

//void PrintDigit (uint32_t number){
//	
//	if (number < 10){
//		number +=48;
//		LCD_send_data(number);
//	} 
//	else{
//		PrintError();
//	}
//}
//void PrintError(void){
//		LCD_send_data(69);
//		LCD_send_data(82);
//		LCD_send_data(82);
//		LCD_send_data(79);
//		LCD_send_data(82);
//}
