#ifndef I2CPAGEPRESENTER_HPP
#define I2CPAGEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class I2CPageView;

class I2CPagePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    I2CPagePresenter(I2CPageView& v);

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

    virtual ~I2CPagePresenter() {};

    virtual void UpdateBSValue( uint16_t value );

    virtual void UpdateSpiDebugValue(uint16_t value);
    virtual void UpdateTempValue(void);



private:
    I2CPagePresenter();

    I2CPageView& view;
};

#endif // I2CPAGEPRESENTER_HPP
