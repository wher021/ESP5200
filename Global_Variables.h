/*
 * Global_Variables.h
 *
 * Created: 10/23/2015 6:17:34 PM
 *  Author: Willy
 */ 


#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

//Volatile Variables!!!
volatile char stopButton = 0;
volatile char robotArm = 0;


//=========EEPROM======================
// macro for easier usage
#define read_eeprom_word(address) eeprom_read_word ((const uint16_t*)address)
#define write_eeprom_word(address,value) eeprom_write_word ((uint16_t*)address,(uint16_t)value)
#define update_eeprom_word(address,value) eeprom_update_word ((uint16_t*)address,(uint16_t)value)

//declare an eeprom array
unsigned int EEMEM  my_eeprom_array[2000];
volatile unsigned char record = 0;
volatile unsigned char play = 0;
volatile unsigned int recordIndex = 0;
//========================================


//RFID Card Variables
const char* ValidSerial = "99223224213137";//pre-defined valid access card
uint8_t byte;//RFID status flag
uint8_t str[MAX_LEN];//RFID raw byte string

//Keypad Variables
#define ValidPin "S11S5S5S1"//S11 S5 S1 S1
char pinCode[12];
char pinCount = 0;//pincode counter
char timeout_counter = 0;//timeout counter

//Servo Control Variables//
uint16_t analogVoltage,previousAnalogVoltage,raw_Analog_values;
char ServoPulseUs[10];

//System Status RGB LIGHT//
#define Red 0x01;//Waiting for valid KeyCard
#define Green 0x02;//Waiting for valid PinNr
#define Blue 0x04;//Robot Arm Active!



#endif /* GLOBAL_VARIABLES_H_ */