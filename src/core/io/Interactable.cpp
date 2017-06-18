#include "Interactable.h"

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

void InteractableCollection::uploadInteractableUniforms(ShaderProgram* shaderProgram)
{
    // upload all enabled interactables that have a uniform name which appears in the shader
    for(int i = 0; i < m_interactableVector.size(); i++)
    {
        Interactable* interactable = m_interactableVector.at(i);
        if(glGetUniformLocation(shaderProgram->getShaderProgramHandle(), interactable->m_uniformName.c_str()) != -1)
        {
            if(interactable->isEnabled()) interactable->uploadUniform(shaderProgram);
        }
    }
}



//_________________________________________________F_SLIDER___________________________________________________________//

FSlider::FSlider(std::string name, float* data, float* min, float* max, std::string uniformName, std::function<bool()> isEnabledFunction)
{
    m_name = name;
    m_data = data;
    m_min = min;
    m_max = max;
    m_uniformName = uniformName;
    if(isEnabledFunction != nullptr) m_isEnabledFunction = isEnabledFunction;
}

void FSlider::render(int num, WidgetManager* p_widgetManager, LayerManager* p_layerManager)
{
    ImGui::SliderFloat(("##"+std::to_string(num)).c_str(), m_data, *m_min, *m_max);
}

void FSlider::uploadUniform(ShaderProgram* shaderProgram)
{
    shaderProgram->update(m_uniformName, *m_data);
}



//_________________________________________________I_SLIDER___________________________________________________________//

ISlider::ISlider(std::string name, int* data, int* min, int* max, std::string uniformName, std::function<bool()> isEnabledFunction)
{
    m_name = name;
    m_data = data;
    m_min = min;
    m_max = max;
    m_uniformName = uniformName;
    if(isEnabledFunction != nullptr) m_isEnabledFunction = isEnabledFunction;
}

void ISlider::render(int num, WidgetManager* p_widgetManager, LayerManager* p_layerManager)
{
    ImGui::SliderInt(("##"+std::to_string(num)).c_str(), m_data, *m_min, *m_max);
}

void ISlider::uploadUniform(ShaderProgram* shaderProgram)
{
    shaderProgram->update(m_uniformName, *m_data);
}



//_________________________________________________CHECKBOX___________________________________________________________//

Checkbox::Checkbox(std::string name, bool* data, std::string uniformName, std::function<bool()> isEnabledFunction)
{
    m_name = name;
    m_data = data;
    m_uniformName = uniformName;
    if(isEnabledFunction != nullptr) m_isEnabledFunction = isEnabledFunction;
}

void Checkbox::render(int num, WidgetManager* p_widgetManager, LayerManager* p_layerManager)
{
    ImGui::Checkbox(("##"+std::to_string(num)).c_str(), m_data);
}

void Checkbox::uploadUniform(ShaderProgram* shaderProgram)
{
    shaderProgram->update(m_uniformName, *m_data);
}



//________________________________________________SELECT_BOX__________________________________________________________//

SelectBox::SelectBox(std::string name, int* selectedOption, std::vector<std::string>* options, std::string uniformName, std::function<bool()> isEnabledFunction)
{
    m_name = name;
    m_selectedOption = selectedOption;
    m_uniformName = uniformName;

    // convert options to char array
    mp_options = new const char*[options->size()];
    for(int optionsIdx = 0; optionsIdx < options->size(); optionsIdx++)
        mp_options[optionsIdx] = options->at(optionsIdx).c_str();

    m_optionsSize = options->size();
    if(isEnabledFunction != nullptr) m_isEnabledFunction = isEnabledFunction;
}

SelectBox::~SelectBox()
{
    delete[] mp_options;
}

void SelectBox::render(int num, WidgetManager* p_widgetManager, LayerManager* p_layerManager)
{
    ImGui::Combo(("##"+std::to_string(num)).c_str(),
                 m_selectedOption,
                 mp_options,
                 m_optionsSize);
}

void SelectBox::uploadUniform(ShaderProgram* shaderProgram)
{
    shaderProgram->update(m_uniformName, *m_selectedOption);
}



//______________________________________________IMAGE_SELECTOR________________________________________________________//

ImageSelector::ImageSelector(std::string name, std::function<bool()> isEnabledFunction)
{
    m_name = name;
    m_isSatisfied = false;
    m_hasImage = true;
    m_isImageLoaded = false;
    m_image = nullptr;
    m_imageHandle = 0;
    m_imageHandleOpen = 0;
    m_dialogIsOpen = false;

    // load open image
    m_imageHandleOpen = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_OPEN);
    if(isEnabledFunction != nullptr) m_isEnabledFunction = isEnabledFunction;
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

void ImageSelector::render(int num, WidgetManager* p_widgetManager, LayerManager* p_layerManager)
{
    m_isSatisfied = false;

    // show image
    if (m_isImageLoaded)
        ImGui::Image((GLuint*)(intptr_t)m_imageHandle, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1));
    else
    {
        ImVec2 imageStart = ImVec2(ImGui::GetCursorPos().x+ImGui::GetWindowPos().x, ImGui::GetCursorPos().y+ImGui::GetWindowPos().y - ImGui::GetScrollY());
        ImVec2 imageEnd   = ImVec2(imageStart.x+19, imageStart.y+19);
        ImGui::GetWindowDrawList()->AddRectFilled(imageStart, imageEnd, NO_IMAGE_COLOR);
    }

    // load image button
    ImGui::SameLine(ImGui::GetWindowWidth()/2+5+19+10);
    ImGui::PushID(("filterImageBtn"+std::to_string(num)).c_str());
    if (m_dialogIsOpen)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
    }
    if(ImGui::ImageButton((GLuint*)(intptr_t)m_imageHandleOpen, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1), 0) && !m_dialogIsOpen)
    {
        p_widgetManager->addWidget(new ImageSelectionWidget(p_layerManager,
            [&, p_layerManager](int status, int selectedImageIndex)
            {
                if(status == 1)
                {
                    m_imageHandle       = p_layerManager->getLayer(selectedImageIndex)->getGpuImageHandle();
                    m_image             = p_layerManager->getLayer(selectedImageIndex)->getImage();
                    m_isImageLoaded     = true;
                    m_dialogIsOpen   = false;
                }
                else if (status == -1)
                {
                    m_dialogIsOpen = false;
                }
            }
        ), 40, true);
    }
    if (m_dialogIsOpen) ImGui::PopStyleColor(3);
    ImGui::PopID();

    m_isSatisfied = m_isImageLoaded;
}

void ImageSelector::uploadUniform(ShaderProgram* shaderProgram)
{
    // do nothing
}