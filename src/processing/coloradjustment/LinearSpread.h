#ifndef SRCCMAKE_LINEARSPREAD_H
#define SRCCMAKE_LINEARSPREAD_H

#include "processing/ImageFilter.h"

class LinearSpread : public ImageFilter
{
public:
    LinearSpread() {
        m_name = "Linear Spread";
        m_filterGroup = "color adjustment";
    };
    ~LinearSpread() {};

    Image* process(Image* in);
};


#endif //SRCCMAKE_LINEARSPREAD_H