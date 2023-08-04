#include <gui/mainmenu_screen/MainMenuView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern "C" void UARTChallengeIgnitionButtonPressed (uint8_t State);
extern "C" void UARTChallengeDoorLockButtonPressed (uint8_t State);
extern "C" void UARTChallengeSeatWarmerButtonPressed (uint8_t State);
extern "C" void CanChallengeButtonPressed (uint8_t State);

extern __IO uint8_t DoorLockState;
extern __IO uint8_t DoorLockUpdated;
extern __IO uint8_t SeatWarmerState;
extern __IO uint8_t SeatWarmerUpdated;
extern __IO uint8_t AutoIgnitionState;
extern __IO uint8_t AutoIgnitionUpdated;

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
  UARTChallengeSeatWarmerButtonPressed(SeatWarmer_Button.getState());
}

void MainMenuView::DoorLockButtonPressed()
{
	UARTChallengeDoorLockButtonPressed(DoorLock_Button.getState());
	CanChallengeButtonPressed(DoorLock_Button.getState());
}

void MainMenuView::AutoIgnitionButtonPressed()
{
	UARTChallengeIgnitionButtonPressed(AutoIgnition_Button.getState());
}

void MainMenuView::UpdateSeatWarmerStatus()
{
	if(SeatWarmerState)
	{
		SeatWarmer_Button.forceState( true );
	}
	else
	{
		SeatWarmer_Button.forceState( false );
	}

	SeatWarmer_Button.invalidate();
}

void MainMenuView::UpdateDoorLockStatus()
{
	if(DoorLockState)
	{
		DoorLock_Button.forceState( true );
	}
	else
	{
		DoorLock_Button.forceState( false );
	}

	DoorLock_Button.invalidate();
}

void MainMenuView::UpdateAutoIgnitionStatus()
{
	if(AutoIgnitionState)
	{
		AutoIgnition_Button.forceState( true );
	}
	else
	{
		AutoIgnition_Button.forceState( false );
	}

	AutoIgnition_Button.invalidate();
}
