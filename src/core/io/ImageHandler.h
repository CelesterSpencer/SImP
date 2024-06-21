#ifndef SRCCMAKE_IMAGEHANDLER_H
#define SRCCMAKE_IMAGEHANDLER_H

// std
#include <vector>
#include <string>
#include <iostream>

// lib
#include <glad/glad.h>

// project
#include "core/util/Types.h"
#include "core/util/Singleton.h"
#include "FileHandler.h"

class Image;

class ImageHandler : public Singleton<ImageHandler>
{
    friend class Singleton<ImageHandler>;
public:
    GLuint getImageHandleFromFilePath(std::string filePath);
    bool loadImage(std::string filePath, std::vector<float>* data, int& width, int& height, int& channelNumber);
    bool saveImage(std::string filePath, Image* p_image);
    void replaceImageWithoutChangeOnGpu(GLuint* imageHandleGpu, float* data, int width, int height, int channelNumber);
    void replaceImageWithChangeOnGpu(GLuint* imageHandleGpu, float* data, int width, int height, int channelNumber);
private:
    ImageHandler() {};
    ~ImageHandler() {};
};

#endif //SRCCMAKE_IMAGEHANDLER_H
