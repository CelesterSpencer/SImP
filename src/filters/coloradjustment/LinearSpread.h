#ifndef SRCCMAKE_LINEARSPREAD_H
#define SRCCMAKE_LINEARSPREAD_H

#include "core/scene/ImageFilter.h"

class LinearSpread : public ImageFilter
{
public:
    LinearSpread();

    void process();
};


#endif //SRCCMAKE_LINEARSPREAD_H
