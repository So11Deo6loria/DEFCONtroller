#ifndef CAN_INSULINPUMPSTATUSVIEW_HPP
#define CAN_INSULINPUMPSTATUSVIEW_HPP

#include <gui_generated/can_insulinpumpstatus_screen/Can_InsulinPumpStatusViewBase.hpp>
#include <gui/can_insulinpumpstatus_screen/Can_InsulinPumpStatusPresenter.hpp>

#ifdef DEVBOARDBUILD
#include "main.h"
#endif


class Can_InsulinPumpStatusView : public Can_InsulinPumpStatusViewBase
{
private:
		virtual void __UpdateDynamicData(void);

public:
    Can_InsulinPumpStatusView();
    virtual ~Can_InsulinPumpStatusView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void ToggleButtonClickVirtFunc( void );

#ifdef DEVBOARDBUILD
    virtual void UpdateBSValue( uint16_t value );
#endif

protected:
};

#endif // CAN_INSULINPUMPSTATUSVIEW_HPP
