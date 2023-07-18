#ifndef SEATWARMERBUTTON_HPP
#define SEATWARMERBUTTON_HPP

#include <gui_generated/containers/SeatWarmerButtonBase.hpp>

class SeatWarmerButton : public SeatWarmerButtonBase
{
public:
    SeatWarmerButton();
    virtual ~SeatWarmerButton() {}

    virtual void initialize();
protected:
};

#endif // SEATWARMERBUTTON_HPP
