#include "ImageSelectionWidget.h"

ImageSelectionWidget::ImageSelectionWidget(LayerManager* p_layerManager, std::function<void(int, int)> callbackFunction)
{
    mp_layerManager = p_layerManager;
    m_selectedImageIdx = -1;
    m_callbackFunction = callbackFunction;
}
ImageSelectionWidget::~ImageSelectionWidget()
{

}

bool ImageSelectionWidget::draw(int width, int height, float deltaTime)
{
    // set window properties
    ImGui::SetWindowSize("Select image", ImVec2(std::max(ImGui::GetWindowWidth()/2, 400.f), -1));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);

    // select image window
    ImGui::Begin("Select image", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::SetWindowPos(  "Select image",
                          ImVec2(width/2.f - ImGui::GetWindowWidth()/2.f,
                                 height/2.f - ImGui::GetWindowHeight()/2.f));

    // draw selected background rect and the corresponding image
    int imageWidth = 100;
    int imageHeight = 100;
    int childWindowHeight = 400;

    ImGui::BeginChild("imagesSelection", ImVec2(ImGui::GetWindowWidth(), childWindowHeight), false);
    for(int i = 0; i < mp_layerManager->getNumberOfLayers(); i++)
    {
        // draw background for selected element
        int layersStartY = ImGui::GetWindowPos().y;
        ImVec2 start = ImVec2(ImGui::GetWindowPos().x, layersStartY + i*(imageHeight+4) - ImGui::GetScrollY());
        ImVec2 end = ImVec2(start.x + ImGui::GetWindowWidth(), start.y + imageHeight);
        auto layerColor = LAYER_COLOR;
        if (ImGui::GetMousePos().x >= start.x &&
            ImGui::GetMousePos().x < end.x &&
            ImGui::GetMousePos().y > start.y &&
            ImGui::GetMousePos().y < end.y &&
            ImGui::GetMousePos().y > layersStartY &&
            ImGui::GetMousePos().y < layersStartY+childWindowHeight)
        {
            layerColor = LAYER_HOVER_COLOR;
            if(ImGui::IsMouseClicked(0, false)) m_selectedImageIdx = i;
        }
        if(i == m_selectedImageIdx) layerColor = LAYER_ACTIVE_COLOR;
        ImGui::GetWindowDrawList()->AddRectFilled(start, end, layerColor);

        // name of the image
        ImGui::Text(("Image " + std::to_string(i)).c_str());

        // draw the image
        Layer* currentLayer = mp_layerManager->getLayer(i);
        if (currentLayer->hasImage())
        {
            ImGui::SameLine(140);
            ImGui::Image((GLuint*)(intptr_t)currentLayer->getGpuImageHandle(), ImVec2(imageWidth,imageHeight), ImVec2(0,0), ImVec2(1,1));
        }
    }
    ImGui::EndChild();

    // return status
    bool status = true;

    // set image selection status as not selected
    bool validImageSelected = m_selectedImageIdx >= 0;

    // apply button
    if (!validImageSelected)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
    }
    ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::SameLine(5);
    if(ImGui::Button("Apply", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)) && validImageSelected)
    {
        status = false;
        m_callbackFunction(1, m_selectedImageIdx);
    }
    if (!validImageSelected) ImGui::PopStyleColor(3);

    // cancel button
    ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
    if(ImGui::Button("Cancel", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
    {
        status = false;
        m_callbackFunction(-1, m_selectedImageIdx);
    }
    ImGui::End();
    ImGui::PopStyleColor(1);

    return status;
}