/*
 * main.c
 *
 *  Created on: Apr 15, 2024
 *      Author: mans
 */


#include "STD_TYPES.h"
#include <util/delay.h>

#include "DIO_interface.h"
#include "PORT_Interface.h"
#include "CLCD_interface.h"
#include "KPD_interface.h"
#include "calculator.h"

void main(void){
	PORT_voidInit();
	CLCD_voidInit();

	uint8 Local_u8KPDKey=NO_KEY_PRESSED;
	uint8 Local_u8InputCounter=0;
	sint16 Local_s16FirstNumber=0;
	sint16 Local_s16SecondNumber=0;
	sint32 Local_s32ResultIntegerPart=0;
	sint32 Local_s32ResultFractionPart=0;
	uint8 Local_u8Operator=0;
	uint8 Local_u8DivisionByZero=0;

	while(1){
		/*store first digit*/
		while(Local_u8InputCounter != MAX_CHAR_COUNT || Local_u8KPDKey != '!' || Local_u8KPDKey != '=' || Local_u8KPDKey != '/' || Local_u8KPDKey != '+' || Local_u8KPDKey != '*' || Local_u8KPDKey != '-')
		{
			if(Local_u8InputCounter == 0)
			{
				do
				{
					Local_u8KPDKey = KPD_u8GetPressedKey();
				}while(Local_u8KPDKey == NO_KEY_PRESSED || Local_u8KPDKey == '=' || Local_u8KPDKey == '/' || Local_u8KPDKey == '+' || Local_u8KPDKey == '*' || Local_u8KPDKey == '-');
			}
			else
			{
				do
				{
					Local_u8KPDKey = KPD_u8GetPressedKey();
				}while(Local_u8KPDKey == NO_KEY_PRESSED);
			}
			if(Local_u8InputCounter > 0)
			{
				if(Local_u8KPDKey == '=')
				{
					break;
				}

				if(Local_u8KPDKey == '/' || Local_u8KPDKey == '+' || Local_u8KPDKey == '*' || Local_u8KPDKey == '-')
				{
					CLCD_voidSendData(Local_u8KPDKey);
					Local_u8Operator = Local_u8KPDKey;
					Local_u8InputCounter++;
					break;
				}

			}

			if(Local_u8KPDKey == '!')
			{
				break;
			}
			CLCD_voidSendNumber(Local_u8KPDKey);
			Local_s16FirstNumber=(sint16)((Local_s16FirstNumber*10U)+(sint16)Local_u8KPDKey);

			Local_u8InputCounter++;

		}

		/* Store FirstNumber  in Result if '=' is pressed  */
		Local_s32ResultIntegerPart = (sint32)Local_s16FirstNumber;

		/*Store second number */
		if(Local_u8Operator != 0)
		{
			while(Local_u8InputCounter != MAX_CHAR_COUNT || Local_u8KPDKey != '!' || Local_u8KPDKey != '=' || Local_u8KPDKey != '/' || Local_u8KPDKey != '+' || Local_u8KPDKey != '*' || Local_u8KPDKey != '-')
			{
				do
				{
					Local_u8KPDKey = KPD_u8GetPressedKey();
				}while(Local_u8KPDKey == NO_KEY_PRESSED || Local_u8KPDKey == '/' || Local_u8KPDKey == '+' || Local_u8KPDKey == '*' || Local_u8KPDKey == '-');

				if(Local_u8KPDKey == '!' || Local_u8KPDKey == '=')
				{
					break;
				}

				CLCD_voidSendNumber(Local_u8KPDKey);
				Local_s16SecondNumber=(sint16)((Local_s16SecondNumber*10U)+(sint16)Local_u8KPDKey);
				Local_u8InputCounter++;

			}

		}

		/* choose the operation */
		switch(Local_u8Operator)
		{
		case '+':	Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber + (sint32)Local_s16SecondNumber);	break;
		case '-': 	Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber - (sint32)Local_s16SecondNumber);	break;
		case '/':
		{
			/* Disallow dividing by zero */
			if(Local_s16SecondNumber == 0)
			{
				Local_u8DivisionByZero = 1;
				CLCD_voidSendCmd(CLCD_u8CLEAR_LCD);
				CLCD_u8SendString("CAN'T DIV BY ZERO");
				_delay_ms(2000);
			}
			else
			{
				Local_s32ResultFractionPart= (sint32)((((sint32)Local_s16FirstNumber % (sint32)Local_s16SecondNumber)*10L)/(sint32)Local_s16SecondNumber);
				Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber / (sint32)Local_s16SecondNumber);
			}
			break;
		}
		case '*': Local_s32ResultIntegerPart = (sint32)((sint32)Local_s16FirstNumber * (sint32)Local_s16SecondNumber);	break;

		}


		/*if result is valid*/

		if(Local_u8DivisionByZero == 0)
		{

			CLCD_voidGoToXY(0,1);
			CLCD_voidSendNumber(Local_s32ResultIntegerPart);
			if(Local_s32ResultFractionPart > 0)
			{
				CLCD_voidSendData('.');
				CLCD_voidSendNumber(Local_s32ResultFractionPart);
			}

		}
			while(Local_u8KPDKey != '!')
			{
				Local_u8KPDKey = KPD_u8GetPressedKey();
			}

			/*Reset calculator*/
			CLCD_voidSendCmd(CLCD_u8CLEAR_LCD);
			Local_s16SecondNumber=0;
			Local_u8DivisionByZero=0;
			Local_s16FirstNumber=0;
			Local_u8InputCounter=0;
			Local_s32ResultIntegerPart=0;
			Local_s32ResultFractionPart=0;
			Local_u8Operator=0;
			Local_u8KPDKey= NO_KEY_PRESSED;








		}


	}
