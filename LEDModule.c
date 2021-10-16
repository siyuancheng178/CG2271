#include "GPIO.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2


void lightUpALlGreen(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < number; i++) {
		setPin(port, pin[i], 1);
	}
}

void flashGreen(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < number; i++) {
		setPin(port, pin[i], 1);
		delay(250000);
		setPin(port, pin[i], 0);
	}
}

int main() {
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	int pin[10] = {0, 1, 2, 3, 4, 5, 20, 21, 22, 23};
	for (int i = 0; i < 10; i++) {
		initGPIO(PORTE, pin[i], 1);
	}
	while(1) {
		flashGreen(PORTE, pin, 10);
	}
}
