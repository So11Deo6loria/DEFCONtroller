#include <gui/mainmenu_screen/MainMenuView.hpp>

MainMenuView::MainMenuView()
{

}

void MainMenuView::setupScreen()
{
    MainMenuViewBase::setupScreen();
}

void MainMenuView::tearDownScreen()
{
    MainMenuViewBase::tearDownScreen();
}
<<<<<<< HEAD

#ifdef DEVBOARDBUILD

void MainMenuView::UpdateSpiDebugValue(uint16_t value)
{
	__UpdateDynamicData();
}

void  MainMenuView::__UpdateDynamicData(void)
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
	    FLAG_FIELD.setWildcard(FLAG_FIELDBuffer);	// TODO: Replace flag field buffer JTAG.   replaces chars
		FLAG_FIELD.resizeToCurrentText();		// resizes object for new char length
		FLAG_FIELD.invalidate();
	}
	else if( debugFlagTouchGFX == 5)//value == 1  debug enabled via UART
	{
	    Unicode::snprintf(DebugFieldBuffer, DEBUGFIELD_SIZE, "Enabled");
	    FLAG_FIELD.setAlpha(255);
	    Unicode::snprintf(FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)xUARTFlag); // TODO: Replace flag field buffer UART
	    FLAG_FIELD.setWildcard(FLAG_FIELDBuffer);
			FLAG_FIELD.resizeToCurrentText();
	    FLAG_FIELD.invalidate();
	}
	else
	{
    Unicode::snprintf(DebugFieldBuffer, DEBUGFIELD_SIZE, "Enabled");
    FLAG_FIELD.setAlpha(255);
    Unicode::snprintf(FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)xSPIFlag); // TODO: Replace flag field buffer SPI
    FLAG_FIELD.setWildcard(FLAG_FIELDBuffer);
	FLAG_FIELD.resizeToCurrentText();
    FLAG_FIELD.invalidate();
	}

	DebugField.setWildcard(DebugFieldBuffer); // update debug field buffers TODO: update all buffers as they are all different
	DebugField.resizeToCurrentText();
	DebugField.invalidate();
}

#endif
=======
>>>>>>> parent of 68647ac (testing)
