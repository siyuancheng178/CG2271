#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2

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




	