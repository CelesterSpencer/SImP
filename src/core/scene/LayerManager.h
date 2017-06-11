#ifndef SRCCMAKE_LAYERMANAGER_H
#define SRCCMAKE_LAYERMANAGER_H

// std
#include <vector>
#include <stdexcept>

// project
#include "core/io/FileHandler.h"
#include "core/io/SystemFiles.h"
#include "Layer.h"

class LayerManager
{
public:
    LayerManager();
    ~LayerManager();

    // layer interaction
    void addLayer();
    Layer* getLayer(int index);
    void deleteLayer(int index);
    void deleteActiveLayer();
    // layer properties
    void setName(std::string name, int layer);
    void setOpacity(float opacity, int layer = -1);
    // layer info
    void setActiveLayerIndex(int layer) { m_activeLayer = layer; }
    int getActiveLayerIndex();
    int getNumberOfLayers();
    bool isActiveLayerValid() { return m_activeLayer >= 0 && m_activeLayer < m_layers.size(); };

    // image
    void loadImage(std::string filepath, int layerIndex);
    Image* getImage(int layerIndex);
    void setImage(Image* image, int layerIndex);
    bool isValidImageSelected();
    Image* getImageOfActiveLayer();

private:
    // layers
    std::vector<Layer*> m_layers;
    int                 m_activeLayer;
};

#endif //SRCCMAKE_LAYERMANAGER_H
