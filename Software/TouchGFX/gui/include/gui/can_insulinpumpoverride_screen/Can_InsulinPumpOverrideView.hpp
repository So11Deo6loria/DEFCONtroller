#ifndef CAN_INSULINPUMPOVERRIDEVIEW_HPP
#define CAN_INSULINPUMPOVERRIDEVIEW_HPP

#include <gui_generated/can_insulinpumpoverride_screen/Can_InsulinPumpOverrideViewBase.hpp>
#include <gui/can_insulinpumpoverride_screen/Can_InsulinPumpOverridePresenter.hpp>

class Can_InsulinPumpOverrideView : public Can_InsulinPumpOverrideViewBase
{
public:
    Can_InsulinPumpOverrideView();
    virtual ~Can_InsulinPumpOverrideView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CAN_INSULINPUMPOVERRIDEVIEW_HPP
