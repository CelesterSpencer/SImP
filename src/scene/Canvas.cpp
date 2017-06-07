#include "Canvas.h"
#include <gtx/rotate_vector.hpp>

Canvas::Canvas()
{
    //__________________LOAD_SYSTEM_IMAGES____________________//

    m_currentActiveLayerIdx = -1;
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

    MenuManager::getInstance().drawMenus();
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
        m_currentActiveLayerIdx = LayerManager::getInstance().getActiveLayer();
        std::cout << FilterManager::getInstance().isActiveFilterValid() << std::endl;
        std::string filterType = FilterManager::getInstance().getActiveFilter()->getFilterType();

        if(filterType == "gpu filter")
        {
            // apply filter for the selected image
            Image* inputImage = LayerManager::getInstance().getImageOfActiveLayer();
            FilterManager::getInstance().addImage(inputImage);
            FilterManager::getInstance().addRequiredImages();
            FilterManager::getInstance().applyFilter();
            m_tempOutputImages = FilterManager::getInstance().getOutputImages();

            // reset input and output images
            FilterManager::getInstance().resetFilter();

            // reset variables to allow further filter applications
            m_isProcessingActive = false;
            m_isImageTransactionDone = false;
        }
        else if(filterType == "cpu filter")
        {
            m_isProcessingActive = true;
            m_isImageTransactionDone = false;
            LayerManager::getInstance().blockInteraction();

            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "Error Test: " << std::to_string(err) << std::endl;
            }

            /*
             * execute filter application in a different thread
             */
            if (validActiveLayer)
            {
                MenuManager::getInstance().showSpinner();
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
    }



    //__________________________________________COPY_OUTPUT_IMAGES_TO_LAYERS__________________________________________//

    if (!m_isProcessingActive && !m_isImageTransactionDone)
    {
        // create new layers for every output image
        for (int i = 0; i < m_tempOutputImages.size(); i++)
        {
            LayerManager::getInstance().addLayer();
            LayerManager::getInstance().setImage(m_tempOutputImages[i], LayerManager::getInstance().getNumberOfLayers()-1);
        }

        // reset temp images
        m_tempOutputImages.clear();

        // finish transaction and unblock layers
        m_isImageTransactionDone = true;
        LayerManager::getInstance().unblockInteraction();
        MenuManager::getInstance().hideSpinner();
    }
}