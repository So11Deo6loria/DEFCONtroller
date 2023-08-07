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
    //Button Events
    virtual void SearWarmerButtonPressed();
    virtual void DoorLockButtonPressed();
    virtual void AutoIgnitionButtonPressed();

    // Button State Update Handlers
    virtual void UpdateDoorLockStatus(void);
    virtual void UpdateAutoIgnitionStatus(void);
    virtual void UpdateSeatWarmerStatus(void);
    virtual void UpdateSPIDebugValue(uint16_t value);
protected:
};

#endif // MAINMENUVIEW_HPP
