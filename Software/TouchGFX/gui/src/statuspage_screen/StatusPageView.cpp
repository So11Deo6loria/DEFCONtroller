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
	if ((debugFlagTouchGFX & (1 << 2)) != 0) // SPI Bit 2
	{
	    Unicode::snprintf(SPI_FLAGBuffer, SPI_FLAG_SIZE, (const char *)xSPIFlag);
	    SPI_FLAG.setWildcard(SPI_FLAGBuffer);
	    SPI_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
	    SPI_FLAG.resizeToCurrentText();
	    SPI_FLAG.invalidate();
	}
	else
	{
		Unicode::snprintf(SPI_FLAGBuffer, SPI_FLAG_SIZE, "               ");
		SPI_FLAG.setWildcard(SPI_FLAGBuffer);
		SPI_FLAG.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		SPI_FLAG.resizeToCurrentText();
		SPI_FLAG.invalidate();
	}
}
