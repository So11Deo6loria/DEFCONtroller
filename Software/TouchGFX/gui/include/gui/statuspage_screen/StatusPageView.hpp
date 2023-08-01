#ifndef STATUSPAGEVIEW_HPP
#define STATUSPAGEVIEW_HPP

#include <gui_generated/statuspage_screen/StatusPageViewBase.hpp>
#include <gui/statuspage_screen/StatusPagePresenter.hpp>

class StatusPageView : public StatusPageViewBase
{
public:
    StatusPageView();
    virtual ~StatusPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // STATUSPAGEVIEW_HPP
