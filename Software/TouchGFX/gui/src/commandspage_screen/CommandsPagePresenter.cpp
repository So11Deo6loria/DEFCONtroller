#include <gui/commandspage_screen/CommandsPageView.hpp>
#include <gui/commandspage_screen/CommandsPagePresenter.hpp>
#include "stdint.h"

CommandsPagePresenter::CommandsPagePresenter(CommandsPageView& v)
    : view(v)
{

}

void CommandsPagePresenter::activate()
{

}

void CommandsPagePresenter::deactivate()
{

}

#ifdef DEVBOARDBUILD

void CommandsPagePresenter::UpdateSpiDebugValue(uint16_t)
{
	//Do Nothing
}

void CommandsPagePresenter::UpdateBSValue(uint16_t)
{
	//Do Nothing
}

void CommandsPagePresenter::UpdateTempValue(void)
{
	//Do Nothing
}

void CommandsPagePresenter::UpdateDoorLock_Status(void)
{
	//Do Nothing
	view.UpdateDoorLock_Status();
}

void CommandsPagePresenter::UpdateAutoIgnition_Status(void)
{
	//Do Nothing
	view.UdateAutoIgnition_Status()
}

void CommandsPagePresenter::UpdateSeatWarmer_Status(void)
{
	//Do Nothing
	view.UpdateSeatWarmer_Status()
}

#endif
