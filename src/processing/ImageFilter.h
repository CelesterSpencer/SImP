#ifndef SRCCMAKE_IMAGEFILTER_H
#define SRCCMAKE_IMAGEFILTER_H

#include <string>

#include "scene/Image.h"
#include "io/UserData.h"

class ImageFilter
{
public:
    std::string getName() { return m_name; };

    virtual Image* process(Image* in) = 0;
    UserDataCollection* getUserDataCollection() { return &m_userDataCollection; };

protected:
    std::string m_name;
    UserDataCollection m_userDataCollection;
};

#endif //SRCCMAKE_IMAGEFILTER_H
