#include <gui/mainmenu_screen/MainMenuView.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

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

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xCANFlag[32];
extern __IO char xJTAGFlag[32];

MainMenuView::MainMenuView()
{
	__UpdateDynamicData();
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

void MainMenuView::UpdateSPIDebugValue(uint16_t value)
{
	__UpdateDynamicData();
}

void  MainMenuView::__UpdateDynamicData(void)
{
	// Don't write over Protiviti unless the flag is there.
	if ((debugFlagTouchGFX & (1 << 3)) != 0) // CAN Bit 1
	{
	    Unicode::snprintf(CAN_FLAGBuffer, CAN_FLAG_SIZE, (const char *)xCANFlag);
	    CAN_FLAG.setWildcard(CAN_FLAGBuffer);
	    CAN_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    CAN_FLAG.resizeToCurrentText();
	    CAN_FLAG.invalidate();
	}
	else
	{
		Unicode::snprintf(CAN_FLAGBuffer, CAN_FLAG_SIZE, ""); // TODO: Replace flag field buffer SPI
		CAN_FLAG.setWildcard(CAN_FLAGBuffer);
		CAN_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		CAN_FLAG.resizeToCurrentText();
		CAN_FLAG.invalidate();
	}
}
