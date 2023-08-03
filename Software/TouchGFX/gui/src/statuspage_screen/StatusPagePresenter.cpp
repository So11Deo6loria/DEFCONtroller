#include <gui/statuspage_screen/StatusPageView.hpp>
#include <gui/statuspage_screen/StatusPagePresenter.hpp>

StatusPagePresenter::StatusPagePresenter(StatusPageView& v)
    : view(v)
{

}

void StatusPagePresenter::activate()
{

}

void StatusPagePresenter::deactivate()
{

}

#ifdef DEVBOARDBUILD

void StatusPagePresenter::UpdateSpiDebugValue(uint16_t value)
{
	//Do Nothing
	view.UpdateSpiDebugValue(value);
}

void StatusPagePresenter::UpdateBSValue(uint16_t)
{
	//Do Nothing
}

void StatusPagePresenter::UpdateTempValue(void)
{
	//Do Nothing
}

void StatusPagePresenter::UpdateDoorLock_Status(void)
{
	//Do Nothing
}

void StatusPagePresenter::UpdateAutoIgnition_Status(void)
{
	//Do Nothing
}

void StatusPagePresenter::UpdateSeatWarmer_Status(void)
{
	//Do Nothing
}

#endif
