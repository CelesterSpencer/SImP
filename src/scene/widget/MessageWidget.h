#ifndef SRCCMAKE_MESSAGEWIDGET_H
#define SRCCMAKE_MESSAGEWIDGET_H

// std
#include <string>
#include <cmath>
#include <iostream>

// lib
#include <imgui.h>

// project
#include "Widget.h"
#include "../../rendering/ColorStyles.h"

class MessageWidget : public Widget
{
public:
    MessageWidget(std::string text, float lifetime);
    ~MessageWidget();

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

private:
    std::string m_text;
    float m_lifetime;
};

#endif //SRCCMAKE_MESSAGEWIDGET_H
