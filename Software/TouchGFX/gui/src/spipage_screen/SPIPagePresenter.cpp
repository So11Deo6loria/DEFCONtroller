#include <gui/spipage_screen/SPIPageView.hpp>
#include <gui/spipage_screen/SPIPagePresenter.hpp>

SPIPagePresenter::SPIPagePresenter(SPIPageView& v)
    : view(v)
{

}

void SPIPagePresenter::activate()
{

}

void SPIPagePresenter::deactivate()
{

}

#ifdef DEVBOARDBUILD
void SPIPagePresenter::UpdateBSValue(uint16_t value)
{

}


void SPIPagePresenter::UpdateSpiDebugValue(uint16_t value)
{
	view.UpdateSpiDebugValue(value);
}

void SPIPagePresenter::UpdateTempValue(void)
{

}
#endif
