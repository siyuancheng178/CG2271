#ifndef GPIO
#define GPIO
#include "MKL25Z4.h"    // Device header
#include "common.h"
GPIO_Type* getGPIO(PORT_Type* port);
void initGPIO(PORT_Type* port, int pin, uint8_t direction);
void setPin(PORT_Type* port, uint8_t pin, int value);
#endif
