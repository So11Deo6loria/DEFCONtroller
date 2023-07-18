#ifndef CARSTATUSPAGEVIEW_HPP
#define CARSTATUSPAGEVIEW_HPP

#include <gui_generated/carstatuspage_screen/CarStatusPageViewBase.hpp>
#include <gui/carstatuspage_screen/CarStatusPagePresenter.hpp>

class CarStatusPageView : public CarStatusPageViewBase
{
public:
    CarStatusPageView();
    virtual ~CarStatusPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CARSTATUSPAGEVIEW_HPP
