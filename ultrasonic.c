#include "GPIO.h"
#include "PWM.h"

void initUltrasonic(PORT_Type* port, int pin[], int number) {
	initGPIO(port, pin[0], 0);	//echo
	initGPIO(port, pin[1], 1);	//trigger
}






void initTPM2() {
 SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK; //power up timer 2
	
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