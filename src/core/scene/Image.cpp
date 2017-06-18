#include "Image.h"

Image::Image()
{
    m_fileName = "Unnamed image";
    m_width = 0;
    m_height = 0;
    m_channelNumber = 0;
    m_hasBeenModified = false;
    m_hasBeenResized = false;
}
Image::Image(const Image* other)
{
    m_fileName      = other->m_fileName;
    m_width         = other->m_width;
    m_height        = other->m_height;
    m_channelNumber = other->m_channelNumber;
    m_data          = other->m_data;
    m_hasBeenResized  = true;
    m_hasBeenModified = true;
}
Image::Image(int width, int height, int channelNumber, float* p_data, std::string filename)
{
    m_fileName      = filename;
    m_width         = width;
    m_height        = height;
    m_channelNumber = channelNumber;
    if(p_data != nullptr)
        m_data = std::vector<float>(p_data, p_data + width*height*channelNumber);
    else
        m_data = std::vector<float>(width*height*channelNumber);
    m_hasBeenResized  = true;
    m_hasBeenModified = true;
}
Image::~Image()
{
    std::cout << "Delete image" << std::endl;
}
Image& Image::operator=(Image&& other)
{
    this->m_fileName = other.m_fileName;
    this->m_width = other.m_width;
    this->m_height = other.m_height;
    this->m_channelNumber = other.m_channelNumber;
    this->m_data = other.m_data;
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
int Image::getXInBounds(int x, int boundMode)
{
    switch(boundMode)
    {
        case BoundMode::CLAMP:
            return std::max(0, std::min(x, m_width-1));
            break;
        case BoundMode::REPEAT:
            if(x < 0) x = m_width-x;
            else if(x >= m_width) x = x - m_width;
            return x;
            break;
        case BoundMode::BINARY:
            return (x >= 0 && x < m_width); // returns true when in bounds
            break;
        default: // clamp is default
            return std::max(0, std::min(x, m_width-1));
            break;
    }
}
int Image::getYInBounds(int y, int boundMode)
{
    switch(boundMode)
    {
        case BoundMode::CLAMP:
            return std::max(0, std::min(y, m_height-1));
            break;
        case BoundMode::REPEAT:
            if(y < 0) y = m_height-y;
            else if(y >= m_height) y = y - m_height;
            return y;
            break;
        case BoundMode::BINARY:
            return (y >= 0 && y < m_height); // returns true when in bounds
            break;
        default: // clamp is default
            return std::max(0, std::min(y, m_height-1));
            break;
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
void Image::parallel(std::function<float(Image* img, int x, int y, int c)> processingFunction)
{
    std::cout << "start" << std::endl;
    Timer timer; timer.getDeltaTime();
    Image* temp = new Image(this);
    int channelNumber = this->getChannelNumber();
    std::cout << "create output image" << std::to_string(timer.getDeltaTime()) << std::endl;

    auto grayScaleFunction = [](Image* p_in, Image* p_out, int fromX, int toX, int fromY, int toY, image_ft func)
    {
        for(int y = fromY; y < toY; y++)
        {
            for(int x = fromX; x < toX; x++)
            {
                p_out->set(func(p_in, x, y, Image::Channel::RED), x, y, Image::Channel::RED);
            }
        }
    };
    auto twoColorFunction = [](Image* p_in, Image* p_out, int fromX, int toX, int fromY, int toY, image_ft func)
    {
        for(int y = fromY; y < toY; y++)
        {
            for(int x = fromX; x < toX; x++)
            {
                p_out->set(func(p_in, x, y, Image::Channel::RED), x, y, Image::Channel::RED);
                p_out->set(func(p_in, x, y, Image::Channel::GREEN), x, y, Image::Channel::GREEN);
            }
        }
    };
    auto rgbFunction = [](Image* p_in, Image* p_out, int fromX, int toX, int fromY, int toY, image_ft func)
    {
        for(int y = fromY; y < toY; y++)
        {
            for(int x = fromX; x < toX; x++)
            {
                p_out->set(func(p_in, x, y, Image::Channel::RED), x, y, Image::Channel::RED);
                p_out->set(func(p_in, x, y, Image::Channel::GREEN), x, y, Image::Channel::GREEN);
                p_out->set(func(p_in, x, y, Image::Channel::BLUE), x, y, Image::Channel::BLUE);
            }
        }
    };
    auto rgbaFunction = [](Image* p_in, Image* p_out, int fromX, int toX, int fromY, int toY, image_ft func)
    {
        for(int y = fromY; y < toY; y++)
        {
            for (int x = fromX; x < toX; x++)
            {
                p_out->set(func(p_in, x, y, Image::Channel::RED), x, y, Image::Channel::RED);
                p_out->set(func(p_in, x, y, Image::Channel::GREEN), x, y, Image::Channel::GREEN);
                p_out->set(func(p_in, x, y, Image::Channel::BLUE), x, y, Image::Channel::BLUE);
                p_out->set(func(p_in, x, y, Image::Channel::ALPHA), x, y, Image::Channel::ALPHA);
            }
        }
    };

    // function executed by every thread
    auto threadFunction = ( channelNumber == 4) ? rgbaFunction :
                          ((channelNumber == 3) ? rgbFunction :
                          ((channelNumber == 2) ? twoColorFunction :
                                                  grayScaleFunction));

    // init threads
    int chunkSizeX = (int)std::ceil((float)this->getWidth() / 2);
    int chunkSizeY = (int)std::ceil((float)this->getHeight() / 2);
    std::vector<std::thread*> threads;
    for(int y = 0; y < 2; y++)
    {
        for(int x = 0; x < 2; x++)
        {
            int fromX = y*chunkSizeX;
            int toX = std::min((y+1)*chunkSizeX, this->getWidth());
            int fromY = x*chunkSizeY;
            int toY = std::min((x+1)*chunkSizeY, this->getHeight());
            threads.push_back(new std::thread(threadFunction, this, temp, fromX, toX,
                                              fromY, toY, processingFunction));
        }
    }

    // wait for threads to finish
    for (auto& th : threads)
        th->join();
    std::cout << "processing: " << std::to_string(timer.getDeltaTime()) << std::endl;

    // copy back results
    *this = temp;
    std::cout << "copy result data: " << std::to_string(timer.getDeltaTime()) << std::endl;

    // cleanup
    delete temp;
}



std::string Image::getFileName()
{
    return m_fileName;
}
int Image::getWidth()
{
    return m_width;
}
int Image::getHeight()
{
    return m_height;
}
int Image::getChannelNumber()
{
    return m_channelNumber;
}



int Image::calculateIndex(int x, int y, int channel)
{
    if(channel < m_channelNumber)
    {
        return y*m_width*m_channelNumber + x*m_channelNumber + channel;
    }
    return y*m_width*m_channelNumber + x*m_channelNumber + Channel::RED;
}
