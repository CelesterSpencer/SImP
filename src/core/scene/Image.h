#ifndef SRCCMAKE_IMAGE_H
#define SRCCMAKE_IMAGE_H

// lib
#include <GL/gl3w.h>
#include <vector>
#include <iostream>

// project
#include "core/io/ImageHandler.h"

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
    bool hasBeenResized();
    bool hasBeenModified();
    void resetImageStatus();

    void load(std::string filePath);
    void save();
    void copyData(Image* in);
    void reserve(int width, int height, int numberOfChannels);
    void setRawData(float* rawData, int width, int height, int channelNumber);
    int getRawDataSize();
    float* getRawData();

    float get(int x, int y, int channel = Channel::RED);
    void set(float value, int x, int y, int channel = Channel::RGB);

    int getWidth();
    int getHeight();
    int getMin();
    int getMax();
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