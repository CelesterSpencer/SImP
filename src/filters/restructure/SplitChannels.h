#ifndef SRCCMAKE_SPLITCHANNELS_H
#define SRCCMAKE_SPLITCHANNELS_H

#include "filters/ImageFilter.h"

class SplitChannels : public ImageFilter
{
public:
    SplitChannels() {
        m_name = "Split channels";
        m_filterGroup = "restructure";
    };
    ~SplitChannels() {};

    void process();
};


#endif //SRCCMAKE_SPLITCHANNELS_H
