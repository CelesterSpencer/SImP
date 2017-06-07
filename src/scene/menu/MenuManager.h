#ifndef SRCCMAKE_MENUMANAGER_H
#define SRCCMAKE_MENUMANAGER_H

// std
#include <string>
#include <vector>
#include <functional>

// lib
#include "imgui.h"

// project
#include "util/Singleton.h"
#include "rendering/WindowManager.h"
#include "rendering/ColorStyles.h"
#include "util/Timer.h"
#include "SpinnerWidget.h"

class MenuManager : public Singleton<MenuManager>
{
    friend class Singleton<MenuManager>;
public:
    struct Menu
    {
        Menu(std::function<bool()> drawFunction, int lifeTime)
        {
            m_drawFunction = drawFunction;
            m_lifeTime = lifeTime;
            m_time = 0;
        }

        bool draw(int deltaTime)
        {
            m_time += deltaTime;
            bool status = false;
            if(m_lifeTime > 0 && m_time > m_lifeTime)
            {
                status = true;
            }
            status |= m_drawFunction();
            return status;
        }

        int m_lifeTime;
        int m_time;
        std::function<bool()> m_drawFunction;
    };

    /**
     * Drawing all specified menus
     */
    void drawMenus();

    /**
     * Shows a yes/no dialog with the specified message.
     * Pressing yes returns 1, while pressing no returns 0.
     * While nothing is pressed -1 is returned.
     */
    void showYesNoDialog(std::string message, std::function<void(int)> callbackFunction);

    /**
     * simply shows the given message
     */
    void showMessage(std::string message, int milliseconds);

    /**
     * Launches a spinner
     */
    void showSpinner();
    void hideSpinner();

private:
    Timer m_timer;
    std::vector<Menu> m_menus;
    SpinnerWidget m_spinner;
};


#endif //SRCCMAKE_MENUMANAGER_H
