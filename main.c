#include "motor.h"
#include "GPIO.h"
#include "common.h"
#include "PWM.h"
#include "UART.h"
#include "LEDModule.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2

volatile int receive_data;
osSemaphoreId_t brainSem;
osMessageQueueId_t motorMsg, audioMsg;

void connection_success() {
}

void UART2_IRQHandler() {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	if (UART2 -> S1 & UART_S1_RDRF_MASK) {
		receive_data = UART2 -> D;
		osSemaphoreRelease(brainSem);
	}
}

void brain_thread(void* argument) {
	for (;;) {
		osSemaphoreAcquire(brainSem, osWaitForever);
		int data = receive_data;
		switch(data) {
			case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05:
		  case 0x06: case 0x07: case 0x08:
				osMessageQueuePut(motorMsg, &data, NULL, 0);
		}
	}
}
	
void motor_thread() {
	int data;
	for (;;) {
		osMessageQueueGet(motorMsg, &data, NULL, osWaitForever);
		switch (receive_data) {
			case 0b00000000: //stop
				stop();
			  break;
			
			case 0b00000001: //move forward
				moving_forward();
			  break;
			
			case 0b00000010: //move backward
				moving_backward();
			  break;
			
			case 0b00000011: //turn left
				moving_left();
			  break;
			
			case 0b00000100: //turn right
				moving_right();
			  break;
		}
	}
}



int pinE[10] = {0, 1, 2, 3, 4, 5, 20, 21, 22, 29};
int pinC[10] = {0, 3, 4, 5, 6, 7, 10, 11, 12, 13};
//PORTB0,1,2,3 are used for motors
//PORTE23 is used for UART receiving data
//PORTE0,1,2,3,4,5,20,21,22,29 are used for LED (front)
//PORTC0,3,4,5,6,7,10,11,12,13 are used for LED (back)
int main() {
	SystemCoreClockUpdate();
	
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	initMotor();
	initUART2();
	initLED(PORTE, pinE, 10);
	initLED(PORTC, pinC, 10);
	offLED(PORTE, pinE, 10);
	offLED(PORTC, pinC, 10);
	stop();
	
	
	osKernelInitialize();
	while (receive_data != 0xff);
	connection_success();
	brainSem = osSemaphoreNew(1, 0, NULL);
	motorMsg = osMessageQueueNew(1, 1, NULL);
	osThreadNew(motor_thread, NULL, NULL);
	osThreadNew(brain_thread, NULL, NULL);
	osKernelStart();
	for (;;){}
}
