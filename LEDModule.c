#include "ShareVariable.h"
#include "GPIO.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "PWM.h"
int pinE[8] = {0, 1, 2, 3, 4, 5, 20, 21};
int pinC[8] = {0, 3, 4, 5, 6, 7, 10, 11};

void initLED(PORT_Type* port, int pin[], int number) {
	for(int i = 0; i < number; i++) {
		initGPIO(port, pin[i], 1);
	}
}

void lightUpALL(PORT_Type* port, int pin[], int number) {
	for(int i = 0; i < number; i++) {
		setPin(port, pin[i], 1);
	}
}

void offLEDALL(PORT_Type* port, int pin[], int number) {
	for(int i = 0; i < number; i++) {
		setPin(port, pin[i], 0);
	}
}

void onLED(PORT_Type* port, int pin) {
	setPin(port, pin, 1);
}
void offLED(PORT_Type* port, int pin) {
	setPin(port, pin, 0);
}

void flash_front(void* argument) {
	osSemaphoreAcquire(connect_event, osWaitForever);
	int i = 0;
	offLEDALL(PORTE, pinE, 8);
	for(;;) {
	if (moving == 1) {
		offLEDALL(PORTE, pinE, 8);
		osDelay(500);
		setPin(PORTE, pinE[i], 1);
		osDelay(500);
	} 
	else {
		lightUpALL(PORTE, pinE, 8);
	}
	i = (i + 1) % 8;
	}
}


void flash_back(void* argument) {
	osSemaphoreAcquire(connect_event, osWaitForever);
	offLEDALL(PORTC, pinC, 8);
	for (;;) {
		if (moving == 1) {
			offLEDALL(PORTC, pinC, 8);
			osDelay(500);
			lightUpALL(PORTC, pinC, 8);;
			osDelay(500);
		} else {
			offLEDALL(PORTC, pinC, 8);
			osDelay(250);
			lightUpALL(PORTC, pinC, 8);
			osDelay(250);
		}
	}
}


void connection_success() {
	onLED(PORTE, 2);
	onLED(PORTE, 3);
	osDelay(500);
	offLED(PORTE, 2);
	offLED(PORTE, 3);
	osDelay(500);
	onLED(PORTE, 2);
	onLED(PORTE, 3);
	osDelay(500);
	offLED(PORTE, 2);
	offLED(PORTE, 3);
	osDelay(500);
}