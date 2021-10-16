#include "MKL25Z4.h"                    // Device header


void delay(uint32_t nof) {
	uint32_t i = 0;
	while (1) {
		if (i >= nof) {
			break;
		}
		i++;
	}
}
