#ifndef SRCCMAKE_SPINNERWIDGET_H
#define SRCCMAKE_SPINNERWIDGET_H

// lib
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>

// project
#include "Widget.h"
#include "core/util/Types.h"
#include "core/io/SystemFiles.h"

class SpinnerWidget : public Widget
{
public:
    SpinnerWidget();
    ~SpinnerWidget();

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

    void show() { m_isActive = true; }
    void hide() { m_isActive = false; }
    bool isActive() { return m_isActive; }

private:
    bool m_isActive;
    GLuint m_imageHandleSpinner;
};

#endif //SRCCMAKE_SPINNERWIDGET_H
