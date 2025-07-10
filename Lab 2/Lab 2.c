#include <MKL25Z4.h>


#define LED8 (8) //Port C bit 8
#define LED9 (9) //Port C bit 9
#define LED10 (10) //Port C bit 10
#define MASK(x) (1UL << (x))
void Delay(unsigned int time_del);

int main (void){
	
	//Turn on clock for PORTC
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	//Establish GPIO pin
	PORTC->PCR[LED8] &= ~PORT_PCR_MUX_MASK; //what is "~"
	PORTC->PCR[LED8] |= PORT_PCR_MUX(1);//_MASK;    // Difference between this and top
	PORTC->PCR[LED9] &= ~PORT_PCR_MUX_MASK; 
	PORTC->PCR[LED9] |= PORT_PCR_MUX(1);
	PORTC->PCR[LED10] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LED10] |= PORT_PCR_MUX(1);
	
	//Setting the PortC to be an output
	PTC->PDDR |= MASK(LED8);
	PTC->PDDR |= MASK(LED9);
	PTC->PDDR |= MASK(LED10);
	while(1){
		//Turn on/off LED
		PTC->PSOR |= MASK(LED8); //Set output register
		Delay(500);
		PTC->PCOR |= MASK(LED8);  //clear output register 
		PTC->PSOR |= MASK(LED9); //Set output register
		Delay(500);
		PTC->PCOR |= MASK(LED9);   
		PTC->PSOR |= MASK(LED10); 
		Delay(500);
		PTC->PCOR |= MASK(LED10);  
	}
}

void Delay(unsigned int time_del){
	volatile int n;
	while(time_del--){
		n= 1000;
		while (n--)
			;
	}
}

