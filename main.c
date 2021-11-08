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

volatile int receive_data = 0, moving = 0, end = 1;
osSemaphoreId_t brainSem, autoSem, autoStopSem;
volatile int ultrasonicRising = 1;
volatile int ultrasonicReading = 0;    //Stores the distance of object away from robot
int pinE[8] = {0, 1, 2, 3, 4, 5, 20, 21};
int pinC[8] = {0, 3, 4, 5, 6, 7, 10, 11};
osMessageQueueId_t motorMsg, audioMsg, LEDMsg, ultraMsg;
osEventFlagsId_t autoStop, drivingMode;

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

void TPM2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(TPM2_IRQn);
	
	TPM2_STATUS |= TPM_STATUS_CH1F_MASK;
	if (ultrasonicRising) { //start of echo pin pulse
		TPM2_CNT = 0; 
		ultrasonicRising = 0;
		TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
		TPM2_C1SC |= TPM_CnSC_ELSB_MASK;
  } else { //end of echo pin pulse
		ultrasonicReading = TPM2_C1V * 0.1143;
		if (ultrasonicReading <= 150 && ultrasonicReading >= 120) {
			osSemaphoreRelease(autoStopSem);
		}
		ultrasonicRising = 1;
		NVIC_DisableIRQ(TPM2_IRQn);
	}
}


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
	initTPM2();
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
