/*
 * Keyboard.h
 *
 * Created: 10/23/2015 7:56:02 PM
 *  Author: Willy
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

char PinCodeCheck()
{
	char tmp;//temporary variable
	char i;//temporary counter
	
	i = 4;//outputs start at 4 for row count// 0-3 are allocated for columns

	while(i<8)
	{
		if(pinCount >3)//if 4-digit pin-code entered do the following
		{
			USART_TX_MulticharLN("Pin Entered=");//print pin-code
			USART_TX_MulticharLN(pinCode);
			if(strcmp(ValidPin, pinCode) == 0)//if pin-code is valid
			{
				USART_TX_MulticharLN("Access Granted");//play corresponding tones by changing frequency on timer 3 (ICR3)
				ICR3 = F3;
				OCR3A = 700;//set volume by setting pulse width
				_delay_ms(400);
				ICR3 = A3s;
				_delay_ms(200);
				ICR3 = B3;
				_delay_ms(200);
				ICR3 = C4;
				_delay_ms(200);
				OCR3A = 0;//set pulse width to 0 so no volume
				
				pinCount = 0;//clear pin count
				memset(pinCode, 0, sizeof pinCode);//clear pincode array
				
				return 1;//return valid
			}
			else//if not valid pin-code
			{
				USART_TX_MulticharLN("Access Denied");//Play Fail Tone
				ICR3 = C3;
				OCR3A = 700;//set volume by setting pulse width
				_delay_ms(1500);
				OCR3A = 0;//set pulse width to 0 so no volume
				
				pinCount = 0;//clear pin count
				memset(pinCode, 0, sizeof pinCode);//clear pincode array
			}
		}
		tmp = ~(0x00 | 1<<i);//set corresponding row to 0 (i=4-->i=7)
		PORTA = tmp;//read specified 0 row
		
		if(PORTA == 0xEF)//if row 0
		{
			if(!(PINA & 0b00000001) )//S4 pressed
			{
				pinCount++;//increment pincode Count
				strcat(pinCode,"S4");//concatinate pincode string with s4
				USART_TX_MulticharLN("S4");//print s4 to screen
				while(!(PINA & 0x01))//while S4 is being pressed
				{
					ICR3 = A2;//play tone
					OCR3A = 700;//set volume
					_delay_ms(50);//small delay
				}
				OCR3A = 0;//when key is released stop tone
			}
			//The rest of the code is repetitive for all remaining buttons
			else if(!(PINA & 0x02))//S3 pressed
			{
				pinCount++;
				strcat(pinCode,"S3");
				USART_TX_MulticharLN("S3");
				while(!(PINA & 0x02))
				{
					ICR3 = A2s;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x04))//S2 pressed
			{
				pinCount++;
				strcat(pinCode,"S2");
				USART_TX_MulticharLN("S2");
				while(!(PINA & 0x04))
				{
					ICR3 = B2;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x08))//S1 pressed
			{
				pinCount++;
				strcat(pinCode,"S1");
				USART_TX_MulticharLN("S1");
				while(!(PINA & 0x08))
				{
					ICR3 = C3;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
		}
		else if(PORTA == 0xDF)//row 1
		{
			if(!(PINA & 0b00000001) )//S8 pressed
			{
				pinCount++;
				strcat(pinCode,"S8");
				USART_TX_MulticharLN("S8");
				while(!(PINA & 0x01))
				{
					ICR3 = E3;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x02))//S7 pressed
			{
				pinCount++;
				strcat(pinCode,"S7");
				USART_TX_MulticharLN("S7");
				while(!(PINA & 0x02))
				{
					ICR3 = D3s;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x04))//S6 pressed
			{
				pinCount++;
				strcat(pinCode,"S6");
				USART_TX_MulticharLN("S6");
				while(!(PINA & 0x04))
				{
					ICR3 = D3;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x08))//S5 pressed
			{
				pinCount++;
				strcat(pinCode,"S5");
				USART_TX_MulticharLN("S5");
				while(!(PINA & 0x08))
				{
					ICR3 = C3s;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			
		}
		else if(PORTA == 0xBF)//row 2
		{
			if(!(PINA & 0b00000001) )//S12 pressed
			{
				pinCount++;
				strcat(pinCode,"S12");
				USART_TX_MulticharLN("S12");
				while(!(PINA & 0x01))
				{
					ICR3 = G3s;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x02))//S11 pressed
			{
				pinCount++;
				strcat(pinCode,"S11");
				USART_TX_MulticharLN("S11");
				while(!(PINA & 0x02))
				{
					ICR3 = G3;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x04))//S10 pressed
			{
				pinCount++;
				strcat(pinCode,"S10");
				USART_TX_MulticharLN("S10");
				while(!(PINA & 0x04))
				{
					ICR3 = F3s;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x08))//S9 pressed
			{
				pinCount++;
				strcat(pinCode,"S9");
				USART_TX_MulticharLN("S9");
				while(!(PINA & 0x08))
				{
					ICR3 = F3;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			
		}
		else if(PORTA == 0x7F)// row 3
		{
			if(!(PINA & 0b00000001) )//S16 pressed
			{
				pinCount++;
				strcat(pinCode,"S16");
				USART_TX_MulticharLN("S16");
				while(!(PINA & 0x01))
				{
					ICR3 = C4;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;

			}
			else if(!(PINA & 0x02))//s15 pressed
			{
				pinCount++;
				strcat(pinCode,"S15");
				USART_TX_MulticharLN("S15");
				while(!(PINA & 0x02))
				{
					ICR3 = B3;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x04))
			{
				pinCount++;
				strcat(pinCode,"S14");
				USART_TX_MulticharLN("S14");
				while(!(PINA & 0x04))
				{
					ICR3 = A3s;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			else if(!(PINA & 0x08))//S13 pressed
			{
				pinCount++;
				strcat(pinCode,"S13");
				USART_TX_MulticharLN("S13");
				while(!(PINA & 0x08))
				{
					ICR3 = A3;
					OCR3A = 700;
					_delay_ms(50);
				}
				OCR3A = 0;
			}
			
		}
		i++;//increment row count
	}
	return 0;
	
}




#endif /* KEYBOARD_H_ */