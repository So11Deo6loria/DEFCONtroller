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
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/canvas/Shape.hpp>
#include <gui/containers/BackButton_toMainScreen.hpp>
#include <gui/containers/ManufactureButtonScreen.hpp>
#include <touchgfx/mixins/ClickListener.hpp>

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
    touchgfx::TextArea Odometer_Text;
    touchgfx::TextArea textArea1_1;
    touchgfx::TextArea debugModeText;
    touchgfx::TextArea activeInsulinText;
    touchgfx::TextArea FirmwareVer_Text;
    touchgfx::TextAreaWithOneWildcard bgVal;
    touchgfx::TextAreaWithOneWildcard bgText;
    touchgfx::TextAreaWithOneWildcard debugModeVal;
    touchgfx::TextAreaWithOneWildcard FirmwareVer_Val;
    touchgfx::TextAreaWithOneWildcard activeInsulinVal;
    touchgfx::Button Credits_Button;
    touchgfx::Line line1;
    touchgfx::PainterRGB565 line1Painter;
    touchgfx::ClickListener< touchgfx::Shape<4> > titleBackground;
    touchgfx::PainterRGB565 titleBackgroundPainter;
    touchgfx::TextAreaWithOneWildcard SPI_FLAG;
    BackButton_toMainScreen backButton_toMainScreen1;
    ManufactureButtonScreen manufactureButtonScreen1;

    /*
     * Wildcard Buffers
     */
    static const uint16_t BGVAL_SIZE = 5;
    touchgfx::Unicode::UnicodeChar bgValBuffer[BGVAL_SIZE];
    static const uint16_t DEBUGMODEVAL_SIZE = 10;
    touchgfx::Unicode::UnicodeChar debugModeValBuffer[DEBUGMODEVAL_SIZE];
    static const uint16_t FIRMWAREVER_VAL_SIZE = 10;
    touchgfx::Unicode::UnicodeChar FirmwareVer_ValBuffer[FIRMWAREVER_VAL_SIZE];
    static const uint16_t ACTIVEINSULINVAL_SIZE = 10;
    touchgfx::Unicode::UnicodeChar activeInsulinValBuffer[ACTIVEINSULINVAL_SIZE];
    static const uint16_t SPI_FLAG_SIZE = 32;
    touchgfx::Unicode::UnicodeChar SPI_FLAGBuffer[SPI_FLAG_SIZE];

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
