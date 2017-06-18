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
    delete m_layers[index];
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
    // get the file name from the path
    auto index = filepath.find_last_of("/\\") + 1;
    std::string fileName = (index < filepath.size()) ? filepath.substr(index) : filepath;
    auto indexFileEnding = fileName.find_last_of(".");
    fileName = (indexFileEnding < fileName.size()) ? fileName.substr(0,indexFileEnding) : fileName;

    // actually loading the data
    std::vector<float> data;
    int width, height, channelCount;
    ImageHandler::getInstance().loadImage(filepath, &data, width, height, channelCount);

    // either create new image or use existing image
    if(!m_layers[layerIndex]->hasImage())
    {
        // create image from provided data
        Image* p_image = new Image;
        p_image->m_fileName = fileName;
        p_image->m_width = width;
        p_image->m_height = height;
        p_image->m_channelNumber = channelCount;
        p_image->m_data = data;
        p_image->m_hasBeenModified = true;
        p_image->m_hasBeenResized = true;

        // add image to layer
        m_layers[layerIndex]->setImage(p_image);
    }
    else
    {
        Image* p_image = m_layers[layerIndex]->getImage();
        p_image->m_fileName = fileName;
        p_image->m_width = width;
        p_image->m_height = height;
        p_image->m_channelNumber = channelCount;
        p_image->m_data = data;
        p_image->m_hasBeenModified = true;
        p_image->m_hasBeenResized = true;
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