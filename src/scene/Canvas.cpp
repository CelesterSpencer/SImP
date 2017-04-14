//
// Created by Windrian on 11.03.2017.
//

#include "Canvas.h"
#include <gtx/rotate_vector.hpp>

Canvas::Canvas()
{
    //__________________LOAD_SYSTEM_IMAGES____________________//

    m_imageHandleSpinner    = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_SPINNER);

    // for testing
    LayerManager::getInstance().addLayer();
    LayerManager::getInstance().addLayer();
    LayerManager::getInstance().addLayer();
    LayerManager::getInstance().addLayer();
    LayerManager::getInstance().addLayer();
    LayerManager::getInstance().addLayer();
    Image* testImage1 = new Image;
    Image* testImage2 = new Image;
    Image* testImage3 = new Image;
    Image* testImage4 = new Image;
    Image* testImage5 = new Image;
    Image* testImage6 = new Image;
    testImage1->load(RESOURCES_PATH"/system/cross.png");
    testImage2->load(RESOURCES_PATH"/system/open.png");
    testImage3->load(RESOURCES_PATH"/system/spinner.png");
    testImage4->load(RESOURCES_PATH"/images/bug.jpg");
    testImage5->load(RESOURCES_PATH"/images/test.png");
    testImage6->load(RESOURCES_PATH"/images/square.png");
    LayerManager::getInstance().setImage(testImage1, 0);
    LayerManager::getInstance().setImage(testImage2, 1);
    LayerManager::getInstance().setImage(testImage3, 2);
    LayerManager::getInstance().setImage(testImage4, 3);
    LayerManager::getInstance().setImage(testImage5, 4);
    LayerManager::getInstance().setImage(testImage6, 5);
}

Canvas::~Canvas()
{

}

void Canvas::draw()
{
    //_________________________DRAW___________________________//

    LayerManager::getInstance().drawLayers();

    //_________________________GUI____________________________//

    LayerManager::getInstance().drawMenu();
    drawFiltersMenu();
}

void Canvas::drawFiltersMenu()
{
    int mainWindowWidth = WindowManager::getInstance().getWidth();
    int mainWindowHeight = WindowManager::getInstance().getHeight();

    bool validActiveLayer = LayerManager::getInstance().isValidImageSelected();
    static bool spinnerActive = false;
    int status = -1;


    //_______________________________________________DRAW_MENU_BAR____________________________________________________//

    ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
    if (ImGui::BeginMainMenuBar())
    {
        /*
         * draw main settings
         */
        if (ImGui::BeginMenu("File"))
        {
            // Only save when a layer with an image is selected
            if (ImGui::MenuItem("Save", "CTRL+S", false, LayerManager::getInstance().isValidImageSelected()))
            {
                std::cout << "Called Save" << std::endl;
                LayerManager::getInstance().getImageOfActiveLayer()->save();
            }
            ImGui::Separator();
            if(ImGui::MenuItem("Exit"))
            {
                WindowManager::getInstance().exit();
            }

            ImGui::EndMenu();
        }

        /*
         * draw list of all available filters
         */
        status = FilterManager::getInstance().drawFilterMenu(LayerManager::getInstance().isValidImageSelected(), m_isProcessingActive);

        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleColor(1);


    //__________________________________________APPLY_SELECTED_FILTER_________________________________________________//

    if (status == 1 && !m_isProcessingActive) // user selected Apply
    {
        // start processing and block layers
        m_isProcessingActive = true;
        m_isImageTransactionDone = false;
        LayerManager::getInstance().blockInteraction();

        if (validActiveLayer)
        {
            spinnerActive = true;
            m_imageProcessingThread = new std::thread([](
                    std::vector<Image*>* outputImages,
                    Image* p_in,
                    bool* p_spinnerActive,
                    bool* p_processingActive)
            {
                // apply filter for the selected image
                FilterManager::getInstance().addImage(p_in);
                FilterManager::getInstance().addRequiredImages();
                FilterManager::getInstance().applyFilter();
                *outputImages = FilterManager::getInstance().getOutputImages();

                // reset input and output images
                FilterManager::getInstance().resetFilter();

                // reset variables to allow further filter applications
                *p_spinnerActive = false;
                *p_processingActive = false;
            },&m_tempOutputImages, LayerManager::getInstance().getImageOfActiveLayer(), &spinnerActive, &m_isProcessingActive
            );
        } else
        {
            m_isProcessingActive = false;
        }
    }

    /*
     * copy the result data to the layers
     */
    if (!m_isProcessingActive && !m_isImageTransactionDone)
    {
        Image* out = m_tempOutputImages[0];
        if (out != nullptr) LayerManager::getInstance().setImage(out, LayerManager::getInstance().getActiveLayer());

        // create new layers for every other image
        for (int i = 1; i < m_tempOutputImages.size(); i++)
        {
            LayerManager::getInstance().addLayer();
            LayerManager::getInstance().setImage(m_tempOutputImages[i], LayerManager::getInstance().getNumberOfLayers()-1);
        }

        // reset temp images
        m_tempOutputImages.clear();

        // finish transaction and unblock layers
        m_isImageTransactionDone = true;
        LayerManager::getInstance().unblockInteraction();
    }


    //________________________________________________DRAW_SPINNER____________________________________________________//

    if(spinnerActive)
    {
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
        draw_list->PrimVtx(ImVec2(v1.x,v1.y),ImVec2(1,0),0xFFFFFFFF);
        draw_list->PrimVtx(ImVec2(v2.x,v2.y),ImVec2(0,0),0xFFFFFFFF);
        draw_list->PrimVtx(ImVec2(v3.x,v3.y),ImVec2(0,1),0xFFFFFFFF);
        draw_list->PrimVtx(ImVec2(v4.x,v4.y),ImVec2(1,1),0xFFFFFFFF);
        draw_list->PrimVtx(ImVec2(v1.x,v1.y),ImVec2(1,0),0xFFFFFFFF);
        draw_list->PrimVtx(ImVec2(v3.x,v3.y),ImVec2(0,1),0xFFFFFFFF);
        draw_list->PopTextureID();
        float T = 2*PI;
        angle += T / 2 / ImGui::GetIO().Framerate;
        if(angle >= T) angle = angle-T;

        ImGui::End();
    }
}