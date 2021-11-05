#include "MKL25Z4.h"                    // Device header

#define MASK(x) (1<<(x))

void initTPM0() {
 SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
 
 SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
 SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //MCGFLLCLK clock or MCGPLLCLK/2
 
 SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
 
 PORTD_PCR0 &= ~PORT_PCR_MUX_MASK;
 PORTD_PCR0 |= PORT_PCR_MUX(4);
 PORTD_PCR1 &= ~PORT_PCR_MUX_MASK;
 PORTD_PCR1 |= PORT_PCR_MUX(4);
 
 TPM0_SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK | TPM_SC_CPWMS_MASK);
 TPM0_SC |= TPM_SC_PS(4); //16
 
 TPM0_MOD = 20000; 
 
 TPM0_C1SC |= TPM_CnSC_CHIE_MASK;
 
 TPM0_C0V = 30; 
 
 NVIC_SetPriority(TPM0_IRQn, 2);
}


void tUltrasonic() {
// for (;;) {

  TPM0_SC &= ~TPM_SC_CMOD_MASK; //Disable LTPM counter
  
  //Timer 0 Channel 0 trigger
  //Enable Output Compare Mode on Channel 0, to generate 10 microsec high pulse when timer starts. Output compare mode. Clear output on match.
  TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK)); 
  TPM0_C0SC |= (TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK);
  
  //Timer 0 Channel 1 echo
  //Configure Input Capture Mode. Capture on rising edge only. Stores the CNT value at the moment of the rising edge into the CnV register.
  TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C1SC |= TPM_CnSC_ELSA_MASK;
  
  //Reset timer count value
  TPM0_CNT = 0;
  

  
  NVIC_EnableIRQ(TPM0_IRQn);
  NVIC_ClearPendingIRQ(TPM0_IRQn);

  TPM0_SC |= TPM_SC_CMOD(1);
 //}
}

	