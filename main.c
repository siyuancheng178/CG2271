#include "motor.h"
#include "GPIO.h"
#include "audio.h"
#include "ultra.h"
#include "common.h"
#include "PWM.h"
#include "UART.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "MKL25z4.h"
#include "PWM.h"
#include "LEDModule.h"
#define PTD2_Pin 2
#define MUSICAL_NOTE_CNT 42
#define END_NOTE_CNT 14

#define FREQ_2_MOD(x) (375000 / x)

#define c 261
#define d 294
#define e 329
#define f 349
#define g 392
#define a 440
#define b 493

int musical_notes[] = {c, c, g, g, a, a, g, f, f, e, e, d, d, c, g, g, f, f, e, e, d, g, g, f, f, e, e, d, c, c, g, g, a, a, g, f, f, e, e, d, d, c};
int musical_end[] = {c, d, e, c, c, d, e, c, e, f, g, e, f, g};

volatile int receive_data = 0, moving = 0, end = 1;
volatile int average = 10;
osSemaphoreId_t brainSem, autoSem, autoStopSem;
volatile int ultrasonicRising = 1;
volatile int ultrasonicReading = 0;    //Stores the distance of object away from robot
int pinE[8] = {0, 1, 2, 3, 4, 5, 20, 21};
int pinC[8] = {0, 3, 4, 5, 6, 7, 10, 11};
osMessageQueueId_t motorMsg, audioMsg, LEDMsg, ultraMsg;
osEventFlagsId_t autoStop, drivingMode;


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


void UART2_IRQHandler() {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	if (UART2 -> S1 & UART_S1_RDRF_MASK) {
		receive_data = UART2 -> D;
		if (receive_data == 0xf0) osSemaphoreRelease(autoSem);
		else osSemaphoreRelease(brainSem);
	}
}

void brain_thread(void* argument) {
	for (;;) {
			osSemaphoreAcquire(brainSem, osWaitForever);
			int data = receive_data;
		  int led_signal = (receive_data == 0)? 0 : 1;
		  switch(data) {
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
	osSemaphoreAcquire(autoSem, osWaitForever);
	moving_forward();
  moving = 1;
	osSemaphoreAcquire(autoStopSem, osWaitForever);
	stop();
	for(;;) {
			
	}
}

void audio_thread(void* Argument) {
	int i = 0;
	for(;;) {
		if (end == 0) {
			TPM0->MOD = FREQ_2_MOD(musical_notes[i]);
			TPM0_C2V = FREQ_2_MOD(musical_notes[i]/2);
			osDelay(500);
			TPM0->MOD = FREQ_2_MOD(0);
			TPM0_C2V = FREQ_2_MOD(0);
			osDelay(100);
			i = (i + 1) % MUSICAL_NOTE_CNT;
		} else {
			TPM0->MOD = FREQ_2_MOD(musical_end[i]);
			TPM0_C2V = FREQ_2_MOD(musical_end[i]/2);
			osDelay(500);
			TPM0->MOD = FREQ_2_MOD(0);
			TPM0_C2V = FREQ_2_MOD(0);
			osDelay(100);
			i = (i + 1) % END_NOTE_CNT;
		}

	}
}



	
void motor_thread(void* Argument) {
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
 } else { //end of echo pin pulse
	 ultrasonicReading = TPM0_C1V * 0.1143;  
	 if (ultrasonicReading < 3.5) {
	     osSemaphoreRelease(autoStopSem);
	 }
	 ultrasonicRising = 1;
	 ultrasonicReading = 0;
 }
}


void tUltrasonic() {
 for (;;) {

  TPM0_SC &= ~TPM_SC_CMOD_MASK; //Disable LTPM counter
  
  //Timer 0 Channel 0 trigger
  //Enable Output Compare Mode on Channel 0, to generate 10 microsec high pulse when timer starts. Output compare mode. Clear output on match.
  TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
  TPM0_C0SC |= (TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK);
  
  //Timer 0 Channel 1 echo
  //Configure Input Capture Mode. Capture on rising edge only. Stores the CNT value at the moment of the rising edge into the CnV register.
  TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C1SC |= TPM_CnSC_ELSA_MASK;
  
  //Reset timer count value
  TPM0_CNT = 0;
	ultrasonicRising = 1;
	ultrasonicReading = 0;
  
  NVIC_EnableIRQ(TPM0_IRQn);
  NVIC_ClearPendingIRQ(TPM0_IRQn);

  TPM0_SC |= TPM_SC_CMOD(1);
	osDelay(100);
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
	iniAudio();
	initTPM0();
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
	autoSem = osSemaphoreNew(1, 0, NULL);
	autoStopSem = osSemaphoreNew(1, 0, NULL);
	motorMsg = osMessageQueueNew(1, 1, NULL);
	ultraMsg = osMessageQueueNew(1, 1, NULL);
	osThreadNew(motor_thread, NULL, NULL);
	osThreadNew(brain_thread, NULL, NULL);
	osThreadNew(flash_front, NULL, NULL);
	osThreadNew(flash_back, NULL, NULL);
	osThreadNew( tUltrasonic, NULL, NULL);
	osThreadNew(auto_drive_thread, NULL, NULL);
	osThreadNew(audio_thread, NULL, NULL);
	osKernelStart();
	for (;;){}
}
