//
// Created by Windrian on 21.03.2017.
//

#include "FilterManager.h"

void FilterManager::registerImageFilter(ImageFilter* imageFilter)
{
    m_imageFilters.push_back(imageFilter);
}

Image* FilterManager::applyFilter(Image* in)
{
    if(m_selectedFilter >= 0)
    {
        Image* out = m_imageFilters[m_selectedFilter]->process(in);
        m_selectedFilter = -1;
        return out;
    }
    return nullptr;
}

int FilterManager::drawFilterMenu(bool validActiveLayer)
{
    if (ImGui::BeginMenu("Filter"))
    {
        for(int i = 0; i < m_imageFilters.size(); i++)
        {
            ImageFilter* imageFilter = m_imageFilters[i];
            if (ImGui::MenuItem(imageFilter->getName().c_str(), NULL, false, validActiveLayer)) {
                m_selectedFilter = i;
            }
        }

        ImGui::EndMenu();
    }

    int selectionStatus = 0;
    if(m_selectedFilter >= 0)
    {
        ImageFilter* imageFilter = m_imageFilters[m_selectedFilter];
        int numberOfUserDataElements = imageFilter->getUserDataCollection()->getNumberOfElements();
        if (numberOfUserDataElements > 0)
        {
            ImGui::Begin("Filter settings", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::Text((imageFilter->getName()+":").c_str());
            ImGui::Separator();
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
            for(int i = 0; i < numberOfUserDataElements; i++)
            {
                UserData* userData = imageFilter->getUserDataCollection()->getUserDataAt(i);
                ImGui::Text(userData->m_name.c_str());
                switch (userData->m_dataType)
                {
                    case UserData::DT_INT:
                        ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
                        ImGui::PushItemWidth(ImGui::GetWindowWidth()/2-20);
                        ImGui::SliderInt(("##"+std::to_string(i)).c_str(), static_cast<int*>(userData->mp_data), *static_cast<int*>(userData->mp_min), *static_cast<int*>(userData->mp_max));
                        break;
                    case UserData::DT_FLOAT:
                        ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
                        ImGui::PushItemWidth(ImGui::GetWindowWidth()/2-20);
                        ImGui::SliderFloat(("##"+std::to_string(i)).c_str(), static_cast<float*>(userData->mp_data), *static_cast<float*>(userData->mp_min), *static_cast<float*>(userData->mp_max));
                        break;
                    case UserData::DT_BOOL:
                        ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
                        ImGui::PushItemWidth(ImGui::GetWindowWidth()/2-20);
                        ImGui::Checkbox(("##"+std::to_string(i)).c_str(), static_cast<bool*>(userData->mp_data));
                        break;
                    case UserData::DT_OPTIONS:
                        std::vector<std::string> options = *(static_cast<std::vector<std::string>*>(userData->mp_data));
                        const char** p_options = new const char*[options.size()];
                        for(int optionsIdx = 0; optionsIdx < options.size(); optionsIdx++)
                            p_options[optionsIdx] = options[optionsIdx].c_str();

                        ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
                        ImGui::PushItemWidth(ImGui::GetWindowWidth()/2-20);
                        ImGui::Combo(("##"+std::to_string(i)).c_str(),
                                       static_cast<int*>(userData->mp_min),
                                       p_options,
                                       *static_cast<int*>(userData->mp_max));
                        delete[] p_options;
                        break;
                }
            }
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::SameLine(5);
            if(ImGui::Button("Apply", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
            {
                selectionStatus = 1;
            }
            ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
            if(ImGui::Button("Cancel", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
            {
                m_selectedFilter = -1;
                selectionStatus = -1;
            }
            ImGui::End();
        }
        else {
            // If no user data elements are specify just return Apply status
            selectionStatus = 1;
        }
    }

    return selectionStatus;
}