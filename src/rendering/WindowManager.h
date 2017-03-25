#ifndef SRCCMAKE_WINDOWMANAGER_H
#define SRCCMAKE_WINDOWMANAGER_H

// standard includes
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <functional>

// glew, include it before gl.h and glfw.h
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>

// imgui
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

// local includes
#include "util/Singleton.h"
#include "rendering/ColorStyles.h"



/**
 * The OpenGLManager is creating and handling the Window and glfw user interaction
 */
class WindowManager : public Singleton<WindowManager>
{
    friend class Singleton<WindowManager>;
public:
    GLFWwindow* getWindow() { return m_window; };
    int getWidth() { return m_viewportWidth; };
    int getHeight() { return m_viewportHeight; };
    bool hasBeenResized()
    {
        bool result = m_hasBeenResized;
        m_hasBeenResized = false;
        return result;
    }

    void init(int width = 1024, int height = 768);
    void exit();
    void render(std::function<void()> renderCallback);
private:
    GLFWwindow* m_window;
    int m_viewportWidth;
    int m_viewportHeight;
    bool m_hasBeenResized = true;

    WindowManager() {};
    ~WindowManager() {};

    /*
     * glfw callbacks
     */
    void errorCallback(int error, const char* description);
    void resizeCallback(GLFWwindow* window, int width, int height);
};


#endif //SRCCMAKE_WINDOWMANAGER_H
