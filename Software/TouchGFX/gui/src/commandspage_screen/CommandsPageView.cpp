#include <gui/commandspage_screen/CommandsPageView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

//#define FLAG_FIELD_SIZE 40

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xJTAGFlag[32];
extern __IO char xI2CFlag[32];

CommandsPageView::CommandsPageView()
{

}

void CommandsPageView::setupScreen()
{
    CommandsPageViewBase::setupScreen();
}

void CommandsPageView::tearDownScreen()
{
    CommandsPageViewBase::tearDownScreen();
}

#ifdef DEVBOARDBUILD

void CommandsPageView::UpdateSeatWarmer_Status(void){

}

void  CommandsPageView::__UpdateDynamicData(void)
{
	if(debugFlagTouchGFX == 0){ //debug disabled
	    Unicode::snprintf(FLAGBuffer, FLAG_SIZE, "Go Find A Flag");
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

		Unicode::snprintf(FLAGBuffer, FLAG_SIZE, (const char *)xSPIFlag); // TODO: Replace flag field buffer SPI
		FLAG.setWildcard(FLAGBuffer);
		FLAG.resizeToCurrentText();
		FLAG.invalidate();
	}

}

#endif
