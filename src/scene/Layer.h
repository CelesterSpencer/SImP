//
// Created by Windrian on 18.03.2017.
//

#ifndef SRCCMAKE_LAYER_H
#define SRCCMAKE_LAYER_H

#include <GL/gl3w.h>

#include "rendering/ShaderProgram.h"
#include "scene/Image.h"

class Layer {
public:
    Layer(int pos = 0);
    ~Layer();

    void setImage(Image* image);
    Image* getImage();
    bool hasImage();
    std::string getName();
    void setName(std::string name);
    void setPos(int pos);
    void setOpacity(float opacity);
    float getOpacity();

    void uploadData();

    void render(ShaderProgram* shaderProgram);

private:
    // layer info
    std::string m_name;
    int m_pos; // position within the array
    float m_opacity;

    // geometry
    GLuint m_vao;
    GLuint m_vertices;
    GLuint m_uvs;

    // image
    Image* m_image;
    GLuint m_gpuImageHandle;

    void makeBuffer(float zPos);
};


#endif //SRCCMAKE_LAYER_H
