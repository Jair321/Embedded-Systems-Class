/*
//		
//		Code written by Jair Cordova with help from N. Smith-Gray, Larry Aamodt, and Joseph Nguyen
//    File Name: Lab 7.c
//    Purpose: This code is to create a sawtooth waveform and sin wave using the DAC
//             Init_DAC() initializes the DAC
//						 Sawtooth() generates a sawtooth wave utilizing the DAC
//						 Sine() generates a sine wave utilizing the DAC 
//           	 delayMs() is supporting code to generate a delay and use in Sine() to generate a 100Hz wave
*/

#include <MKL25Z4.h>

#define DAC_POS (30)

void Init_DAC(void);
void Sawtooth(void);
void Sine(void);
void delayMs(uint32_t n);

int main (void){
	Init_DAC();
//	Sawtooth();
	Sine();
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

void Sawtooth(void){
	int i = 0;
	int change = 1;
	
	while(1){
		DAC0 -> DAT[0].DATL = i & 0xFF;
		DAC0 -> DAT[0].DATH = (i >> 8) & 0x0F;
		
		i += change;
		
		if (i == 0x1000-1){
			i = 0;
		}
	}
}


void Sine(void) {
    const uint16_t sineLookupTable[] = {
		0x80, 0x88, 0x8f, 0x97, 0x9f, 0xa7, 0xae, 0xb6,
		0xbd, 0xc4, 0xca, 0xd1, 0xd7, 0xdc, 0xe2, 0xe7,
		0xeb, 0xef, 0xf3, 0xf6, 0xf9, 0xfb, 0xfd, 0xfe,
		0xff, 0xff, 0xff, 0xfe, 0xfd, 0xfb, 0xf9, 0xf6,
		0xf3, 0xef, 0xeb, 0xe7, 0xe2, 0xdc, 0xd7, 0xd1,
		0xca, 0xc4, 0xbd, 0xb6, 0xae, 0xa7, 0x9f, 0x97,
		0x8f, 0x88, 0x80, 0x77, 0x70, 0x68, 0x60, 0x58,
		0x51, 0x49, 0x42, 0x3b, 0x35, 0x2e, 0x28, 0x23,
		0x1d, 0x18, 0x14, 0x10, 0x0c, 0x09, 0x06, 0x04,
		0x02, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04,
		0x06, 0x09, 0x0c, 0x10, 0x14, 0x18, 0x1d, 0x23,
		0x28, 0x2e, 0x35, 0x3b, 0x42, 0x49, 0x51, 0x58,
		0x60, 0x68, 0x70, 0x77};

    while (1) {
        for (int j = 0; j < sizeof(sineLookupTable) / sizeof(sineLookupTable[0]); j++) {
            uint16_t i = sineLookupTable[j];
            DAC0->DAT[0].DATL = i & 0xFF;
            DAC0->DAT[0].DATH = (i >> 8) & 0x0F;
					delayMs(1);
        }
    }
}

void delayMs(uint32_t n){
uint32_t i;
uint32_t j;
for(i=0; i < n; i++)
		for(j=0; j < 341; j++) {}
}
