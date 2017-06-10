#ifndef SRCCMAKE_DIALOGWIDGET_H
#define SRCCMAKE_DIALOGWIDGET_H

// std
#include <string>
#include <functional>

// lib
#include <imgui.h>

// project
#include "Widget.h"
#include "../../rendering/ColorStyles.h"

class DialogWidget : public Widget
{
public:
    DialogWidget(std::string message, std::function<void(int)> callbackFuntion);
    ~DialogWidget();

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

private:
    std::string m_message;
    std::function<void(int)> m_callbackFunction;
};

#endif //SRCCMAKE_DIALOGWIDGET_H
