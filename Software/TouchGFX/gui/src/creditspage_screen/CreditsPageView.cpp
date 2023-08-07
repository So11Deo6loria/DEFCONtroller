#include <gui/creditspage_screen/CreditsPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xCANFlag[32];
extern __IO char xJTAGFlag[32];

CreditsPageView::CreditsPageView()
{
	__UpdateDynamicData();
}

void CreditsPageView::setupScreen()
{
    CreditsPageViewBase::setupScreen();
}

void CreditsPageView::tearDownScreen()
{
    CreditsPageViewBase::tearDownScreen();
}
void CreditsPageView::UpdateSPIDebugValue(uint16_t value)
{
	__UpdateDynamicData();
}

void  CreditsPageView::__UpdateDynamicData(void)
{
	if(debugFlagTouchGFX == 0){ //debug disabled
	    Unicode::snprintf(JTAG_FLAGBuffer, JTAG_FLAG_SIZE, "Go Find A Flag!");
	    JTAG_FLAG.setWildcard(JTAG_FLAGBuffer);
	    JTAG_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
	    JTAG_FLAG.setAlpha(255);		// Make it visible (opacity)
	    JTAG_FLAG.invalidate();	// Redraws object
	}
	else if ((debugFlagTouchGFX & (1 << 4)) != 0) // JTAG Bit 4
	{
	    Unicode::snprintf(JTAG_FLAGBuffer, JTAG_FLAG_SIZE, (const char *)xJTAGFlag);
	    JTAG_FLAG.setWildcard(JTAG_FLAGBuffer);
	    JTAG_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    JTAG_FLAG.resizeToCurrentText();
	    JTAG_FLAG.invalidate();
	}
	else
	{
		Unicode::snprintf(JTAG_FLAGBuffer, JTAG_FLAG_SIZE, ""); // TODO: Replace flag field buffer SPI
		JTAG_FLAG.setWildcard(JTAG_FLAGBuffer);
		JTAG_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		JTAG_FLAG.resizeToCurrentText();
		JTAG_FLAG.invalidate();
	}
}
