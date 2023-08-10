#include <gui/commandspage_screen/CommandsPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

//#define FLAG_FIELD_SIZE 40

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xCANFlag[32];
extern __IO char xJTAGFlag[32];

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
	    Unicode::snprintf(UART_FLAGBuffer, UART_FLAG_SIZE, "Go Find A Flag!");
	    UART_FLAG.setWildcard(UART_FLAGBuffer);
	    UART_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
	    UART_FLAG.setAlpha(255);		// Make it visible (opacity)
	    UART_FLAG.invalidate();	// Redraws object
	}
	else if ((debugFlagTouchGFX & (1 << 1)) != 0) // UART Bit 1
	{
	    Unicode::snprintf(UART_FLAGBuffer, UART_FLAG_SIZE, (const char *)xUARTFlag);
	    UART_FLAG.setWildcard(UART_FLAGBuffer);
	    UART_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    UART_FLAG.resizeToCurrentText();
	    UART_FLAG.invalidate();
	}
	else
	{
		Unicode::snprintf(UART_FLAGBuffer, UART_FLAG_SIZE, "               "); // TODO: Replace flag field buffer SPI
		UART_FLAG.setWildcard(UART_FLAGBuffer);
		UART_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		UART_FLAG.resizeToCurrentText();
		UART_FLAG.invalidate();
	}
}
