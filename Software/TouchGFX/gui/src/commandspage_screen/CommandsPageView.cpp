#include <gui/commandspage_screen/CommandsPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

//#define FLAG_FIELD_SIZE 40

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xJTAGFlag[32];
extern __IO char xI2CFlag[32];

CommandsPageView::CommandsPageView()
{
	__UpdateDynamicData();
}

void CommandsPageView::setupScreen()
{
    CommandsPageViewBase::setupScreen();
}

void CommandsPageView::tearDownScreen()
{
    CommandsPageViewBase::tearDownScreen();
}

void CommandsPageView::UpdateDoorLock_Status()
{
	//Do Nothing
}

void CommandsPageView::UdateAutoIgnition_Status()
{
	//Do Nothing
}

void CommandsPageView::UpdateSeatWarmer_Status()
{
	//Do Nothing
}

void CommandsPageView::UpdateSPIDebugValue(uint16_t value)
{
	__UpdateDynamicData();
}

void  CommandsPageView::__UpdateDynamicData(void)
{
	if(debugFlagTouchGFX == 0){ //debug disabled
	    Unicode::snprintf(FLAGBuffer, FLAG_SIZE, "");
	    FLAG.setAlpha(255);		// Make it visible (opacity)
	    FLAG.invalidate();	// Redraws object
	}
	else if( debugFlagTouchGFX == 5)//value == 1  debug enabled via UART
	{
	    Unicode::snprintf(FLAGBuffer, FLAG_SIZE, (const char *)xUARTFlag);
	    FLAG.setWildcard(FLAGBuffer);
	    FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    FLAG.resizeToCurrentText();
	    FLAG.invalidate();
	}
	else
	{
		Unicode::snprintf(FLAGBuffer, FLAG_SIZE, ""); // TODO: Replace flag field buffer SPI
		FLAG.setWildcard(FLAGBuffer);
		FLAG.resizeToCurrentText();
		FLAG.invalidate();
	}
}
