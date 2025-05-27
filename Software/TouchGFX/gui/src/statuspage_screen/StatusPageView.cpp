#include <gui/statuspage_screen/StatusPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "string.h"

extern __IO uint8_t debugFlagTouchGFX;
extern __IO char xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xCANFlag[32];
extern __IO char xJTAGFlag[32];
extern "C" {
    #include "device_status.h"  // This brings in the DeviceStatus_t definition
}


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
	memset(SPI_FLAGBuffer, 0, sizeof(SPI_FLAGBuffer));  // Clear all 16-bit values
	if ((debugFlagTouchGFX & (1 << 2)) != 0) // SPI Bit 2
	{
	    Unicode::snprintf(SPI_FLAGBuffer, SPI_FLAG_SIZE, (const char *)xSPIFlag);
	    SPI_FLAG.setWildcard(SPI_FLAGBuffer);
	    SPI_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    SPI_FLAG.resizeToCurrentText();
//	    SPI_FLAG.invalidate();
	}
	else
	{
		Unicode::snprintf(SPI_FLAGBuffer, SPI_FLAG_SIZE, "               ");
		SPI_FLAG.setWildcard(SPI_FLAGBuffer);
		SPI_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		SPI_FLAG.resizeToCurrentText();
//		SPI_FLAG.invalidate();
	}

	// Pin
	memset(bgValBuffer, 0, sizeof(bgValBuffer));  // Clear all 16-bit values
	Unicode::strncpy(bgValBuffer, gDeviceStatus.pin, BGVAL_SIZE - 1);  // Leave room for null terminator
	bgVal.setWildcard(bgValBuffer);
	bgVal.resizeToCurrentTextWithAlignment();
//	bgVal.invalidate();

	// HUD State
	memset(activeInsulinValBuffer, 0, sizeof(activeInsulinValBuffer));  // Clear all 16-bit values
	Unicode::strncpy(activeInsulinValBuffer, gDeviceStatus.hudState, ACTIVEINSULINVAL_SIZE - 1);  // Leave room for null terminator
	activeInsulinVal.setWildcard(activeInsulinValBuffer);
	activeInsulinVal.resizeToCurrentTextWithAlignment();
//	activeInsulinVal.invalidate();

	// Debug Mode
	memset(debugModeValBuffer, 0, sizeof(debugModeValBuffer));
	if( gDeviceStatus.debugEnabled == 1 )
	{
		Unicode::snprintf(debugModeValBuffer, DEBUGMODEVAL_SIZE, "Enabled");
	}
	else
	{
		Unicode::snprintf(debugModeValBuffer, DEBUGMODEVAL_SIZE, "Disabled");
	}
	debugModeVal.setWildcard(debugModeValBuffer);
	debugModeVal.resizeToCurrentTextWithAlignment();
//	debugModeVal.invalidate();

	// Firmware Version
	memset(FirmwareVer_ValBuffer, 0, sizeof(FirmwareVer_ValBuffer));
	Unicode::strncpy(FirmwareVer_ValBuffer, gDeviceStatus.firmwareVersion, FIRMWAREVER_VAL_SIZE - 1);  // Leave room for null terminator
	FirmwareVer_Val.setWildcard(FirmwareVer_ValBuffer);
	FirmwareVer_Val.resizeToCurrentTextWithAlignment();
//	FirmwareVer_Val.invalidate();
	Background.invalidate();
}
