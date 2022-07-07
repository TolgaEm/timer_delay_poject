

#include "TM4C123.h"                    

#define RED (1U<<1); // PORTF1

/*

1. Ensure the timer is disabled (the TnEN bit in the GPTMCTL register is cleared) before making
any changes.

2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000.

3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
a. Write a value of 0x1 for One-Shot mode.
b. Write a value of 0x2 for Periodic mode.

4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
to select whether to capture the value of the free-running timer at time-out, use an external
trigger to start counting, configure an additional trigger or interrupt, and count up or down.

5. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).

6. If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register
(GPTMIMR).

7. Set the TnEN bit in the GPTMCTL register to enable the timer and start counting.

8. Poll the GPTMRIS register or wait for the interrupt to be generated (if enabled). In both cases,
the status flags are cleared by writing a 1 to the appropriate bit of the GPTM Interrupt Clear
Register (GPTMICR).


*/
void timer1_oneshot(uint16_t );

int main(){
	
	SYSCTL->RCGCGPIO |= (1U<<5);
	GPIOF->DIR |= RED;
	GPIOF->DEN |= RED;
	
	
	while(1){
		
		GPIOF->DATA ^= RED; // toggle red led each 4 milisecond
		timer1_oneshot(4);
	}
	
	
}

void timer1_oneshot(uint16_t mdelay){
	
	SYSCTL->RCGCTIMER |= (1U<<0); // clock enable for timer0
	TIMER0->CTL = 0x00; //BEFORE CONFIGURATION WE MUST DISABLE TIMER0
	TIMER0->CFG |= 0X04; // SET 16-BIT TIMER
	TIMER0->TAMR |= 0X01; // SET ONESHOT MODE
	TIMER0->TAILR = 16000 *mdelay - 1; // LOAD TIME VALUE 
	TIMER0->ICR |= (1<<0); // CLEAR 
	TIMER0->CTL = 0x01; // ENABLE TIMER0 AND READY TO COUNT
	
	while((TIMER0->RIS & 0X01) == 0); // WAIT FOR TIMEOUT FLAG TO SET

}
