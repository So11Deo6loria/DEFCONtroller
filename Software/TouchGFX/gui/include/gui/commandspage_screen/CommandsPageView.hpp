#ifndef COMMANDSPAGEVIEW_HPP
#define COMMANDSPAGEVIEW_HPP

#include <gui_generated/commandspage_screen/CommandsPageViewBase.hpp>
#include <gui/commandspage_screen/CommandsPagePresenter.hpp>

class CommandsPageView : public CommandsPageViewBase
{
private:
	virtual void __UpdateDynamicData();

public:
    CommandsPageView();
    virtual ~CommandsPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void UpdateDoorLock_Status();
    virtual void UdateAutoIgnition_Status();
    virtual void UpdateSeatWarmer_Status();
protected:
};

#endif // COMMANDSPAGEVIEW_HPP
