#ifndef LED
#define LED
#include "MKL25Z4.h"                    // Device header
void initLED();
void flashGreen(PORT_Type* port, int pin[], int number);
void lightUpALlGreen(PORT_Type* port, int pin[], int number);
#endif
