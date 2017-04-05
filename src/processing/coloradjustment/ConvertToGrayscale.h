#ifndef SRCCMAKE_CONVERTTOGRAYSCALE_H
#define SRCCMAKE_CONVERTTOGRAYSCALE_H

#include <vector>
#include <string>

#include "processing/ImageFilter.h"

class ConvertToGrayscale : public ImageFilter
{
public:
    ConvertToGrayscale() {
        m_name = "Convert to grayscale";
        m_filterGroup = "color adjustment";

        m_options = {"Lightness", "Average", "Luminocity"};
        m_selectedOption = 0;

        m_interactableCollection.addInteractable(
                new SelectBox("Convert to gray",&m_selectedOption, &m_options)
        );
    };
    ~ConvertToGrayscale() {};

    void process();

private:
    std::vector<std::string> m_options;
    int m_selectedOption;
};

#endif //SRCCMAKE_CONVERTTOGRAYSCALE_H
