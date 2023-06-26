#include <gui/i2cpage_screen/I2CPageView.hpp>
#include <gui/i2cpage_screen/I2CPagePresenter.hpp>

I2CPagePresenter::I2CPagePresenter(I2CPageView& v)
    : view(v)
{

}

void I2CPagePresenter::activate()
{

}

void I2CPagePresenter::deactivate()
{

}

void I2CPagePresenter::UpdateBSValue( uint16_t value )
{
  // Do nothing
}

void I2CPagePresenter::UpdateSpiDebugValue( uint16_t value )
{
  // Do nothing
}

void I2CPagePresenter::UpdateTempValue( void )
{
	view.UpdateTempValue();
}
