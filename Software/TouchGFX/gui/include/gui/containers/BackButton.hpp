#ifndef BACKBUTTON_HPP
#define BACKBUTTON_HPP

#include <gui_generated/containers/BackButtonBase.hpp>

class BackButton : public BackButtonBase
{
public:
    BackButton();
    virtual ~BackButton() {}

    virtual void initialize();
protected:
};

#endif // BACKBUTTON_HPP
