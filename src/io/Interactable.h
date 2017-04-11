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

// style
#include "rendering/ColorStyles.h"

class Interactable
{
public:
    std::string m_name;

    virtual void render(int num) = 0;
};

class InteractableCollection
{
public:
    void addInteractable(Interactable* interactable)
    {
        m_interactableVector.push_back(interactable);
        m_nameIndexMap[interactable->m_name] = m_interactableVector.size()-1;
    }
    Interactable* getInteractable(std::string name)
    {
        auto it = m_nameIndexMap.find(name);
        if(it != m_nameIndexMap.end())
        {
            return m_interactableVector[it->second];
        }
        else
        {
            std::cerr << "Could not find user data of name " << name << std::endl;
            return nullptr;
        }
    }
    int getNumberOfElements() { return m_interactableVector.size(); }
    Interactable* getInteractableAt(int index) { return  m_interactableVector[index]; }
private:
    std::vector<Interactable*> m_interactableVector;
    std::map<std::string,int> m_nameIndexMap;
};

class FSlider : public Interactable
{
public:
    FSlider(std::string name, float* data, float* min, float* max)
    {
        m_name = name;
        m_data = data;
        m_min = min;
        m_max = max;
    }

    void render(int num)
    {
        ImGui::SliderFloat(("##"+std::to_string(num)).c_str(), m_data, *m_min, *m_max);
    }
private:
    float* m_data;
    float* m_min;
    float* m_max;
};

class ISlider : public Interactable
{
public:
    ISlider(std::string name, int* data, int* min, int* max)
    {
        m_name = name;
        m_data = data;
        m_min = min;
        m_max = max;
    }

    void render(int num)
    {
        ImGui::SliderInt(("##"+std::to_string(num)).c_str(), m_data, *m_min, *m_max);
    }
private:
    int* m_data;
    int* m_min;
    int* m_max;
};

class Checkbox : public Interactable
{
public:
    Checkbox(std::string name, bool* data)
    {
        m_name = name;
        m_data = data;
    }

    void render(int num)
    {
        ImGui::Checkbox(("##"+std::to_string(num)).c_str(), m_data);
    }
private:
    bool* m_data;
};

class SelectBox : public Interactable
{
public:
    SelectBox(std::string name, int* selectedOption, std::vector<std::string>* options)
    {
        m_name = name;
        m_selectedOption = selectedOption;

        // convert options to char array
        mp_options = new const char*[options->size()];
        for(int optionsIdx = 0; optionsIdx < options->size(); optionsIdx++)
            mp_options[optionsIdx] = options->at(optionsIdx).c_str();

        m_optionsSize = options->size();
    }
    ~SelectBox()
    {
        delete[] mp_options;
    }

    void render(int num)
    {
        ImGui::Combo(("##"+std::to_string(num)).c_str(),
                     m_selectedOption,
                     mp_options,
                     m_optionsSize);
    }
private:
    bool* m_data;
    int *m_selectedOption;
    const char** mp_options;
    int m_optionsSize;
};

class ImageSelector : public Interactable
{
public:
    ImageSelector(std::string name)
    {
        m_name = name;
        m_isImageLoaded = false;
        m_imageHandle = 0;
        m_imageHandleOpen = 0;

        // load open image
        m_imageHandleOpen = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_OPEN);
    }
    ~ImageSelector()
    {
        if (m_imageHandle != 0)
        {
            glDeleteTextures(1, &m_imageHandle);
            m_imageHandle = 0;
        }
        if (m_imageHandleOpen != 0)
        {
            glDeleteTextures(1, &m_imageHandleOpen);
            m_imageHandleOpen = 0;
        }
    }

    void render(int num)
    {
        bool shouldLoadImage = false;

        /*
         * show image
         */
        if (m_isImageLoaded)
            ImGui::Image((GLuint*)m_imageHandle, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1));
        else
        {
            ImVec2 imageStart = ImVec2(ImGui::GetCursorPos().x+ImGui::GetWindowPos().x, ImGui::GetCursorPos().y+ImGui::GetWindowPos().y - ImGui::GetScrollY());
            ImVec2 imageEnd   = ImVec2(imageStart.x+19, imageStart.y+19);
            ImGui::GetWindowDrawList()->AddRectFilled(imageStart, imageEnd, NO_IMAGE_COLOR);
        }

        /*
         * load image button
         */
        ImGui::SameLine(ImGui::GetWindowWidth()/2+5+19+10);
        ImGui::PushID(("filterImageBtn"+std::to_string(num)).c_str());
        if(ImGui::ImageButton((ImTextureID)m_imageHandleOpen, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1), 0))
        {
            shouldLoadImage = true;
        }
        ImGui::PopID();

        /*
         * open file dialog and load image icon
         */
        if(shouldLoadImage)
        {
            std::string filePath = "";
            if(FileHandler::getInstance().openFilePathDialog(filePath))
            {
                m_imageHandle = ImageHandler::getInstance().getImageHandleFromFilePath(filePath);
                m_isImageLoaded = true;
            }
        }
    }
private:
    GLuint m_imageHandle;
    GLuint m_imageHandleOpen;
    bool m_isImageLoaded;
};

#endif //SRCCMAKE_INTERACTABLE_H
