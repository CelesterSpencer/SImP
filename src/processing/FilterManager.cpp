//
// Created by Windrian on 21.03.2017.
//

#include "FilterManager.h"

void FilterManager::registerImageFilter(ImageFilter* imageFilter)
{
    m_imageFilters.push_back(imageFilter);
    auto filterGroupIt = m_imageGroupFilterIdsMap.find(imageFilter->getFilterGroup());
    if (filterGroupIt == m_imageGroupFilterIdsMap.end())
    {
        std::vector<int> filterIdVector;
        filterIdVector.push_back(m_imageFilters.size()-1);
        m_imageGroupFilterIdsMap[imageFilter->getFilterGroup()] = filterIdVector;
    }
    else
    {
        filterGroupIt->second.push_back(m_imageFilters.size()-1);
    }
}

void FilterManager::addImage(Image* in)
{
    m_inputImages.push_back(in);
}

std::vector<Image*> FilterManager::getOutputImages()
{
    return m_outputImages;
}

void FilterManager::resetFilter()
{
    m_inputImages.clear();
    m_outputImages.clear();
    if(m_selectedFilter >= 0) m_imageFilters[m_selectedFilter]->clearImages();
    m_selectedFilter = -1;
}

void FilterManager::applyFilter()
{
    if(m_selectedFilter >= 0)
    {
        ImageFilter* filter = m_imageFilters[m_selectedFilter];
        for (int i = 0; i < m_inputImages.size(); i++)
        {
            filter->addInputImage(m_inputImages[i]);
        }

        filter->process();
        m_outputImages = filter->getOutputImages();
    }
}

int FilterManager::drawFilterMenu(bool validActiveLayer, bool processingActive)
{
    int mainWindowWidth = WindowManager::getInstance().getWidth();
    int mainWindowHeight = WindowManager::getInstance().getHeight();

    if (ImGui::BeginMenu("Filter"))
    {
        for(auto filterGroupIt = m_imageGroupFilterIdsMap.begin(); filterGroupIt != m_imageGroupFilterIdsMap.end(); ++filterGroupIt)
        {
            ImGui::TextColored(TEXT_HIGHLIGHTED_COLOR, filterGroupIt->first.c_str());
            ImGui::Indent();
            for(auto filterIdsIt = filterGroupIt->second.begin(); filterIdsIt != filterGroupIt->second.end(); ++filterIdsIt)
            {
                int filterIndex = *filterIdsIt;
                auto imageFilter = m_imageFilters[filterIndex];
                if (ImGui::MenuItem(imageFilter->getName().c_str(), NULL, false, validActiveLayer && !processingActive)) {
                    m_selectedFilter = filterIndex;
                    m_isFilterMenuOpen = true;
                }
            }
            ImGui::Unindent();
        }
        ImGui::EndMenu();
    }

    // call the dialog
    if(!processingActive) drawFilterSettingsDialog();

    return m_selectionStatus;
}

void FilterManager::drawFilterSettingsDialog()
{
    int mainWindowWidth = WindowManager::getInstance().getWidth();
    int mainWindowHeight = WindowManager::getInstance().getHeight();

    m_selectionStatus = 0;
    if(m_isFilterMenuOpen)
    {
        ImGui::SetWindowSize("Filter settings", ImVec2(std::min(ImGui::GetWindowWidth()/2, 400.f), -1));

        ImageFilter* imageFilter = m_imageFilters[m_selectedFilter];
        int numberOfUserDataElements = imageFilter->getUserDataCollection()->getNumberOfElements();
        if (numberOfUserDataElements > 0)
        {
            ImGui::Begin("Filter settings", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(  "Filter settings",
                                  ImVec2(mainWindowWidth/2 - ImGui::GetWindowWidth()/2,
                                         mainWindowHeight/2 - ImGui::GetWindowHeight()/2));

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
                m_selectionStatus = 1;
                m_isFilterMenuOpen = false;
            }
            ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
            if(ImGui::Button("Cancel", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
            {
                m_selectedFilter = -1;
                m_selectionStatus = -1;
                m_isFilterMenuOpen = false;
            }
            ImGui::End();
        }
        else {
            // If no user data elements are specify just return Apply status
            m_selectionStatus = 1;
            m_isFilterMenuOpen = false;
        }
    }
}