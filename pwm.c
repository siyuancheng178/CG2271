#include "common.h"
#define PTB0_Pin 0 //Timer 1 Channel 0
#define PTB1_Pin 1 //Timer 1 Channel 1
#define PTB2_Pin 2 //Timer 2 Channel 0
#define PTB3_Pin 3 //Timer 2 Channel 1


void initPort(PORT_Type* port, uint8_t pin, uint8_t alt) {
	port -> PCR[pin] &= ~PORT_PCR_MUX_MASK;
	port -> PCR[pin] |= PORT_PCR_MUX(alt);
}

void enableTPM(uint8_t channel) {
	switch (channel) {
		case 0:
			SIM -> SCGC6 |= SIM_SCGC6_TPM0_MASK;
		  break;
		case 1:
			SIM -> SCGC6 |= SIM_SCGC6_TPM1_MASK;
		  break;
		case 2:
			SIM -> SCGC6 |= SIM_SCGC6_TPM2_MASK;
		  break;
	}
}

void set_clock_source() {
	SIM -> SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM -> SOPT2 |= SIM_SOPT2_TPMSRC(1);
}

void initTimer(int timer) {
	switch (timer) {
		case 0:
			TPM0 -> SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK);
		  TPM0 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(4));
	    TPM0 -> SC &= ~(TPM_SC_CPWMS_MASK);
		  TPM0 -> MOD = 7500;
		break;
		
		case 1:
			TPM1 -> SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK);
	    TPM1 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	    TPM1 -> SC &= ~(TPM_SC_CPWMS_MASK);
		  TPM1 -> MOD = 7500;
		break;
		
		case 2:
			TPM2 -> SC &= ~(TPM_SC_CMOD_MASK | TPM_SC_PS_MASK);
	    TPM2 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	    TPM2 -> SC &= ~(TPM_SC_CPWMS_MASK);
		  TPM2 -> MOD = 7500;
		break;
	}
}	
	


/* initialize channels for different timers. Timer 0 Channel 0 - 5, Timer 1 Channel 0 - 1,
and Timer 2 Channel 0 - 1 can be used */
void initChannel(int timer, int channel) {
	if (timer == 0) {
		switch(channel) {
			case 0:
				TPM0_C0SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
			
			case 1:
				TPM0_C1SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
			
			case 2:
				TPM0_C2SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
			
			case 3:
				TPM0_C3SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
			
			case 4:
				TPM0_C4SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
			
			case 5:
				TPM0_C5SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
		}
	}
	
	else if (timer == 1) {
		switch(channel) {
			case 0:
				TPM1_C0SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
			
			case 1:
				TPM1_C1SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
		}
	}
	
	else if (timer == 2) {
		switch(channel) {
			case 0:
				TPM2_C0SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
			
			case 1:
				TPM2_C1SC &= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);
			  TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
			break;
		}
	}
}
				
	








/*duty cycle is ranged between 0 and 7500. 7500 is 100% duty-cycle, and 0 means no PWM output.
Timer 0 Channel 0 - 5, Timer 1 Channel 0 - 1,
and Timer 2 Channel 0 - 1 can be used */
void set_duty_cycle(int duty_cycle, int timer, int channel) {
		if (timer == 0) {
		switch(channel) {
			case 0:
				TPM0_C0V = duty_cycle;
			break;
			
			case 1:
				TPM0_C1V = duty_cycle;
			break;
			
			case 2:
				TPM0_C2V = duty_cycle;
			break;
			
			case 3:
				TPM0_C3V = duty_cycle;
			break;
			
			case 4:
				TPM0_C4V = duty_cycle;
			break;
			
			case 5:
				TPM0_C5V = duty_cycle;
			break;
		}
	}
	
	else if (timer == 1) {
		switch(channel) {
			case 0:
				TPM1_C0V = duty_cycle;
			break;
			
			case 1:
				TPM1_C1V = duty_cycle;
			break;
		}
	}
	
	else if (timer == 2) {
		switch(channel) {
			case 0:
				TPM2_C0V = duty_cycle;;
			break;
			
			case 1:
				TPM2_C1V = duty_cycle;;
			break;
		}
	}
}


/* attention: before calling this function, remember to enable the clock gate for cooresponding pin
By default, it enables the PWM to full duty cycle
*/
void initPWM(PORT_Type* port, int pin, int alt, int timer, int channel) {
	initPort(port, pin, alt);
	enableTPM(timer);
	set_clock_source();
	initTimer(timer);
	initChannel(timer, channel);
	set_duty_cycle(0, timer, channel);
}

/*	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	initPWM(PORTB, PTB0_Pin, 3, 1, 0);
	initPWM(PORTB, PTB1_Pin, 3, 2, 0);
	initPWM(PORTB, PTB2_Pin, 3, 2, 1);
	initPWM(PORTB, PTB3_Pin, 3, 1, 1);
	//forward pin0(left) -> Timer1 channel 0;  pin3(right) -> Timer1 channel 1
  //backward: pin1(left) -> Timer2 Channel 0; pin2(right) -> Timer2 Channel 1;

	while (1) {
		set_duty_cycle(3750, 1, 0);
		set_duty_cycle(0, 1, 1);
		set_duty_cycle(3750, 2, 1);
		set_duty_cycle(0, 2, 0);
		delay(0x800000);
		set_duty_cycle(0, 1, 0);
		set_duty_cycle(3750, 1, 1);
		set_duty_cycle(0, 2, 1);
		set_duty_cycle(3750, 2, 0);
		delay(0x800000);

} */
