#ifndef SRCCMAKE_LAPLACEFILTER_H
#define SRCCMAKE_LAPLACEFILTER_H

#include <vector>

#include "processing/ImageFilter.h"

class LaplaceFilter : public ImageFilter
{
public:
    LaplaceFilter() {
        m_name = "Laplace Filter";
        m_filterGroup = "edge detection";

        static const int data[] = { 0,-1, 0,
                                   -1, 4,-1,
                                    0,-1, 0 };
        m_laplaceKernel = std::vector<int>(data, data + 9);
    };
    ~LaplaceFilter() {};

    void process();

private:
    std::vector<int> m_laplaceKernel;
};


#endif //SRCCMAKE_LAPLACEFILTER_H
