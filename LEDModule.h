#ifndef LED
#define LED
#include "MKL25Z4.h"                    // Device header
void initLED(PORT_Type* port, int pin[], int number);
void connection_success();
void flash(PORT_Type* port, int pin[], int number, int millisecond);
void lightUpALl(PORT_Type* port, int pin[], int number);
void offLED(PORT_Type* port, int pin[], int number);
#endif
