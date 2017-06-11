#ifndef SRCCMAKE_IMAGESELECTIONWIDGET_H
#define SRCCMAKE_IMAGESELECTIONWIDGET_H

// lib
#include <imgui.h>

// project
#include "Widget.h"
#include "core/scene/LayerManager.h"
#include "core/io/SystemFiles.h"

class ImageSelectionWidget : public Widget
{
public:
    ImageSelectionWidget(LayerManager* p_layerManager, std::function<void(int, int)> callbackFunction);
    ~ImageSelectionWidget();

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

private:
    LayerManager*                   mp_layerManager;
    int                             m_selectedImageIdx;
    std::function<void(int, int)>   m_callbackFunction;
};


#endif //SRCCMAKE_IMAGESELECTIONWIDGET_H
