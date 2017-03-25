//
// Created by Windrian on 11.03.2017.
//

#include "Canvas.h"

Canvas::Canvas()
{
    //_____________________SETUP_MEMBER_______________________//

    m_activeLayer = -1;

    //__________________LOAD_SYSTEM_IMAGES____________________//

    loadSystemImages();

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

        // draw background for selected element
        float scrollY = ImGui::GetScrollY();
        ImVec2 start = ImVec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y + 24 + i*27 - scrollY);
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

        // layer name
        ImGui::AlignFirstTextHeightToWidgets();
        ImGui::Text(("Layer " + std::to_string(i)).c_str());

        // layer opacity
        ImGui::SameLine(70);
        float layerOpacity = currentLayer->getOpacity();
        ImGui::PushItemWidth(50);
        ImGui::SliderFloat(("##" + std::to_string(i)).c_str(), &layerOpacity, 0.0, 1.0);
        currentLayer->setOpacity(layerOpacity);

        // image
        ImGui::SameLine(140);
        if (currentLayer->hasImage())
            ImGui::Image((ImTextureID*)currentLayer->getGpuImageHandle(), ImVec2(19,19), ImVec2(0,1), ImVec2(1,0));
        else
        {
            ImVec2 imageStart = ImVec2(ImGui::GetCursorPos().x+ImGui::GetWindowPos().x, ImGui::GetCursorPos().y+ImGui::GetWindowPos().y);
            ImVec2 imageEnd   = ImVec2(imageStart.x+19, imageStart.y+19);
            ImGui::GetWindowDrawList()->AddRectFilled(imageStart, imageEnd, NO_IMAGE_COLOR);
        }

        // load image button
        ImGui::SameLine(170);
        ImGui::PushID(("imageBtn"+std::to_string(i)).c_str());
        if(ImGui::ImageButton((ImTextureID*)m_imageHandleOpen, ImVec2(19,19), ImVec2(0,1), ImVec2(1,0), 0))
        {
            m_activeLayer = i;
            shouldLoadImage = true;
        }
        ImGui::PopID();

        // delete layer button
        ImGui::SameLine(ImGui::GetWindowWidth()-28);
        ImGui::PushID(("deleteLayerBtn"+std::to_string(i)).c_str());
        if(ImGui::ImageButton((GLuint*)m_imageHandleDelete, ImVec2(19,19), ImVec2(0,1), ImVec2(1,0), 0))
        {
            shouldDeleteLayer = true;
        }
        ImGui::PopID();

        ImGui::EndGroup();

        ImGui::Separator();
    }
    ImGui::Spacing();
    ImGui::SameLine(ImGui::GetWindowWidth()/2.0 - 35);
    makeNewLayer = ImGui::Button("New Layer");
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
        deleteLayer(m_activeLayer);
    }
}

void Canvas::drawFiltersMenu()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File"))
        {
            // Disabled item
            if (ImGui::MenuItem("Save", "CTRL+S", false, false))
            {
                std::cout << "Called Save" << std::endl;
                std::cout << "But it's not implemented yet" << std::endl;
            }
            ImGui::Separator();
            if(ImGui::MenuItem("Exit"))
            {
                WindowManager::getInstance().exit();
            }
            ImGui::EndMenu();
        }
        bool validActiveLayer = m_activeLayer >= 0 && m_layers[m_activeLayer]->hasImage();
        int status = FilterManager::getInstance().drawFilterMenu(validActiveLayer);
        if (status == 1) // user selected Apply
        {
            if (validActiveLayer)
            {
                Image* in = m_layers[m_activeLayer]->getImage();
                Image* out = FilterManager::getInstance().applyFilter(in);
                if (out != nullptr) m_layers[m_activeLayer]->setImage(out);
            }
        }
        ImGui::EndMainMenuBar();
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
        //delete m_layers[layer];
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

void Canvas::loadSystemImages()
{
    Image openImage;
    Image deleteImage;

    std::string systemPath = RESOURCES_PATH"/system/";

    openImage.load(systemPath+"open4.png");
    deleteImage.load(systemPath+"cross3.png");

    uploadImage(&openImage, &m_imageHandleOpen);
    uploadImage(&deleteImage, &m_imageHandleDelete);
}

void Canvas::uploadImage(Image* image, GLuint* imageHandle)
{
    /*
         * create new texture
         */
    glGenTextures(1, imageHandle);

    /*
     * bind the texture for uploading data
     */
    glBindTexture(GL_TEXTURE_2D, *imageHandle);

    /*
     * send image data to the new texture
     */
    switch(image->getChannelNumber())
    {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RED, image->getWidth(), image->getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, image->getRawData());
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getRawData());
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getRawData());
            break;
        default:
            std::cerr << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getRawData());
            break;
    }

    /*
     * texture settings
     */
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /*
     * unbind texture
     */
    glBindTexture(GL_TEXTURE_2D, 0);
}