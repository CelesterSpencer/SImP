//
// Created by Windrian on 11.03.2017.
//

#include <gtx/rotate_vector.hpp>
#include "Canvas.h"

Canvas::Canvas()
{
    //_____________________SETUP_MEMBER_______________________//

    m_activeLayer = -1;

    //__________________LOAD_SYSTEM_IMAGES____________________//

    m_imageHandleDelete     = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_DELETE);
    m_imageHandleOpen       = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_OPEN);
    m_imageHandleSpinner    = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_SPINNER);

    //_____________________SETUP_SHADER_______________________//

    // gbuffer shader that renders
    m_shaderProgram = new ShaderProgram("/simple.vert", "/simple.frag");
}

Canvas::~Canvas()
{

}

void Canvas::draw()
{
    //_________________________DRAW___________________________//

    for(int i = 0; i < m_layers.size(); i++)
    {
        m_layers[i]->render(m_shaderProgram);
    }

    //_________________________GUI____________________________//

    drawLayersMenu();
    drawFiltersMenu();
}

void Canvas::drawLayersMenu()
{
    int mainWindowWidth = WindowManager::getInstance().getWidth();
    int mainWindowHeight = WindowManager::getInstance().getHeight();

    // set window dimension and position
    ImGui::SetWindowSize( "Layers" , ImVec2(250, ImGui::GetWindowHeight()) );
    if(WindowManager::getInstance().hasBeenResized()) ImGui::SetWindowPos("Layers", ImVec2(mainWindowWidth-260, mainWindowHeight/4));
    ImGui::SetWindowSize( "Filter settings", ImVec2(mainWindowWidth/2, 0));


    // variables that store the result of the buttons
    bool shouldDeleteLayer  = false;
    bool makeNewLayer       = false;
    bool shouldLoadImage    = false;

    int numberOfLayers = m_layers.size();

    ImGui::Begin("Layers", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    for (int i = 0; i < numberOfLayers; i++)
    {
        ImGui::BeginGroup();
        Layer* currentLayer = m_layers[i];

        /*
         * draw background for selected element
         */
        ImVec2 start = ImVec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y + 24 + i*27 - ImGui::GetScrollY());
        ImVec2 end = ImVec2(start.x + ImGui::GetWindowWidth(), start.y + 26);
        auto layerColor = LAYER_COLOR;
        if (ImGui::GetMousePos().x >= start.x &&
            ImGui::GetMousePos().x < end.x &&
            ImGui::GetMousePos().y > start.y &&
            ImGui::GetMousePos().y < end.y)
        {
            layerColor = LAYER_HOVER_COLOR;
            if(ImGui::IsMouseClicked(0, false)) m_activeLayer = i;
            /*
             * select current layer and hide all other layers when double clicked
             */
            if(ImGui::IsMouseDoubleClicked(0))
            {
                for(int layerIdx = 0; layerIdx < m_layers.size(); layerIdx++)
                {
                    if(layerIdx != i)
                    {
                        m_layers[layerIdx]->setOpacity(0);
                    }
                    else {
                        m_layers[layerIdx]->setOpacity(1);
                    }
                }
            }
        }
        if(i == m_activeLayer) layerColor = LAYER_ACTIVE_COLOR;
        ImGui::GetWindowDrawList()->AddRectFilled(start, end, layerColor);

        /*
         * layer name
         */
        ImGui::AlignFirstTextHeightToWidgets();
        ImGui::Text(("Layer " + std::to_string(i)).c_str());

        /*
         * layer opacity
         */
        ImGui::SameLine(70);
        float layerOpacity = currentLayer->getOpacity();
        ImGui::PushItemWidth(50);
        ImGui::SliderFloat(("##" + std::to_string(i)).c_str(), &layerOpacity, 0.0, 1.0);
        currentLayer->setOpacity(layerOpacity);

        /*
         * image
         */
        ImGui::SameLine(140);
        if (currentLayer->hasImage())
            ImGui::Image((GLuint*)currentLayer->getGpuImageHandle(), ImVec2(19,19), ImVec2(0,0), ImVec2(1,1));
        else
        {
            ImVec2 imageStart = ImVec2(ImGui::GetCursorPos().x+ImGui::GetWindowPos().x, ImGui::GetCursorPos().y+ImGui::GetWindowPos().y - ImGui::GetScrollY());
            ImVec2 imageEnd   = ImVec2(imageStart.x+19, imageStart.y+19);
            ImGui::GetWindowDrawList()->AddRectFilled(imageStart, imageEnd, NO_IMAGE_COLOR);
        }

        /*
         * load-image button
         */
        ImGui::SameLine(170);
        ImGui::PushID(("imageBtn"+std::to_string(i)).c_str());
        // color button gray when inactive
        if (m_isProcessingActive)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
        }
        if(ImGui::ImageButton((ImTextureID)m_imageHandleOpen, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1), 0)
           && !m_isProcessingActive)
        {
            m_activeLayer = i;
            shouldLoadImage = true;
        }
        if (m_isProcessingActive) ImGui::PopStyleColor(3);
        ImGui::PopID();

        if (currentLayer->hasImage())
        {
            std::string channelInfo = "";
            switch(currentLayer->getImage()->getChannelNumber())
            {
                case 1:
                    channelInfo = "G";
                    break;
                case 3:
                    channelInfo = "RGB";
                    break;
                case 4:
                    channelInfo = "RGBa";
                    break;
                default:
                    channelInfo = "N/A";
                    break;
            }
            float charWidth = 5.0;
            ImGui::SameLine(203-channelInfo.size()/2.0*charWidth);
            ImGui::Text(channelInfo.c_str());
        }

        /*
         * delete layer button
         */
        ImGui::SameLine(ImGui::GetWindowWidth()-28);
        ImGui::PushID(("deleteLayerBtn"+std::to_string(i)).c_str());
        if (m_isProcessingActive)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
        }
        if(ImGui::ImageButton((ImTextureID)m_imageHandleDelete, ImVec2(19,19), ImVec2(0,0), ImVec2(1,1), 0)
           && !m_isProcessingActive)
        {
            shouldDeleteLayer = true;
        }
        if (m_isProcessingActive) ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::EndGroup();
        ImGui::Separator();
    }
    ImGui::Spacing();
    ImGui::SameLine(ImGui::GetWindowWidth()/2.0 - 35);
    if (m_isProcessingActive)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, INACTIVE_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, INACTIVE_COLOR);
    }
    if(ImGui::Button("New Layer") && !m_isProcessingActive)
    {
        makeNewLayer = true;
    }
    if (m_isProcessingActive) ImGui::PopStyleColor(3);
    ImGui::End();

    if(shouldLoadImage)
    {
        std::string filePath = "";
        if(FileHandler::getInstance().openFilePathDialog(filePath))
        {
            if (m_activeLayer >= 0 && filePath.length() > 0)
            {
                if(!m_layers[m_activeLayer]->hasImage())
                {
                    Image* p_image = new Image;
                    p_image->load(filePath);
                    m_layers[m_activeLayer]->setImage(p_image);
                }
                else
                {
                    m_layers[m_activeLayer]->getImage()->load(filePath);
                }
            }
        }
    }

    if(makeNewLayer)
    {
        addLayer();
    }

    if(shouldDeleteLayer)
    {
        bool shouldDecreaseActiveLayerId = m_activeLayer == m_layers.size()-1;
        deleteLayer(m_activeLayer);
        if(shouldDecreaseActiveLayerId) m_activeLayer--;
    }
}

void Canvas::drawFiltersMenu()
{
    int mainWindowWidth = WindowManager::getInstance().getWidth();
    int mainWindowHeight = WindowManager::getInstance().getHeight();

    bool validActiveLayer = false;
    static bool spinnerActive = false;
    int status = -1;


    //_______________________________________________DRAW_MENU_BAR____________________________________________________//

    ImGui::PushStyleColor(ImGuiCol_WindowBg, WINDOW_CONTENT_COLOR_O);
    if (ImGui::BeginMainMenuBar())
    {
        // draw main settings
        if (ImGui::BeginMenu("File"))
        {
            // Disabled item
            bool validImage = m_activeLayer >= 0 && m_layers[m_activeLayer]->hasImage();
            if (ImGui::MenuItem("Save", "CTRL+S", false, validImage))
            {
                std::cout << "Called Save" << std::endl;
                m_layers[m_activeLayer]->getImage()->save();
            }
            ImGui::Separator();
            if(ImGui::MenuItem("Exit"))
            {
                WindowManager::getInstance().exit();
            }

            ImGui::EndMenu();
        }

        // draw filters
        validActiveLayer = m_activeLayer >= 0 && m_layers[m_activeLayer]->hasImage();
        status = FilterManager::getInstance().drawFilterMenu(validActiveLayer, m_isProcessingActive);

        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleColor(1);


    //__________________________________________APPLY_SELECTED_FILTER_________________________________________________//

    if (status == 1 && !m_isProcessingActive) // user selected Apply
    {
        m_isProcessingActive = true;
        m_isImageTransactionDone = false;
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
                FilterManager::getInstance().applyFilter();
                *outputImages = FilterManager::getInstance().getOutputImages();

                // reset input and output images
                FilterManager::getInstance().resetFilter();

                // reset variables to allow further filter applications
                *p_spinnerActive = false;
                *p_processingActive = false;
            },&m_tempOutputImages, m_layers[m_activeLayer]->getImage(), &spinnerActive, &m_isProcessingActive
            );
        } else
        {
            m_isProcessingActive = false;
        }
    }

    // copy the result data to the layers
    if (!m_isProcessingActive && !m_isImageTransactionDone)
    {
        Image* out = m_tempOutputImages[0];
        if (out != nullptr) setImage(out, m_activeLayer);

        // create new layers for every other image
        for (int i = 1; i < m_tempOutputImages.size(); i++)
        {
            addLayer();
            setImage(m_tempOutputImages[i], m_layers.size()-1);
        }

        // reset temp images
        m_tempOutputImages.clear();

        m_isImageTransactionDone = true;
    }


    //_____________________________________________DRAW_SPINNER___________________________________________________//

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
        draw_list->PushTextureID((GLuint*)m_imageHandleSpinner);
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

void Canvas::addLayer()
{
    m_layers.push_back(new Layer(m_layers.size()));
    m_activeLayer = m_layers.size()-1;
}

void Canvas::deleteLayer(int index)
{
    if (index < m_layers.size()) m_layers.erase(m_layers.begin() + index);
}

void Canvas::setName(std::string name, int layer)
{
    if (layer >= 0 && layer < m_layers.size())
    {
        m_layers.at(layer)->setName(name);
    }
}

void Canvas::setImage(Image* image, int layer)
{
    if (layer >= 0 && layer < m_layers.size())
    {
        m_layers[layer]->setImage(image);
    }
}

void Canvas::setOpacity(float opacity, int layer)
{
    if (layer >= 0 && layer < m_layers.size())
    {
        m_layers.at(layer)->setOpacity(opacity);
    }
}