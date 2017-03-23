#ifndef SRCCMAKE_FILTERMANAGER_H
#define SRCCMAKE_FILTERMANAGER_H

#include <imgui.h>

#include "util/Singleton.h"
#include "processing/ImageFilter.h"

class FilterManager : public Singleton<FilterManager>
{
    friend class Singleton<FilterManager>;
public:
    void registerImageFilter(ImageFilter*);
    Image* applyFilter(Image* in);
    int drawFilterMenu(bool validActiveLayer);
private:
    int m_selectedFilter = -1;
    std::vector<ImageFilter*> m_imageFilters;
};


#endif //SRCCMAKE_FILTERMANAGER_H
