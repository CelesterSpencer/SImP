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



/**
 * The OpenGLManager is creating and handling the Window and glfw user interaction
 */
class WindowManager {
private:
    GLFWwindow* m_window;
    int m_viewportWidth;
    int m_viewportHeight;

    WindowManager() {}; // constructor call is not allowed

    /*
     * glfw callbacks
     */
    void errorCallback(int error, const char* description);
    void resizeCallback(GLFWwindow* window, int width, int height);
public:
    static WindowManager& getInstance()
    {
        static WindowManager instance;
        return instance;
    }
    WindowManager(WindowManager const&)  = delete;
    void operator=(WindowManager const&) = delete;

    GLFWwindow* getWindow() { return m_window; };
    int getWidth() { return m_viewportWidth; };
    int getHeight() { return m_viewportHeight; };

    void init(int width = 1024, int height = 768);
    void exit();
    void render(std::function<void()> renderCallback);
};


#endif //SRCCMAKE_WINDOWMANAGER_H
