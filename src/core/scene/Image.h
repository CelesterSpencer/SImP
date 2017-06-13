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
public:
    Image();
    ~Image();

    enum Channel
    {
        RED   = 0,
        GREEN = 1,
        BLUE  = 2,
        ALPHA = 3,
        RGB   = 4,
        RGBA  = 5
    };

    std::string getFileName();
    void setFileName(std::string fileName);


    // used by the core
    void load(std::string filePath);
    void save();
    bool hasBeenResized();
    bool hasBeenModified();
    void resetImageStatus();


    // image initialization
    void copyData(Image* in);
    void reserve(int width, int height, int numberOfChannels);
    void setRawData(float* rawData, int width, int height, int channelNumber);
    float* getRawData();


    // data manipulation
    float get(int x, int y, int channel = Channel::RED);
    void set(float value, int x, int y, int channel = Channel::RGB);
    void parallel(std::function<float(int w, int h, int c, float val)> processingFunction, int threadCount);

    // image dimensions
    int getWidth();
    int getHeight();
    int getChannelNumber();

private:
    std::string m_fileName;
    std::vector<float> m_data;
    int m_width;
    int m_height;
    int m_bytesPerPixel;
    bool m_hasBeenModified;
    bool m_hasBeenResized;

    int calculateIndex(int x, int y, int channel);
};


#endif //SRCCMAKE_IMAGE_H
