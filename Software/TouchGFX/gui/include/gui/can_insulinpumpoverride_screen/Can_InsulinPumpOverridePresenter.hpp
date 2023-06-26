#ifndef CAN_INSULINPUMPOVERRIDEPRESENTER_HPP
#define CAN_INSULINPUMPOVERRIDEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Can_InsulinPumpOverrideView;

class Can_InsulinPumpOverridePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Can_InsulinPumpOverridePresenter(Can_InsulinPumpOverrideView& v);

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

    virtual ~Can_InsulinPumpOverridePresenter() {};

    virtual void UpdateSpiDebugValue(uint16_t value);
    virtual void UpdateTempValue(void);


#ifdef DEVBOARDBUILD
    virtual void UpdateBSValue( uint16_t value );
#endif

private:
    Can_InsulinPumpOverridePresenter();

    Can_InsulinPumpOverrideView& view;
};

#endif // CAN_INSULINPUMPOVERRIDEPRESENTER_HPP
