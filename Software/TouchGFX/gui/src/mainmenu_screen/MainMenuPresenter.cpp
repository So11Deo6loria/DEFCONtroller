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


void MainMenuPresenter::UpdateBSValue(uint16_t)
{
	//Do Nothing
}

void MainMenuPresenter::UpdateSpiDebugValue(uint16_t)
{
    //Do Nothing
}
