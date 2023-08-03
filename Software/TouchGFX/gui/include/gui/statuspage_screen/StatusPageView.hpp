#ifndef STATUSPAGEVIEW_HPP
#define STATUSPAGEVIEW_HPP

#include <gui_generated/statuspage_screen/StatusPageViewBase.hpp>
#include <gui/statuspage_screen/StatusPagePresenter.hpp>

class StatusPageView : public StatusPageViewBase
{
private:
	virtual void __UpdateDynamicData();
public:
    StatusPageView();
    virtual ~StatusPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void UpdateSpiDebugValue(uint16_t value);
protected:
};

#endif // STATUSPAGEVIEW_HPP
