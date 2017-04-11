#ifndef SRCCMAKE_IMAGEHANDLER_H
#define SRCCMAKE_IMAGEHANDLER_H

// std
#include <vector>
#include <string>
#include <iostream>

// lib
#include <GL/gl3w.h>

// project
#include "util/Types.h"
#include "util/Singleton.h"

class ImageHandler : public Singleton<ImageHandler>
{
    friend class Singleton<ImageHandler>;
public:
    GLuint getImageHandleFromFilePath(std::string filePath);
    bool loadImage(std::string filePath, std::vector<uchar>* data, int& width, int& height, int& channelNumber);
    void replaceImageWithoutChangeOnGpu(GLuint* imageHandleGpu, uchar* data, int width, int height, int channelNumber);
    void replaceImageWithChangeOnGpu(GLuint* imageHandleGpu, uchar* data, int width, int height, int channelNumber);
private:
    ImageHandler() {};
    ~ImageHandler() {};
};

#endif //SRCCMAKE_IMAGEHANDLER_H