
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
