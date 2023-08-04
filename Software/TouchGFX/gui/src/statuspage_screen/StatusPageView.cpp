#include <gui/statuspage_screen/StatusPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern __IO uint8_t debugFlagTouchGFX;
extern __I char xUARTFlag[32];
extern __I char xSPIFlag[32];
extern __I char xJTAGFlag[32];
extern __I char xI2CFlag[32];

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
//	    Unicode::snprintf(SELFDESTRUCT_WC, FLAG_SIZE, "Go Find A Flag");
//	    FLAG.setAlpha(255);		// Make it visible (opacity)
//	    FLAG.invalidate();	// Redraws object
	}
	else if( debugFlagTouchGFX == 6)//value == 1  SELF_DESTRUCT Enabled via SPI
	{
	    Unicode::snprintf(SelfDestCount_ValBuffer, SELFDESTCOUNT_VAL_SIZE, (const char *)xSPIFlag);
	    SelfDestCount_Val.setWildcard(SelfDestCount_ValBuffer);
	    SelfDestCount_Val.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    SelfDestCount_Val.resizeToCurrentText();
	    SelfDestCount_Val.invalidate();
	}
}
