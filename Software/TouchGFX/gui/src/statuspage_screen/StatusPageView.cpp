#include <gui/statuspage_screen/StatusPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xCANFlag[32];
extern __IO char xJTAGFlag[32];

StatusPageView::StatusPageView()
{
	__UpdateDynamicData();
}

void StatusPageView::setupScreen()
{
    StatusPageViewBase::setupScreen();
}

void StatusPageView::tearDownScreen()
{
    StatusPageViewBase::tearDownScreen();
}

void StatusPageView::UpdateSpiDebugValue(uint16_t value)
{
	__UpdateDynamicData();
}

void  StatusPageView::__UpdateDynamicData(void)
{
	if(debugFlagTouchGFX == 0){ //debug disabled
	    Unicode::snprintf(SelfDestCount_ValBuffer, SELFDESTCOUNT_VAL_SIZE, "Not Initiated");
	    SelfDestCount_Val.setWildcard(SelfDestCount_ValBuffer);
	    SelfDestCount_Val.setColor (touchgfx::Color::getColorFromRGB (26, 255, 0));
	    SelfDestCount_Val.setAlpha(255);		// Make it visible (opacity)
	    SelfDestCount_Val.invalidate();	// Redraws object
	}
	else if ((debugFlagTouchGFX & (1 << 2)) != 0) // SPI Bit 2
	{
	    Unicode::snprintf(SelfDestCount_ValBuffer, SELFDESTCOUNT_VAL_SIZE, (const char *)xSPIFlag);
	    SelfDestCount_Val.setWildcard(SelfDestCount_ValBuffer);
	    SelfDestCount_Val.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    SelfDestCount_Val.resizeToCurrentText();
	    SelfDestCount_Val.invalidate();
	}
	else
	{
		Unicode::snprintf(SelfDestCount_ValBuffer, SELFDESTCOUNT_VAL_SIZE, "Not Initiated"); // TODO: Replace flag field buffer SPI
		SelfDestCount_Val.setWildcard(SelfDestCount_ValBuffer);
		SelfDestCount_Val.setColor (touchgfx::Color::getColorFromRGB (26, 255, 0));
		SelfDestCount_Val.setWildcard(SelfDestCount_ValBuffer);
		SelfDestCount_Val.resizeToCurrentText();
		SelfDestCount_Val.invalidate();
	}
}
