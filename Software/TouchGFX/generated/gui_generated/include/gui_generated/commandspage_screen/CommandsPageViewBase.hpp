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
#include <gui/containers/BackButton_toStatusPage.hpp>
#include <touchgfx/widgets/TextArea.hpp>

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
    BackButton_toStatusPage backButton_toCarStatusPage1;
    touchgfx::Image DVHID_Image;
    touchgfx::TextArea UART_Text;
    touchgfx::TextArea textArea1_1;

private:

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 3600;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // COMMANDSPAGEVIEWBASE_HPP