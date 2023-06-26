#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifdef DEVBOARDBUILD
#include "main.h"

extern __IO uint8_t CanTask_BSUpdated;
extern __IO uint16_t CanTask_BSValue;
extern __IO uint8_t debugUpdatedTouchGFX;
extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t i2c_isValueUpdated;
#endif

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
#ifdef DEVBOARDBUILD
	if ( CanTask_BSUpdated )
	{
		modelListener->UpdateBSValue( CanTask_BSValue );
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
#endif
}
