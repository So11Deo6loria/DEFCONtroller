#ifndef MAINMENUVIEW_HPP
#define MAINMENUVIEW_HPP

#include <gui_generated/mainmenu_screen/MainMenuViewBase.hpp>
#include <gui/mainmenu_screen/MainMenuPresenter.hpp>

class MainMenuView : public MainMenuViewBase
{

private:
	virtual void  __UpdateDynamicData(void);
public:
    MainMenuView();
    virtual ~MainMenuView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // Button State Update Handlers
    virtual void UpdateSpiDebugValue(uint16_t value);
    void ToggleButtonClickVirtFunc( void );
protected:
};

#endif // MAINMENUVIEW_HPP
