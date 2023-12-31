/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef STATUSPAGEVIEWBASE_HPP
#define STATUSPAGEVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/statuspage_screen/StatusPagePresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <gui/containers/BackButton_toMainScreen.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/Button.hpp>

class StatusPageViewBase : public touchgfx::View<StatusPagePresenter>
{
public:
    StatusPageViewBase();
    virtual ~StatusPageViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Image Background;
    BackButton_toMainScreen backButton_toMainScreen1;
    touchgfx::TextArea Odometer_Text;
    touchgfx::TextArea textArea1_1;
    touchgfx::TextArea SelfDestruction_Text;
    touchgfx::TextAreaWithOneWildcard SelfDestCount_Val;
    touchgfx::TextArea VIN_Text;
    touchgfx::TextArea OilLife_Text;
    touchgfx::TextArea MilesToEmpty_Text;
    touchgfx::Line line1;
    touchgfx::PainterRGB565 line1Painter;
    touchgfx::Button Commands_Button;
    touchgfx::TextArea FirmwareVer_Text;
    touchgfx::TextAreaWithOneWildcard Odometer_Val;
    touchgfx::TextAreaWithOneWildcard Odometer_Text_1_2_1;
    touchgfx::TextAreaWithOneWildcard OilLife_Val;
    touchgfx::TextAreaWithOneWildcard VIN_Val;
    touchgfx::TextAreaWithOneWildcard FirmwareVer_Val;
    touchgfx::TextAreaWithOneWildcard MilesToEmpty_Val;

    /*
     * Wildcard Buffers
     */
    static const uint16_t SELFDESTCOUNT_VAL_SIZE = 32;
    touchgfx::Unicode::UnicodeChar SelfDestCount_ValBuffer[SELFDESTCOUNT_VAL_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<StatusPageViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 3600;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // STATUSPAGEVIEWBASE_HPP
