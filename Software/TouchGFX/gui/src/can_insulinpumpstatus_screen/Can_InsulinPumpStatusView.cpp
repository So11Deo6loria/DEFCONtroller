#include <gui/can_insulinpumpstatus_screen/Can_InsulinPumpStatusView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include "stdint.h"

extern "C" void CanChallengeButtonPressed(uint8_t State);
extern __IO uint8_t CanTask_BSUpdated;
extern __IO uint16_t CanTask_BSValue;
extern __IO uint8_t CanTask_isBSGood;
extern __IO uint8_t debugFlagTouchGFX;
extern __IO uint8_t CanTask_ToggleState;
extern __IO uint xUARTFlag[32];
extern __IO char xSPIFlag[32];
extern __IO char xJTAGFlag[32];
extern __IO char xI2CFlag[32];

Can_InsulinPumpStatusView::Can_InsulinPumpStatusView()
{
	__UpdateDynamicData();
}

void Can_InsulinPumpStatusView::setupScreen()
{
    Can_InsulinPumpStatusViewBase::setupScreen();
}

void Can_InsulinPumpStatusView::tearDownScreen()
{
    Can_InsulinPumpStatusViewBase::tearDownScreen();
}

void Can_InsulinPumpStatusView::ToggleButtonClickVirtFunc( void )
{
#ifdef DEVBOARDBUILD
	CanChallengeButtonPressed(InsulinToggle.getState() );
#endif
}

#ifdef DEVBOARDBUILD
void Can_InsulinPumpStatusView::UpdateBSValue( uint16_t value )
{
	__UpdateDynamicData();
}

void Can_InsulinPumpStatusView::__UpdateDynamicData( void )
{
	Unicode::snprintf(BloodSugarValueFieldBuffer, BLOODSUGARVALUEFIELD_SIZE, "%d", CanTask_BSValue);
	BloodSugarValueField.setWildcard(BloodSugarValueFieldBuffer);
	BloodSugarValueField.resizeToCurrentText();
	BloodSugarValueField.invalidate ();

	if (CanTask_ToggleState)
	{
		InsulinToggle.forceState(true);
		box1.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		box1.setAlpha (255);

		BloodSugarValueField.setColor (touchgfx::Color::getColorFromRGB (0, 0, 0));
		BloodSugarValueField.setAlpha (200);

		PumpTitle.setColor (touchgfx::Color::getColorFromRGB (0, 0, 0));
		PumpTitle.setAlpha (200);

		PumpStatus.setColor (touchgfx::Color::getColorFromRGB (0, 0, 0));
		PumpStatus.setAlpha (100);
		Unicode::snprintf (PumpStatusBuffer, PUMPSTATUS_SIZE, "Active");
		PumpStatus.setWildcard (PumpStatusBuffer);
		PumpStatus.resizeToCurrentText ();
		PumpStatus.setTypedText (touchgfx::TypedText (T___SINGLEUSE_2APL));

		unit_top.setColor (touchgfx::Color::getColorFromRGB (0, 0, 0));
		unit_top.setAlpha (200);

		unit_bottom.setColor (touchgfx::Color::getColorFromRGB (0, 0, 0));
		unit_bottom.setAlpha (200);
	}
	else
	{
		InsulinToggle.forceState(false);

		box1.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		box1.setAlpha (57);

		BloodSugarValueField.setColor (
				touchgfx::Color::getColorFromRGB (255, 255, 255));
		BloodSugarValueField.setAlpha (255);

		PumpTitle.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		PumpTitle.setAlpha (255);

		PumpStatus.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		PumpStatus.setAlpha (180);
		Unicode::snprintf (PumpStatusBuffer, PUMPSTATUS_SIZE, "Inactive");
		PumpStatus.setWildcard (PumpStatusBuffer);
		PumpStatus.resizeToCurrentText ();
		PumpStatus.setTypedText (touchgfx::TypedText (T___SINGLEUSE_2APL));


		unit_top.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
		unit_top.setAlpha(255);

		unit_bottom.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
		unit_bottom.setAlpha(255);
	}

	if (debugFlagTouchGFX)
	{
		ProtectionLabel.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		ProtectionLabel.setAlpha (200);
		ProtectionLabel.setTypedText (touchgfx::TypedText (T___SINGLEUSE_EZHH));

		ProtectionActiveField.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
		ProtectionActiveField.setAlpha (255);
		Unicode::snprintf (ProtectionActiveFieldBuffer, PROTECTIONACTIVEFIELD_SIZE, "Disabled");
		ProtectionActiveField.setWildcard (ProtectionActiveFieldBuffer);
		ProtectionActiveField.resizeToCurrentText ();

		OverrideBackground.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		OverrideBackground.setAlpha (57);

		UnlockedIcon.setAlpha(255);
		LockedIcon.setAlpha(0);
	}
	else
	{
		ProtectionLabel.setColor (touchgfx::Color::getColorFromRGB (0, 0, 0));
		ProtectionLabel.setTypedText (touchgfx::TypedText (T___SINGLEUSE_EZHH));

		ProtectionActiveField.setColor (touchgfx::Color::getColorFromRGB (00, 163, 0));
		Unicode::snprintf (ProtectionActiveFieldBuffer, PROTECTIONACTIVEFIELD_SIZE, "Active");
		ProtectionActiveField.setWildcard (ProtectionActiveFieldBuffer);
		ProtectionActiveField.resizeToCurrentText ();

		OverrideBackground.setColor (touchgfx::Color::getColorFromRGB (255, 255, 255));
		OverrideBackground.setAlpha (255);

		UnlockedIcon.setAlpha(0);
		LockedIcon.setAlpha(200);
	}

	if (debugFlagTouchGFX == 0)
	{ //debug disabled
		FLAG_FIELD.setAlpha (0);
		FLAG_FIELD.invalidate ();
	}
	else if (debugFlagTouchGFX == 7) //value == 1  debug enabled via RE
	{
		FLAG_FIELD.setAlpha (220);
		Unicode::snprintf (FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)(xJTAGFlag));
		FLAG_FIELD.setWildcard (FLAG_FIELDBuffer);
		FLAG_FIELD.resizeToCurrentText();
		FLAG_FIELD.invalidate ();

	}
	else if (debugFlagTouchGFX == 5) //value == 1  debug enabled via UART
	{
		FLAG_FIELD.setAlpha (255);
		Unicode::snprintf (FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)xUARTFlag);
		FLAG_FIELD.setWildcard (FLAG_FIELDBuffer);
		FLAG_FIELD.resizeToCurrentText();
		FLAG_FIELD.invalidate ();
	}
	else
	{
		FLAG_FIELD.setAlpha (255);
		Unicode::snprintf (FLAG_FIELDBuffer, FLAG_FIELD_SIZE, (const char *)xSPIFlag);
		FLAG_FIELD.setWildcard (FLAG_FIELDBuffer);
		FLAG_FIELD.resizeToCurrentText();
		FLAG_FIELD.invalidate ();
	}


	if (CanTask_isBSGood)
	{
		box2.setColor (touchgfx::Color::getColorFromRGB (0, 0, 0));
		box2.setAlpha(0);
		Image3.setAlpha (0);
	}
	else
	{
		box2.setColor (touchgfx::Color::getColorFromRGB (255, 0, 0));
		Image3.setAlpha (255);
		box2.setAlpha(150);
	}


	box1.invalidate();
	BloodSugarValueField.invalidate();
	PumpTitle.invalidate();
	PumpStatus.invalidate();
	ProtectionLabel.invalidate();
	ProtectionActiveField.invalidate();
	OverrideBackground.invalidate();
	box2.invalidate();
	Image3.invalidate();
	UnlockedIcon.invalidate();
	LockedIcon.invalidate();
	InsulinToggle.invalidate();
}
#endif
