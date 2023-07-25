#ifndef MAINMENUVIEW_HPP
#define MAINMENUVIEW_HPP

#include <gui_generated/mainmenu_screen/MainMenuViewBase.hpp>
#include <gui/mainmenu_screen/MainMenuPresenter.hpp>

class MainMenuView : public MainMenuViewBase
{
private:
	virtual void __UpdateDynamicData();

public:
    MainMenuView();
    virtual ~MainMenuView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

#ifdef DEVBOARDBUILD
    virtual void UpdateSpiDebugValue(uint16_t value);
#endif

protected:
};

#endif // MAINMENUVIEW_HPP
