#ifndef SRCCMAKE_SATURATE_H
#define SRCCMAKE_SATURATE_H

#include "filters/ImageFilter.h"

class Saturate : public ImageFilter
{
public:
    Saturate() {
        m_name = "Saturate";
        m_filterGroup = "color adjustment";
    };
    ~Saturate() {};

    void process();
};


#endif //SRCCMAKE_SATURATE_H
