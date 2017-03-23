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
        m_options = {"Lightness", "Average", "Luminocity"};
        m_selectedOption = 0;

        m_userDataCollection.addUserData(
                UserData("Convert to gray",
                         UserData::DataType::DT_OPTIONS,
                         &m_options,
                         &m_selectedOption,
                         new int(3))
        );
    };
    ~ConvertToGrayscale() {};

    Image* process(Image* in);

private:
    std::vector<std::string> m_options;
    int m_selectedOption;
};

#endif //SRCCMAKE_CONVERTTOGRAYSCALE_H
