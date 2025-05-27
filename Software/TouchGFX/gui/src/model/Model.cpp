#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifdef DEVBOARDBUILD
#include "main.h"

extern __IO uint8_t CanTask_BSUpdated;
extern __IO uint16_t CanTask_BSValue;
extern __IO uint8_t debugUpdatedTouchGFX;
extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t i2c_isValueUpdated;


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

#endif
}
