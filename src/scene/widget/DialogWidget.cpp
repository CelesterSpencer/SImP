#include "DialogWidget.h"

DialogWidget::DialogWidget(std::string message, std::function<void(int)> callbackFuntion)
{
    m_message = message;
    m_callbackFunction = callbackFuntion;
}
DialogWidget::~DialogWidget()
{

}

// inherit from Widget
bool DialogWidget::draw(int width, int height, float deltaTime)
{
    bool status = true;

    // get main window dimensions
    int mainWindowWidth = width;
    int mainWindowHeight = height;
    ImGui::SetWindowSize("YesNoDialog", ImVec2(std::min(ImGui::GetWindowWidth()/2, 400.f), -1));
    ImGui::SetWindowPos(  "YesNoDialog",
                          ImVec2(mainWindowWidth /2 - ImGui::GetWindowWidth() /2,
                                 mainWindowHeight/2 - ImGui::GetWindowHeight()/2));

    // create new window
    ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
    ImGui::Begin("YesNoDialog", NULL,
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize   |
                 ImGuiWindowFlags_NoMove     |
                 ImGuiWindowFlags_NoTitleBar);
    // show message
    ImGui::TextWrapped(m_message.c_str());

    // show yes button
    ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::SameLine(5);
    if(ImGui::Button("Yes##YesNoDialog", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
    {
        m_callbackFunction(1);
        status = false;
    }

    // show no button
    ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
    if(ImGui::Button("No##YesNoDialog", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
    {
        m_callbackFunction(0);
        status = false;
    }
    ImGui::End();
    ImGui::PopStyleColor(1);

    return status;
}