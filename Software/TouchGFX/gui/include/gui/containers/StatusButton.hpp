#ifndef STATUSBUTTON_HPP
#define STATUSBUTTON_HPP

#include <gui_generated/containers/StatusButtonBase.hpp>

class StatusButton : public StatusButtonBase
{
public:
    StatusButton();
    virtual ~StatusButton() {}

    virtual void initialize();
protected:
};

#endif // STATUSBUTTON_HPP
