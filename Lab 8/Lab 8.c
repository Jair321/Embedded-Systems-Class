/*
//		
//		Code written by Jair Cordova with help from Joseph Nguyen
//    File Name: Lab 8.c
//    Purpose: This code uses can convert an analog single to a digital and convert that same code back to analog continuously
//             Init_DAC() initializes the DAC
//						 Init_ADC() initializes the ADC
//						 Init_LED() initializes the LED
//						 Convert_ADC_DAC() gets the analog signal and converts it to digital
//           	 In the main function a while loop is used to change the LED color when the potentiometer is turned
//						 The main function also checks the coco flag
//
*/

#include <MKL25Z4.h>

#define ADC_POS (20)
#define BLUE_LED (1)		// on port D
#define RED_LED (18)    //on port B
#define GREEN_LED (19)    //on port B
#define MASK(x) (1UL << (x))
#define DAC_POS (30)


void Init_ADC(void);
void Init_LED(void);
void Init_DAC(void);
void Convert_ADC_DAC(void);

int main (void){
	Init_ADC();
	Init_DAC();
	Init_LED();
	
	while(1){

		if(ADC0 ->SC1[0] & ADC_SC1_COCO_MASK){
			
			if(ADC0_RA >= 3724){
				
				PTB->PSOR |= MASK(GREEN_LED);
				PTB->PSOR |= MASK(RED_LED);
				PTD->PCOR |= MASK(BLUE_LED);
				
			} else if (ADC0_RA >= 2482){
				
				PTD->PSOR |= MASK(BLUE_LED);
				PTB->PSOR |= MASK(RED_LED);
				PTB->PCOR |= MASK(GREEN_LED);
				
			} else if (ADC0_RA >= 1241){
				
				PTD->PSOR |= MASK(BLUE_LED);
				PTB->PSOR |= MASK(GREEN_LED);
				PTB->PCOR |= MASK(RED_LED);
				
			}
			
			Convert_ADC_DAC();
			ADC0 ->SC1[0] &= ~ADC_SC1_COCO_MASK;
		}
	}
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

void Init_LED (void){
	// Setup Clocks 
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	// Setup GPIOs
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PTB->PDDR |= MASK(RED_LED);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;          
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);   
	PTD->PDDR |= MASK(BLUE_LED);
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);   
	PTB->PDDR |= MASK(GREEN_LED);
	
	
	// make sure LEDs are off
	PTB->PSOR |= MASK(RED_LED);
	PTD->PSOR |= MASK(BLUE_LED);
	PTB->PSOR |= MASK(GREEN_LED);
	


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


void Convert_ADC_DAC(void){
	DAC0 -> DAT[0].DATL = ADC0_RA & 0xFF;
	DAC0 -> DAT[0].DATH = (ADC0_RA >> 8) & 0x0F;
}
