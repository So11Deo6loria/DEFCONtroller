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


/*
 * Event Handlers for TouchGFX Events
 */


void MainMenuPresenter::UpdateTempValue(void)
{
    //Do Nothing
}

void MainMenuPresenter::UpdateDoorLock_Status(void)
{
	view.UpdateDoorLockStatus();
}

void MainMenuPresenter::UpdateSeatWarmer_Status(void)
{
	view.UpdateSeatWarmerStatus();
}

void MainMenuPresenter::UpdateAutoIgnition_Status(void)
{
	view.UpdateAutoIgnitionStatus();
}

void MainMenuPresenter::UpdateBSValue(uint16_t)
{
	//Do Nothing
}

void MainMenuPresenter::UpdateSpiDebugValue(uint16_t)
{
    //Do Nothing
}
