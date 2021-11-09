#include "MKL25Z4.h"                    // Device header


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
volatile int ultrasonicRising = 1;
volatile uint32_t ultrasonicReading = 0;    //Stores the distance of object away from robot

void TPM2_IRQHandler(void) {
 //Not necessary. Auto clears flag within the NVIC block.
 NVIC_ClearPendingIRQ(TPM2_IRQn);
 //Check the status flag of Channel 1 in Timer 2
 TPM2_STATUS |= TPM_STATUS_CH1F_MASK;
 if (ultrasonicRising) { //start of echo pin pulse
  //Set timer count to 0
  TPM2_CNT = 0; 
  //Reset
  ultrasonicRising = 0;
  //Timer 2 Channel 1
  //Configure Input Capture Mode. Capture on falling edge only. Stores the CNT value at the moment of the falling edge into the CnV register.
  TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM2_C1SC |= TPM_CnSC_ELSB_MASK;
  
 } else { //end of echo pin pulse
  ultrasonicReading = TPM2_C1V * 0.1143;  //How did you get the value 0.1143?
  ultrasonicRising = 1;
  //Why disable the Interrupt?
  NVIC_DisableIRQ(TPM2_IRQn);
 }
}

int main() {
	initTPM2();
 for (;;) {
  //Check if semaphore attained
  //osSemaphoreAcquire(ultrasonicSemaphore, osWaitForever);
  //Stop timer. Why??
  TPM2_SC &= ~TPM_SC_CMOD_MASK; //Disable LTPM counter
  
  //Timer 2 Channel 0
  //Enable Output Compare Mode on Channel 0, to generate 10 microsec high pulse when timer starts. Output compare mode. Clear output on match.
  TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
  TPM2_C0SC |= (TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK);
  
  //Timer 3 Channel 1
  //Configure Input Capture Mode. Capture on rising edge only. Stores the CNT value at the moment of the rising edge into the CnV register.
  TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM2_C1SC |= TPM_CnSC_ELSA_MASK;
  
  //Reset timer count value
  TPM2_CNT = 0;
  
  ultrasonicRising = 1;
  ultrasonicReading = 0;
  
  //Qn: don't you clear pending IRQ first, then enable?
  //Enable interrupts
  //Qn: When is the interrupt triggered?
  NVIC_EnableIRQ(TPM2_IRQn);
  NVIC_ClearPendingIRQ(TPM2_IRQn);
  
  // Set LTPM counter to increment on every LTPM clock count.
  TPM2_SC |= TPM_SC_CMOD(1);
	int i = 0;
	while (i < 999990) {
		i += 1;
	}
  
  //osDelay(100);  //Qn: What is this osDelay for?
  //MessageObjectType messageObject;
  //messageObject.message = ultrasonicReading;
 // osMessageQueuePut(ultrasonicMessageQueue, &messageObject, 0,0);
 }
}