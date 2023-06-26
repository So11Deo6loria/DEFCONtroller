#include <gui/uartpage_screen/UARTPageView.hpp>
#include <gui/uartpage_screen/UARTPagePresenter.hpp>

UARTPagePresenter::UARTPagePresenter(UARTPageView& v)
    : view(v)
{

}

void UARTPagePresenter::activate()
{

}

void UARTPagePresenter::deactivate()
{

}

void UARTPagePresenter::UpdateBSValue( uint16_t value )
{
  // Do nothing
}

void UARTPagePresenter::UpdateSpiDebugValue( uint16_t value )
{
  // Do nothing
}

void UARTPagePresenter::UpdateTempValue( void )
{
}
