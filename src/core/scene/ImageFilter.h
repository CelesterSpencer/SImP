#ifndef SRCCMAKE_IMAGEFILTER_H
#define SRCCMAKE_IMAGEFILTER_H

#include <string>

#include "core/scene/Image.h"
#include "core/io/Interactable.h"

class ImageFilter
{
    friend class ImageFilterGpu;
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
    void init(std::string filterName, std::string filterGroupName)
    {
        m_name = filterName;
        m_filterGroup = filterGroupName;
    }

    void addUserInput(std::string text,
                      std::function<bool()> isEnabledFunction = nullptr)
    {
        m_interactableCollection.addInteractable(new ImageSelector(text, isEnabledFunction));
    }
    void addUserInput(std::string text, bool* val,
                      std::function<bool()> isEnabledFunction = nullptr)
    {
        m_interactableCollection.addInteractable(new Checkbox(text, val, "", isEnabledFunction));
    }
    void addUserInput(std::string text, int* selectedOption, std::vector<std::string>* options,
                      std::function<bool()> isEnabledFunction = nullptr)
    {
        m_interactableCollection.addInteractable(
                new SelectBox(text, selectedOption, options, "", isEnabledFunction)
        );
    }
    void addUserInput(std::string text, int* val, int* min, int* max,
                      std::function<bool()> isEnabledFunction = nullptr)
    {
        m_interactableCollection.addInteractable(new ISlider(text, val, min, max, "", isEnabledFunction));
    }
    void addUserInput(std::string text, float* val, float* min, float* max,
                      std::function<bool()> isEnabledFunction = nullptr)
    {
        m_interactableCollection.addInteractable(new FSlider(text, val, min, max, "", isEnabledFunction));
    }


    int getNumberOfInputImages() { return m_inputImages.size(); }
    Image* getInputImage(int index) { return m_inputImages[index]; }
    void returnImage(Image* out) { m_outputImages.push_back(out); }

private:
    std::vector<Image*> m_inputImages;
    std::vector<Image*> m_outputImages;

    std::string m_name = "unnamed";
    std::string m_filterGroup = "default";
    std::string m_filterType = "cpu filter";
    InteractableCollection m_interactableCollection;
};

#endif //SRCCMAKE_IMAGEFILTER_H
