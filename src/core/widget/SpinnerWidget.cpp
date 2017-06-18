#include "SpinnerWidget.h"

SpinnerWidget::SpinnerWidget()
{
    m_isActive = false;
    m_imageHandleSpinner = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_SPINNER);
}
SpinnerWidget::~SpinnerWidget()
{

}

bool SpinnerWidget::draw(int width, int height, float deltaTime)
{
    if(!m_isActive) return true;

    // window dimensions
    int mainWindowWidth = width;
    int mainWindowHeight = height;
    float windowWidth = 100;
    float windowHeight= 100;

    ImGui::SetNextWindowPos(ImVec2(mainWindowWidth/2.0-windowWidth/2.0f,
                                   mainWindowHeight/2.0-windowHeight/2.0f));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::Begin("LoadingSpinner", NULL,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove);

    static float angle = 0;
    auto draw_list = ImGui::GetWindowDrawList();
    glm::vec2 offset = glm::vec2(ImGui::GetWindowPos().x+windowWidth/2,
                                 ImGui::GetWindowPos().y+windowHeight/2);
    float radius = 50;
    glm::vec2 v1 = glm::vec2(radius*cos(angle),       radius*sin(angle))        + offset;
    glm::vec2 v2 = glm::vec2(radius*cos(angle+PI/2),  radius*sin(angle+PI/2))   + offset;
    glm::vec2 v3 = glm::vec2(radius*cos(angle+PI),    radius*sin(angle+PI))     + offset;
    glm::vec2 v4 = glm::vec2(radius*cos(angle+3*PI/2),radius*sin(angle+3*PI/2)) + offset;
    draw_list->PushTextureID((GLuint*)(intptr_t)m_imageHandleSpinner);
    draw_list->PrimReserve(6, 6);
    draw_list->PrimVtx(ImVec2(v1.x,v1.y),ImVec2(0,1),0xFFFFFFFF);
    draw_list->PrimVtx(ImVec2(v2.x,v2.y),ImVec2(0,0),0xFFFFFFFF);
    draw_list->PrimVtx(ImVec2(v3.x,v3.y),ImVec2(1,0),0xFFFFFFFF);
    draw_list->PrimVtx(ImVec2(v4.x,v4.y),ImVec2(1,1),0xFFFFFFFF);
    draw_list->PrimVtx(ImVec2(v1.x,v1.y),ImVec2(0,1),0xFFFFFFFF);
    draw_list->PrimVtx(ImVec2(v3.x,v3.y),ImVec2(1,0),0xFFFFFFFF);
    draw_list->PopTextureID();
    float T = 2*PI;
    angle += T / 2 / ImGui::GetIO().Framerate;
    if(angle >= T) angle = angle-T;

    ImGui::End();

    return true;
}