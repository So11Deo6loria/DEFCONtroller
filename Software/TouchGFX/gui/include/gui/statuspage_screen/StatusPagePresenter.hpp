#ifndef STATUSPAGEPRESENTER_HPP
#define STATUSPAGEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class StatusPageView;

class StatusPagePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    StatusPagePresenter(StatusPageView& v);

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

    virtual ~StatusPagePresenter() {};

private:
    StatusPagePresenter();

    StatusPageView& view;
};

#endif // STATUSPAGEPRESENTER_HPP
