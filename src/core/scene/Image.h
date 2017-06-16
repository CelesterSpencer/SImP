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

    // image initialization
/*    void copyData(Image* in);
    void reserve(int width, int height, int numberOfChannels);*/

    std::string getFileName();
    int getWidth();
    int getHeight();
    int getChannelNumber();

    float get(int x, int y, int channel = Channel::RED);
    void set(float value, int x, int y, int channel = Channel::RGB);
    void parallel(std::function<float(Image* img, int x, int y, int c)> processingFunction);

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
