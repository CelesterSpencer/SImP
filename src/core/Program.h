#ifndef SRCCMAKE_PROGRAM_H
#define SRCCMAKE_PROGRAM_H

// project
#include "core/widget/WidgetManager.h"
#include "core/widget/FilterSettingsWidget.h"
#include "core/widget/SpinnerWidget.h"
#include "core/widget/MessageWidget.h"
#include "core/widget/DialogWidget.h"
#include "core/widget/LayerWidget.h"
#include "core/scene/FilterManager.h"
#include "core/io/ImageHandler.h"

class Program
{
public:
    Program();
    ~Program();

    void addFilter(ImageFilter* imagefilter);
    void run();

private:
    // window
    float WINDOW_WIDTH = 1024;
    float WINDOW_HEIGHT = 768;
};

#endif //SRCCMAKE_PROGRAM_H
