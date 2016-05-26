/*
 * Timer2.h
 *
 * Created: 10/23/2015 7:52:26 PM
 *  Author: Willy
 */ 


#ifndef TIMER3_H_
#define TIMER3_H_

void InitialiseTimer3_FastPWM_ChannelA()//for Piezo element
{
	// TCCR3A – Timer/Counter 3 Control Register A
	// Bit 7:6 – COMnA1:0: Compare Output Mode for Channel A (For FAST PWM 10 = Clear OC3A on Compare match (Non-Inverting))
	// Bit 5:4 – COMnB1:0: Compare Output Mode for Channel B (For FAST PWM 10 = Clear OC3B on Compare match (Non-Inverting))
	// Bit 3:2 – COMnC1:0: Compare Output Mode for Channel C (For FAST PWM 10 = Clear OC3C on Compare match (Non-Inverting))
	// Bit 1:0 – WGMn1:0: Waveform Generation Mode (Waveform bits WGM3(3..0) 1110 Fast PWM ICR3 is TOP)
	TCCR3A = 0b10100010;// Fast PWM channel A&B non inverting, ICR3 used as TOP (channel B is not being used)
	
	// TCCR3B – Timer/Counter 3 Control Register B
	// Bit 7 – ICNCn: Input Capture Noise Canceler
	// Bit 6 – ICESn: Input Capture Edge Select
	// Bit 5 – Reserved Bit
	// Bit 4:3 – WGMn3:2: Waveform Generation Mode
	// Bit 2:0 – CSn2:0: Clock Select
	TCCR3B = 0b00011001;	// Fast PWM, Use Prescaler '1'

	// TCCR3C – Timer/Counter 3 Control Register C
	// Bit 7 – FOCnA: Force Output Compare for Channel A
	// Bit 6 – FOCnB: Force Output Compare for Channel B
	// Bit 5 – FOCnC: Force Output Compare for Channel C
	TCCR3C = 0b00000000;

	// Set Timer/Counter3 Input Capture Register (16 bit) ICR3
	// Can only be written to when using a waveform generation mode that uses ICR3 to define the TOP value
	
	//Regular Motor can use full register for setting top Value
	//ICR3H = 0xFF; // 16-bit access (write high byte first, read low byte first)
	//ICR3L = 0xFF;
	//Servo However requires approximately 20ms = 50hz period//andra den har beroende pa tone!!!
	ICR3H = 0x0E; // Sets period, F = 261 Hz tone C
	ICR3L = 0xF7;

	// Set Timer/Counter count/value registers (16 bit) TCNT1H and TCNT1L
	TCNT3H = 0; // 16-bit access (write high byte first, read low byte first)
	TCNT3L = 0;

	// Initialise Channel B to motor high-speed setting (to overcome stall speed on program startup)
	// Set Timer/Counter Output Compare Registers (16 bit) OCR3AH and OCR3AL
	OCR3A = 0; //Sets Volume

	// TIMSK3 – Timer/Counter 3 Interrupt Mask Register
	// Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
	// Bit 3 – OCIEnC: Timer/Countern, Output Compare C Match Interrupt Enable
	// Bit 2 – OCIEnB: Timer/Countern, Output Compare B Match Interrupt Enable
	// Bit 1 – OCIEnA: Timer/Countern, Output Compare A Match Interrupt Enable
	// Bit 0 – TOIEn: Timer/Countern, Overflow Interrupt Enable
	TIMSK3 = 0b00000000;	// No interrupts needed, PWM pulses appear directly on OC3A (Port E Bit3)
	
	// TIFR3 – Timer/Counter3 Interrupt Flag Register, not necessary in PWM
	TIFR3 = 0b00101111;		// Clear all interrupt flags
}




#endif /* TIMER3_H_ */