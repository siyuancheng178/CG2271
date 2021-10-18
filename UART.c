#include "MKL25Z4.h"                    // Device header
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define BAUD_RATE 9600

void initUART2(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	SIM -> SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTE -> PCR[UART_TX_PORTE22] &= ~(PORT_PCR_MUX_MASK);
	PORTE -> PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);
	
	PORTE -> PCR[UART_RX_PORTE23] &= ~(PORT_PCR_MUX_MASK);
	PORTE -> PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	UART2 -> C2 &= ~((UART_C2_TE_MASK) & (UART_C2_RE_MASK));
	
	bus_clock = (DEFAULT_SYSTEM_CLOCK) / 2;
	divisor = bus_clock / (16 * baud_rate);
	
	UART2 -> BDH = (divisor >> 8);
	UART2 -> BDL = divisor;
	
	UART2 -> C1 = 0;
	UART2 -> S2 = 0;
	UART2 -> C3 = 0;
	
	UART2 -> C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}

void UART_Transmit_Poll(uint8_t data) {
	while (!(UART2 -> S1 & UART_S1_TDRE_MASK));
	UART2 -> D = data;
}

uint8_t UART_Receive_Poll() {
	while (!(UART2 -> S1 & UART_S1_RDRF_MASK));
	return (UART2->D);
}

static void delay(volatile uint32_t nof) {
	while (nof != 0) {
		__asm("NOP");
		nof--;
	}
}

int main() {
	uint8_t rx_data = 0x69;
	SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	
	while (1) {
		UART_Transmit_Poll(0x69);
		delay(0x80000);
	}
}


