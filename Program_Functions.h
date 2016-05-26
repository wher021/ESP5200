/*
 * ProgramFunctions.h
 *
 * Created: 10/23/2015 8:35:37 PM
 *  Author: Willy
 */ 


#ifndef PROGRAM_FUNCTIONS_H_
#define PROGRAM_FUNCTIONS_H_

char KeyCardCheck();//keycard decoding + check
void ServoController();//State 3 implementation, provides manual control + record function
void playFunction();//playback function

long mapPosition(unsigned long x, long in_min, long in_max, long out_min, long out_max);//map function (converts PWM pulses to degress);

char buffer [10];//temporary buffer to hold conversion values from the mapping function

void ServoController()
{
	if(!play)//if playback mode is inactive
	{
		
		analogVoltage = (3000/255)*raw_Analog_values;//scale raw analog values giving maximum output of 3000
		
		if(abs(analogVoltage - previousAnalogVoltage) > 2 && analogVoltage > 700)//filter values from noise and duplications
		{
			OCR1A = analogVoltage;//write scaled servo positions to OCR1A
			
			int mapped_value = mapPosition(OCR1A,750,2700,0,180);//map values ranging (750-2700 us to 0,180 deg)
			itoa (mapped_value,buffer,10);//convert integer and store into temporary buffer char array
			USART_TX_Multichar(buffer);//print servo positions in degrees
			USART_TX_MulticharLN(" deg");
							
			if(record && recordIndex < 2000)//if record mode is active and recorded samples < 2000
			{
				write_eeprom_word(&my_eeprom_array[recordIndex], analogVoltage);//store servo positions to EEPROM
				recordIndex++;
			}
			else if(record && recordIndex >= 2000)//else if record mode and greater equal to 2000 samples
			{
				write_eeprom_word(&my_eeprom_array[recordIndex], NULL);//write null byte
				USART_TX_MulticharLN("EEPROM FULL");//Tell user EEPROM is full
				record = 0;//stop record mode
			}
		}
		
		previousAnalogVoltage = analogVoltage;//save previous analog voltages for filtering
	}
	else//if playback mode is active
	{
		playFunction();//call playback function
		play = 0;//stop playback
	}
}

void playFunction()
{
	int i =0;//sample counter
	while(read_eeprom_word(&my_eeprom_array[i]) != NULL)//read servo positions from EEPROM until NULL byte
	{
		if(!play)//break from playback
		{
			break;
		}
		OCR1A = read_eeprom_word(&my_eeprom_array[i]);//write servo positions to OCR1A
		
		int mapped_value = mapPosition(OCR1A,750,2700,0,180);//map values ranging (750-2700 us to 0,180 deg)
		itoa (mapped_value,buffer,10);//convert integer and store into temporary buffer char array
		USART_TX_Multichar(buffer);//print servo positions in degrees
		USART_TX_MulticharLN(" deg");
					
		_delay_ms(10);//100ms delay between each sample
		i++;
	}
	USART_TX_MulticharLN("Finished Playback");
}

char KeyCardCheck()//check RFID card
{
	byte = mfrc522_request(PICC_REQALL,str);//has a timeout of 1 sec to hold processor
	
	if(byte == CARD_FOUND)//If card is found, byte == 1
	{
		byte = mfrc522_get_card_serial(str);//get card serial and store into byte array called str

		if(byte == CARD_FOUND)
		{
			char tmp[MAX_LEN];//create temporary char array of maximum possible serial number
			char SerialNumber[MAX_LEN];//create a large char array to hold the converted serial number
			memset(SerialNumber, 0, sizeof SerialNumber);//clear saved SerialNumber

			char i = 0;//counter
			USART_TX_MulticharLN("RFID Serial=");
			while(i<5)//total 5 byte sets will be 10 hex digits: "99 223 224 213 137"
			{
				itoa(str[i], tmp, 10);//convert each decimal number to characters and store in tmp char array
				strcat(SerialNumber,tmp);//append each set of characters to SerialNumber
				i++;
				
			}
			i = 0;//counter = 0
			
			while(SerialNumber[i] != '\0')//while not Null byte print each character
			{
				USART_TX_Singlechar(SerialNumber[i]);
				i++;
			}
			USART_TX_MulticharLN("");//print new line and return
			
			//Check if decoded card is valid
			if(strcmp(SerialNumber,ValidSerial) == 0)//if valid
			{
				USART_TX_MulticharLN("Access Granted");
				//Play Following tones by writing corresponding tone frequency to ICR3
				ICR3 = F3;
				OCR3A = 700;//PWM determines buzzer volume on Timer 3
				_delay_ms(400);
				ICR3 = A3s;
				_delay_ms(200);
				ICR3 = B3;
				_delay_ms(200);
				ICR3 = C4;
				_delay_ms(200);
				OCR3A = 0;//0 pulse length on Timer 3 = no volume

				return 1;
			}
			else//if not valid key-card play error tone
			{
				USART_TX_MulticharLN("Access Denied");
				ICR3 = C3;
				OCR3A = 700;
				_delay_ms(1500);
				OCR3A = 0;
				
				return 0;
			}
			USART_TX_Singlechar('\n');
			USART_TX_Singlechar('\r');
		}//end card found
		else
		{
			USART_TX_MulticharLN("Error Reading Card");
		}
	}
	else
	{
		//card not found
		return 0;
	}
}

//map function
long mapPosition(unsigned long x, long in_min, long in_max, long out_min, long out_max)
{
	unsigned int mapped = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	if(x <= 750)//if inputed value is less than 750, default it to lowest mapped 0 value
	{
		mapped = 0;
	}
	return mapped;
}


#endif /* PROGRAM_FUNCTIONS_H_ */