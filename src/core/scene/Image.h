#ifndef SRCCMAKE_IMAGE_H
#define SRCCMAKE_IMAGE_H

// std
#include <functional>
#include <thread.h>

// lib
#include <GL/gl3w.h>
#include <vector>
#include <iostream>

// project
#include "core/io/ImageHandler.h"
#include "core/util/Timer.h"

using image_ft = std::function<float(Image* img, int x, int y, int c)>;

class Image {
    friend class ImageFilterGpu;
    friend class LayerManager;
    friend class ImageHandler;
    friend class LayerWidget;
    friend class Layer;
public:
    Image();
    Image(const Image* other);
    Image(int width, int height, int channelNumber, float* p_data = nullptr, std::string filename = "Unnamed image");
    ~Image();
    Image& operator=(Image&& other);


    enum Channel
    {
        RED   = 0,
        GREEN = 1,
        BLUE  = 2,
        ALPHA = 3,
        RGB   = 4,
        RGBA  = 5
    };
    enum BoundMode
    {
        CLAMP = 0,
        REPEAT= 1,
        BINARY= 2
    };

    std::string getFileName();
    int getWidth();
    int getHeight();
    int getChannelNumber();

    float get(int x, int y, int channel = Channel::RED);
    int getXInBounds(int x, int boundMode = BoundMode::CLAMP);
    int getYInBounds(int y, int boundMode = BoundMode::CLAMP);
    void set(float value, int x, int y, int channel = Channel::RGB);
    void parallel(image_ft processingFunction);

private:
    std::string m_fileName;

    std::vector<float> m_data;
    int m_width;
    int m_height;
    int m_channelNumber;

    bool m_hasBeenModified;
    bool m_hasBeenResized;

    int calculateIndex(int x, int y, int channel);
};


#endif //SRCCMAKE_IMAGE_H
