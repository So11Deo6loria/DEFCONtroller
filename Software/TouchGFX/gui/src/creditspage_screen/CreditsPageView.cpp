#include <gui/creditspage_screen/CreditsPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xJTAGFlag[32];
extern __IO char xI2CFlag[32];

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
	    Unicode::snprintf(FLAGBuffer, FLAG_SIZE, "Go Find A Flag!");
	    FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
	    FLAG.setAlpha(255);		// Make it visible (opacity)
	    FLAG.invalidate();	// Redraws object
	}
	else if ((debugFlagTouchGFX & (1 << 7)) != 0)
	{
	    Unicode::snprintf(FLAGBuffer, FLAG_SIZE, (const char *)xJTAGFlag);
	    FLAG.setWildcard(FLAGBuffer);
	    FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    FLAG.resizeToCurrentText();
	    FLAG.invalidate();
	}
	else
	{
		Unicode::snprintf(FLAGBuffer, FLAG_SIZE, ""); // TODO: Replace flag field buffer SPI
		FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		FLAG.setWildcard(FLAGBuffer);
		FLAG.resizeToCurrentText();
		FLAG.invalidate();
	}
}
