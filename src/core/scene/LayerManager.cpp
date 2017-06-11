#include "LayerManager.h"

LayerManager::LayerManager()
{
    m_activeLayer = -1;
}
LayerManager::~LayerManager()
{

}

void LayerManager::addLayer()
{
    m_layers.push_back(new Layer(m_layers.size()));
    m_activeLayer = m_layers.size()-1;
}
Layer* LayerManager::getLayer(int index)
{
    if (index >= 0 && index < m_layers.size())
        return m_layers[index];
    return nullptr;
}

void LayerManager::deleteLayer(int index)
{
    bool shouldDecreaseActiveLayerId = m_activeLayer == m_layers.size()-1;
    if (index < m_layers.size()) m_layers.erase(m_layers.begin() + index);
    if(shouldDecreaseActiveLayerId) m_activeLayer--;
}
void LayerManager::deleteActiveLayer()
{
    deleteLayer(m_activeLayer);
}

void LayerManager::setName(std::string name, int layer)
{
    if (layer >= 0 && layer < m_layers.size())
    {
        m_layers.at(layer)->setName(name);
    }
}
void LayerManager::setOpacity(float opacity, int layer)
{
    if (layer >= 0 && layer < m_layers.size())
    {
        m_layers.at(layer)->setOpacity(opacity);
    }
}

int LayerManager::getActiveLayerIndex()
{
    return m_activeLayer;
}
int LayerManager::getNumberOfLayers()
{
    return m_layers.size();
}



void LayerManager::loadImage(std::string filepath, int layerIndex)
{
    if(!m_layers[layerIndex]->hasImage())
    {
        Image* p_image = new Image;
        p_image->load(filepath);
        m_layers[layerIndex]->setImage(p_image);
    }
    else
    {
        m_layers[layerIndex]->getImage()->load(filepath);
    }
}
void LayerManager::setImage(Image* image, int layerIndex)
{
    if (layerIndex >= 0 && layerIndex < m_layers.size())
    {
        m_layers[layerIndex]->setImage(image);
    }
}

bool LayerManager::isValidImageSelected()
{
    return isActiveLayerValid() && m_layers[m_activeLayer]->hasImage();
}

Image* LayerManager::getImageOfActiveLayer()
{
    if(isValidImageSelected())
        return m_layers[m_activeLayer]->getImage();
    throw std::out_of_range("Accessing layers out of range.");
}

Image* LayerManager::getImage(int layerIndex)
{
    if(layerIndex >= 0 && m_layers[layerIndex]->hasImage())
        return m_layers[layerIndex]->getImage();
    return nullptr;
}