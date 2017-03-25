#ifndef SRCCMAKE_MEANFILTER_H
#define SRCCMAKE_MEANFILTER_H

#include "processing/ImageFilter.h"

class MeanFilter: public ImageFilter
{
    friend class ImageFilter;
public:
    MeanFilter() {
        m_name = "Mean Filter";
        m_filterGroup = "linear";

        m_filterRadius = 5;
        m_userDataCollection.addUserData(
                UserData("Filter radius",
                         UserData::DataType::DT_INT,
                         &m_filterRadius,
                         new int(0),
                         new int(10))
        );
    };
    ~MeanFilter() {};

    Image* process(Image* in);

private:
    int m_filterRadius;
};


#endif //SRCCMAKE_MEANFILTER_H