#include "MKL25z4.h"
#include "PWM.h"
#define PTB0_Pin 0
#define PTB1_Pin 1
#define MUSICAL_NOTE_CNT 7
#define FREQ_2_MOD(x) (375000 / x)

int musical_notes[] = {256, 256, 384, 384, 427, 427, 384};
int musical_end[] = {341, 341, 320, 320, 288, 288, 256};

void initPWM(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	//enable clock gating for timer1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //mcgfllclk
	
	// set module value 48000000 / 128 = 575000 /7500 = 50hz
	TPM1->MOD = 7500;
	
	
	//edge aligened pwm
	//update snc register: cmod=01 ps = 111(128)
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//enable pwm on tpm1 channel 0->ptb0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK)| (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
}


static void delay (volatile uint32_t nof)
{
	while(nof!=0)
	{
		__asm("NOP");
		nof--;
	}
}

int main(void)
{
	char i=0;
	SystemCoreClockUpdate();
	initPWM();
	TPM1_C0V = 0x0EA6;
	
	while(1)
	{
		for(i=0; i<MUSICAL_NOTE_CNT; i++)
		{
			TPM1->MOD = FREQ_2_MOD(musical_notes[i])*2;
			TPM1_C0V = FREQ_2_MOD(musical_notes[i]);
			delay(0xFFFF);
		}
		TPM1_C0V++;
		delay(0xFFFF);
	}
}





