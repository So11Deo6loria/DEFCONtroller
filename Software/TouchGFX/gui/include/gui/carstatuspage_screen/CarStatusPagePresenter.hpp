#ifndef CARSTATUSPAGEPRESENTER_HPP
#define CARSTATUSPAGEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CarStatusPageView;

class CarStatusPagePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CarStatusPagePresenter(CarStatusPageView& v);

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

    virtual ~CarStatusPagePresenter() {};

private:
    CarStatusPagePresenter();

    CarStatusPageView& view;
};

#endif // CARSTATUSPAGEPRESENTER_HPP
