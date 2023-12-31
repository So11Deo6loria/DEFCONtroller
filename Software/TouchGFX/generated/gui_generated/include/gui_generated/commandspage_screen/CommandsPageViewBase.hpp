/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef COMMANDSPAGEVIEWBASE_HPP
#define COMMANDSPAGEVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/commandspage_screen/CommandsPagePresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <gui/containers/BackButton_toMainScreen.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class CommandsPageViewBase : public touchgfx::View<CommandsPagePresenter>
{
public:
    CommandsPageViewBase();
    virtual ~CommandsPageViewBase() {}
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
    touchgfx::TextArea ManufacturerInterface;
    touchgfx::Button Commands_Button;
    touchgfx::TextArea ManufacturerInterface_1;
    BackButton_toMainScreen backButton_toMainScreen1;
    touchgfx::Line line1;
    touchgfx::PainterRGB565 line1Painter;
    touchgfx::TextArea Line1_Text;
    touchgfx::TextArea Line2_Text;
    touchgfx::TextArea Line3_Text;
    touchgfx::TextArea Line1_Text_1;
    touchgfx::TextArea Line1_Text_2;
    touchgfx::TextAreaWithOneWildcard UART_FLAG;

    /*
     * Wildcard Buffers
     */
    static const uint16_t UART_FLAG_SIZE = 32;
    touchgfx::Unicode::UnicodeChar UART_FLAGBuffer[UART_FLAG_SIZE];

private:

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 3600;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // COMMANDSPAGEVIEWBASE_HPP
