#ifndef I2C_HPP
#define I2C_HPP

#include <gui_generated/containers/I2CBase.hpp>

class I2C : public I2CBase
{
public:
    I2C();
    virtual ~I2C() {}

    virtual void initialize();
protected:
};

#endif // I2C_HPP
