//
// Created by Windrian on 13.04.2017.
//

#include "Interactable.h"

#include "scene/LayerManager.h"

//_________________________________________INTERACTABLE_COLLECTION____________________________________________________//

void InteractableCollection::addInteractable(Interactable* interactable)
{
    m_interactableVector.push_back(interactable);
    m_nameIndexMap[interactable->m_name] = m_interactableVector.size()-1;
}

Interactable* InteractableCollection::getInteractable(std::string name)
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

int InteractableCollection::getNumberOfElements() { return m_interactableVector.size(); }

Interactable* InteractableCollection::getInteractableAt(int index) { return  m_interactableVector[index]; }



//_________________________________________________F_SLIDER___________________________________________________________//

FSlider::FSlider(std::string name, float* data, float* min, float* max)
{
    m_name = name;
    m_data = data;
    m_min = min;
    m_max = max;
}

void FSlider::render(int num)
{
    ImGui::SliderFloat(("##"+std::to_string(num)).c_str(), m_data, *m_min, *m_max);
}



//_________________________________________________I_SLIDER___________________________________________________________//

ISlider::ISlider(std::string name, int* data, int* min, int* max)
{
    m_name = name;
    m_data = data;
    m_min = min;
    m_max = max;
}

void ISlider::render(int num)
{
    ImGui::SliderInt(("##"+std::to_string(num)).c_str(), m_data, *m_min, *m_max);
}



//_________________________________________________CHECKBOX___________________________________________________________//

Checkbox::Checkbox(std::string name, bool* data)
{
    m_name = name;
    m_data = data;
}

void Checkbox::render(int num)
{
    ImGui::Checkbox(("##"+std::to_string(num)).c_str(), m_data);
}



//________________________________________________SELECT_BOX__________________________________________________________//

SelectBox::SelectBox(std::string name, int* selectedOption, std::vector<std::string>* options)
{
    m_name = name;
    m_selectedOption = selectedOption;

    // convert options to char array
    mp_options = new const char*[options->size()];
    for(int optionsIdx = 0; optionsIdx < options->size(); optionsIdx++)
        mp_options[optionsIdx] = options->at(optionsIdx).c_str();

    m_optionsSize = options->size();
}

SelectBox::~SelectBox()
{
    delete[] mp_options;
}

void SelectBox::render(int num)
{
    ImGui::Combo(("##"+std::to_string(num)).c_str(),
                 m_selectedOption,
                 mp_options,
                 m_optionsSize);
}



//______________________________________________IMAGE_SELECTOR________________________________________________________//

ImageSelector::ImageSelector(std::string name)
{
    m_name = name;
    m_isImageLoaded = false;
    m_image = nullptr;
    m_imageHandle = 0;
    m_imageHandleOpen = 0;

    // load open image
    m_imageHandleOpen = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_OPEN);
}

ImageSelector::~ImageSelector()
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

void ImageSelector::render(int num)
{
    bool shouldLoadImage = false;

    /*
     * show image
     */
    if (m_isImageLoaded)
        ImGui::Image((GLuint*)(intptr_t)m_imageHandle, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1));
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
    if(ImGui::ImageButton((GLuint*)(intptr_t)m_imageHandleOpen, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1), 0))
    {
        shouldLoadImage = true;
        LayerManager::getInstance().resetLayerImageSelectionMenu();
    }
    ImGui::PopID();

    /*
     * open file dialog and load image icon
     */
    if(shouldLoadImage)
    {
        LayerManager::getInstance().drawLayerImageSelectionMenu();
        if(LayerManager::getInstance().getLayerImageSelectionStatus() == 1)
        {
            m_imageHandle = LayerManager::getInstance().getLayer(
                    LayerManager::getInstance().getLayerImageSelectionIndex())->getGpuImageHandle();
            m_image = LayerManager::getInstance().getImage(LayerManager::getInstance().getLayerImageSelectionIndex());
            m_isImageLoaded = true;
            shouldLoadImage = false;
        }
        else if (LayerManager::getInstance().getLayerImageSelectionStatus() == -1)
        {
            shouldLoadImage = false;
        }
    }
}