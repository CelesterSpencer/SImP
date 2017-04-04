#ifndef SRCCMAKE_TESTFILTER_H
#define SRCCMAKE_TESTFILTER_H

#include "processing/ImageFilter.h"

class TestFilter : public ImageFilter
{
public:
    TestFilter() {
        m_name = "Test filter";
        m_filterGroup = "test";
    };
    ~TestFilter() {};

    void process();
};


#endif //SRCCMAKE_TESTFILTER_H
