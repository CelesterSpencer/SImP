#ifndef SRCCMAKE_FILTERSETTINGSWIDGET_H
#define SRCCMAKE_FILTERSETTINGSWIDGET_H

// std
#include <functional>

// lib
#include <imgui.h>

// project
#include "Widget.h"
#include "../Layer.h"
#include "../../scene/SystemFiles.h"
#include "processing/ImageFilter.h"

// styles
#include "../../rendering/ColorStyles.h"

class FilterSettingsWidget : public Widget
{
public:
    FilterSettingsWidget(ImageFilter* imageFilter, std::function<void(int)> callbackFunction);
    ~FilterSettingsWidget();

    // inherit from Widget
    bool draw(int width, int height, float deltaTime);

private:
    ImageFilter* m_imagefilter;
    std::function<void(int)> m_callbackFunction;
};


#endif //SRCCMAKE_FILTERSETTINGSWIDGET_H
