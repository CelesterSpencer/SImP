#ifndef SRCCMAKE_FILTERMANAGER_H
#define SRCCMAKE_FILTERMANAGER_H

#include <imgui.h>
#include <map>

#include "util/Singleton.h"
#include "rendering/WindowManager.h"
#include "processing/ImageFilter.h"
#include "rendering/ColorStyles.h"

class FilterManager : public Singleton<FilterManager>
{
    friend class Singleton<FilterManager>;
public:
    void registerImageFilter(ImageFilter*);
    void addImage(Image* in);
    std::vector<Image*> getOutputImages();
    void resetFilter();
    void applyFilter();

    int drawFilterMenu(bool validActiveLayer, bool processingActive);
private:

    int m_selectionStatus = -1;
    int m_selectedFilter = -1;
    bool m_isFilterMenuOpen = false;

    std::vector<ImageFilter*> m_imageFilters;
    std::map<std::string,std::vector<int> > m_imageGroupFilterIdsMap;

    std::vector<Image*> m_inputImages;
    std::vector<Image*> m_outputImages;

    void drawFilterSettingsDialog();
};


#endif //SRCCMAKE_FILTERMANAGER_H
