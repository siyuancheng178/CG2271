#include "ShareVariable.h"
#include "GPIO.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2


void initLED(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < 10; i++) {
		initGPIO(port, pin[i], 1);
	}
}

void lightUpALL(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < number; i++) {
		setPin(port, pin[i], 1);
	}
}

void offLED(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < number; i++) {
		setPin(port, pin[i], 0);
	}
}

void onLED(PORT_Type* port, int pin) {
	setPin(port, pin, 1);
}

void flash_front(void* argument) {
	int i = 0;
	offLED(PORTE, pinE, 8);
	for(;;) {
	if (moving == 1) {
		offLED(PORTE, pinE, 8);
		osDelay(500);
		setPin(PORTE, pinE[i], 1);;
		osDelay(500);
	} 
	else {
		lightUpALL(PORTE, pinE, 8);
	}
	i = (i + 1) % 8;
	}
}


void flash_back(void* argument) {
	offLED(PORTC, pinC, 8);
	int i = 0;
	for (;;) {
	if (moving == 1) {
		offLED(PORTC, pinC, 8);
		osDelay(500);
		setPin(PORTC, pinC[i], 1);;
		osDelay(500);
	} else {
		offLED(PORTC, pinC, 8);
		osDelay(250);
		setPin(PORTC, pinC[i], 1);;
		osDelay(250);
	}
	i = (i + 1) % 8;
	}
}