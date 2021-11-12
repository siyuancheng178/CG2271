#include "MKL25z4.h"
#include "PWM.h"
#include "ShareVariable.h"

#define PTD2_Pin 2
#define MUSICAL_NOTE_CNT 48
#define END_NOTE_CNT 29

#define FREQ_2_MOD(x) (375000 / x)

#define skip 375000
#define lc 1046
#define ld 1175
#define le 1319
#define lf 1397
#define lg 1568
#define la 1760
#define lb 1976
#define c 2093
#define d 2349
#define e 2637
#define f 2794
#define g 3136
#define a 3520
#define b 3951
#define hc 4186
#define hd 4699
#define he 5274
#define hf 5588
#define hg 6272
#define ha 7040
#define hb 7902

int musical_start[] = {he, hf, hg, ha};
int musical_notes[] = {lc, lc, lg, lg, la, la, lg, skip, lf, lf, le, le, ld, ld, lc, skip, lg, lg, lf, lf, le, le, ld, skip, lg, lg, lf, lf, le, le, ld, skip, lc, lc, lg, lg, la, la, lg, skip, lf, lf, le, le, ld, ld, lc, skip};
int musical_end[] = {lg, lg, la, lg, c, lb, skip, lg, lg, la, lg, d, c, skip, lg, lg, g, e, c, lb, la, skip, f, f, e, c, d, c, skip};


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

void audio_connect(void* Argument) {
	for (int i = 0; i < 4; i++) {
	    TPM0->MOD = FREQ_2_MOD(musical_start[i]);
	    TPM0_C2V = FREQ_2_MOD(musical_start[i])/2; 
	    osDelay(500);
	    TPM0_C2V = 0;
	    osDelay(100);
	}
	osSemaphoreRelease(connect_event);
	osSemaphoreRelease(connect_event);
	osSemaphoreRelease(connect_event);
	osSemaphoreRelease(connect_event);
	osSemaphoreRelease(connect_event);
	osSemaphoreRelease(connect_event);
	osSemaphoreRelease(connect_event);
	for(;;) {}
}

void audio_thread(void* Argument) {
	osSemaphoreAcquire(connect_event, osWaitForever);
	int i = 0, j= 0;
	
	for(;;) {
		if (end == 0) {
			TPM0->MOD = FREQ_2_MOD(musical_notes[i]);
			TPM0_C2V = FREQ_2_MOD(musical_notes[i])/2; 
			osDelay(500);
			TPM0_C2V = 0;
			osDelay(100);
			i++;
			if (i == MUSICAL_NOTE_CNT) i = 0;
			
		} else {
			TPM0->MOD = FREQ_2_MOD(musical_end[j]);
			TPM0_C2V = FREQ_2_MOD(musical_end[j])/2;
			osDelay(500);
			TPM0_C2V = 0;
			osDelay(100);
			j = (j + 1) % END_NOTE_CNT;
		}
	}
}



