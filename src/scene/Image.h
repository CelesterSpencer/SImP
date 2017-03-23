//
// Created by Windrian on 18.03.2017.
//

#ifndef SRCCMAKE_IMAGE_H
#define SRCCMAKE_IMAGE_H

#include <GL/gl3w.h>
#include <glm.hpp>
#include <vector>
#include <iostream>

class Image {
public:
    Image();
    ~Image();

    enum Channel
    {
        RED,
        GREEN,
        BLUE,
        ALPHA,
        RGB,
        RGBA
    };

    std::string getFileName();
    bool hasBeenResized();
    bool hasBeenModified();
    void resetImageStatus();

    int load(std::string filePath);
    void copyData(Image* in);
    unsigned char* getRawData();

    int get(int x, int y, int channel = Channel::RED);
    void set(int value, int x, int y, int channel = Channel::RGB);

    int getWidth();
    int getHeight();
    int getChannelNumber();

private:
    std::string m_fileName;
    std::vector<unsigned char> m_data;
    int m_width;
    int m_height;
    int m_bytesPerPixel;
    bool m_hasBeenModified;
    bool m_hasBeenResized;

    int calculateIndex(int x, int y, int channel);
};


#endif //SRCCMAKE_IMAGE_H
