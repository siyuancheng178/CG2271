#ifndef SHARE
#define SHARE
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
extern volatile int receive_data, moving, end, autoDriving;
extern osSemaphoreId_t brainSem, autoStartSem, autoStopSem, autoMeasure, connect_event;
extern volatile int ultrasonicRising;
extern volatile int ultrasonicReading;    //Stores the distance of object away from robot
//extern int* pinE;
//extern int* pinC;
extern osMessageQueueId_t motorMsg, audioMsg, LEDMsg, ultraMsg;
extern osEventFlagsId_t autoStop, drivingMode;

#endif