/*
* HW_Interrupts.h
*
* Created: 10/24/2015 2:19:11 PM
*  Author: Willy
*/


#ifndef HW_INTERRUPTS_H_
#define HW_INTERRUPTS_H_

void Initialise_HW_Interrupts()
{
	//INT0 = System Reset
	//INT1 = Robot Movement Record
	//INT2 = Robot Playback
	EICRA = 0b00101010;		// INT 3 not used, Interrupt Sense (INT0,INT1,INT2) falling-edge triggered
	EICRB = 0b00000000;		// INT7 ... 4 not used
	
	EIMSK = 0b00000111;		// Enable INT0,INT1,INT2
	EIFR = 0b00000111;		// Clear INT0,INT1,INT2 interrupt flags (in case a spurious interrupt has occurred during chip startup)
}

ISR(INT0_vect) // Hardware_INT0_Handler (Interrupt Handler for INT0)
{
	PORTC = Red;//set red RGB light
	stopButton = 1;
	record = 0;
	play = 0;
	robotArm = 0;//robot active state 3 is disabled
	TIMSK1 = 0b00000000; //stop Timer1 INT overflow vector for timeout
	TCCR1A = 0b00000000; //stop all Timer1 outputs
	timeout_counter = 0;//clear timeout
}

ISR(INT1_vect) // Hardware_INT1_Handler (Interrupt Handler for INT1)
{
	if(robotArm)//if state 3
	{
		if(!play)//if playback is inactive
		{
			if(!(PIND&(1<<PD1)))//if PD1 was pressed (active low)
			{
				record = !record;//invert record mode
				if(record)//if record mode is enabled
				{
					recordIndex = 0;//reset sample index
					USART_TX_MulticharLN("Recording");//record mode started
				}
				else//else if recording stopped
				{
					write_eeprom_word(&my_eeprom_array[recordIndex], NULL);//write null byte
					USART_TX_MulticharLN("Stopped Recording");//record mode stopped
				}
			}
		}
	}
	_delay_ms(20);//debounce delay
}

ISR(INT2_vect) // Hardware_INT2_Handler (Interrupt Handler for INT2)
{
	if(robotArm)
	{
		if(!record)
		{
			if(!(PIND&(1<<PD2)))//if PD2 was pressed (active low)
			{
				play = !play;//invert play mode
				if(play)//if playback mode is enabled
				{
					USART_TX_MulticharLN("Started Playback");
				}
				else//if playback mode is disabled
				{
					USART_TX_MulticharLN("Stopped Playback");
				}
			}
		}
	}
	_delay_ms(20);//debounce delay
}

#endif /* HW_INTERRUPTS_H_ */