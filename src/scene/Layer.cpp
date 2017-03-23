//
// Created by Windrian on 18.03.2017.
//

#include "Layer.h"

Layer::Layer(int pos)
{
    m_name = "Unnamed";
    m_pos = pos;
    m_opacity = 1.0;
    m_image = nullptr;
    m_gpuImageHandle = 0;

    makeBuffer(pos / 10);
}

Layer::~Layer()
{
    if (m_gpuImageHandle != 0)
    {
        glDeleteTextures(1, &m_gpuImageHandle);
        m_gpuImageHandle = 0;
    }
    m_image = nullptr;
    glDeleteVertexArrays(1, &m_vao);
}

void Layer::setImage(Image* image)
{
    m_image = image;
}

Image* Layer::getImage()
{
    return m_image;
}

bool Layer::hasImage()
{
    return m_image != nullptr;
}

std::string Layer::getName()
{
    return m_name;
}

void Layer::setName(std::string name)
{
    m_name = name;
}

void Layer::setPos(int pos)
{
    m_pos = pos;
}

void Layer::setOpacity(float opacity)
{
    m_opacity = opacity;
}

float Layer::getOpacity()
{
    return m_opacity;
}

void Layer::render(ShaderProgram* shaderProgram)
{
    if (hasImage())
    {
        if (m_image->hasBeenModified()) uploadData();
        shaderProgram->update("transparency", m_opacity);
        shaderProgram->update("isGrayScale", m_image->getChannelNumber() == 1);
        shaderProgram->bindTextureOnUse("tex", m_gpuImageHandle);
        shaderProgram->use();
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void Layer::makeBuffer(float zPos)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLuint vertexBufferHandles[2];
    glGenBuffers(2, vertexBufferHandles);

    m_vertices = vertexBufferHandles[0];
    m_uvs = vertexBufferHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
    float positions[] = {
            // left triangle
            -1, 1, zPos,
            -1,-1, zPos,
             1,-1, zPos,
            // right triangle
            -1, 1, zPos,
             1,-1, zPos,
             1, 1, zPos
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLfloat uvCoordinates[] = {
            // Front face
            0,1,
            0,0,
            1,0,

            0,1,
            1,0,
            1,1
    };
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoordinates), uvCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Layer::uploadData()
{
    //________________________________________________REPLACE_IMAGE___________________________________________________//
    if (m_gpuImageHandle == 0 || m_image->hasBeenResized())
    {
        /*
         * delete old data
         */
        if (m_gpuImageHandle != 0)
        {
            glDeleteTextures(1, &m_gpuImageHandle);
            m_gpuImageHandle = 0;
        }

        /*
         * create new texture
         */
        glGenTextures(1, &m_gpuImageHandle);

        /*
         * bind the texture for uploading data
         */
        glBindTexture(GL_TEXTURE_2D, m_gpuImageHandle);

        /*
         * send image data to the new texture
         */
        switch(m_image->getChannelNumber())
        {
            case 1:
                glTexImage2D(GL_TEXTURE_2D, 0,GL_RED, m_image->getWidth(), m_image->getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, m_image->getRawData());
                break;
            case 3:
                glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_image->getWidth(), m_image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image->getRawData());
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, m_image->getWidth(), m_image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image->getRawData());
                break;
            default:
                std::cerr << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
                glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, m_image->getWidth(), m_image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image->getRawData());
                break;
        }

        /*
         * texture settings
         */
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /*
         * unbind texture
         */
        glBindTexture(GL_TEXTURE_2D, 0);

        // image is no longer in modified or resized state
        m_image->resetImageStatus();
    }
    //_____________________________________________NO_IMAGE_UPLOADED_YET______________________________________________//
    else
    {
        // copy data to gpu
        glBindTexture(GL_TEXTURE_2D, m_gpuImageHandle);
        switch(m_image->getChannelNumber())
        {
            case 1:
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_image->getWidth(), m_image->getHeight(), GL_RED, GL_UNSIGNED_BYTE, m_image->getRawData());
                break;
            case 3:
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_image->getWidth(), m_image->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, m_image->getRawData());
                break;
            case 4:
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_image->getWidth(), m_image->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, m_image->getRawData());
                break;
            default:
                std::cerr << "Unknown format for bytes per pixel. Texture won't be updated" << std::endl;
                break;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        m_image->resetImageStatus();
    }
}