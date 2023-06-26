#ifndef CAN_HPP
#define CAN_HPP

#include <gui_generated/containers/CANBase.hpp>

class CAN : public CANBase
{
public:
    CAN();
    virtual ~CAN() {}

    virtual void initialize();
protected:
};

#endif // CAN_HPP
