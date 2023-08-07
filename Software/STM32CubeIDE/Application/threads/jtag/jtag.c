/*
 * jtag.c
 *
 *  Created on: Jul 21, 2022
 *      Author: calebdavis
 */

#include "jtag.h"
#include "string.h"

typedef enum {
	JTAG_STATUS_LOCKED,
	JTAG_STATUS_UNLOCKED,
	JTAG_STATUS_UNSET
} eJTAGStatus_t;

extern uint8_t debugFlagTouchGFX;
eJTAGStatus_t jtagLockStatus = JTAG_STATUS_UNSET; /// 0 = Unlocked / 1 = Locked / 2 = Unset
uint8_t JTAGFlag = 0; /// 0 = false use to communicate with touchgfx

static void __toggleJTAGStatus( uint8_t status )
{
	if( status != jtagLockStatus )
	{
		jtagLockStatus = status;

		if( status )
		{
			debugFlagTouchGFX |= (1<<4);
		}
		else
		{
			debugFlagTouchGFX &= ~(1<<4);
		}
	}
}

void JTAGChallengeThread( void * argument )
{
	int compareValue = 0;
	uint8_t jtagKeyString[]       = "MANUFACTURER_SECRET_KEY_STRING";
	uint8_t jtagSimulatedString[] = "JTAG_SIMULATED_KEY_STRING_1234";

	for( ;; )
	{
		compareValue = strncmp( (char *)jtagKeyString, (char *)jtagSimulatedString, strlen((char * )jtagKeyString) );
//		compareValue = 0;
		if( compareValue == 0 )
		{
			// JTAG Unlocked
			__toggleJTAGStatus( JTAG_STATUS_UNLOCKED );
		}
		else
		{
			// JTAG Locked
			__toggleJTAGStatus( JTAG_STATUS_LOCKED );
		}
		osDelay(5000); // Is this sleep too long?
	}
}
