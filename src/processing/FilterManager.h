#ifndef SRCCMAKE_FILTERMANAGER_H
#define SRCCMAKE_FILTERMANAGER_H

#include <imgui.h>
#include <map>

#include "util/Singleton.h"
#include "util/Timer.h"
#include "rendering/WindowManager.h"
#include "processing/ImageFilter.h"
#include "rendering/ColorStyles.h"

class FilterManager : public Singleton<FilterManager>
{
    friend class Singleton<FilterManager>;
public:
    void registerImageFilter(ImageFilter*);
    void addImage(Image* in);
    void addRequiredImages();
    std::vector<Image*> getOutputImages();
    void resetFilter();
    void applyFilter();
    ImageFilter* getActiveFilter();
    std::vector<ImageFilter*>& getImageFilters() { return m_imageFilters; }
    bool isActiveFilterValid();
    bool isRunning() { return m_isRunning; }

    int drawFilterMenu(bool validActiveLayer, bool processingActive);
private:

    // status
    int m_selectionStatus = -1;
    int m_selectedFilter = -1;
    bool m_isFilterMenuOpen = false;
    bool m_isRunning = false;

    // filters
    std::vector<ImageFilter*> m_imageFilters;
    std::map<std::string,std::vector<int> > m_imageGroupFilterIdsMap;

    // images
    std::vector<Image*> m_inputImages;
    std::vector<Image*> m_outputImages;

    Timer m_timer;

    void drawFilterSettingsDialog();
};


#endif //SRCCMAKE_FILTERMANAGER_H
