#ifndef SPIMENUBUTTON_HPP
#define SPIMENUBUTTON_HPP

#include <gui_generated/containers/SpiMenuButtonBase.hpp>

class SpiMenuButton : public SpiMenuButtonBase
{
public:
    SpiMenuButton();
    virtual ~SpiMenuButton() {}

    virtual void initialize();
protected:
};

#endif // SPIMENUBUTTON_HPP
