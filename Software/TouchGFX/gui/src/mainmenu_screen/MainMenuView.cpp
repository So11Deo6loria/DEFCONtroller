#include <gui/mainmenu_screen/MainMenuView.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

extern "C" void CanChallengeButtonPressed (uint8_t State);

extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t CanTask_BSUpdated;
extern __IO uint16_t CanTask_BSValue;
extern __IO uint8_t CanTask_isBSGood;
extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t CanTask_ToggleState;
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

void MainMenuView::ToggleButtonClickVirtFunc( void )
{
	// Classic Caleb Hackery to Switch the State
	if( CanTask_ToggleState )
	{
		CanChallengeButtonPressed(0);
	}
	else
	{
		CanChallengeButtonPressed(1);
	}

	__UpdateDynamicData();

}

void MainMenuView::UpdateSPIDebugValue(uint16_t value)
{
	__UpdateDynamicData();
}

void  MainMenuView::__UpdateDynamicData(void)
{
	if( CanTask_ToggleState )
	{
		therapyStatusLabel.setColor (touchgfx::Color::getColorFromRGB (136, 203, 3));
		therapyStatusLabel.setAlpha (255);
		Unicode::snprintf (therapyStatusLabelBuffer, THERAPYSTATUSLABEL_SIZE, "Enabled");
		therapyStatusLabel.setWildcard (therapyStatusLabelBuffer);
		therapyStatusLabel.resizeToCurrentText ();
	}
	else
	{
		therapyStatusLabel.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
		therapyStatusLabel.setAlpha (255);
		Unicode::snprintf (therapyStatusLabelBuffer, THERAPYSTATUSLABEL_SIZE, "Disabled");
		therapyStatusLabel.setWildcard (therapyStatusLabelBuffer);
		therapyStatusLabel.resizeToCurrentText ();
	}


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
		Unicode::snprintf(CAN_FLAGBuffer, CAN_FLAG_SIZE, "               "); // TODO: Replace flag field buffer SPI
		CAN_FLAG.setWildcard(CAN_FLAGBuffer);
		CAN_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		CAN_FLAG.resizeToCurrentText();
		CAN_FLAG.invalidate();
	}
}
