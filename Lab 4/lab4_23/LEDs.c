//***************************************************************************
//    file:  LEDS.c
//***************************************************************************
#include <MKL25Z4.H>

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D

// Debug status bits
#define DBG_ISR_POS (0)
#define DBG_MAIN_POS (2)

#define DEBUG_PORT PTD

#define MASK(x) (1UL << (x))

// function prototypes
void init_RGB_LEDs(void);
void control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);
void toggle_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);

//****************************************************************************
void init_RGB_LEDs(void) {
	// Enable clock to ports B and D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;;
	
	// Make 3 pins GPIO
	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);          
	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD->PDDR |= MASK(BLUE_LED_POS);
	
	control_RGB_LEDs(1, 1, 1);
	control_RGB_LEDs(0, 0, 0);
}

void control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
	if (red_on) {
			PTB->PCOR = MASK(RED_LED_POS);
	} else {
			PTB->PSOR = MASK(RED_LED_POS); 
	}
	if (green_on) {
			PTB->PCOR = MASK(GREEN_LED_POS);
	}	else {
			PTB->PSOR = MASK(GREEN_LED_POS); 
	} 
	if (blue_on) {
			PTD->PCOR = MASK(BLUE_LED_POS);
	}	else {
			PTD->PSOR = MASK(BLUE_LED_POS); 
	}
}	

void toggle_RGB_LEDs(unsigned int red_tog, unsigned int green_tog, unsigned int blue_tog) {
	if (red_tog) {
			PTB->PTOR = MASK(RED_LED_POS);
	} 
	if (green_tog) {
			PTB->PTOR = MASK(GREEN_LED_POS);
	} 
	if (blue_tog) {
			PTD->PTOR = MASK(BLUE_LED_POS);
	}	
}	
