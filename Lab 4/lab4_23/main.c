/*----------------------------------------------------------------------------
//  file:  main.c  for Lab 4
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
// #include <stdio.h>
// #include "gpio_defs.h"
// #include "LEDs.h"
// #include "switches.h"

// Debug status bits
#define DBG_ISR_POS (0)
#define DBG_MAIN_POS (2)

#define DEBUG_PORT PTD

#define MASK(x) (1UL << (x))
volatile unsigned count=0;

extern volatile unsigned switch_pressed;
extern void init_debug_signals(void);
extern void init_switch(void);
void init_RGB_LEDs(void);
void control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);
void toggle_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	
	init_switch();
	init_RGB_LEDs();
	init_debug_signals();
	__enable_irq();
	
	while (1) {
		DEBUG_PORT->PTOR = MASK(DBG_MAIN_POS);
		control_RGB_LEDs(count&1, count&2, count&4);
	}
}

// **********based on  ARM University Program Copyright © ARM Ltd 2013**********   
