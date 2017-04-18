#ifndef SRCCMAKE_CANVAS_H
#define SRCCMAKE_CANVAS_H

// project
#include "rendering/WindowManager.h"
#include "processing/FilterManager.h"
#include "io/FileHandler.h"
#include "scene/Layer.h"
#include "scene/SystemFiles.h"
#include "scene/LayerManager.h"

// style
#include "rendering/ColorStyles.h"
#include "util/Types.h"

// lib
#include <thread.h>

class Canvas {
public:
    Canvas();
    ~Canvas();

    void draw();
    void drawFiltersMenu();

private:
    // processing
    std::thread*        m_imageProcessingThread;
    std::vector<Image*> m_tempOutputImages;
    bool                m_isProcessingActive = false;
    bool                m_isImageTransactionDone = true;
    int                 m_currentActiveLayerIdx;

    // loading spinner icon
    GLuint              m_imageHandleSpinner;
};


#endif //SRCCMAKE_CANVAS_H
