#ifndef LED
#define LED
#include "MKL25Z4.h"                    // Device header
void lightUpALL(PORT_Type* port, int pin[], int number);
void offLED(PORT_Type* port, int pin[], int number);
void onLED(PORT_Type* port, int pin);
void initLED(PORT_Type* port, int pin[], int number);
#endif
