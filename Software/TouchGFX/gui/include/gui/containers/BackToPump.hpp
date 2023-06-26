#ifndef BACKTOPUMP_HPP
#define BACKTOPUMP_HPP

#include <gui_generated/containers/BackToPumpBase.hpp>

class BackToPump : public BackToPumpBase
{
public:
    BackToPump();
    virtual ~BackToPump() {}

    virtual void initialize();
protected:
};

#endif // BACKTOPUMP_HPP
