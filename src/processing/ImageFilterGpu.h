#ifndef SRCCMAKE_IMAGEFILTERGPU_H
#define SRCCMAKE_IMAGEFILTERGPU_H

// std
#include <vector>

// project
#include "processing/ImageFilter.h"
#include "rendering/ShaderProgram.h"

class ImageFilterGpu : public ImageFilter
{
public:
    struct OutputImageDescription
    {
        OutputImageDescription(std::string name, int width, int height, int channelNumber)
        {
            this->name = name;
            this->width = width;
            this->height = height;
            this->channelNumber = channelNumber;
        }
        std::string name;
        int width;
        int height;
        int channelNumber;
    };

    ImageFilterGpu() { }

    // actually execute the filter
    void process()
    {
        /*
         * create ssbos for all input images
         */
        std::vector<GLuint> ssboHandles;
        int inputImagesNumber = getNumberOfInputImages();
        for (int i = 0; i < inputImagesNumber; i++)
        {
            Image* inputImage = getInputImage(i);
            GLuint ssboHandle = 0;
            int imageSize = inputImage->getWidth()*inputImage->getHeight()*inputImage->getChannelNumber();
            createSSBO<uchar>(&ssboHandle, imageSize);
            copyDataToSSBO<uchar>(&ssboHandle, inputImage->getRawData(), imageSize);
            ssboHandles.push_back(ssboHandle);
        }

        /*
         * create ssbos for all output images
         */
        int outputImagesNumber = m_outputImageDescriptions.size();
        for (int i = 0; i < outputImagesNumber; i++)
        {
            OutputImageDescription outputImageDescription = m_outputImageDescriptions[i];
            GLuint ssboHandle = 0;
            int imageSize = outputImageDescription.width*outputImageDescription.height*outputImageDescription.channelNumber;
            createSSBO<uchar>(&ssboHandle, imageSize);
            fillSSBOwithInitialValue(&ssboHandle, 0, imageSize);
            ssboHandles.push_back(ssboHandle);
        }

        /*
         * bind all ssbos
         */
        for (int i = 0; i < ssboHandles.size(); i++)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, ssboHandles[i]);
        }

        /*
         * upload all user defined variables and execute the compute shader
         */
        Image* firstInputImage = getInputImage(0);
        int pixelNumber = firstInputImage->getWidth()*firstInputImage->getHeight();
        m_computeShader->use();
        m_computeShader->update("width", firstInputImage->getWidth());
        m_computeShader->update("height", firstInputImage->getHeight());
        m_computeShader->update("channelNumber", firstInputImage->getChannelNumber());
        m_computeShader->update("pixelNumber", pixelNumber);
        glDispatchCompute(pixelNumber, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        /*
         * unbind all ssbos
         */
        for (int i = 0; i < ssboHandles.size(); i++)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
        }

        /*
         * download all output images
         * if any  dimension of the output image description is 0 that dimension is grabbed from
         * the first input image
         */
        for (int i = 0; i < m_outputImageDescriptions.size(); i++)
        {
            OutputImageDescription outputImageDescription = m_outputImageDescriptions[i];
            int width = (outputImageDescription.width > 0) ? outputImageDescription.width : firstInputImage->getWidth();
            int height = (outputImageDescription.height > 0) ? outputImageDescription.height : firstInputImage->getHeight();
            int channelNumber = (outputImageDescription.channelNumber > 0) ? outputImageDescription.channelNumber : firstInputImage->getChannelNumber();
            int imageSize = width*height*channelNumber;
            uchar* rawData = getDataFromSSBO<uchar>(&ssboHandles[i+inputImagesNumber], imageSize);
            Image* outputImage = new Image;
            outputImage->setRawData(rawData, outputImageDescription.width, outputImageDescription.height, outputImageDescription.channelNumber);
            returnImage(outputImage);
            delete rawData;
        }
    }

protected:
    void setComputeShader(std::string computeShaderPath)
    {
        m_computeShader = new ShaderProgram(SHADERS_PATH+computeShaderPath+".comp");
    }

    void addOutputImageDescription(std::string name, int width, int height, int channelNumber)
    {
        m_outputImageDescriptions.push_back(OutputImageDescription(name, width, height, channelNumber));
    }

private:
    ShaderProgram* m_computeShader;
    std::vector<OutputImageDescription> m_outputImageDescriptions;

    /**
     * Create an ssbo with the given length.
     * It will be uninitialized. Use either fillSSBOwithInitalValue
     * or copyDataToSSBO to initialize it.
     */
    template<class T>
    void createSSBO(GLuint* ssboHandle, int length)
    {
        glGenBuffers(1, ssboHandle);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssboHandle);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T)*length, NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Error while initializing SSBO: " << std::to_string(err) << std::endl;
        }
    }

    /**
     * Fill every block of the ssbo with the provided value
     */
    template<class T>
    void fillSSBOwithInitialValue(GLuint* ssboHandle, T value, int length)
    {
        T* values = new T[length];
        std::fill_n(values, length, value);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssboHandle);
        GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        memcpy(p, values, sizeof(T)*length);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Error while filling SSBO with ints: " << std::to_string(err) << std::endl;
        }

        delete[] values;
    }

    /**
     * transfer data from or to the ssbo
     */
    template<class T>
    static void copyDataToSSBO(GLuint* targetHandle, T* data, int length){
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *targetHandle);
        GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        memcpy(p, &data, sizeof(T)*length);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }

    /**
     * Download data from the ssbo to the cpu
     */
    template<class T>
    static T* getDataFromSSBO(GLuint* ssboHandle, int length)
    {
        T* data = new T[length];
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssboHandle);
        GLuint *ptr;
        ptr = (GLuint*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        for (int i = 0; i < length; i++) {
            data[i] = *(((T*)ptr)+i);
        }
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        return data;
    }

    /**
     * Deleting ssbo
     */
    static void deleteSSBO(GLuint* ssboHandler)
    {
        glDeleteBuffers(1, ssboHandler);
    }
};

#endif //SRCCMAKE_IMAGEFILTERGPU_H
