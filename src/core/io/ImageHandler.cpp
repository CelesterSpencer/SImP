#include "ImageHandler.h"
#include "core/scene/Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

GLuint ImageHandler::getImageHandleFromFilePath(std::string filePath)
{
    std::vector<float> data;
    int width = 0;
    int height = 0;
    int channelNumber = 0;

    loadImage(filePath, &data, width, height, channelNumber);

    GLuint imageHandleGpu = 0;
    replaceImageWithChangeOnGpu(&imageHandleGpu, data.data(), width, height, channelNumber);

    return imageHandleGpu;
}

bool ImageHandler::loadImage(std::string filePath, std::vector<float>* data, int& width, int& height, int& channelNumber)
{
    stbi_set_flip_vertically_on_load(false);
    unsigned char* stbData = stbi_load(filePath.c_str(), &width, &height, &channelNumber, 0);

    if(stbData == nullptr)
    {
        std::cerr << "ERROR : Unable to open image " << filePath << std::endl;
        std::cerr << "Loaded data is empty" << std::endl;
        return false;
    }

    data->clear();
    for(int i = 0; i < width*height*channelNumber; i++)
    {
        data->push_back(static_cast<float>(stbData[i]) / 255); // values must be between 0 and 1
    }
    stbi_image_free(stbData);

    return true;
}

bool ImageHandler::saveImage(std::string filePath, Image* p_image)
{
    std::vector<unsigned char> outData;
    for(int i = 0; i < p_image->m_width*p_image->m_height*p_image->m_channelNumber; i++)
        outData.push_back(p_image->m_data[i]*255);
    int result = stbi_write_png(filePath.c_str(), p_image->m_width, p_image->m_height, p_image->m_channelNumber, outData.data(), 0);
    return !(result == 0);
}

void ImageHandler::replaceImageWithoutChangeOnGpu(GLuint* imageHandleGpu, float* data, int width, int height, int channelNumber)
{
    // bind handle
    glBindTexture(GL_TEXTURE_2D, *imageHandleGpu);

    /*
     * this should prevent images with odd dimensions from crashing
     */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    /*
     * actually upload the data
     */
    switch(channelNumber)
    {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RED, width, height, 0, GL_RED, GL_FLOAT, data);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
            break;
        default:
            std::cerr << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
            break;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageHandler::replaceImageWithChangeOnGpu(GLuint* imageHandleGpu, float* data, int width, int height, int channelNumber)
{
    /*
     * delete old data
     */
    if (*imageHandleGpu != 0)
    {
        glDeleteTextures(1, imageHandleGpu);
        *imageHandleGpu = 0;
    }


    /*
     * create new texture
     */
    glGenTextures(1, imageHandleGpu);

    /*
     * bind the texture for uploading data
     */
    glBindTexture(GL_TEXTURE_2D, *imageHandleGpu);

    /*
     * this should prevent images with odd dimensions from crashing
     */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    /*
     * actually upload the data
     */
    switch(channelNumber)
    {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RED, width, height, 0, GL_RED, GL_FLOAT, data);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
            break;
        default:
            std::cerr << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);
            break;
    }

    /*
     * texture settings
     */
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


    /*
     * unbind texture
     */
    glBindTexture(GL_TEXTURE_2D, 0);
}