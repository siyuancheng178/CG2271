#include "MKL25z4.h"
#include "PWM.h"

#define PTD2_Pin 2
#define MUSICAL_NOTE_CNT 42
#define END_NOTE_CNT 14

#define FREQ_2_MOD(x) (375000 / x)

#define c 261
#define d 294
#define e 329
#define f 349
#define g 392
#define a 440
#define b 493

//int musical_notes[] = {c, c, g, g, a, a, g, f, f, e, e, d, d, c, g, g, f, f, e, e, d, g, g, f, f, e, e, d, c, c, g, g, a, a, g, f, f, e, e, d, d, c};
//int musical_end[] = {c, d, e, c, c, d, e, c, e, f, g, e, f, g};

void iniAudio(void)
{
	initPWM(PORTD, 2, 4, 0, 2);
	/*SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	
	PORTD->PCR[PTD2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD2_Pin] |= PORT_PCR_MUX(4);
	
	
	//enable clock gating for timer1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); //mcgfllclk
	
	// set module value 48000000 / 128 = 575000 /7500 = 50hz
	TPM0->MOD = 7500;
	
	
	//edge aligened pwm
	//update snc register: cmod=01 ps = 111(128)
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(4));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//enable pwm on tpm1 channel 0->ptb0
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK)| (TPM_CnSC_MSA_MASK));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));*/
	
}

/*
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
	iniPWM();
	TPM0_C2V = 0x0EA6;
	
	while(1)
	{
		for(i=0; i<MUSICAL_NOTE_CNT; i++)
		{
			TPM0->MOD = FREQ_2_MOD(musical_notes[i]);
			TPM0_C2V = FREQ_2_MOD(musical_notes[i]/2);
			delay(0xFFFFF);
			TPM0->MOD = FREQ_2_MOD(0);
			TPM0_C2V = FREQ_2_MOD(0);
			delay(0x3FFFF);
		}
		delay(0xFFFFF);
		
		for(i=0; i<END_NOTE_CNT; i++)
		{
			TPM0->MOD = FREQ_2_MOD(musical_end[i]);
			TPM0_C2V = FREQ_2_MOD(musical_end[i]/2);
			delay(0xFFFFF);
			TPM0->MOD = FREQ_2_MOD(0);
			TPM0_C2V = FREQ_2_MOD(0);
			delay(0x3FFFF);
		}
		delay(0xFFFFF);
	}
}
*/





