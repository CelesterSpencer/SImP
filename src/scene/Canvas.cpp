//
// Created by Windrian on 11.03.2017.
//

#include "Canvas.h"

Canvas::Canvas()
{
    //_____________________SETUP_MEMBER_______________________//

    m_activeLayer = -1;

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

    ImGui::SetWindowSize( "Layers" , ImVec2(330, ImGui::GetWindowHeight()) );
    ImGui::SetWindowSize( "Filter settings", ImVec2(mainWindowWidth/2, 0));
    ImGui::SetWindowPos(  "Filter settings", ImVec2(mainWindowWidth/4, mainWindowHeight/4));

    bool makeNewLayer = false;
    bool shouldLoadImage = false;

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
        auto layerColor = ImColor(0,0,0,50);
        if (ImGui::GetMousePos().x >= start.x &&
            ImGui::GetMousePos().x < end.x &&
            ImGui::GetMousePos().y > start.y &&
            ImGui::GetMousePos().y < end.y)
        {
            layerColor = ImColor(255, 255, 255, 50);
            if(ImGui::IsMouseClicked(0, false)) m_activeLayer = i;
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
        if(i == m_activeLayer) layerColor = ImColor(155,155,255,70);
        ImGui::GetWindowDrawList()->AddRectFilled(start, end, layerColor);

        // layer name
        ImGui::Text(("Layer " + std::to_string(i)).c_str());

        // layer opacity
        ImGui::SameLine(70);
        float layerOpacity = currentLayer->getOpacity();
        ImGui::PushItemWidth(50);
        ImGui::SliderFloat(("##" + std::to_string(i)).c_str(), &layerOpacity, 0.0, 1.0);
        currentLayer->setOpacity(layerOpacity);

        // image name
        ImGui::SameLine(150);
        std::string imageName = (currentLayer->hasImage()) ? currentLayer->getImage()->getFileName() : "empty";
        if (imageName.length() > 7) imageName = imageName.substr(0, 7) + "...";
        ImGui::Text(imageName.c_str());

        // load image button right aligned
        ImGui::SameLine(ImGui::GetWindowWidth()-85);
        if(ImGui::Button(("Load Image##"+std::to_string(i)).c_str()))
        {
            m_activeLayer = i;
            shouldLoadImage = true;
        }
        ImGui::EndGroup();
        ImGui::Separator();
    }
    ImGui::Spacing();
    ImGui::SameLine(ImGui::GetWindowWidth()/2.0 - 50);
    makeNewLayer = ImGui::Button("New Layer");
    ImGui::End();

    if(shouldLoadImage)
    {
        std::string filePath = "";
        if(FileHandler::getInstance().openFilePathDialog(filePath))
        {
            if (m_activeLayer >= 0)
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
        shouldLoadImage = false;
    }

    if(makeNewLayer)
    {
        addLayer();
    }
}

void Canvas::drawFiltersMenu()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {

            }
            // Disabled item
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
            {

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