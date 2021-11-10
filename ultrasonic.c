#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "ShareVariable.h"
#define MASK(x) (1<<(x))

void initTPM2() {
 SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;
 
 SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
 SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //MCGFLLCLK clock or MCGPLLCLK/2
 
 SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
 
 PORTA_PCR1 &= ~PORT_PCR_MUX_MASK;
 PORTA_PCR1 |= PORT_PCR_MUX(3);
 PORTA_PCR2 &= ~PORT_PCR_MUX_MASK;
 PORTA_PCR2 |= PORT_PCR_MUX(3);
 
 TPM2_SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK | TPM_SC_CPWMS_MASK);
 TPM2_SC |= TPM_SC_PS(4); //16
 
 TPM2_MOD = 20000; 
 
 TPM2_C1SC |= TPM_CnSC_CHIE_MASK;
 
 TPM2_C0V = 30; 
 
 NVIC_SetPriority(TPM2_IRQn, 2);
}

void tUltrasonic() {
 osSemaphoreAcquire(autoSem, osWaitForever);
 for (;;) {

  //Check if semaphore attained
  //osSemaphoreAcquire(ultrasonicSemaphore, osWaitForever);
  TPM2_SC &= ~TPM_SC_CMOD_MASK; //Disable LTPM counter
  
  //Timer 2 Channel 0
  //Enable Output Compare Mode on Channel 0, to generate 10 microsec high pulse when timer starts. Output compare mode. Clear output on match.
  TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
  TPM2_C0SC |= (TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK);
  TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM2_C1SC |= TPM_CnSC_ELSA_MASK;
  
  //Reset timer count value
  TPM2_CNT = 0;
  
  ultrasonicRising = 1;
  ultrasonicReading = 0;
	NVIC_EnableIRQ(TPM2_IRQn);
	NVIC_ClearPendingIRQ(TPM2_IRQn);
  TPM2_SC |= TPM_SC_CMOD(1);
	osDelay(100);
}
}




	