/*
 * RFID_Reader_Atmega.c
 *
 * Created: 5/9/2022 12:45:52 PM
 * Author : Ghaith
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/delay.h>
#include "USART0_Interrupt.h"
#include "mfrc522.h"
#include "millis.h"
uint8_t value_data[128] = "";
int k = 0;
int serial_number = 0;
int msg_len = 0;
unsigned char read_flag = 0x00;
int i = 0, j = 0;

#ifndef MFRC522_Card_Reader_Functions

unsigned char read_rfid_value(uint8_t rfid_val[], unsigned long timeout)
{
	unsigned long now = millis();
	uint8_t byte = 0x00;
	while(millis() - now <= timeout)
	{
		byte = mfrc522_request(PICC_REQALL,rfid_val);
		//USART0_TxChar(byte);
		
		if(byte == CARD_FOUND)
		{
			byte = mfrc522_get_card_serial(rfid_val);
			if(byte == CARD_FOUND)
			{
				return 1;
			}
			else
			{
				//USART0_SendString("Error\n");
				continue;
			}
		}
	}
	return 0;
}

#endif

#ifndef dealing_with_hex

char value_2[2];
char value_4[4];
char value_8[8];
char value_16[16];
unsigned int low, high;
unsigned long int lowl, highl;

static const uint16_t crctab16[] =
{
	0X0000, 0X1189, 0X2312, 0X329B, 0X4624, 0X57AD, 0X6536, 0X74BF,
	0X8C48, 0X9DC1, 0XAF5A, 0XBED3, 0XCA6C, 0XDBE5, 0XE97E, 0XF8F7,
	0X1081, 0X0108, 0X3393, 0X221A, 0X56A5, 0X472C, 0X75B7, 0X643E,
	0X9CC9, 0X8D40, 0XBFDB, 0XAE52, 0XDAED, 0XCB64, 0XF9FF, 0XE876,
	0X2102, 0X308B, 0X0210, 0X1399, 0X6726, 0X76AF, 0X4434, 0X55BD,
	0XAD4A, 0XBCC3, 0X8E58, 0X9FD1, 0XEB6E, 0XFAE7, 0XC87C, 0XD9F5,
	0X3183, 0X200A, 0X1291, 0X0318, 0X77A7, 0X662E, 0X54B5, 0X453C,
	0XBDCB, 0XAC42, 0X9ED9, 0X8F50, 0XFBEF, 0XEA66, 0XD8FD, 0XC974,
	0X4204, 0X538D, 0X6116, 0X709F, 0X0420, 0X15A9, 0X2732, 0X36BB,
	0XCE4C, 0XDFC5, 0XED5E, 0XFCD7, 0X8868, 0X99E1, 0XAB7A, 0XBAF3,
	0X5285, 0X430C, 0X7197, 0X601E, 0X14A1, 0X0528, 0X37B3, 0X263A,
	0XDECD, 0XCF44, 0XFDDF, 0XEC56, 0X98E9, 0X8960, 0XBBFB, 0XAA72,
	0X6306, 0X728F, 0X4014, 0X519D, 0X2522, 0X34AB, 0X0630, 0X17B9,
	0XEF4E, 0XFEC7, 0XCC5C, 0XDDD5, 0XA96A, 0XB8E3, 0X8A78, 0X9BF1,
	0X7387, 0X620E, 0X5095, 0X411C, 0X35A3, 0X242A, 0X16B1, 0X0738,
	0XFFCF, 0XEE46, 0XDCDD, 0XCD54, 0XB9EB, 0XA862, 0X9AF9, 0X8B70,
	0X8408, 0X9581, 0XA71A, 0XB693, 0XC22C, 0XD3A5, 0XE13E, 0XF0B7,
	0X0840, 0X19C9, 0X2B52, 0X3ADB, 0X4E64, 0X5FED, 0X6D76, 0X7CFF,
	0X9489, 0X8500, 0XB79B, 0XA612, 0XD2AD, 0XC324, 0XF1BF, 0XE036,
	0X18C1, 0X0948, 0X3BD3, 0X2A5A, 0X5EE5, 0X4F6C, 0X7DF7, 0X6C7E,
	0XA50A, 0XB483, 0X8618, 0X9791, 0XE32E, 0XF2A7, 0XC03C, 0XD1B5,
	0X2942, 0X38CB, 0X0A50, 0X1BD9, 0X6F66, 0X7EEF, 0X4C74, 0X5DFD,
	0XB58B, 0XA402, 0X9699, 0X8710, 0XF3AF, 0XE226, 0XD0BD, 0XC134,
	0X39C3, 0X284A, 0X1AD1, 0X0B58, 0X7FE7, 0X6E6E, 0X5CF5, 0X4D7C,
	0XC60C, 0XD785, 0XE51E, 0XF497, 0X8028, 0X91A1, 0XA33A, 0XB2B3,
	0X4A44, 0X5BCD, 0X6956, 0X78DF, 0X0C60, 0X1DE9, 0X2F72, 0X3EFB,
	0XD68D, 0XC704, 0XF59F, 0XE416, 0X90A9, 0X8120, 0XB3BB, 0XA232,
	0X5AC5, 0X4B4C, 0X79D7, 0X685E, 0X1CE1, 0X0D68, 0X3FF3, 0X2E7A,
	0XE70E, 0XF687, 0XC41C, 0XD595, 0XA12A, 0XB0A3, 0X8238, 0X93B1,
	0X6B46, 0X7ACF, 0X4854, 0X59DD, 0X2D62, 0X3CEB, 0X0E70, 0X1FF9,
	0XF78F, 0XE606, 0XD49D, 0XC514, 0XB1AB, 0XA022, 0X92B9, 0X8330,
	0X7BC7, 0X6A4E, 0X58D5, 0X495C, 0X3DE3, 0X2C6A, 0X1EF1, 0X0F78,
};

uint8_t convertCharToHex(char ch)
{
	int returnType;
	switch(ch)
	{
		case '0':
		returnType = 0;
		break;
		case  '1' :
		returnType = 1;
		break;
		case  '2':
		returnType = 2;
		break;
		case  '3':
		returnType = 3;
		break;
		case  '4' :
		returnType = 4;
		break;
		case  '5':
		returnType = 5;
		break;
		case  '6':
		returnType = 6;
		break;
		case  '7':
		returnType = 7;
		break;
		case  '8':
		returnType = 8;
		break;
		case  '9':
		returnType = 9;
		break;
		case  'A':
		returnType = 10;
		break;
		case  'B':
		returnType = 11;
		break;
		case  'C':
		returnType = 12;
		break;
		case  'D':
		returnType = 13;
		break;
		case  'E':
		returnType = 14;
		break;
		case  'F' :
		returnType = 15;
		break;
		default:
		returnType = 0;
		break;
	}
	return returnType;
}

void to_String_X2(unsigned int v)
{
	v &= 0xff;
	memset(value_2, 0, sizeof(value_2));
	sprintf(value_2, "%02X", v);
}

void to_String_X4(unsigned int v)
{
	v &= 0xffff;
	memset(value_4, 0, sizeof(value_4));
	sprintf(value_4, "%04X", v);
}

void to_String_X8(unsigned long v)
{
	memset(value_8, 0, sizeof(value_8));
	memset(value_4, 0, sizeof(value_4));
	
	v &= 4294967295UL;
	low = (unsigned int)(v % 65536UL);
	high = (unsigned int)(v / 65536UL);
	
	sprintf(value_4, "%04X", high);
	strcat(value_8, value_4);
	
	memset(value_4, 0, sizeof(value_4));
	sprintf(value_4, "%04X", low);
	strcat(value_8, value_4);
}

void to_String_X16(uint64_t v){
	memset(value_16, 0 , sizeof(value_16));
	memset(value_8, 0, sizeof(value_8));
	
	v &= 18446744073709551615ULL;
	lowl = (unsigned long int)(v % 4294967296UL);
	highl = (unsigned long int)(v / 4294967296UL);
	to_String_X8(highl);
	strcat(value_16,value_8);
	memset(value_8, 0, sizeof(value_8));
	to_String_X8(lowl);
	strcat(value_16,value_8);
}

uint16_t GetCrc16(const uint8_t pData[], int nLength)
{
	int i = 2;
	uint16_t fcs = 0xffff; // initialization
	nLength = nLength - 6;
	while (nLength > 0) {
		fcs = (fcs >> 8) ^ crctab16[(fcs ^ pData[i]) & 0xff];
		//USART0_TxChar(pData[i]);
		nLength--;
		i++;
	}
	return ~fcs; // negated
}
#endif

#ifndef gen_messages_to_tracker

int gen_rfid_message(uint8_t rfid[], int serial_number)
{
	memset(value_data, 0, sizeof(value_data));
	to_String_X4(0xFFFF);
	strcat(value_data, value_4); // 2 (start bit)
	to_String_X2(0x0c);
	strcat(value_data, value_2); // 1 (packet length)
	to_String_X2(0x01);
	strcat(value_data, value_2); // 1 (protocol number)
	for(k = 0; k < 4; k++)
	{
		to_String_X2(rfid[k]);
		strcat(value_data, value_2); // 1 (IMEI[k])
	}
	to_String_X4(serial_number);
	strcat(value_data, value_4); // 4 (serial number language)
	to_String_X4(0x0d0a);
	strcat(value_data, value_4); // 4 (stop bit)
	int msg_len = 0;
	msg_len = 0;
	for(int i = 0,r=0; i<2*(12);i+=2,r++){
		value_data[r] = (char)(convertCharToHex(value_data[i])<<4)|convertCharToHex(value_data[i+1]);
		msg_len++;
	}
	return msg_len;
}

int gen_error_message()
{
	memset(value_data, 0, sizeof(value_data));
	to_String_X4(0xFFFF);
	strcat(value_data, value_4); // 2 (start bit)
	to_String_X2(0x08);
	strcat(value_data, value_2); // 1 (packet length)
	to_String_X2(0x0a);
	strcat(value_data, value_2); // 10 (protocol number)
	to_String_X4(serial_number);
	strcat(value_data, value_4); // 4 (serial number language)
	to_String_X4(0x0d0a);
	strcat(value_data, value_4); // 4 (stop bit)
	int msg_len = 0;
	msg_len = 0;
	for(int i = 0,r=0; i<2*(8);i+=2,r++){
		value_data[r] = (char)(convertCharToHex(value_data[i])<<4)|convertCharToHex(value_data[i+1]);
		msg_len++;
	}
	return msg_len;
}

#endif

#ifndef led_manipulation_functions

void turn_len_red(void)
{
	PORTC=0b00000001;
}

void turn_len_green(void)
{
	PORTC=0b00000010;
}

void turn_len_blue(void)
{
	PORTC=0b00000100;
}

void power_off_leds(void)
{
	PORTC=0b00000000;
}

#endif

#ifndef passive_buzzer_functions

void delay_us(uint64_t count) {
	while(count--) {
		_delay_us(1);

	}
}

void delay_ms(uint64_t count) {
	while(count--) {
		_delay_ms(1);

	}
}
void make_tone(int num_subtones, int dur_subtones, int duration, double fequency)
{
	double freq_KH = fequency/1000;
	double dur_ms  = (1/(2*fequency))*1000;
	int num_trys =  duration*freq_KH;
	for(j = 0; j < num_subtones; j++)
	{
		for(i = 0; i < num_trys; i++)
		{
			PORTC |= 0b0001000;
			delay_us((int) (dur_ms*1000));
			PORTC ^= 0b0001000;
			delay_us((int) (dur_ms*1000));
		}
		delay_us(dur_subtones*1000);
	}
	
}

#endif

#ifndef external_interrupt

void init_external_interrupt(void)
{
	GICR = 1<<INT0;		/* Enable INT0*/
	MCUCR = 1<<ISC01 | 1<<ISC00;  /* Trigger INT0 on rising edge */
}

ISR(INT0_vect)
{
	read_flag = 0x01;
}

#endif

int main(void)
{
	DDRC = 0b0001111;
	uint8_t card_serial[32] = "";
	uint8_t byte;
	uint8_t str[MAX_LEN];
	unsigned char rf_exists = 0;
    /* Replace with your application code */
	init_external_interrupt();
	USART0_Init(115200);
	spi_init();
	_delay_ms(1000);
	mfrc522_init();
	init_millis(16000000UL);
	sei();
	byte = mfrc522_read(VersionReg);
	//USART0_TxChar(byte);
	if(byte == 0x92)
	{
		//USART0_SendString("MIFARE RC522v2 Detected\n");
	}else if(byte == 0x91 || byte==0x90)
	{
		//USART0_SendString("MIFARE RC522v1 Detected\n");
	}else
	{
		//USART0_SendString("No reader found\n");
	}
	
	byte = mfrc522_read(ComIEnReg);
	mfrc522_write(ComIEnReg,byte|0x20);
	byte = mfrc522_read(DivIEnReg);
	mfrc522_write(DivIEnReg,byte|0x80);
	
    while (1) 
    {
		read_flag = 0x00;
		power_off_leds();
		while(read_flag == 0x00);
		turn_len_blue();
		rf_exists = read_rfid_value(str, 60000);
		if(rf_exists == 1)
		{
			msg_len = gen_rfid_message(str, serial_number); serial_number++;
			USART0_SendString_With_Num_Bytes(value_data, msg_len);
			turn_len_green();
			make_tone(6, 5, 20, 10000);
		}
		else
		{
			
			msg_len = gen_error_message();
			USART0_SendString_With_Num_Bytes(value_data, msg_len);
			turn_len_red();
			make_tone(1, 0, 100, 1000);
		}
		
		_delay_ms(200);
		
    }
}

