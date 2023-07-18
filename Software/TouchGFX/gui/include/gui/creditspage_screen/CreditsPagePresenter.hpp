#ifndef CREDITSPAGEPRESENTER_HPP
#define CREDITSPAGEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CreditsPageView;

class CreditsPagePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CreditsPagePresenter(CreditsPageView& v);

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

    virtual ~CreditsPagePresenter() {};

private:
    CreditsPagePresenter();

    CreditsPageView& view;
};

#endif // CREDITSPAGEPRESENTER_HPP
