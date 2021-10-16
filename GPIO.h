#ifndef GPIO
#define GPIO
#include "MKL25Z4.h"                    // Device header
GPIO_Type* getGPIO(PORT_Type* port);
void initGPIO(PORT_Type* port, int pin, uint8_t direction, uint8_t high);
#endif
