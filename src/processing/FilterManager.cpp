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

void FilterManager::addRequiredImages()
{
    /*
     * collect all images from the interactables
     */
    ImageFilter* imageFilter = m_imageFilters[m_selectedFilter];
    int numberOfInteractableElements = imageFilter->getInteractableCollection()->getNumberOfElements();
    for(int i = 0; i < numberOfInteractableElements; i++)
    {
        Interactable* interactable = imageFilter->getInteractableCollection()->getInteractableAt(i);
        if(interactable->m_hasImage)
        {
            m_inputImages.push_back(interactable->m_image);
        }
    }
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

    ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
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
    ImGui::PopStyleColor(1);

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
        int numberOfInteractableElements = imageFilter->getInteractableCollection()->getNumberOfElements();
        if (numberOfInteractableElements > 0)
        {
            // show filter dialog
            ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
            ImGui::Begin("Filter settings", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(  "Filter settings",
                                  ImVec2(mainWindowWidth/2 - ImGui::GetWindowWidth()/2,
                                         mainWindowHeight/2 - ImGui::GetWindowHeight()/2));

            // image filter name
            ImGui::Text((imageFilter->getName()+":").c_str());
            ImGui::Separator();
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();

            // render all image filter options
            bool isSatisfied = true;
            for(int i = 0; i < numberOfInteractableElements; i++)
            {
                // interactable name
                Interactable*interactable = imageFilter->getInteractableCollection()->getInteractableAt(i);
                ImGui::Text(interactable->m_name.c_str());

                // interactable content
                ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
                ImGui::PushItemWidth(ImGui::GetWindowWidth()/2-20);
                interactable->render(i);

                // collect if interactable's requirements are satisfied
                isSatisfied = isSatisfied && interactable->m_isSatisfied;
            }

            // apply button
            if(!isSatisfied)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
            }
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::SameLine(5);
            if(ImGui::Button("Apply", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)) && isSatisfied)
            {
                // change state to
                m_selectionStatus = 1;
                m_isFilterMenuOpen = false;
            }
            if(!isSatisfied) ImGui::PopStyleColor(3);

            // cancel button
            ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
            if(ImGui::Button("Cancel", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
            {
                m_selectedFilter = -1;
                m_selectionStatus = -1;
                m_isFilterMenuOpen = false;
            }
            ImGui::End();
            ImGui::PopStyleColor(1);
        }
        else {
            // If no user data elements are specify just return Apply status
            m_selectionStatus = 1;
            m_isFilterMenuOpen = false;
        }
    }
}