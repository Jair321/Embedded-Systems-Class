/*
//		
//		Code written by Jair Cordova with help from Joseph Nguyen
//    File Name: Lab9.c
//    Purpose: This code uses can measure the period of an inputted analog signal
//						 PIT_Running() returns a bool depending if the pit is running or not
//             Set_LDVAL() calculates the desired load value
//						 Enable_ADC() turns the ADC on
//						 ADC_Value() returns the value of ADC
//						 Print_Period() gets the p_count and displays it in ms
//           	 In the main function is what actually calculates the period
//						 There is alot of supporting code from previous labs
*/



#include <MKL25Z4.H>
#include <stdbool.h>
#include <string.h>

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


#define ADC_POS (20)
#define DAC_POS (30)

#define MASK(x) (1UL << (x))

void Init_PIT(uint32_t period);
void Start_PIT(void);
void Stop_PIT(void);
void PIT_IRQHandler(void);
bool PIT_Running(void);
void Set_LDVAL(uint32_t period);
void Print_Period(uint32_t p_count);
void Init_ADC(void);
void Enable_ADC(void);
unsigned int ADC_Value(void);

void pulse_the_LCD_enable(void);
void LCD_send_data(uint32_t data);
void PrintChar (char input[]);	
void LCD_init(void);
void LCD_command(uint32_t command);
void SetUp(void);
void PrintError(void);


#define MASK(x) (1UL << (x))

#define ADC_Channel_IN1     0b0

int main(void) {
    unsigned int current;
    unsigned int max = 0;
    unsigned int min = 0xFFF;
    unsigned int Period_High;
    unsigned int Period_Low;
    unsigned int Thresh_Tolerance;
    unsigned int p_count = 0;
    bool P_Incremet_Flag = false;
    
    // Enable Clock
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    LCD_init();                   // initialize LCD display
    LCD_command(0x01);            //clear screen
    
    Init_ADC();
    
    Init_PIT(62914559); // interrupt every 6 sec
    
    Start_PIT();
    Enable_ADC();
    
    while (PIT_Running()) {
        if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) { // Check Flag
            current = ADC_Value();
            if (current > max) {
                max = current;
            }
            if (current < min) {
                min = current;
            }
            ADC0->SC1[0] &= ~ADC_SC1_COCO_MASK; // Clear Flag
        }
    }
    
    Period_High = (0.8 * (max - min)) + min;
    Period_Low  = (0.2 * (max - min)) + min;
    Thresh_Tolerance = (max - min) * 0.10; // 10% of PP Voltage
    

    Set_LDVAL(104857599); // interrupt every 10 sec
    Enable_ADC();
    p_count = 0;
    
    while (p_count != 2) {
        if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) {
            current = ADC_Value();
            
            if (((Period_High - Thresh_Tolerance) < current) & (current < (Period_High + Thresh_Tolerance)) & (P_Incremet_Flag == false)) {
                p_count += 1;
                P_Incremet_Flag = true;
            }
                
            if (((Period_Low - Thresh_Tolerance) < current) & (current < (Period_Low + Thresh_Tolerance)) & (P_Incremet_Flag == true)) {
                P_Incremet_Flag = false;
            }
            ADC0->SC1[0] &= ~ADC_SC1_COCO_MASK; // Clear Flag
        }
    }
    
    p_count = 0;
    Start_PIT();
    
    while (PIT_Running()) {
        if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) {
            current = ADC_Value();
            
            if (((Period_High - Thresh_Tolerance) < current)  & (current < (Period_High + Thresh_Tolerance)) & (P_Incremet_Flag == false)) {
                p_count += 1;
                P_Incremet_Flag = true;
            }
                
            if (((Period_Low - Thresh_Tolerance) < current) & (current < (Period_Low + Thresh_Tolerance)) & (P_Incremet_Flag == true)) {
                P_Incremet_Flag = false;
            }
            
            ADC0->SC1[0] &= ~ADC_SC1_COCO_MASK; // Clear Conversion Complete Flag
        }
    }
    Print_Period(p_count);
}



void Init_PIT(uint32_t period) {
    // Enable Clock
    SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK;
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    
    // Enable module, freeze timers in debug mode
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    PIT->MCR |= PIT_MCR_FRZ_MASK;
    
    // Initialize PIT0 to count down from argument 
    PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);
    
    // No chaining
    PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
    
    // Generate interrupts
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
    
    /* Enable Interrupts */
    NVIC_SetPriority(PIT_IRQn, 3); // 0, 64, 128 or 192
    NVIC_ClearPendingIRQ(PIT_IRQn); 
    NVIC_EnableIRQ(PIT_IRQn);
}

void Init_DAC(void){
	// clock setup
	SIM -> SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//select analog for pin
	PORTE -> PCR[DAC_POS] &= ~PORT_PCR_MUX_MASK;
	PORTE -> PCR[DAC_POS] |= PORT_PCR_MUX(0);
	
	// Disable buffer mode
	DAC0 -> C1 = 0;
	DAC0 -> C2 = 0;
	
	//Enable DAC, select VDDA as reference voltage
	DAC0 -> C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
	
}

void Init_ADC(void){
	// clock setup
	SIM -> SCGC6 |= SIM_SCGC6_ADC0_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//select analog for pin
	PORTE -> PCR[ADC_POS] &= ~PORT_PCR_MUX_MASK;
	PORTE -> PCR[ADC_POS] |= PORT_PCR_MUX(0);
	
	//Select ADC settings 
	ADC0_SC2 = 0x00;
	ADC0_CFG1 = 0x54;
	ADC0_SC1A = 0x00;  
}


void Start_PIT(void) {
    // Enable counter
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}


void Stop_PIT(void) {
    // Disable counter
    PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

void PIT_IRQHandler() {
    //clear pending IRQ
    NVIC_ClearPendingIRQ(PIT_IRQn);
    
    // clear status flag for timer channel 0
    PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK; // Writing 1 to PIT_TFLG TIF bit 31 clears the Timer Interrupt Flag
    
    // Disable ADC
    ADC0->SC1[0] |= ADC_SC1_ADCH_MASK;
    
    Stop_PIT();
}


bool PIT_Running(void) {
    if (PIT->CHANNEL[0].TCTRL & PIT_TCTRL_TEN_MASK) {
        return true;
    } else {
        return false;
    }
}


void Set_LDVAL(uint32_t period) {
    PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);
}

void Enable_ADC(void){
	ADC0 ->SC1[0] &= ~ADC_SC1_ADCH_MASK;
	ADC0 ->SC1[0] |= ADC_SC1_ADCH(ADC_Channel_IN1);
}

unsigned int ADC_Value(void){
	return ADC0_RA & 0xFFF;
}

void Print_Period(uint32_t p_count) {
  LCD_command(0x01);            //clear screen
  float period_counter_float = p_count;
  float period_ms = (10 / period_counter_float) * 1000;
  int temp_period = period_ms * 100;

  uint16_t intArray[7];
  
  for (int i = 0; i < 7; i++) {
    intArray[i] = 0 + 48;
  }
	
  uint16_t intArray_Index = 6;
  while (temp_period != 0) {
    intArray[intArray_Index] = (temp_period % 10) + 48;
    temp_period /= 10;
    intArray_Index--;
  }
  intArray[0] = intArray[1];
  intArray[1] = intArray[2];
  intArray[2] = intArray[3];
  intArray[3] = intArray[4];
  intArray[4] = 0b00101110;
  
  for (int i = 0; i < 7; i++) {
    LCD_send_data(intArray[i]);
  }
  LCD_command(0xC0);           
  
  for (int i = 0; i < 5; i++) {
    LCD_send_data(0b00100000);
  }
  
  LCD_send_data(0b01101101);
  LCD_send_data(0b01110011);
}

////////////////////////////////////////////////////
// LCD Code 
////////////////////////////////////////////////////

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




void PrintChar (char input[]){
	int i;
	for (i = 0; i < strlen(input); i++ ){
		LCD_send_data(input[i]);
	}
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

void PrintDigit (uint32_t number){
	
	if (number < 10){
		number +=48;
		LCD_send_data(number);
	} 
	else{
		PrintError();
	}
}
void PrintError(void){
		LCD_send_data(69);
		LCD_send_data(82);
		LCD_send_data(82);
		LCD_send_data(79);
		LCD_send_data(82);
}
