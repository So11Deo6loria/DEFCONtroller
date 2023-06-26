#ifndef SPIPAGEVIEW_HPP
#define SPIPAGEVIEW_HPP

#include <gui_generated/spipage_screen/SPIPageViewBase.hpp>
#include <gui/spipage_screen/SPIPagePresenter.hpp>

class SPIPageView : public SPIPageViewBase
{
private:
		virtual void __UpdateDynamicData();

public:
    SPIPageView();
    virtual ~SPIPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

#ifdef DEVBOARDBUILD
    virtual void UpdateSpiDebugValue(uint16_t value);
#endif
protected:
};

#endif // SPIPAGEVIEW_HPP
