#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifdef DEVBOARDBUILD
#include "main.h"

extern __IO uint8_t CanTask_BSUpdated;
extern __IO uint16_t CanTask_BSValue;
extern __IO uint8_t debugUpdatedTouchGFX;
extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t i2c_isValueUpdated;

//extern __IO bool DoorLock_Toggle; 		// __IO gives ability to read/write to external variables
//extern __IO bool AutoIgnition_Toggle;
//extern __IO bool SeatWarmer_Toggle;

extern __IO uint8_t DoorLockState;
extern __IO uint8_t DoorLockUpdated;
extern __IO uint8_t SeatWarmerState;
extern __IO uint8_t SeatWarmerUpdated;
extern __IO uint8_t AutoIgnitionState;
extern __IO uint8_t AutoIgnitionUpdated;

extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t debugFlagUpdated;

#endif

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
#ifdef DEVBOARDBUILD
	if ( CanTask_BSUpdated )
	{
		modelListener->UpdateBSValue( CanTask_BSValue ); // goes and finds value
		CanTask_BSUpdated = 0;
	}

	if (debugFlagUpdated)
	{
		modelListener->UpdateSpiDebugValue(debugFlagTouchGFX);
		debugFlagUpdated =0;
	}

	if (DoorLockUpdated)
	{
		modelListener->UpdateDoorLock_Status();
		DoorLockUpdated=0;
	}

	if (AutoIgnitionUpdated)
	{
		modelListener->UpdateAutoIgnition_Status();
		AutoIgnitionUpdated=0;
	}

	if (SeatWarmerUpdated)
	{
		modelListener->UpdateSeatWarmer_Status();
		SeatWarmerUpdated=0;
	}

#endif
}
