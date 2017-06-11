#include "FilterSettingsWidget.h"

FilterSettingsWidget::FilterSettingsWidget(ImageFilter* imageFilter, WidgetManager* p_widgetManager, LayerManager* p_layerManager, std::function<void(int)> callbackFunction)
{
    m_imagefilter = imageFilter;
    mp_widgetManager = p_widgetManager;
    mp_layerManager = p_layerManager;
    m_callbackFunction = callbackFunction;
}
FilterSettingsWidget::~FilterSettingsWidget()
{

}

bool FilterSettingsWidget::draw(int width, int height, float deltaTime)
{
    int mainWindowWidth = width;
    int mainWindowHeight = height;
    ImGui::SetWindowSize("Filter settings", ImVec2(std::min((float)mainWindowWidth, 400.f), -1));


    bool status = true;
    int numberOfInteractableElements = m_imagefilter->getInteractableCollection()->getNumberOfElements();
    if (numberOfInteractableElements > 0)
    {
        // show filter dialog
        ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
        ImGui::Begin("Filter settings", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowPos(  "Filter settings",
                              ImVec2(mainWindowWidth/2.f - ImGui::GetWindowWidth()/2.f,
                                     mainWindowHeight/2.f - ImGui::GetWindowHeight()/2.f));

        // image filter name
        ImGui::Text((m_imagefilter->getName()+":").c_str());
        ImGui::Separator();
        ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();

        // render all image filter options
        bool isSatisfied = true;
        for(int i = 0; i < numberOfInteractableElements; i++)
        {
            // interactable name
            Interactable* interactable = m_imagefilter->getInteractableCollection()->getInteractableAt(i);
            ImGui::Text(interactable->m_name.c_str());

            // interactable content
            ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
            ImGui::PushItemWidth(ImGui::GetWindowWidth()/2-20);
            interactable->render(i, mp_widgetManager, mp_layerManager);

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
            m_callbackFunction(1);
            status = false;
        }
        if(!isSatisfied) ImGui::PopStyleColor(3);

        // cancel button
        ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
        if(ImGui::Button("Cancel", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
        {
            m_callbackFunction(0);
            status = false;
        }
        ImGui::End();
        ImGui::PopStyleColor(1);
    }
    else {
        // If no user data elements are specify just return Apply status
        m_callbackFunction(1);
        status = false;
    }

    return status;
}