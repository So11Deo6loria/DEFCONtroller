#include <gui/jtagpage_screen/JTAGPageView.hpp>
#include <gui/jtagpage_screen/JTAGPagePresenter.hpp>

JTAGPagePresenter::JTAGPagePresenter(JTAGPageView& v)
    : view(v)
{

}

void JTAGPagePresenter::activate()
{

}

void JTAGPagePresenter::deactivate()
{

}


void JTAGPagePresenter::UpdateBSValue( uint16_t value )
{
  // Do nothing
}

void JTAGPagePresenter::UpdateSpiDebugValue( uint16_t value )
{
  // Do nothing
}

void JTAGPagePresenter::UpdateTempValue( void )
{
}
