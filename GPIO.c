#include "common.h"                    // Device header
#define MASK(x) (1<<(x))

GPIO_Type* getGPIO(PORT_Type* port) {
	if (port == PORTA) {
		return PTA;
	}
	
	if (port == PORTB) {
		return PTB;
	}
	
	if (port == PORTC) {
		return PTC;
	}
	
	if (port == PORTD) {
		return PTD;
	}
	
	return PTE;
}

//direction: 0 -> input 1 -> output
//high: for output pin, 1 -> HIGH 0 -> LOW
void initGPIO(PORT_Type* port, int pin, uint8_t direction, uint8_t high) {
	port -> PCR[pin] &= ~PORT_PCR_MUX_MASK;
	port -> PCR[pin] |= PORT_PCR_MUX(1);
	
	getGPIO(port) -> PDDR |= MASK(pin);
	
	if (direction == 1) {
		if (high == 1) {
			getGPIO(port) -> PDOR |= MASK(pin);
		} else {
			getGPIO(port) -> PDOR &= ~MASK(pin);
		}
	}
}





int main() {
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	initGPIO(PORTE, 0, 1, 1);
	initGPIO(PORTE, 1, 1, 1);
	initGPIO(PORTE, 2, 1, 1);
	initGPIO(PORTE, 3, 1, 1);
	initGPIO(PORTE, 4, 1, 1);
	initGPIO(PORTE, 5, 1, 1);
	initGPIO(PORTB, 11, 1, 1);
	initGPIO(PORTB, 10, 1, 1);
	initGPIO(PORTB, 9, 1, 1);
	initGPIO(PORTB, 8, 1, 1);
}
	
