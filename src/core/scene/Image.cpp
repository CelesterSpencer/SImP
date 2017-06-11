#include "Image.h"

Image::Image()
{
    m_fileName = "Unnamed";
    m_width = 0;
    m_height = 0;
    m_bytesPerPixel = 0;
    m_hasBeenModified = false;
    m_hasBeenResized = false;
}

Image::~Image()
{
    std::cout << "Delete image" << std::endl;
}

std::string Image::getFileName()
{
    return m_fileName;
}

void Image::setFileName(std::string fileName)
{
    m_fileName = fileName;
}

int Image::getChannelNumber()
{
    return m_bytesPerPixel;
}

bool Image::hasBeenResized()
{
    return m_hasBeenResized;
}

bool Image::hasBeenModified()
{
    return m_hasBeenModified;
}

void Image::resetImageStatus()
{
    m_hasBeenResized = false;
    m_hasBeenModified = false;
}

void Image::load(std::string filePath)
{
    /*
     * gets the file name from the path
     */
    auto index = filePath.find_last_of("/\\") + 1;
    m_fileName = (index < filePath.size()) ? filePath.substr(index) : filePath;
    auto indexFileEnding = m_fileName.find_last_of(".");
    m_fileName = (indexFileEnding < m_fileName.size()) ? m_fileName.substr(0,indexFileEnding) : m_fileName;

    /*
     * actually loading the data
     */
    ImageHandler::getInstance().loadImage(filePath, &m_data, m_width, m_height, m_bytesPerPixel);

    m_hasBeenModified = true;
    m_hasBeenResized = true;
}

void Image::save()
{
    std::string filePath = RESOURCES_PATH"/output/"+m_fileName+"_modified.png";
    std::cout << std::to_string(m_fileName.size()) << std::endl;
    std::cout << filePath << std::endl;
    ImageHandler::getInstance().saveImage(filePath, m_data, m_width, m_height, m_bytesPerPixel);
}

void Image::copyData(Image* in)
{
    if (m_width != in->getWidth() || m_height != in->getHeight() || m_bytesPerPixel != in->m_bytesPerPixel)
    {
        m_hasBeenResized = true;
    }

    m_fileName = in->m_fileName;
    m_width = in->m_width;
    m_height = in->m_height;
    m_bytesPerPixel = in->m_bytesPerPixel;
    m_data = in->m_data;

    m_hasBeenModified = true;
}

void Image::reserve(int width, int height, int numberOfChannels)
{
    m_width = width;
    m_height = height;
    m_bytesPerPixel = numberOfChannels;

    m_data = std::vector<float>(width*height*numberOfChannels);

    m_hasBeenModified = true;
    m_hasBeenResized = true;
}

float Image::get(int x, int y, int channel)
{
    if(x >= 0 && y >= 0 && x < m_width && y < m_height && channel >= 0 && channel < 6)
    {
        switch(channel)
        {
            case Channel::RGBA:
                return m_data[calculateIndex(x,y,Channel::RED)];
                break;
            case Channel::RGB:
                return m_data[calculateIndex(x,y,Channel::RED)];
                break;
            default:
                return m_data[calculateIndex(x,y,channel)];
                break;
        }
    }
    else
    {
        std::cerr << "Accessing image out of bounds" << std::endl;
        return 0;
    }
}

void Image::set(float value, int x, int y, int channel)
{
    if(x >= 0 && y >= 0 && x < m_width && y < m_height && channel >= 0 && channel < 6)
    {
        switch(channel)
        {
            case Channel::RGBA:
                m_data[calculateIndex(x,y,Channel::RED)]   = value;
                m_data[calculateIndex(x,y,Channel::GREEN)] = value;
                m_data[calculateIndex(x,y,Channel::BLUE)]  = value;
                m_data[calculateIndex(x,y,Channel::ALPHA)] = value;
                break;
            case Channel::RGB:
                m_data[calculateIndex(x,y,Channel::RED)]   = value;
                m_data[calculateIndex(x,y,Channel::GREEN)] = value;
                m_data[calculateIndex(x,y,Channel::BLUE)]  = value;
                break;
            default:
                m_data[calculateIndex(x,y,channel)] = value;
                break;
        }
        m_hasBeenModified = true;
    }
    else
    {
        std::cerr << "Accessing image out of bounds" << std::endl;
    }
}

void Image::setRawData(float* rawData, int width, int height, int channelNumber)
{
    if (m_width != width || m_height != height || m_bytesPerPixel != channelNumber)
    {
        m_hasBeenResized = true;
    }

    m_data.clear();
    m_data.insert(m_data.end(), rawData, rawData+width*height*channelNumber);
    m_width = width;
    m_height = height;
    m_bytesPerPixel = channelNumber;

    m_hasBeenModified = true;
}

float* Image::getRawData()
{
    return m_data.data();
}

int Image::getRawDataSize()
{
    return m_data.size();
}

int Image::getWidth()
{
    return m_width;
}

int Image::getHeight()
{
    return m_height;
}

int Image::getMin()
{
    int min = INT_MAX;
    for (int i = 0; i < m_data.size(); i++)
    {
        min = (m_data[i] < min) ? m_data[i] : min;
    }
    return min;
}

int Image::getMax()
{
    int max = INT_MIN;
    for (int i = 0; i < m_data.size(); i++)
    {
        max = (m_data[i] > max) ? m_data[i] : max;
    }
    return max;
}

int Image::calculateIndex(int x, int y, int channel)
{
    if(channel < m_bytesPerPixel)
    {
        return y*m_width*m_bytesPerPixel + x*m_bytesPerPixel + channel;
    }
    return y*m_width*m_bytesPerPixel + x*m_bytesPerPixel + Channel::RED;
}
