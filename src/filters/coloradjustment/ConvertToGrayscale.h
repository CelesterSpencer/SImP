#ifndef SRCCMAKE_CONVERTTOGRAYSCALE_H
#define SRCCMAKE_CONVERTTOGRAYSCALE_H

#include <vector>
#include <string>

#include "filters/ImageFilter.h"

class ConvertToGrayscale : public ImageFilter
{
public:
    ConvertToGrayscale();

    void process();

private:
    std::vector<std::string> m_options;
    int m_selectedOption;
};

#endif //SRCCMAKE_CONVERTTOGRAYSCALE_H
