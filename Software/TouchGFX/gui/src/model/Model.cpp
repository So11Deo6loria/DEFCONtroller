#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifdef DEVBOARDBUILD
#include "main.h"

extern __IO uint8_t CanTask_BSUpdated;
extern __IO uint16_t CanTask_BSValue;
extern __IO uint8_t debugUpdatedTouchGFX;
extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t i2c_isValueUpdated;

extern __IO bool DoorLock_Toggle; 		// __IO gives ability to read/write to external variables
extern __IO bool AutoIgnition_Toggle;
extern __IO bool SeatWarmer_Toggle;

extern __IO uint8_t DoorLock_ToggleState;
extern __IO uint8_t DoorLock_Toggle;
extern __IO uint8_t SeatWarmer_ToggleState;
extern __IO uint8_t SeatWarmer_Toggle;
extern __IO uint8_t AutoIgnition_ToggleState;
extern __IO uint8_t AutoIgnition_Toggle;

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

	if (debugUpdatedTouchGFX)
	{
		modelListener->UpdateSpiDebugValue(debugFlagTouchGFX);
		debugUpdatedTouchGFX =0;
	}

	if (i2c_isValueUpdated)
	{
		modelListener->UpdateTempValue();
		i2c_isValueUpdated=0;
	}

	if (DoorLock_Toggle)
	{
		modelListener->UpdateDoorLock_Status();
		DoorLock_Toggle=0;
	}

	if (AutoIgnition_Toggle)
	{
		modelListener->UpdateAutoIgnition_Status();
		AutoIgnition_Toggle=0;
	}

	if (SeatWarmer_Toggle)
	{
		modelListener->UpdateSeatWarmer_Status();
		SeatWarmer_Toggle=0;
	}

#endif
}
