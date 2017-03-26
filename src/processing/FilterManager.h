#ifndef SRCCMAKE_FILTERMANAGER_H
#define SRCCMAKE_FILTERMANAGER_H

#include <imgui.h>

#include "util/Singleton.h"
#include "rendering/WindowManager.h"
#include "processing/ImageFilter.h"
#include "rendering/ColorStyles.h"

class FilterManager : public Singleton<FilterManager>
{
    friend class Singleton<FilterManager>;
public:
    void registerImageFilter(ImageFilter*);
    Image* applyFilter(Image* in);
    int drawFilterMenu(bool validActiveLayer, bool processingActive);
private:
    int m_selectionStatus = -1;
    int m_selectedFilter = -1;
    bool m_isFilterMenuOpen = false;
    std::vector<ImageFilter*> m_imageFilters;
    std::map<std::string,std::vector<int> > m_imageGroupFilterIdsMap;

    void drawFilterSettingsDialog();
};


#endif //SRCCMAKE_FILTERMANAGER_H
