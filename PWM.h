#ifndef PWM
#define PWM
#include "MKL25Z4.h"                    // Device header
void set_duty_cycle(int duty_cycle, int timer, int channel);
void initPWM(PORT_Type* port, int pin, int alt, int timer, int channel);
static void delay(volatile uint32_t nof);
void initChannel(int timer, int channel);
void initTimer(int timer);
void set_clock_source();
void initPort(PORT_Type* port, uint8_t pin, uint8_t alt);
#endif