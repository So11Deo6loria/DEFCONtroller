/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/ManufactureButtonScreenBase.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

ManufactureButtonScreenBase::ManufactureButtonScreenBase() :
    flexButtonCallback(this, &ManufactureButtonScreenBase::flexButtonCallbackHandler)
{
    setWidth(64);
    setHeight(64);
    scalableImage1.setBitmap(touchgfx::Bitmap(BITMAP_RECON_50X64_ID));
    scalableImage1.setPosition(0, 7, 64, 50);
    scalableImage1.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    manufactureButton.setBoxWithBorderPosition(0, 0, 240, 64);
    manufactureButton.setBorderSize(10);
    manufactureButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(0, 102, 153), touchgfx::Color::getColorFromRGB(0, 153, 204), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(51, 102, 153));
    manufactureButton.setPosition(0, 0, 64, 64);
    manufactureButton.setAlpha(0);
    manufactureButton.setAction(flexButtonCallback);

    add(scalableImage1);
    add(manufactureButton);
}

void ManufactureButtonScreenBase::initialize()
{

}

void ManufactureButtonScreenBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &manufactureButton)
    {
        //Interaction1
        //When manufactureButton clicked change screen to CommandsPage
        //Go to CommandsPage with screen transition towards North
        application().gotoCommandsPageScreenSlideTransitionNorth();
    }
}

