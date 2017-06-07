#include "MenuManager.h"

void MenuManager::drawMenus()
{
    std::vector<Menu> newMenus;
    int deltaTMillis = m_timer.getDeltaTime();
    for (int i = 0; i < m_menus.size(); i++)
    {
        if(!m_menus[i].draw(deltaTMillis))
            newMenus.push_back(m_menus[i]);
    }

    m_menus = newMenus;
}

void MenuManager::showYesNoDialog(std::string message, std::function<void(int)> callbackFuntion)
{
    auto func = [=]() -> bool
    {
        bool status = false;

        /*
         * get main window dimensions
         */
        int mainWindowWidth = WindowManager::getInstance().getWidth();
        int mainWindowHeight = WindowManager::getInstance().getHeight();

        /*
         * create new window
         */
        ImGui::SetWindowSize("YesNoDialog", ImVec2(std::min(ImGui::GetWindowWidth()/2, 400.f), -1));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
        ImGui::Begin("YesNoDialog", NULL,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize   |
                     ImGuiWindowFlags_NoMove     |
                     ImGuiWindowFlags_NoTitleBar);
        ImGui::SetWindowPos(  "YesNoDialog",
                              ImVec2(mainWindowWidth /2 - ImGui::GetWindowWidth() /2,
                                     mainWindowHeight/2 - ImGui::GetWindowHeight()/2));

        /*
         * show message
         */
        ImGui::TextWrapped(message.c_str());

        /*
         * show yes button
         */
        ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::SameLine(5);
        if(ImGui::Button("Yes##YesNoDialog", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
        {
            callbackFuntion(1);
            status = true;
        }

        /*
         * show no button
         */
        ImGui::SameLine(ImGui::GetWindowWidth()/2 + 5);
        if(ImGui::Button("No##YesNoDialog", ImVec2(ImGui::GetWindowWidth()/2 - 12, 20)))
        {
            callbackFuntion(0);
            status = true;
        }
        ImGui::End();
        ImGui::PopStyleColor(1);

        return status;
    };
    m_menus.push_back(Menu(func, 0));
}

void MenuManager::showMessage(std::string message, int milliseconds)
{
    std::cout << message << std::endl;
    auto func = [=]() -> bool
    {
        /*
         * get main window dimensions
         */
        int mainWindowWidth = WindowManager::getInstance().getWidth();
        int mainWindowHeight = WindowManager::getInstance().getHeight();

        /*
         * create new window
         */
        ImGui::SetWindowSize("Show Message", ImVec2(std::min(ImGui::GetWindowWidth() / 2, 400.f), -1));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
        ImGui::Begin("Show Message", NULL,
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize   |
                     ImGuiWindowFlags_NoMove     |
                     ImGuiWindowFlags_NoTitleBar);
        ImGui::SetWindowPos("Show Message",
                            ImVec2(mainWindowWidth  / 2 - ImGui::GetWindowWidth()  / 2,
                                   mainWindowHeight / 2 - ImGui::GetWindowHeight() / 2));

        /*
         * show message
         */
        ImGui::TextWrapped(message.c_str());

        ImGui::End();
        ImGui::PopStyleColor(1);

        return false;
    };
    m_menus.push_back(Menu(func, milliseconds));
}

void MenuManager::showSpinner()
{
    if(!m_spinner.isActive())
    {
        m_spinner.show();
        auto func = [=]() -> bool
        {
            m_spinner.draw();
            return !m_spinner.isActive();
        };
        m_menus.push_back(Menu(func, 0));
    }
}
void MenuManager::hideSpinner()
{
    m_spinner.hide();
}