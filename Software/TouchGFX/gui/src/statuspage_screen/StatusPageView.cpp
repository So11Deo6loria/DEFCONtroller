#include <gui/statuspage_screen/StatusPageView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern __IO uint8_t debugFlagTouchGFX;
extern __I char xUARTFlag[32];
extern __I char xSPIFlag[32];
extern __I char xJTAGFlag[32];
extern __I char xI2CFlag[32];

StatusPageView::StatusPageView()
{

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
	    Unicode::snprintf(SELFDESTRUCT_WC, FLAG_SIZE, "Go Find A Flag");
	    FLAG.setAlpha(255);		// Make it visible (opacity)
	    FLAG.invalidate();	// Redraws object
	}
	else if( debugFlagTouchGFX == 7)//value == 1  debug enabled via RE
	{

	    Unicode::snprintf(FLAGBuffer, FLAG_SIZE, (const char *) xJTAGFlag);
	    FLAG.setWildcard(FLAGBuffer);	// TODO: Replace flag field buffer JTAG.   replaces chars
		FLAG.resizeToCurrentText();		// resizes object for new char length
		FLAG.invalidate();
	}
	else if( debugFlagTouchGFX == 5)//value == 1  debug enabled via UART
	{

	    Unicode::snprintf(FLAGBuffer, FLAG_SIZE, (const char *)xUARTFlag); // TODO: Replace flag field buffer UART
	    FLAG.setWildcard(FLAGBuffer);
		FLAG.resizeToCurrentText();
	    FLAG.invalidate();
	}
	else
	{
	    Unicode::snprintf(SELFDESTRUCT_WC, SELFDESTCOUNT_VAL_SIZE, (const char *)xSPIFlag);
	    SelfDestCount_Val.setWildcard(SELFDESTRUCT_WC);
	    SelfDestCount_Val.resizeToCurrentText();
	    SelfDestCount_Val.invalidate();
//	    SelfDestCount_Val.setTypedText(touchgfx::TypedText(T_SELFDEST_VAL));

		Unicode::snprintf(FLAGBuffer, FLAG_SIZE, (const char *)xSPIFlag); // TODO: Replace flag field buffer SPI
		FLAG.setWildcard(FLAGBuffer);
		FLAG.resizeToCurrentText();
		FLAG.invalidate();
	}
}
