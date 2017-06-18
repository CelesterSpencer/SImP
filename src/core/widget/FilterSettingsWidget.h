#ifndef SRCCMAKE_FILTERSETTINGSWIDGET_H
#define SRCCMAKE_FILTERSETTINGSWIDGET_H

// std
#include <functional>

// lib
#include <imgui.h>

// project
#include "Widget.h"
#include "core/scene/LayerManager.h"
#include "core/io/SystemFiles.h"
#include "core/widget/WidgetManager.h"
#include "core/scene/ImageFilter.h"

// styles
#include "core/rendering/ColorStyles.h"

class FilterSettingsWidget : public Widget
{
public:
    FilterSettingsWidget(ImageFilter* imageFilter, WidgetManager* p_widgetManager, LayerManager* p_layerManager, std::function<void(int)> callbackFunction);
    ~FilterSettingsWidget();

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

private:
    ImageFilter*                m_imagefilter;
    WidgetManager*              mp_widgetManager;
    LayerManager*               mp_layerManager;
    std::function<void(int)>    m_callbackFunction;
};


#endif //SRCCMAKE_FILTERSETTINGSWIDGET_H
