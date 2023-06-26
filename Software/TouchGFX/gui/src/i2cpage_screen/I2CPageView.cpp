#include <gui/i2cpage_screen/I2CPageView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>


extern __IO uint8_t i2c_isValueUpdated;
extern __IO float i2c_temperature;
extern __IO uint8_t i2c_isPumpAllowed;
extern __I char xI2CFlag[32];

//static void I2CPageView::__UpdateDynamicValues( void );

I2CPageView::I2CPageView()
{
	__UpdateDynamicValues();
}

void I2CPageView::setupScreen()
{
    I2CPageViewBase::setupScreen();
}

void I2CPageView::tearDownScreen()
{
    I2CPageViewBase::tearDownScreen();
}

void I2CPageView::UpdateTempValue()
{
	__UpdateDynamicValues();
}

void I2CPageView::__UpdateDynamicValues( void )
{
	Unicode::snprintfFloat (TempValueBuffer, TEMPVALUE_SIZE, "%#3.1f", i2c_temperature);
	TempValue.setWildcard (TempValueBuffer);
	TempValue.setTypedText (touchgfx::TypedText (T_TEMPFIELD));
	TempValue.resizeToCurrentText();
//	TempValue.invalidate ();

	if (i2c_isPumpAllowed)
	{
		Unicode::snprintf (PumpAllowedStatusBuffer, PUMPALLOWEDSTATUS_SIZE, "Allowed");
		PumpAllowedStatus.setColor (touchgfx::Color::getColorFromRGB (34, 255, 0));
		PumpAllowedStatus.setWildcard (PumpAllowedStatusBuffer);
    PumpAllowedStatus.setAlpha(150);
		PumpAllowedStatus.resizeToCurrentText();
    FLAG_FIELD.setAlpha(0);
    FLAG_FIELD.invalidate();
		TempValue.setAlpha(255);
		TempValue.invalidate();
	}
	else
	{
		Unicode::snprintf (PumpAllowedStatusBuffer, PUMPALLOWEDSTATUS_SIZE, "Restricted");
		PumpAllowedStatus.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
		PumpAllowedStatus.setAlpha(255);
		PumpAllowedStatus.resizeToCurrentText();
		PumpAllowedStatus.setWildcard (PumpAllowedStatusBuffer);
    FLAG_FIELD.setAlpha(255);
    Unicode::snprintf(FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)xI2CFlag);
    FLAG_FIELD.setWildcard(FLAG_FIELDBuffer);
		FLAG_FIELD.resizeToCurrentText();
		FLAG_FIELD.invalidate();
		TempValue.setAlpha(0);
		TempValue.invalidate();
	}
	PumpAllowedStatus.invalidate ();
}
