#ifndef SRCCMAKE_IMAGEFILTER_H
#define SRCCMAKE_IMAGEFILTER_H

#include <string>

#include "core/scene/Image.h"
#include "core/io/Interactable.h"

class ImageFilter
{
public:
    ImageFilter()
    {
        m_filterType = "cpu filter";
    }

    // Filter information
    std::string getName() { return m_name; }
    std::string getFilterGroup() { return m_filterGroup; }
    std::string getFilterType() { return m_filterType; }

    // IO Images
    void addInputImage(Image* in) { m_inputImages.push_back(in); }
    std::vector<Image*> getOutputImages() { return m_outputImages; }

    // Parameters that can be specified by the user
    InteractableCollection* getInteractableCollection() { return &m_interactableCollection; }

    // clear input and output images
    void clearImages() { m_inputImages.clear(); m_outputImages.clear(); }

    // actually execute the filter
    virtual void process() = 0;

protected:
    std::string m_name = "unnamed";
    std::string m_filterGroup = "default";
    std::string m_filterType = "cpu filter";
    InteractableCollection m_interactableCollection;

    int getNumberOfInputImages() { return m_inputImages.size(); }
    Image* getInputImage(int index) { return m_inputImages[index]; }
    void returnImage(Image* out) { m_outputImages.push_back(out); }

private:
    std::vector<Image*> m_inputImages;
    std::vector<Image*> m_outputImages;
};

#endif //SRCCMAKE_IMAGEFILTER_H
