//
// Created by Windrian on 10.03.2017.
//

#include "WindowManager.h"

/*
 * glfw callbacks
 */
void WindowManager::errorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void WindowManager::resizeCallback(GLFWwindow* window, int width, int height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
    glViewport(0,0, width, height);
}

void WindowManager::init(int width, int height)
{
    // set viewport dimensions
    m_viewportWidth = width;
    m_viewportHeight = height;



    //_______________________INIT_GLFW________________________//

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        std::exit(1);
    }
    // set error callback function
    glfwSetErrorCallback([](int error, const char* description) {
        WindowManager::getInstance().errorCallback(error, description);
    });


    // create glfw window
    m_window = glfwCreateWindow(m_viewportWidth, m_viewportHeight, "SImP", NULL, NULL);
    if (!m_window) {
        std::cout << "GLFW Window couldn't be initialized" << std::endl;
        std::exit(1);
    }

    // set glfw context and relevant callbacks
    glfwMakeContextCurrent(m_window);
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowManager::getInstance().resizeCallback(window, width, height);
    });



    //_______________________INIT_GL3W________________________//

    if (gl3wInit()) {
        std::cout << "GLEW couldn't be initialized" << std::endl;
        std::exit(1);
    }

    // check for glew features
    if (!gl3wIsSupported(3,2)) {
        std::cout << "OpenGL 3.2 not supported" << std::endl;
        std::exit(1);
    }



    //___________________OPENGL_FEATURES______________________//
    glClearColor(0.3,0.3,0.3,1.0);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    //_____________________INIT_IMG_UI________________________//
    ImGui_ImplGlfwGL3_Init(m_window, true);
    auto& style = ImGui::GetStyle();
    style.FrameRounding = 0.0f;
    style.WindowRounding = 5.0f;
    style.ScrollbarSize = 15.0f;
    style.WindowFillAlphaDefault = 1.0f;
    style.GrabMinSize = 15.0f;
    style.GrabRounding = 3.0f;
    style.ItemInnerSpacing = ImVec2(8, 6);

    auto controlsColor = ImVec4(0.4f, 0.5f, 0.8f, 1.00f);
    auto controlsColorBright = ImVec4(0.6f, 0.7f, 0.9f, 1.00f);
    auto controlsColorDark = ImVec4(0.4f, 0.5f, 0.8f, 1.00f);

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_Button] = controlsColor;
    style.Colors[ImGuiCol_ButtonHovered] = controlsColorBright;
    style.Colors[ImGuiCol_ButtonActive] = controlsColorDark;
    style.Colors[ImGuiCol_SliderGrab] = controlsColor;
    style.Colors[ImGuiCol_SliderGrabActive] = controlsColorBright;
}

void WindowManager::exit()
{
    ImGui_ImplGlfwGL3_Shutdown();
    glfwSetWindowShouldClose(m_window, true);
}

void WindowManager::render(std::function<void()> renderCallback)
{
    // do the actual rendering
    while (!glfwWindowShouldClose(m_window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // request new frame
        ImGui_ImplGlfwGL3_NewFrame();

        // call render callback function
        renderCallback();

        // finally show gui
        ImGui::Render();

        // swap buffers after new frame is filled successfully
        glfwSwapBuffers(m_window);
        // let glfw check for key events
        glfwPollEvents();
    }

    // destroy window after render function has been exited
    glfwDestroyWindow(m_window);
    glfwTerminate();
}