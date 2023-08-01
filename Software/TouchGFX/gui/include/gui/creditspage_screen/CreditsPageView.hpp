#ifndef CREDITSPAGEVIEW_HPP
#define CREDITSPAGEVIEW_HPP

#include <gui_generated/creditspage_screen/CreditsPageViewBase.hpp>
#include <gui/creditspage_screen/CreditsPagePresenter.hpp>

class CreditsPageView : public CreditsPageViewBase
{
public:
    CreditsPageView();
    virtual ~CreditsPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CREDITSPAGEVIEW_HPP
