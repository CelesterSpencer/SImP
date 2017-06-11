#ifndef SRCCMAKE_LAYERWIDGET_H
#define SRCCMAKE_LAYERWIDGET_H

// lib
#include <imgui.h>

// project
#include "Widget.h"
#include "core/scene/LayerManager.h"
#include "core/io/SystemFiles.h"

// styles
#include "core/rendering/ColorStyles.h"

class LayerWidget : public Widget
{
public:
    LayerWidget(LayerManager* layerManager, int width, int height);

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

    void enable()   { m_isActive = true; }
    void disable()  { m_isActive = false; }

private:
    LayerManager*       mp_layerManager;
    bool                m_isActive;
    // button icons
    GLuint              m_imageHandleDelete;
    GLuint              m_imageHandleOpen;
    // simple image shader
    ShaderProgram       m_shaderProgram;
};

#endif //SRCCMAKE_LAYERWIDGET_H
