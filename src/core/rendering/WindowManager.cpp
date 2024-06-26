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
    m_hasBeenResized = true;
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

    // select opengl version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // create glfw window
    m_window = glfwCreateWindow(m_viewportWidth, m_viewportHeight, "SImP", NULL, NULL);
    if (!m_window) {
        std::cerr << "GLFW Window couldn't be initialized" << std::endl;
        std::exit(1);
    }

    // set glfw context and relevant callbacks
    glfwMakeContextCurrent(m_window);
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        WindowManager::getInstance().resizeCallback(window, width, height);
    });



    //_______________________INIT_GLAD________________________//

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(1);
    }

    //___________________OPENGL_FEATURES______________________//
    glClearColor(0.3,0.3,0.3,1.0);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //_____________________INIT_IMG_UI________________________//
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    auto& style = ImGui::GetStyle();

    style.FrameRounding                     = 0.0f;
    style.WindowRounding                    = 5.0f;
    style.ScrollbarSize                     = 5.0f;
    style.Colors[ImGuiCol_WindowBg].w       = 1.0f;
    style.GrabMinSize                       = 15.0f;
    style.GrabRounding                      = 3.0f;
    style.ItemInnerSpacing                  = ImVec2(8, 6);

    // window header
    style.Colors[ImGuiCol_TitleBg]          = HEADER_COLOR;
    style.Colors[ImGuiCol_TitleBgActive]    = HEADER_ACTIVE_COLOR;
    style.Colors[ImGuiCol_HeaderHovered]    = HEADER_ACTIVE_COLOR;

    // window body
    style.Colors[ImGuiCol_WindowBg]         = WINDOW_CONTENT_COLOR;
    style.Colors[ImGuiCol_MenuBarBg]        = WINDOW_CONTENT_COLOR;

    style.Colors[ImGuiCol_ScrollbarBg]      = INVISIBLE_COLOR;

    // controls
    style.Colors[ImGuiCol_Button]           = CONTROLS_NORMAL_COLOR;
    style.Colors[ImGuiCol_ButtonHovered]    = CONTROLS_HOVER_COLOR;
    style.Colors[ImGuiCol_ButtonActive]     = CONTROLS_ACTIVE_COLOR;
    style.Colors[ImGuiCol_SliderGrab]       = CONTROLS_NORMAL_COLOR;
    style.Colors[ImGuiCol_SliderGrabActive] = CONTROLS_ACTIVE_COLOR;
    style.Colors[ImGuiCol_FrameBg]          = INACTIVE_COLOR;
    style.Colors[ImGuiCol_FrameBgActive]    = INACTIVE_COLOR;
    style.Colors[ImGuiCol_CheckMark]        = CONTROLS_ACTIVE_COLOR;

    style.Colors[ImGuiCol_Text]             = TEXT_COLOR;
}

void WindowManager::exit()
{
    glfwSetWindowShouldClose(m_window, true);
}

void WindowManager::render(std::function<void()> renderCallback)
{
    // do the actual rendering
    while (!glfwWindowShouldClose(m_window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // request new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // call render callback function
        renderCallback();

        // finally show gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers after new frame is filled successfully
        glfwSwapBuffers(m_window);
        // let glfw check for key events
        glfwPollEvents();
    }

    // destroy window after render function has been exited
    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}