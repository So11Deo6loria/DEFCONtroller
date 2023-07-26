#include <gui/mainmenu_screen/MainMenuView.hpp>
#include <gui/mainmenu_screen/MainMenuPresenter.hpp>

MainMenuPresenter::MainMenuPresenter(MainMenuView& v)
    : view(v)
{

}

void MainMenuPresenter::activate()
{

}

void MainMenuPresenter::deactivate()
{

}

#ifdef DEVBOARDBUILD

void MainMenuPresenter::UpdateSpiDebugValue(uint16_t)
{
	//Do Nothing
}

void MainMenuPresenter::UpdateBSValue(uint16_t)
{
	//Do Nothing
}

void MainMenuPresenter::UpdateTempValue(void)
{
	//Do Nothing
}

void MainMenuPresenter::UpdateDoorLock_Status(void)
{
	//Do Nothing
}

void MainMenuPresenter::UpdateAutoIgnition_Status(void)
{
	//Do Nothing
}

void MainMenuPresenter::UpdateSeatWarmer_Status(void)
{
	//Do Nothing
}

#endif
