#include "MKL25Z4.h"                    // Device header
//#define UART_TX_PORTC3 3
#define UART_RX_PORTE23 23
#define BAUD_RATE 9600


void initUART2() {
	uint32_t divisor, bus_clock;
	
	SIM -> SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTE -> PCR[UART_RX_PORTE23] &= ~(PORT_PCR_MUX_MASK);
	PORTE -> PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	UART2 -> C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	bus_clock = (DEFAULT_SYSTEM_CLOCK) / 2;
	divisor = bus_clock / (16 * BAUD_RATE);
	
	UART2 -> BDH = (divisor >> 8);
	UART2 -> BDL = divisor;
	
	UART2 -> C1 = 0;
	UART2 -> S2 = 0;
	UART2 -> C3 = 0;
	
	UART2 -> C2 |= UART_C2_RE_MASK;
	NVIC_SetPriority(UART2_IRQn, 128);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	UART2 -> C2 |= UART_C2_RIE_MASK;
}

/*
void UART2_IRQHandler() {
	NVIC_ClearPendingIRQ(UART1_IRQn);
	
	if (UART2 -> S1 & UART_S1_RDRF_MASK) {
		receive_data = UART2 -> D;
	}
	
}
*/
/*
int main() {
	uint8_t rx_data = 0x69;
	SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	
	while (1) {
	}
}
*/

