#include <gui/mainmenu_screen/MainMenuView.hpp>
//#include <text/TextKeysAndLanguages.hpp>

extern "C" void cFun_AutoIgnitionButtonPressed (uint8_t State);
extern "C" void cFun_DoorLockButtonPressed (uint8_t State);
extern "C" void cFun_SeatWarmerButtonPressed (uint8_t State);
extern __IO uint8_t SeatWarmer_ToggleState;

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

void MainMenuView::SearWarmerButtonPressed()
{
	cFun_SeatWarmerButtonPressed(SeatWarmer_Button.getState());
}

void MainMenuView::DoorLockButtonPressed()
{
	cFun_DoorLockButtonPressed(DoorLock_Button.getState());
}

void MainMenuView::AutoIgnitionButtonPressed()
{
	cFun_AutoIgnitionButtonPressed(AutoIgnition_Button.getState());
}

void MainMenuView::UpdateSeatWarmerButton()
{
//	SeatWarmer_Button.setBitmaps(touchgfx::Bitmap(BITMAP_DOORLOCK_DISENGAGED_ID));
}
