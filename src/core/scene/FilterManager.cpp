#include "FilterManager.h"

FilterManager::FilterManager()
{
    m_filterStatus = FilterStatus::READY;
}
FilterManager::~FilterManager()
{

}

void FilterManager::registerImageFilter(ImageFilter* imageFilter)
{
    m_imageFilters.push_back(imageFilter);
    auto filterGroupIt = m_imageGroupFilterIdsMap.find(imageFilter->getFilterGroup());
    if (filterGroupIt == m_imageGroupFilterIdsMap.end())
    {
        std::vector<int> filterIdVector;
        filterIdVector.push_back(m_imageFilters.size()-1);
        m_imageGroupFilterIdsMap[imageFilter->getFilterGroup()] = filterIdVector;
    }
    else
    {
        filterGroupIt->second.push_back(m_imageFilters.size()-1);
    }
}

void FilterManager::execute(ImageFilter* p_imagefilter, Image* inputImage)
{
    std::string filterType = p_imagefilter->getFilterType();
    m_filterStatus = FilterStatus::RUNNING;

    // add input image as first image
    m_inputImages.push_back(inputImage);

    // collect all other images from the interactables
    int numberOfInteractableElements = p_imagefilter->getInteractableCollection()->getNumberOfElements();
    for(int i = 0; i < numberOfInteractableElements; i++)
    {
        Interactable* interactable = p_imagefilter->getInteractableCollection()->getInteractableAt(i);
        if(interactable->m_hasImage)
        {
            m_inputImages.push_back(interactable->m_image);
        }
    }

    // execute gpu filter
    auto action = [](ImageFilter* p_imagefilter) { FilterManager::getInstance().applyFilter(p_imagefilter); };
    if(filterType == "gpu filter")
        action(p_imagefilter);
    // execute cpu filter in a different thread
    else if(filterType == "cpu filter")
        m_imageProcessingThread = new std::thread(action, p_imagefilter);
}

void FilterManager::cleanup()
{
    m_inputImages.clear();
    m_outputImages.clear();
    m_filterStatus = FilterStatus::READY;
}

std::vector<Image*> FilterManager::getOutputImages()
{
    return m_outputImages;
}

void FilterManager::applyFilter(ImageFilter* imageFilter)
{
    for (int i = 0; i < m_inputImages.size(); i++)
    {
        imageFilter->addInputImage(m_inputImages[i]);
    }
    m_timer.getDeltaTime();
    imageFilter->process();

    #ifdef DEBUG
    std::cout << "Filter " << imageFilter->getName() << ": " << std::to_string(m_timer.getDeltaTime()) << std::endl;
    #endif

    m_outputImages = std::vector<Image*>(imageFilter->getOutputImages());

    // filter execution is done
    imageFilter->clearImages();
    m_filterStatus = FilterStatus::DONE;
}