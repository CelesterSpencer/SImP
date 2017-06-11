#ifndef SRCCMAKE_IMAGEFILTERGPU_H
#define SRCCMAKE_IMAGEFILTERGPU_H

// std
#include <vector>

// project
#include "filters/ImageFilter.h"
#include "core/rendering/ShaderProgram.h"

class ImageFilterGpu : public ImageFilter
{
public:
    struct ImageDimensions
    {
        int width;
        int height;
        int channelNumber;
        int isInputImage;
    };

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

    ImageFilterGpu() { m_filterType = "gpu filter"; }

    // actually execute the filter
    void process()
    {
        /*
         * this image is mandatory
         * and has to exist
         */
        Image* firstInputImage = getInputImage(0);

        /*
         * gpu image struct
         */
        std::vector<ImageDimensions> imageDimensions;

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
            createSSBO<float>(&ssboHandle, imageSize);
            copyDataToSSBO<float>(&ssboHandle, inputImage->getRawData(), imageSize);
            ssboHandles.push_back(ssboHandle);
            imageDimensions.push_back(ImageDimensions{inputImage->getWidth(),inputImage->getHeight(),inputImage->getChannelNumber(),1});
        }
        std::cout << "created input ssbos" << std::endl;

        /*
         * create ssbos for all output images
         */
        int outputImagesNumber = m_outputImageDescriptions.size();
        for (int i = 0; i < outputImagesNumber; i++)
        {
            OutputImageDescription outputImageDescription = m_outputImageDescriptions[i];
            int width = (outputImageDescription.width > 0) ? outputImageDescription.width : firstInputImage->getWidth();
            int height = (outputImageDescription.height > 0) ? outputImageDescription.height : firstInputImage->getHeight();
            int channelNumber = (outputImageDescription.channelNumber > 0) ? outputImageDescription.channelNumber : firstInputImage->getChannelNumber();
            int imageSize = width*height*channelNumber;

            GLuint ssboHandle = 0;
            createSSBO<float>(&ssboHandle, imageSize);
            std::cout << std::to_string(ssboHandle) << std::endl;
            fillSSBOwithInitialValue<float>(&ssboHandle, 0.0f, imageSize);
            ssboHandles.push_back(ssboHandle);
            imageDimensions.push_back(ImageDimensions{width,height,channelNumber,0});
        }
        std::cout << "created output ssbos" << std::endl;

        /*
         * create ssbo for gpu image descriptions
         */
        GLuint gpuImageDescriptionSSBOHandle = 0;
        createSSBO<ImageDimensions>(&gpuImageDescriptionSSBOHandle, imageDimensions.size());
        copyDataToSSBO<ImageDimensions>(&gpuImageDescriptionSSBOHandle, imageDimensions.data(), imageDimensions.size());

        /*
         * bind all ssbos
         */
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gpuImageDescriptionSSBOHandle);
        for (int i = 0; i < ssboHandles.size(); i++)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i+1, ssboHandles[i]);
        }
        std::cout << "bound buffers" << std::endl;

        /*
         * upload all user defined variables and execute the compute shader
         */
        int pixelNumber = firstInputImage->getWidth()*firstInputImage->getHeight();
        m_computeShader->use();
        m_computeShader->update("pixelNumber", pixelNumber);
        m_interactableCollection.uploadInteractableUniforms(m_computeShader);
        glDispatchCompute(pixelNumber/256, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glFinish;

        GLuint err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Error after shader execution: " << std::to_string(err) << std::endl;
        }
        std::cout << "executed compute shader" << std::endl;



        /*
         * unbind all ssbos
         */
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
        for (int i = 0; i < ssboHandles.size(); i++)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i+1, 0);
        }
        std::cout << "unbound ssbos" << std::endl;

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

            GLuint ssboHandle = ssboHandles[i+inputImagesNumber];
            float* rawData = getDataFromSSBO<float>(&ssboHandle, imageSize);
            Image* outputImage = new Image;
            outputImage->setRawData(rawData, width, height, channelNumber);
            outputImage->setFileName(firstInputImage->getFileName());
            returnImage(outputImage);
            delete rawData;
        }
        std::cout << "downloaded ssbos" << std::endl;

        /*
         * delete all ssbos
         */
        deleteSSBO(&gpuImageDescriptionSSBOHandle);
        for (int i = 0; i < ssboHandles.size(); i++)
        {
            deleteSSBO(&ssboHandles[i]);
        }
        std::cout << "deleted all ssbos" << std::endl;
    }

protected:
    void setComputeShader(std::string computeShaderPath)
    {
        std::string fullPath = computeShaderPath+".comp";
        m_computeShader = new ShaderProgram(fullPath);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Error while creating shader program: " << std::to_string(err) << std::endl;
        }
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
            std::cerr << "Error while creating SSBO: " << std::to_string(err) << std::endl;
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
    static void copyDataToSSBO(GLuint* ssboHandle, T* data, int length){
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssboHandle);
        GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        memcpy(p, data, sizeof(T)*length);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Error while copying data to SSBO: " << std::to_string(err) << std::endl;
        }
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

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Error while getting data from ssbo: " << std::to_string(err) << std::endl;
        }

        return data;
    }

    /**
     * Deleting ssbo
     */
    static void deleteSSBO(GLuint* ssboHandler)
    {
        glDeleteBuffers(1, ssboHandler);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "Error while deleting SSBO: " << std::to_string(err) << std::endl;
        }
    }
};

#endif //SRCCMAKE_IMAGEFILTERGPU_H
