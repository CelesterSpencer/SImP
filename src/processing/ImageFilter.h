#ifndef SRCCMAKE_IMAGEFILTER_H
#define SRCCMAKE_IMAGEFILTER_H

#include <string>

#include "scene/Image.h"
#include "io/UserData.h"

class ImageFilter
{
public:
    // Filter information
    std::string getName() { return m_name; };
    std::string getFilterGroup() { return m_filterGroup; };

    // IO Images
    void addInputImage(Image* in) { m_inputImages.push_back(in); }
    std::vector<Image*> getOutputImages() { return m_outputImages; };

    // Parameters that can be specified by the user
    UserDataCollection* getUserDataCollection() { return &m_userDataCollection; };

    // clear input and output images
    void clearImages() { m_inputImages.clear(); m_outputImages.clear(); }

    // actually execute the filter
    virtual void process() = 0;

protected:
    std::string m_name = "unnamed";
    std::string m_filterGroup = "default";
    UserDataCollection m_userDataCollection;

    int getNumberOfInputImages() { return m_inputImages.size(); }
    Image* getInputImage(int index) { return m_inputImages[index]; }
    void returnImage(Image* out) { m_outputImages.push_back(out); }

private:
    std::vector<Image*> m_inputImages;
    std::vector<Image*> m_outputImages;
};

#endif //SRCCMAKE_IMAGEFILTER_H
