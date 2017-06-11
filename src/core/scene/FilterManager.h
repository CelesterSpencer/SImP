#ifndef SRCCMAKE_FILTERMANAGER_H
#define SRCCMAKE_FILTERMANAGER_H

// lib
#include <imgui.h>
#include <vector>
#include <thread.h>
#include <map>

// project
#include "core/util/Singleton.h"
#include "core/util/Timer.h"
#include "core/rendering/WindowManager.h"
#include "core/rendering/ColorStyles.h"
#include "core/scene/Image.h"
#include "filters/ImageFilter.h"

class FilterManager : public Singleton<FilterManager>
{
    friend class Singleton<FilterManager>;
public:
    enum FilterStatus
    {
        READY   = 1,
        RUNNING = 2,
        DONE    = 3
    };

    FilterManager();
    ~FilterManager();

    // filters
    void registerImageFilter(ImageFilter*);
    std::vector<ImageFilter*>& getImageFilters() { return m_imageFilters; }
    void cleanup();

    void execute(ImageFilter* p_imagefilter, Image* inputImage);

    // images
    std::vector<Image*> getOutputImages();

    // status
    FilterStatus getFilterStatus() { return m_filterStatus; }
private:

    // status
    FilterStatus m_filterStatus;

    // filters
    std::vector<ImageFilter*> m_imageFilters;
    std::map<std::string,std::vector<int> > m_imageGroupFilterIdsMap;

    // images
    std::vector<Image*> m_inputImages;
    std::vector<Image*> m_outputImages;

    // filters
    std::thread* m_imageProcessingThread;

    Timer m_timer;



    void applyFilter(ImageFilter* imageFilter);
};


#endif //SRCCMAKE_FILTERMANAGER_H
