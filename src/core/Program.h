#ifndef SRCCMAKE_PROGRAM_H
#define SRCCMAKE_PROGRAM_H

// std
#include <thread.h>

// project
#include "scene/widget/WidgetManager.h"
#include "scene/widget/FilterSettingsWidget.h"
#include "scene/widget/SpinnerWidget.h"
#include "scene/widget/MessageWidget.h"
#include "scene/widget/DialogWidget.h"
#include "scene/widget/LayerWidget.h"
#include "processing/FilterManager.h"
#include "io/ImageHandler.h"

class Program
{
public:
    Program();
    ~Program();

    void addFilter(ImageFilter* imagefilter);
    void run();

private:
    std::thread* m_imageProcessingThread;
    // window
    float WINDOW_WIDTH = 1024;
    float WINDOW_HEIGHT = 768;
};

#endif //SRCCMAKE_PROGRAM_H
