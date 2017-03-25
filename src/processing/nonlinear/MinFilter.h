#ifndef SRCCMAKE_MINFILTER_H
#define SRCCMAKE_MINFILTER_H

#include "processing/ImageFilter.h"

class MinFilter : public ImageFilter
{
public:
    MinFilter() {
        m_name = "Min Filter";
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
    ~MinFilter() {};

    Image* process(Image* in);

private:
    int m_filterRadius;
};


#endif //SRCCMAKE_MINFILTER_H
