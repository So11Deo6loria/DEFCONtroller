#ifndef SPIPAGEPRESENTER_HPP
#define SPIPAGEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SPIPageView;

class SPIPagePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SPIPagePresenter(SPIPageView& v);

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

    virtual ~SPIPagePresenter() {};

    //Custom Callback Handling

    virtual void UpdateBSValue( uint16_t value );

#ifdef DEVBOARDBUILD
    virtual void UpdateSpiDebugValue(uint16_t value);
    virtual void UpdateTempValue(void);

#endif

private:
    SPIPagePresenter();

    SPIPageView& view;
};

#endif // SPIPAGEPRESENTER_HPP
