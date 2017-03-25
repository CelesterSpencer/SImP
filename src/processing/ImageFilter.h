#ifndef SRCCMAKE_IMAGEFILTER_H
#define SRCCMAKE_IMAGEFILTER_H

#include <string>

#include "scene/Image.h"
#include "io/UserData.h"

class ImageFilter
{
public:
    std::string getName() { return m_name; };
    std::string getFilterGroup() { return m_filterGroup; };

    virtual Image* process(Image* in) = 0;
    UserDataCollection* getUserDataCollection() { return &m_userDataCollection; };

protected:
    std::string m_name = "unnamed";
    std::string m_filterGroup = "default";
    UserDataCollection m_userDataCollection;
};

#endif //SRCCMAKE_IMAGEFILTER_H
