#include "motor.h"
#include "GPIO.h"
#include "common.h"
#include "PWM.h"
#include "UART.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2

volatile int receive_data = 0, moving = 0, autoDriving;
osSemaphoreId_t brainSem;
volatile int ultrasonicRising = 1;
volatile uint32_t ultrasonicReading = 0;    //Stores the distance of object away from robot
int pinE[8] = {0, 1, 2, 3, 4, 5, 20, 21};
int pinC[8] = {0, 3, 4, 5, 6, 7, 10, 11};
osMessageQueueId_t motorMsg, audioMsg, LEDMsg;
osEventFlagsId_t autoStop, drivingMode;

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
	} else {
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


void initLED(PORT_Type* port, int pin[], int number) {
	for (int i = 0; i < 10; i++) {
		initGPIO(port, pin[i], 1);
	}
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
		int led_signal = (receive_data == 0)? 0 : 1;
		switch(data) {
			case 0xf0:
				osEventFlagsSet(drivingMode, 0x001);
		    break;
			case 0x00: 
			  moving = 0;
			case 0x01: case 0x02: case 0x03: case 0x04: case 0x05:
		  case 0x06: case 0x07: case 0x08:
				osMessageQueuePut(motorMsg, &data, NULL, 0);
			  moving = 1;
			break;
		}
	}
}

void auto_drive_thread() {
	moving = 0;
	osEventFlagsWait(drivingMode, 0x001, osFlagsWaitAll, osWaitForever);
	moving_forward();
	moving = 1;
	osEventFlagsWait(autoStop, 0x0001, osFlagsWaitAll, osWaitForever);
	stop();
	moving_left();
	osDelay(100);
	stop();
	moving_forward();
	osDelay(100);
	stop();
	moving_right();
	osDelay(60);
	moving_forward();
	osDelay(80);
	stop();
	moving_right();
	osDelay(70);
	moving_forward();
	osEventFlagsWait(autoStop, 0x0001, osFlagsWaitAll, osWaitForever);
	stop();
	moving = 0;
	for(;;) {}
}



	
void motor_thread() {
	int data;
	for (;;) {
		osMessageQueueGet(motorMsg, &data, NULL, osWaitForever);
		switch (receive_data) {
			case 0b00000000: //stop
				stop();
			  moving = 0;
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
			
			case 0b00000101: //left-forward
				moving_left_forward();
			  break;
			
			case 0b00000110: //left-backward
				moving_left_backward();
			  break;
			
			case 0b00000111: //right-forward
				moving_right_forward();
			  break;
			
			case 0b00001000: //right-backward
				moving_right_backward();
			  break;
		}
	}
}

void TPM0_IRQHandler(void) {
 NVIC_ClearPendingIRQ(TPM0_IRQn);
 //Check the status flag of Channel 1 in Timer 2
 TPM0_STATUS |= TPM_STATUS_CH1F_MASK;
 if (ultrasonicRising) { //start of echo pin pulse
  //Set timer count to 0
  TPM0_CNT = 0; 
  //Reset
  ultrasonicRising = 0;
  //Timer 2 Channel 1
  //Configure Input Capture Mode. Capture on falling edge only. Stores the CNT value at the moment of the falling edge into the CnV register.
  TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C1SC |= TPM_CnSC_ELSB_MASK;
 } 
 else { //end of echo pin pulse
  ultrasonicReading = TPM0_C1V * 0.0572;  
	if (ultrasonicReading <= 150) {
		osEventFlagsSet(autoStop, 0x0001);
	}
  ultrasonicRising = 1;
  NVIC_DisableIRQ(TPM0_IRQn);
 }
}

//PORTB0,1,2,3 are used for motors
//PORTE23 is used for UART receiving data
//PORTE0,1,2,3,4,5,20,21 are used for LED (front)
//PORTC0,3,4,5,6,7,10,11 are used for LED (back)
int main() {
	SystemCoreClockUpdate();

  SIM -> SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;	
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	initMotor();
	initUART2();
	initLED(PORTE, pinE, 8);
	initLED(PORTC, pinC, 8);
	offLED(PORTE, pinE, 8);
	offLED(PORTC, pinC, 8);
	lightUpALL(PORTE, pinE, 8);
	lightUpALL(PORTC, pinC, 8);
	stop();
	
  //while (receive_data != 0xff);
	//connection_success();
	osKernelInitialize();
	
	stop();
	brainSem = osSemaphoreNew(1, 0, NULL);
	motorMsg = osMessageQueueNew(1, 1, NULL);
	LEDMsg = osMessageQueueNew(1, 1, NULL);
	osThreadNew(motor_thread, NULL, NULL);
	osThreadNew(flash_front, NULL, NULL);
	osThreadNew(brain_thread, NULL, NULL);
	osThreadNew(flash_back, NULL, NULL);
	osThreadNew(auto_drive_thread, NULL, NULL);
	osKernelStart();
	for (;;){}
}
