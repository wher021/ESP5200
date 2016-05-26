/*
* Embedded_Systems_Project.c
*
* Created: 10/12/2015 2:40:48 PM
*  Author: Willy
*/
//Official Libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <string.h>

//Lookup Tables
#include "RFID_Register_Set/register_lookup.h"
#include "Tone_Lookup.h"

//Modules//
#include "Global_Variables.h"
#include "ADC.h"
#include "Uart.h"
#include "Timer1.h"
#include "Timer3.h"
#include "Keyboard.h"
#include "Program_Functions.h"
#include "HW_Interrupts.h"

void InitialiseGeneral();


int main()
{
	Initialise_HW_Interrupts();//External switches init
	Initialise_ADC();//ADC Init
	USART_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();//UART init
	InitialiseTimer3_FastPWM_ChannelA();//Piezo init

	spi_init();//external Spi protocol init
	mfrc522_init();//external RFID reader init
	
	InitialiseGeneral();//General Register setup Init
	

	while(1)//Main State Machine
	{
		USART_TX_MulticharLN("Welcome to Willy's Embedded Systems Project");//Welcome message
		USART_TX_MulticharLN("Use your Access card to activate pin input");
		
		char valid = 0;//clear valid status flag
		stopButton = 0;//clear reset flag
		PORTC = Red;//inactive state, rgb module lights red
		
		while(!valid && !stopButton)//State 1
		{	
			valid = KeyCardCheck();//wait until valid access card has been presented or reset has been pressed
		}
		
		if(!stopButton)
		{
			valid = 0;//clear valid flag
			PORTC = Blue//rgb module lights blue
			USART_TX_MulticharLN("Enter 4-digit pin");
			InitialiseTimer1_Timeout_ChannelA();//initialize Timer1 timeout mode for state 2
		}
		
		while(!valid && !stopButton)//State 2 with 10 sec timeout before stopButton flag is activated
		{
			valid = PinCodeCheck();
		}
		
		if(!stopButton)
		{	
			robotArm = 1;//signal robotArm flag is active
			PORTC = Green;//rgb module lights green
			InitialiseTimer1_FastPWM_ChannelA();//initialize Timer1 PWM mode for servo
			USART_TX_MulticharLN("The Robot Arm is now active, use POT to move robot arm");
		}

		while (!stopButton)//State 3
		{
			ServoController();//manage servo controller while reset is inactive
		}
		
	}//end endless while
}//end of program

void InitialiseGeneral()
{
	//Scale Main Clock to 1Mhz
	CLKPR = 0x80;//(1<<CLKPCE);//Enable clock pre-scaler
	CLKPR = 0b00000100;//scale clock to 1 Mhz
	
	//Keypad Registers
	DDRA = 0xF0;//columns input//rows output
	PORTA = 0xFF;// set pullups for columns and set row output to be high initially
	
	//Timer 3 Buzzer Registers
	DDRE = 0xFF;// Port E bit 3 must be set as OUTPUT to provide the PWM pulse on OC3A
	// Port E bit 7 Input Capture 3 Pin (ICP3) must be set as OUTPUT to prevent
	// random / noise values entering ICR3 (ICR3 used as TOP value for PWM counter)
	PORTE = 0x00;// Set all bits initially
	
	//Timer 1 Servo Robotic Arm
	DDRB |= 0x20;//set PB5 as output OC1A pin
	PORTB &= ~(0x20);//set PB5 output to low
	
	//RGB Status Led
	DDRC = 0xFF;
	PORTC = 0x00;//Pin 0 = Red, Pin 1 = Green, Pin 2 = Blue
	
	//HW interrupts set as Inputs and attach Pullups
	DDRD = 0<<PD0;//INT0//Reset
	PORTD = 1<<PD0;
	
	DDRD = 0<<PD1;//INT1//Record
	PORTD = 1<<PD1;
	
	DDRD = 0<<PD2;//INT2//Playback
	PORTD = 1<<PD2;
	
	sei();// Enable interrupts at global level set Global Interrupt Enable (I) bit
}
