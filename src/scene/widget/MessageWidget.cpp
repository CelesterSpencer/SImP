#include "MessageWidget.h"

MessageWidget::MessageWidget(std::string text, float lifetime)
{
    m_text = text;
    m_lifetime = lifetime;
}
MessageWidget::~MessageWidget()
{

}

// inherit from Widget
bool MessageWidget::draw(int width, int height, float deltaTime)
{
    // get main window dimensions
    int mainWindowWidth = width;
    int mainWindowHeight = height;
    ImGui::SetWindowSize("Show Message", ImVec2(std::min(ImGui::GetWindowWidth() / 2, 400.f), -1));
    ImGui::SetWindowPos("Show Message",
                        ImVec2(mainWindowWidth  / 2 - ImGui::GetWindowWidth()  / 2,
                               mainWindowHeight / 2 - ImGui::GetWindowHeight() / 2));

    // create new window
    ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
    ImGui::Begin("Show Message", NULL,
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize   |
                 ImGuiWindowFlags_NoMove     |
                 ImGuiWindowFlags_NoTitleBar);
    // show message
    ImGui::TextWrapped(m_text.c_str());

    ImGui::End();
    ImGui::PopStyleColor(1);

    // reduce lifetime
    m_lifetime -= deltaTime;

    return (m_lifetime > 0);
}