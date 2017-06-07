#ifndef SRCCMAKE_SPINNERWIDGET_H
#define SRCCMAKE_SPINNERWIDGET_H

// lib
#include <GL/gl3w.h>
#include <glm.hpp>

// project
#include "util/Types.h"

class SpinnerWidget
{
public:
    SpinnerWidget();
    ~SpinnerWidget();

    void draw();

    void show() { m_isActive = true; }
    void hide() { m_isActive = false; }
    bool isActive() { return m_isActive; }

private:
    bool m_isActive;
    GLuint m_imageHandleSpinner;
};


#endif //SRCCMAKE_SPINNERWIDGET_H
