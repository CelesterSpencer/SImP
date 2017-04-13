#ifndef SRCCMAKE_LAYERMANAGER_H
#define SRCCMAKE_LAYERMANAGER_H

// std
#include <vector>

// project
#include "util/Singleton.h"

#include "scene/Layer.h"
#include "io/FileHandler.h"
#include "scene/SystemFiles.h"

class LayerManager : public Singleton<LayerManager>
{
    friend class Singleton<LayerManager>;
public:
    void drawLayers();
    void drawMenu();

    // layer interaction
    void addLayer();
    Layer* getLayer(int index);
    void deleteLayer(int index);
    void setName(std::string name, int layer);
    void setImage(Image* image, int layer);
    void setOpacity(float opacity, int layer = -1);
    int getActiveLayer();
    int getNumberOfLayers();

    // logic
    void blockInteraction();
    void unblockInteraction();

    // image
    bool isValidImageSelected();
    Image* getImageOfActiveLayer();
    Image* getImage(int layerIndex);

    // layer image selection
    void drawLayerImageSelectionMenu();
    void resetLayerImageSelectionMenu();
    int getLayerImageSelectionStatus();
    int getLayerImageSelectionIndex();

private:
    // drawing
    ShaderProgram*      m_shaderProgram;

    // layers
    std::vector<Layer*> m_layers;
    int                 m_activeLayer;

    // processing logic
    bool                m_isInteractionBlocked;
    int                 m_layerImageSelectionStatus;
    int                 m_selectedLayerImageIndex;

    // button icons
    GLuint              m_imageHandleDelete;
    GLuint              m_imageHandleOpen;

    LayerManager()
    {
        // logic
        m_activeLayer = -1;
        m_isInteractionBlocked = false;
        m_layerImageSelectionStatus = 0;

        // load system images for the image buttons
        m_imageHandleDelete     = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_DELETE);
        m_imageHandleOpen       = SystemFiles::getInstance().getImageHandle(SystemFiles::ICON_OPEN);

        // loading simple image shader
        m_shaderProgram = new ShaderProgram("/simple.vert", "/simple.frag");
    };
    ~LayerManager() {};
};


#endif //SRCCMAKE_LAYERMANAGER_H
