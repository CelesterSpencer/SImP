#ifndef SRCCMAKE_FASTFOURIERTRANSFORM_H
#define SRCCMAKE_FASTFOURIERTRANSFORM_H

#include "processing/ImageFilter.h"
#include "util/Types.h"

// http://paulbourke.net/miscellaneous/dft/
// http://www.imagemagick.org/Usage/fourier/

class FastFourierTransform : public ImageFilter
{
public:
    FastFourierTransform() {
        m_name = "Fast Fourier Transform";
        m_filterGroup = "fft";

        m_userDataCollection.addUserData(
                UserData(
                        "Inverse",
                        UserData::DataType::DT_BOOL,
                        &m_isInverse)
        );
    };
    ~FastFourierTransform() {};

    void process();

private:
    std::vector<comp> fft(std::vector<comp> in);
    int getNextPowerOfTwo(int number);
    void scaleCoefficients(std::vector<comp>& coefficients);

    bool m_isInverse = false;
};


#endif //SRCCMAKE_FASTFOURIERTRANSFORM_H
