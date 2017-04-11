#include "SystemFiles.h"

void SystemFiles::init()
{
    m_deleteIconHandle  = ImageHandler::getInstance().getImageHandleFromFilePath(RESOURCES_PATH"/system/cross.png");
    m_openIconHandle    = ImageHandler::getInstance().getImageHandleFromFilePath(RESOURCES_PATH"/system/open.png");
    m_spinnerIconHandle = ImageHandler::getInstance().getImageHandleFromFilePath(RESOURCES_PATH"/system/spinner.png");
}

GLuint SystemFiles::getImageHandle(int iconType)
{
    if (!m_isInitialized) init();

    switch (iconType)
    {
        case ICON_DELETE:
            return m_deleteIconHandle;
            break;
        case ICON_OPEN:
            return m_openIconHandle;
            break;
        case ICON_SPINNER:
            return m_spinnerIconHandle;
            break;
        default:
            return -1;
            break;
    }
}