#ifndef SRCCMAKE_LAPLACEFILTER_H
#define SRCCMAKE_LAPLACEFILTER_H

#include <vector>

#include "core/scene/ImageFilter.h"

class LaplaceFilter : public ImageFilter
{
public:
    LaplaceFilter();

    void process();

private:
    std::vector<int> m_laplaceKernel;
};


#endif //SRCCMAKE_LAPLACEFILTER_H
