#ifndef COMMANDSPAGEPRESENTER_HPP
#define COMMANDSPAGEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CommandsPageView;

class CommandsPagePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CommandsPagePresenter(CommandsPageView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~CommandsPagePresenter() {};

    virtual void UpdateBSValue(uint16_t value);
	virtual void UpdateSpiDebugValue(uint16_t value);
	virtual void UpdateTempValue(void);
    virtual void UpdateDoorLock_Status(void);
    virtual void UpdateAutoIgnition_Status(void);
    virtual void UpdateSeatWarmer_Status(void);

private:
    CommandsPagePresenter();

    CommandsPageView& view;
};

#endif // COMMANDSPAGEPRESENTER_HPP
