#ifndef I2CPAGEVIEW_HPP
#define I2CPAGEVIEW_HPP

#include <gui_generated/i2cpage_screen/I2CPageViewBase.hpp>
#include <gui/i2cpage_screen/I2CPagePresenter.hpp>

class I2CPageView : public I2CPageViewBase
{

private:
		virtual void __UpdateDynamicValues(void);
public:
    I2CPageView();
    virtual ~I2CPageView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void UpdateTempValue();
protected:
};

#endif // I2CPAGEVIEW_HPP
