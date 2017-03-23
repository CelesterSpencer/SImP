#ifndef SRCCMAKE_MEDIANFILTER_H
#define SRCCMAKE_MEDIANFILTER_H

#include <algorithm>

#include "processing/ImageFilter.h"

class MedianFilter : public ImageFilter
{
public:
    MedianFilter() {
        m_name = "Median Filter";

        m_filterRadius = 5;
        m_userDataCollection.addUserData(
                UserData("Filter radius",
                         UserData::DataType::DT_INT,
                         &m_filterRadius,
                         new int(0),
                         new int(10))
        );
    };
    ~MedianFilter() {};

    Image* process(Image* in);

private:
    int m_filterRadius;
};


#endif //SRCCMAKE_MEDIANFILTER_H
