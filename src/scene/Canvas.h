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

#include "rendering/ColorStyles.h"
#include "util/Types.h"

#include <thread.h>

class Canvas {
public:
    Canvas();
    ~Canvas();

    void draw();
    void drawLayersMenu();
    void drawFiltersMenu();

    void addLayer();
    void deleteLayer(int index);
    void setName(std::string name, int layer);
    void setImage(Image* image, int layer);
    void setOpacity(float opacity, int layer = -1);

private:
    ShaderProgram*      m_shaderProgram;
    std::vector<Layer*> m_layers;
    int                 m_activeLayer;
    std::thread*        m_imageProcessingThread;

    GLuint              m_imageHandleDelete;
    GLuint              m_imageHandleOpen;
    GLuint              m_imageHandleSpinner;

    void loadSystemImages();
    void uploadImage(Image* image, GLuint* imageHandle);
};


#endif //SRCCMAKE_CANVAS_H
