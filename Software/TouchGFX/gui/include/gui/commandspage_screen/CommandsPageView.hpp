#ifndef COMMANDSPAGEVIEW_HPP
#define COMMANDSPAGEVIEW_HPP

#include <gui_generated/commandspage_screen/CommandsPageViewBase.hpp>
#include <gui/commandspage_screen/CommandsPagePresenter.hpp>

class CommandsPageView : public CommandsPageViewBase
{
public:
    CommandsPageView();
    virtual ~CommandsPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // COMMANDSPAGEVIEW_HPP