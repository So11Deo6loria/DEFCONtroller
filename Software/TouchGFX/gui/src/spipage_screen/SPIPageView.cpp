#include <gui/spipage_screen/SPIPageView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern __IO uint8_t debugFlagTouchGFX;
extern __I char xUARTFlag[32];
extern __I char xSPIFlag[32];
extern __I char xJTAGFlag[32];
extern __I char xI2CFlag[32];

SPIPageView::SPIPageView()
{
  __UpdateDynamicData();
}

void SPIPageView::setupScreen ()
{
}

void SPIPageView::tearDownScreen()
{
    SPIPageViewBase::tearDownScreen();
}

#ifdef DEVBOARDBUILD

void SPIPageView::UpdateSpiDebugValue(uint16_t value)
{
	__UpdateDynamicData();
}

void  SPIPageView::__UpdateDynamicData(void)
{
	if(debugFlagTouchGFX == 0){ //debug disabled
	    Unicode::snprintf(DebugFieldBuffer, DEBUGFIELD_SIZE, "Not Enabled");
	    FLAG_FIELD.setAlpha(0);		// Make it invisible (opacity)
	    FLAG_FIELD.invalidate();	// Redraws object
	}
	else if( debugFlagTouchGFX == 7)//value == 1  debug enabled via RE
	{
	    Unicode::snprintf(DebugFieldBuffer, DEBUGFIELD_SIZE, "Enabled");
	    FLAG_FIELD.setAlpha(255);
	    Unicode::snprintf(FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *) xJTAGFlag);
	    FLAG_FIELD.setWildcard(FLAG_FIELDBuffer);	// replaces chars
			FLAG_FIELD.resizeToCurrentText();		// resizes object for new char length
			FLAG_FIELD.invalidate();
	}
	else if( debugFlagTouchGFX == 5)//value == 1  debug enabled via UART
	{
	    Unicode::snprintf(DebugFieldBuffer, DEBUGFIELD_SIZE, "Enabled");
	    FLAG_FIELD.setAlpha(255);
	    Unicode::snprintf(FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)xUARTFlag);
	    FLAG_FIELD.setWildcard(FLAG_FIELDBuffer);
			FLAG_FIELD.resizeToCurrentText();
	    FLAG_FIELD.invalidate();
	}
	else
	{
    Unicode::snprintf(DebugFieldBuffer, DEBUGFIELD_SIZE, "Enabled");
    FLAG_FIELD.setAlpha(255);
    Unicode::snprintf(FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)xSPIFlag);
    FLAG_FIELD.setWildcard(FLAG_FIELDBuffer);
		FLAG_FIELD.resizeToCurrentText();
    FLAG_FIELD.invalidate();
	}

	DebugField.setWildcard(DebugFieldBuffer); // update debug field buffers
	DebugField.resizeToCurrentText();
	DebugField.invalidate();
}

#endif
