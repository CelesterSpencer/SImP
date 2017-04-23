#ifndef SRCCMAKE_INTERACTABLE_H
#define SRCCMAKE_INTERACTABLE_H

// std
#include <iostream>
#include <string>
#include <vector>
#include <map>

// lib
#include "imgui.h"

// project
#include "scene/Image.h"
#include "io/FileHandler.h"
#include "io/ImageHandler.h"
#include "scene/SystemFiles.h"
#include "scene/LayerManager.h"

// style
#include "rendering/ColorStyles.h"

class Interactable
{
public:
    std::string m_name;
    std::string m_uniformName = "";
    bool m_isSatisfied = true;
    bool m_hasImage = false;
    Image* m_image = nullptr;

    virtual void render(int num) = 0;
    virtual void uploadUniform(ShaderProgram* shaderProgram) = 0;
    void setUniformName(std::string uniformName) { m_uniformName = uniformName; }
};

class InteractableCollection
{
public:
    void addInteractable(Interactable* interactable);
    Interactable* getInteractable(std::string name);
    int getNumberOfElements();
    Interactable* getInteractableAt(int index);
    void uploadInteractableUniforms(ShaderProgram* shaderProgram);
private:
    std::vector<Interactable*> m_interactableVector;
    std::map<std::string,int> m_nameIndexMap;
};

class FSlider : public Interactable
{
public:
    FSlider(std::string name, float* data, float* min, float* max, std::string uniformName = "");

    void render(int num);
    void uploadUniform(ShaderProgram* shaderProgram);
private:
    float* m_data;
    float* m_min;
    float* m_max;
};

class ISlider : public Interactable
{
public:
    ISlider(std::string name, int* data, int* min, int* max, std::string uniformName = "");

    void render(int num);
    void uploadUniform(ShaderProgram* shaderProgram);
private:
    int* m_data;
    int* m_min;
    int* m_max;
};

class Checkbox : public Interactable
{
public:
    Checkbox(std::string name, bool* data, std::string uniformName = "");

    void render(int num);
    void uploadUniform(ShaderProgram* shaderProgram);
private:
    bool* m_data;
};

class SelectBox : public Interactable
{
public:
    SelectBox(std::string name, int* selectedOption, std::vector<std::string>* options, std::string uniformName = "");
    ~SelectBox();

    void render(int num);
    void uploadUniform(ShaderProgram* shaderProgram);

private:
    bool* m_data;
    int *m_selectedOption;
    const char** mp_options;
    int m_optionsSize;
};

class ImageSelector : public Interactable
{
public:
    ImageSelector(std::string name);
    ~ImageSelector();

    void render(int num);
    void uploadUniform(ShaderProgram* shaderProgram);

private:
    GLuint m_imageHandle;
    GLuint m_imageHandleOpen;
    bool m_isImageLoaded;
    bool m_shouldLoadImage;
};

#endif //SRCCMAKE_INTERACTABLE_H
