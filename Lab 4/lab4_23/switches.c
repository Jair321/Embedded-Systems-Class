//***************************************************************************
//  file:  switches.c
//***************************************************************************
#include <MKL25Z4.H>

#define MASK(x) (1UL << (x))

// Debug status bits
#define DBG_ISR_POS (0)
#define DBG_MAIN_POS (2)

#define DEBUG_PORT PTD //changed 
// Switches is on port A for interrupt support //changed
#define SW_POS (1)

// Function prototypes
extern void init_switch(void);

// Shared variables
extern volatile unsigned count;

//***************************************************************************
void init_switch(void) {
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; /* enable clock for port A (was D) */

	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	PORTA->PCR[SW_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	
	/* Set port D switch bit to inputs */
	PTA->PDDR &= ~MASK(SW_POS);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTA_IRQn); 
	NVIC_EnableIRQ(PORTA_IRQn);
}

void PORTA_IRQHandler(void) {  
	DEBUG_PORT->PSOR = MASK(DBG_ISR_POS);
	// clear pending interrupts
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	if ((PORTA->ISFR & MASK(SW_POS))) {
		count++;
	}
	// clear status flags 
	PORTA->ISFR = 0xffffffff;
	DEBUG_PORT->PCOR = MASK(DBG_ISR_POS);
}
 
