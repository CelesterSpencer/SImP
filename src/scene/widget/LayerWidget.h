#ifndef SRCCMAKE_LAYERWIDGET_H
#define SRCCMAKE_LAYERWIDGET_H

// lib
#include <imgui.h>

// project
#include "Widget.h"
#include "../Layer.h"
#include "../../scene/SystemFiles.h"

// styles
#include "../../rendering/ColorStyles.h"

class LayerWidget : public Widget
{
public:
    LayerWidget(int width, int height);

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

    void addLayer() { m_layers.push_back(new Layer); }
    void deleteLayer(int index) { if (index < m_layers.size()) m_layers.erase(m_layers.begin() + index); }
    Image* getSelectedImage()
    {
        if(m_activeLayer >= 0 && m_layers[m_activeLayer]->hasImage())
            return m_layers[m_activeLayer]->getImage();
        return nullptr;
    }
    int  getActiveLayerIndex() { return m_activeLayer; }

    void enable()   { m_isActive = true; }
    void disable()  { m_isActive = false; }

private:
    std::vector<Layer*> m_layers;
    int                 m_activeLayer;
    bool                m_isActive;
    // button icons
    GLuint              m_imageHandleDelete;
    GLuint              m_imageHandleOpen;
    // simple image shader
    ShaderProgram       m_shaderProgram;
};

#endif //SRCCMAKE_LAYERWIDGET_H
