//
// Created by Windrian on 11.03.2017.
//

#ifndef SRCCMAKE_CANVAS_H
#define SRCCMAKE_CANVAS_H

#include "rendering/WindowManager.h"
#include "rendering/ShaderProgram.h"
#include "processing/FilterManager.h"
#include "io/FileHandler.h"
#include "scene/Layer.h"

class Canvas {
private:
    ShaderProgram*               m_shaderProgram;
    std::vector<Layer*>          m_layers;
    int                          m_activeLayer;

public:
    Canvas();
    ~Canvas();

    void draw();
    void drawLayersMenu();
    void drawFiltersMenu();

    void addLayer();
    void setName(std::string name, int layer);
    void setImage(Image* image, int layer);
    void setOpacity(float opacity, int layer = -1);
};


#endif //SRCCMAKE_CANVAS_H
