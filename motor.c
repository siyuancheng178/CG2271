#include "MKL25Z4.h"                    // Device header
#include "rtx_os.h"                     // ARM::CMSIS:RTOS2:Keil RTX5
#include "pwm.h"
#include "ShareVariable.h"
#define PTB0_Pin 0 //Timer 1 Channel 0
#define PTB1_Pin 1 //Timer 1 Channel 1
#define PTD0_Pin 0 //Timer 0 Channel 0
#define PTD1_Pin 1 //Timer 0 Channel 1


volatile int duty_cycle = 7500;
volatile int duty_cycle_auto_left = 4650;
volatile int duty_cycle_auto_right = 3200;
volatile int duty_cycle_turning = 3750;
volatile int moving_while_turning = 500;



/*
1, 0(left)  2, 1(right) forward
1,1(left)   2,0(right) backward
//forward \\pin0(left) -> Timer1 channel 0;   pin3(right) -> Timer0 channel 1
//backward: pin1(left) -> Timer1 Channel 1; \\pin2(right) -> Timer0 Channel 0;*/
void stop() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(0, 0, 1);
	set_duty_cycle(0, 0, 0);
	moving = 0;
}

void moving_forward() {
	set_duty_cycle(duty_cycle * 0.87, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(duty_cycle, 0, 1);
	set_duty_cycle(0, 0, 0);
	moving = 1;
}

void moving_forward_auto() {
	set_duty_cycle(duty_cycle_auto_left, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(duty_cycle_auto_right, 0, 1);
	set_duty_cycle(0, 0, 0);
	moving = 1;
}

void moving_backward() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(duty_cycle, 1, 1);
	set_duty_cycle(0, 0, 1);
	set_duty_cycle(duty_cycle, 0, 0);
	moving = 1;
}

void moving_right() {
	set_duty_cycle(duty_cycle_turning, 1, 0); //forward left
	set_duty_cycle(0, 1, 1); //backward left
	set_duty_cycle(0, 0, 1); //forward right
	set_duty_cycle(duty_cycle_turning, 0, 0); //backward right
	moving = 1;
}

void moving_left() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(duty_cycle_turning, 1, 1);
	set_duty_cycle(duty_cycle_turning, 0, 1);
	set_duty_cycle(0, 0, 0);
	moving = 1;
}

void moving_left_forward() {
  set_duty_cycle(moving_while_turning, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(duty_cycle, 0, 1);
	set_duty_cycle(0, 0, 0);
	moving = 1;
}

void moving_right_forward() {
	set_duty_cycle(duty_cycle, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(moving_while_turning, 0, 1);
	set_duty_cycle(0, 0, 0);
	moving = 1;
}

void moving_left_backward() {
	set_duty_cycle(moving_while_turning, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(0, 0, 1);
	set_duty_cycle(duty_cycle, 0, 0);
	moving = 1;
}

void moving_right_backward() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(duty_cycle, 1, 1);
	set_duty_cycle(moving_while_turning, 0, 1);
	set_duty_cycle(0, 0, 0);
}


void change_duty_cycle(int new_duty_cycle) {
	duty_cycle = new_duty_cycle;
}

void initMotor() {
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	initPWM(PORTB, PTB0_Pin, 3, 1, 0);
	initPWM(PORTB, PTB1_Pin, 3, 1, 1);
	initPWM(PORTD, PTD0_Pin, 4, 0, 0);
	initPWM(PORTD, PTD1_Pin, 4, 0, 1);
}


void motor_thread(void* Argument) {
	osSemaphoreAcquire(connect_event, osWaitForever);
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
