#include "STD_TYPES.h"


#include "PORT_Prv.h"
#include "PORT_Cfg.h"



#include "PORT_Interface.h"
#include "PORT_Reg.h"



void PORT_voidInit(void)
{
	// Set the direction registers for each port based on the configuration
	DDRA = PORTA_DIR; 
	DDRB = PORTB_DIR;
	DDRC = PORTC_DIR;
	DDRD = PORTD_DIR;

	// Set the initial values for each port based on the configuration
	PORTA = PORTA_INIT_VAL;
	PORTB = PORTB_INIT_VAL;
	PORTC = PORTC_INIT_VAL;
	PORTD = PORTD_INIT_VAL;



}
