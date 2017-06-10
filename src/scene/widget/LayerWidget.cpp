#include "LayerWidget.h"

LayerWidget::LayerWidget(int width, int height) : m_shaderProgram("/simple.vert", "/simple.frag")
{
    // load system images for the image buttons
    m_imageHandleDelete     = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_DELETE);
    m_imageHandleOpen       = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_OPEN);

    m_activeLayer = -1;
    m_isActive = true;
    ImGui::SetWindowPos("Layers", ImVec2(width-260, height/4));
}

bool LayerWidget::draw(int width, int height, float deltaTime)
{
    // draw layers
    for(int i = 0; i < m_layers.size(); i++)
    {
        m_layers[i]->render(&m_shaderProgram);
    }



    int mainWindowWidth = width;
    int mainWindowHeight = height;



    // set window dimension and position
    ImGui::SetWindowSize( "Layers" , ImVec2(300, ImGui::GetWindowHeight()) );



    // variables that store the result of the buttons
    bool shouldDeleteLayer  = false;
    bool makeNewLayer       = false;
    bool shouldLoadImage    = false;



    int numberOfLayers = m_layers.size();



    ImGui::Begin("Layers", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    for (int i = 0; i < numberOfLayers; i++)
    {
        ImGui::BeginGroup();
        Layer* currentLayer = m_layers[i];



        // draw background for selected element
        ImVec2 start = ImVec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y + 24 + i*27 - ImGui::GetScrollY());
        ImVec2 end = ImVec2(start.x + ImGui::GetWindowWidth(), start.y + 26);
        auto layerColor = LAYER_COLOR;
        if (ImGui::GetMousePos().x >= start.x &&
            ImGui::GetMousePos().x < end.x &&
            ImGui::GetMousePos().y > start.y &&
            ImGui::GetMousePos().y < end.y)
        {
            layerColor = LAYER_HOVER_COLOR;
            if(ImGui::IsMouseClicked(0, false)) m_activeLayer = i;
            // select current layer and hide all other layers when double clicked
            if(ImGui::IsMouseDoubleClicked(0))
            {
                for(int layerIdx = 0; layerIdx < m_layers.size(); layerIdx++)
                {
                    if(layerIdx != i)
                    {
                        m_layers[layerIdx]->setOpacity(0);
                    }
                    else {
                        m_layers[layerIdx]->setOpacity(1);
                    }
                }
            }
        }
        if(i == m_activeLayer) layerColor = LAYER_ACTIVE_COLOR;
        ImGui::GetWindowDrawList()->AddRectFilled(start, end, layerColor);



        // layer name
        int pos = 0;
        ImGui::AlignFirstTextHeightToWidgets();
        if(currentLayer->hasImage())
        {
            std::string fileName = currentLayer->getImage()->getFileName();
            int bufferSize = std::max(200, (int)fileName.size()+1);
            char buffer[bufferSize];
            for(int strIdx = 0; strIdx < fileName.size(); strIdx++)
            {
                buffer[strIdx] = fileName[strIdx];
            }
            buffer[fileName.size()] = '\0';
            ImGui::PushItemWidth(73);
            ImGui::InputText(("##Layer " + std::to_string(i)).c_str(),
                 buffer, bufferSize, ImGuiInputTextFlags_CallbackCharFilter,
                 [](ImGuiTextEditCallbackData* data)
                 {
                     ImWchar c = data->EventChar;
                     if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' '))
                         return 1;
                     return 0;
                 }
            );
            std::string outputText = "";
            for(int strIdx = 0; strIdx < bufferSize; strIdx++)
            {
                if (buffer[strIdx] == '\0') break;
                outputText.push_back(buffer[strIdx]);
            }

            currentLayer->getImage()->setFileName(outputText);
        }
        else
        {
            ImGui::Text(("Layer " + std::to_string(i)).c_str());
        }



        // layer opacity
        pos += 100;
        ImGui::SameLine(pos);
        float layerOpacity = currentLayer->getOpacity();
        ImGui::PushItemWidth(50);
        ImGui::SliderFloat(("##" + std::to_string(i)).c_str(), &layerOpacity, 0.0, 1.0);
        currentLayer->setOpacity(layerOpacity);



        // image
        pos += 70;
        ImGui::SameLine(pos);
        if (currentLayer->hasImage())
            ImGui::Image((GLuint*)(intptr_t)currentLayer->getGpuImageHandle(), ImVec2(19,19), ImVec2(0,0), ImVec2(1,1));
        else
        {
            ImVec2 imageStart = ImVec2(ImGui::GetCursorPos().x+ImGui::GetWindowPos().x, ImGui::GetCursorPos().y+ImGui::GetWindowPos().y - ImGui::GetScrollY());
            ImVec2 imageEnd   = ImVec2(imageStart.x+19, imageStart.y+19);
            ImGui::GetWindowDrawList()->AddRectFilled(imageStart, imageEnd, NO_IMAGE_COLOR);
        }



        // load-image button
        pos += 30;
        ImGui::SameLine(pos);
        ImGui::PushID(("imageBtn"+std::to_string(i)).c_str());
        // color button gray when inactive
        if (!m_isActive)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
        }
        if(ImGui::ImageButton((GLuint*)(intptr_t)m_imageHandleOpen, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1), 0)
        && m_isActive)
        {
            m_activeLayer = i;
            shouldLoadImage = true;
        }
        if (!m_isActive) ImGui::PopStyleColor(3);
        ImGui::PopID();

        if (currentLayer->hasImage())
        {
            std::string channelInfo = "";
            switch(currentLayer->getImage()->getChannelNumber())
            {
                case 1:
                    channelInfo = "G";
                    break;
                case 3:
                    channelInfo = "RGB";
                    break;
                case 4:
                    channelInfo = "RGBa";
                    break;
                default:
                    channelInfo = "N/A";
                    break;
            }
            float charWidth = 5.0;
            pos += 42;
            ImGui::SameLine(pos-channelInfo.size()/2.0*charWidth);
            ImGui::Text(channelInfo.c_str());
        }



        // delete layer button
        ImGui::SameLine(ImGui::GetWindowWidth()-28);
        ImGui::PushID(("deleteLayerBtn"+std::to_string(i)).c_str());
        if (!m_isActive)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
        }
        if(ImGui::ImageButton((GLuint*)(intptr_t)m_imageHandleDelete, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1), 0)
        && m_isActive)
        {
            shouldDeleteLayer = true;
        }
        if (!m_isActive) ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::EndGroup();
        ImGui::Separator();
    }
    ImGui::Spacing();
    ImGui::SameLine(ImGui::GetWindowWidth()/2.0 - 35);
    if (!m_isActive)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
    }
    if(ImGui::Button("New Layer") && m_isActive)
    {
        makeNewLayer = true;
    }
    if (!m_isActive) ImGui::PopStyleColor(3);
    ImGui::End();



    if(shouldLoadImage)
    {
        std::string filePath = "";
        if(FileHandler::getInstance().openFilePathDialog(filePath))
        {
            if (m_activeLayer >= 0 && filePath.length() > 0)
            {
                if(!m_layers[m_activeLayer]->hasImage())
                {
                    Image* p_image = new Image;
                    p_image->load(filePath);
                    m_layers[m_activeLayer]->setImage(p_image);
                }
                else
                {
                    m_layers[m_activeLayer]->getImage()->load(filePath);
                }
            }
        }
    }

    if(makeNewLayer)
    {
        addLayer();
    }

    if(shouldDeleteLayer)
    {
        bool shouldDecreaseActiveLayerId = m_activeLayer == m_layers.size()-1;
        deleteLayer(m_activeLayer);
        if(shouldDecreaseActiveLayerId) m_activeLayer--;
    }

    return true;
}