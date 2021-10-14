#include "MKL25Z4.h"                    // Device header
#define PTB0_Pin 0
#define PTB1_Pin 1
#define PTB2_Pin 2
#define PTB3_Pin 3

int frequency[7] = {262, 1294, 3330, 449, 2392, 4440, 1494};


void initPWM() {
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	PORTB -> PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB -> PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	PORTB -> PCR[PTB2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB2_Pin] |= PORT_PCR_MUX(3);
	
	PORTB -> PCR[PTB3_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB3_Pin] |= PORT_PCR_MUX(3);
	
	SIM -> SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM -> SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	
	SIM -> SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM -> SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM1 -> MOD = 7500; 
  TPM2 -> MOD = 7500; 

	TPM1 -> SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK);
	TPM1 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1 -> SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM1_C0SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM1_C1SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM2 -> SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK);
	TPM2 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM2 -> SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM2_C0SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM2_C1SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

static void delay(volatile uint32_t nof) {
	while(nof != 0) {
		__asm("NOP");
		nof--;
	}
}


int main() {
	initPWM();
	//forward pin0(left) pin3(right)
  //backward: pin1(left) pin2(right)
  	
	TPM1_C0V = 3750; //pin0
	TPM1_C1V = 3750;  //pin1
	TPM2_C0V = 3750;  //pin2
	TPM2_C1V = 3750;  //pin3
	while (1) {
		TPM1_C0V = 3750;
		TPM2_C1V = 3750;
		TPM1_C1V = 0;  //pin1
	  TPM2_C0V = 0;
		delay(0x8000000);
		TPM1_C0V = 0;
		TPM2_C1V = 0;
		TPM1_C1V = 3750;  //pin1
	  TPM2_C0V = 3750;
		delay(0x8000000);
		
	}
}
