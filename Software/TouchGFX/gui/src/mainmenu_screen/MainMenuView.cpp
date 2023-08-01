#include <gui/mainmenu_screen/MainMenuView.hpp>
//#include <text/TextKeysAndLanguages.hpp>

extern "C" void AutoIgnitionButtonPressed (uint8_t State);
extern "C" void DoorLockButtonPressed (uint8_t State);
extern "C" void SeatWarmerButtonPressed (uint8_t State);

MainMenuView::MainMenuView()
{

}

void MainMenuView::setupScreen()
{
    MainMenuViewBase::setupScreen();
}

void MainMenuView::tearDownScreen()
{
    MainMenuViewBase::tearDownScreen();
}

