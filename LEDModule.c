/*
#include "GPIO.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2

void lightUpALL(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < number; i++) {
		setPin(port, pin[i], 1);
	}
}

void flash(PORT_Type* port, int pin[], int number, int millisecond) {
	for (int i = 0; i < number; i++) {
		setPin(port, pin[i], 1);
		osDelay(millisecond);
		setPin(port, pin[i], 0);
	}
}

void connection_success() {
	int pin[2] = {4, 5};
	flash(PORTB, pin, 2, 500);
}

void initLED(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < 10; i++) {
		initGPIO(port, pin[i], 1);
	}
}

void offLED(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < number; i++) {
		setPin(port, pin[i], 0);
	}
}
*/