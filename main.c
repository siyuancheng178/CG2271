#include "motor.h"
#include "GPIO.h"
#include "common.h"
#include "PWM.h"
#include "UART.h"
#include "LEDModule.h"

volatile int receive_data;
void UART2_IRQHandler() {
	NVIC_ClearPendingIRQ(UART1_IRQn);
	
	if (UART2 -> S1 & UART_S1_RDRF_MASK) {
		receive_data = UART2 -> D;
	}
}

//PORTB0,1,2,3 are used for motors
//PORTE23 is used for UART receiving data
//PORTE0,1,2,3,4,5,20,21,22,29 are used for LED
int main() {
	initMotor();
	initUART2();
	initLED();
	
	while(1) {
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
