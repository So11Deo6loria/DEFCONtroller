#ifndef BACKBUTTON_TOMAINSCREEN_HPP
#define BACKBUTTON_TOMAINSCREEN_HPP

#include <gui_generated/containers/BackButton_toMainScreenBase.hpp>

class BackButton_toMainScreen : public BackButton_toMainScreenBase
{
public:
    BackButton_toMainScreen();
    virtual ~BackButton_toMainScreen() {}

    virtual void initialize();
protected:
};

#endif // BACKBUTTON_TOMAINSCREEN_HPP
