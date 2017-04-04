#ifndef SRCCMAKE_MAXFILTER_H
#define SRCCMAKE_MAXFILTER_H

#include "processing/ImageFilter.h"

class MaxFilter : public ImageFilter
{
public:
    MaxFilter() {
        m_name = "Max Filter";
        m_filterGroup = "non linear";

        m_filterRadius = 5;
        m_userDataCollection.addUserData(
                UserData("Filter radius",
                         UserData::DataType::DT_INT,
                         &m_filterRadius,
                         new int(0),
                         new int(10))
        );
    };
    ~MaxFilter() {};

    void process();

private:
    int m_filterRadius;
};


#endif //SRCCMAKE_MAXFILTER_H
