#include <gui/creditspage_screen/CreditsPageView.hpp>
#include <gui/creditspage_screen/CreditsPagePresenter.hpp>

CreditsPagePresenter::CreditsPagePresenter(CreditsPageView& v)
    : view(v)
{

}

void CreditsPagePresenter::activate()
{

}

void CreditsPagePresenter::deactivate()
{

}

#ifdef DEVBOARDBUILD

void CreditsPagePresenter::UpdateSpiDebugValue(uint16_t)
{
	//Do Nothing
}

void CreditsPagePresenter::UpdateBSValue(uint16_t)
{
	//Do Nothing
}

void CreditsPagePresenter::UpdateTempValue(void)
{
	//Do Nothing
}

void CreditsPagePresenter::UpdateDoorLock_Status(void)
{
	//Do Nothing
}

void CreditsPagePresenter::UpdateAutoIgnition_Status(void)
{
	//Do Nothing
}

void CreditsPagePresenter::UpdateSeatWarmer_Status(void)
{
	//Do Nothing
}

#endif
