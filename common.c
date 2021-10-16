#include "MKL25Z4.h"                    // Device header


static void delay(volatile uint32_t nof) {
	while(nof != 0) {
		__asm("NOP");
		nof--;
	}
}