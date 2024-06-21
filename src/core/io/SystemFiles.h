#ifndef SRCCMAKE_SYSTEMFILES_H
#define SRCCMAKE_SYSTEMFILES_H

// lib
#include <glad/glad.h>

// project
#include "core/util/Singleton.h"
#include "ImageHandler.h"

class SystemFiles : public Singleton<SystemFiles>
{
    friend class Singleton<SystemFiles>;
public:
    enum ICON
    {
        ICON_DELETE     = 0,
        ICON_OPEN       = 1,
        ICON_SPINNER    = 2
    };
    GLuint getImageHandle(int iconType);

private:
    GLuint m_deleteIconHandle;
    GLuint m_openIconHandle;
    GLuint m_spinnerIconHandle;

    bool m_isInitialized;

    SystemFiles() { m_isInitialized = false; };
    ~SystemFiles() {};

    void init();
};


#endif //SRCCMAKE_SYSTEMFILES_H
