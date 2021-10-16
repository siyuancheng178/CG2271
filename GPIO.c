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
void initGPIO(PORT_Type* port, int pin, uint8_t direction) {
	port -> PCR[pin] &= ~PORT_PCR_MUX_MASK;
	port -> PCR[pin] |= PORT_PCR_MUX(1);
	
	getGPIO(port) -> PDDR |= MASK(pin);
}

void setPin(PORT_Type* port, uint8_t pin, int value) {
	GPIO_Type* gpio = getGPIO(port);
	if (value == 1) {
		gpio -> PDOR |= MASK(pin);
	} else {
		gpio -> PDOR &= ~MASK(pin);
	}
}


	
