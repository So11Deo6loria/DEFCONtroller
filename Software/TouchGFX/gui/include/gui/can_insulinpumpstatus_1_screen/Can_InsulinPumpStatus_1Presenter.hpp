#ifndef CAN_INSULINPUMPSTATUS_1PRESENTER_HPP
#define CAN_INSULINPUMPSTATUS_1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Can_InsulinPumpStatus_1View;

class Can_InsulinPumpStatus_1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Can_InsulinPumpStatus_1Presenter(Can_InsulinPumpStatus_1View& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Can_InsulinPumpStatus_1Presenter() {};
    virtual void UpdateTempValue(void);


private:
    Can_InsulinPumpStatus_1Presenter();

    Can_InsulinPumpStatus_1View& view;
};

#endif // CAN_INSULINPUMPSTATUS_1PRESENTER_HPP
