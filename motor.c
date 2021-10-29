#include "MKL25Z4.h"                    // Device header
#include "rtx_os.h"                     // ARM::CMSIS:RTOS2:Keil RTX5
#include "pwm.h"
#define PTB0_Pin 0 //Timer 1 Channel 0
#define PTB1_Pin 1 //Timer 1 Channel 1
#define PTB2_Pin 2 //Timer 2 Channel 0
#define PTB3_Pin 3 //Timer 2 Channel 1

volatile int duty_cycle = 7500;
volatile int duty_cycle_turning = 3750;
volatile int moving_while_turning = 500;

/*
1, 0(left)  2, 1(right) forward
1,1(left)   2,0(right) backward
//forward pin0(left) -> Timer1 channel 0;  pin3(right) -> Timer2 channel 1
//backward: pin1(left) -> Timer2 Channel 0; pin2(right) -> Timer1 Channel 1;*/
void stop() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(0, 2, 1);
	set_duty_cycle(0, 2, 0);
}

void moving_forward() {
	set_duty_cycle(duty_cycle, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(duty_cycle, 2, 1);
	set_duty_cycle(0, 2, 0);
}

void moving_backward() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(duty_cycle, 1, 1);
	set_duty_cycle(0, 2, 1);
	set_duty_cycle(duty_cycle, 2, 0);
}

void moving_right() {
	set_duty_cycle(duty_cycle_turning, 1, 0); //forward left
	set_duty_cycle(0, 1, 1); //backward left
	set_duty_cycle(0, 2, 1); //forward right
	set_duty_cycle(duty_cycle_turning, 2, 0); //backward right
}

void moving_left() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(duty_cycle_turning, 1, 1);
	set_duty_cycle(duty_cycle_turning, 2, 1);
	set_duty_cycle(0, 2, 0);
}

void moving_left_forward() {
	//set_duty_cycle(0, 1, 0);
	//set_duty_cycle(moving_while_turning, 1, 1);
	
  set_duty_cycle(moving_while_turning, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(duty_cycle, 2, 1);
	set_duty_cycle(0, 2, 0);
}

void moving_right_forward() {
	set_duty_cycle(duty_cycle, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(0, 2, 1);
	set_duty_cycle(moving_while_turning, 2, 0);
}

void moving_left_backward() {
	set_duty_cycle(moving_while_turning, 1, 0);
	set_duty_cycle(0, 1, 1);
	set_duty_cycle(0, 2, 1);
	set_duty_cycle(duty_cycle, 2, 0);
}

void moving_right_backward() {
	set_duty_cycle(0, 1, 0);
	set_duty_cycle(duty_cycle, 1, 1);
	set_duty_cycle(moving_while_turning, 2, 1);
	set_duty_cycle(0, 2, 0);
}


void change_duty_cycle(int new_duty_cycle) {
	duty_cycle = new_duty_cycle;
}

void initMotor() {
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	initPWM(PORTB, PTB0_Pin, 3, 1, 0);
	initPWM(PORTB, PTB1_Pin, 3, 2, 0);
	initPWM(PORTB, PTB2_Pin, 3, 2, 1);
	initPWM(PORTB, PTB3_Pin, 3, 1, 1);
}
