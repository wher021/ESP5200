/*
 * Timer1.h
 *
 * Created: 10/23/2015 7:52:16 PM
 *  Author: Willy
 */ 


#ifndef TIMER1_H_
#define TIMER1_H_

#include <avr/interrupt.h>

void InitialiseTimer1_FastPWM_ChannelA()//Servo PWM config
{
	// TCCR1A – Timer/Counter 1 Control Register A
	// Bit 7:6 – COMnA1:0: Compare Output Mode for Channel A (For FAST PWM 10 = Clear OC3A on Compare match (Non-Inverting))
	// Bit 5:4 – COMnB1:0: Compare Output Mode for Channel B (For FAST PWM 10 = Clear OC3B on Compare match (Non-Inverting))
	// Bit 3:2 – COMnC1:0: Compare Output Mode for Channel C (For FAST PWM 10 = Clear OC3C on Compare match (Non-Inverting))
	// Bit 1:0 – WGMn1:0: Waveform Generation Mode (Waveform bits WGM3(3..0) 1110 Fast PWM ICR3 is TOP)
	TCCR1A = 0b10100010;	// Fast PWM channel A&B non inverting, ICR3 used as TOP
	
	// TCCR1B – Timer/Counter 1 Control Register B
	// Bit 7 – ICNCn: Input Capture Noise Canceler
	// Bit 6 – ICESn: Input Capture Edge Select
	// Bit 5 – Reserved Bit
	// Bit 4:3 – WGMn3:2: Waveform Generation Mode
	// Bit 2:0 – CSn2:0: Clock Select
	TCCR1B = 0b00011001;	// Fast PWM, Use Prescaler '1'

	// TCCR1C – Timer/Counter 1 Control Register C
	// Bit 7 – FOCnA: Force Output Compare for Channel A
	// Bit 6 – FOCnB: Force Output Compare for Channel B
	// Bit 5 – FOCnC: Force Output Compare for Channel C
	TCCR1C = 0b00000000;

	// Set Timer/Counter1 Input Capture Register (16 bit) ICR1
	// Can only be written to when using a waveform generation mode that uses ICR1 to define the TOP value
	
	//Regular Motor can use full register for setting top Value
	//ICR3H = 0xFF; // 16-bit access (write high byte first, read low byte first)
	//ICR3L = 0xFF;
	//Servo However requires approximately 20ms = 50hz period//andra den har beroende pa tone!!!
	ICR1H = 0x4E; //Set Frequency
	ICR1L = 0x20;

	// Set Timer/Counter count/value registers (16 bit) TCNT1H and TCNT1L
	TCNT1H = 0; // 16-bit access (write high byte first, read low byte first)
	TCNT1L = 0;

	// Initialise Channel A to motor high-speed setting (to overcome stall speed on program startup)
	// Set Timer/Counter Output Compare Registers (16 bit) OCR3AH and OCR3AL
	OCR1A = 700;//0x2800;

	// TIMSK3 – Timer/Counter 3 Interrupt Mask Register
	// Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
	// Bit 3 – OCIEnC: Timer/Countern, Output Compare C Match Interrupt Enable
	// Bit 2 – OCIEnB: Timer/Countern, Output Compare B Match Interrupt Enable
	// Bit 1 – OCIEnA: Timer/Countern, Output Compare A Match Interrupt Enable
	// Bit 0 – TOIEn: Timer/Countern, Overflow Interrupt Enable
	TIMSK1 = 0b00000000;	// No interrupts needed, PWM pulses appear directly on OC3A (Port E Bit3)
	
	// TIFR1 – Timer/Counter1 Interrupt Flag Register is not needed in Fast PWM mode
	TIFR1 = 0b00101111;// Clear all interrupt flags//not neccesary in PWM
}

void InitialiseTimer1_Timeout_ChannelA()//Timer 1 Timeout config
{
		TCCR1A = 0b00000000;	// Normal port operation (OC1A, OC1B, OC1C), Clear Timer on 'Compare Match' (CTC) waveform mode)
		TCCR1B = 0b00000010;	// Normal mode need to clear manually (but will be automatically cleared in OVF INT), use prescaler 8
		TCCR1C = 0b00000000;

		TCNT1H = 0b00000000;	// Timer/Counter count/value registers (16 bit) TCNT1H and TCNT1L
		TCNT1L = 0b00000000;
		
		TIMSK1 = 0b00000001;	// bit 0 TOIE1 Use 'Overflow' Interrupt, i.e. generate an interrupt
		
		USART_TX_MulticharLN("Timeout started!");
}

ISR(TIMER1_OVF_vect) // TIMER1_Overflow_Handler (Interrupt Handler for Timer 1)//works well in Normal MODE
{//overflow every 0.5	
	if(timeout_counter > 20)//20 * 0.5 = 10 sec
	{
		USART_TX_MulticharLN("Timeout!");
		stopButton = 1;
		timeout_counter = 0;
		TIMSK1 = 0b00000000;
	}
	timeout_counter++;
}

#endif /* TIMER1_H_ */