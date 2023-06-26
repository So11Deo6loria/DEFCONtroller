#include <gui/can_insulinpumpstatus_screen/Can_InsulinPumpStatusView.hpp>
#include <gui/can_insulinpumpstatus_screen/Can_InsulinPumpStatusPresenter.hpp>
#include "stdint.h"

Can_InsulinPumpStatusPresenter::Can_InsulinPumpStatusPresenter(Can_InsulinPumpStatusView& v)
    : view(v)
{

}

void Can_InsulinPumpStatusPresenter::activate()
{

}

void Can_InsulinPumpStatusPresenter::deactivate()
{

}

void Can_InsulinPumpStatusPresenter::UpdateSpiDebugValue( uint16_t value )
{
  // Do nothing
}


#ifdef DEVBOARDBUILD

// Adding these Functions
void Can_InsulinPumpStatusPresenter::UpdateBSValue( uint16_t value )
{
	view.UpdateBSValue( value );

}

void Can_InsulinPumpStatusPresenter::UpdateTempValue( void )
{
}

#endif




