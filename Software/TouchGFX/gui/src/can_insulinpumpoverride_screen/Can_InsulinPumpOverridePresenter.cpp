#include <gui/can_insulinpumpoverride_screen/Can_InsulinPumpOverrideView.hpp>
#include <gui/can_insulinpumpoverride_screen/Can_InsulinPumpOverridePresenter.hpp>

Can_InsulinPumpOverridePresenter::Can_InsulinPumpOverridePresenter(Can_InsulinPumpOverrideView& v)
    : view(v)
{

}

void Can_InsulinPumpOverridePresenter::activate()
{

}

void Can_InsulinPumpOverridePresenter::deactivate()
{

}

void Can_InsulinPumpOverridePresenter::UpdateSpiDebugValue( uint16_t value )
{
  // Do nothing
}

#ifdef DEVBOARDBUILD

// Adding these Functions
void Can_InsulinPumpOverridePresenter::UpdateBSValue( uint16_t value )
{
}

void Can_InsulinPumpOverridePresenter::UpdateTempValue( void )
{
}
#endif
