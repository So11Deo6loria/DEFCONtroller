/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef BSUNITS_1BASE_HPP
#define BSUNITS_1BASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class BSUnits_1Base : public touchgfx::Container
{
public:
    BSUnits_1Base();
    virtual ~BSUnits_1Base() {}
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::TextArea unit_top;
    touchgfx::TextArea textArea1_1;
    touchgfx::Line unit_divider;
    touchgfx::PainterRGB565 unit_dividerPainter;

private:

};

#endif // BSUNITS_1BASE_HPP