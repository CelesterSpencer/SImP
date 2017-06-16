#ifndef SRCCMAKE_FASTFOURIERTRANSFORM_H
#define SRCCMAKE_FASTFOURIERTRANSFORM_H

#include "filters/ImageFilter.h"
#include "core/util/Types.h"

// http://paulbourke.net/miscellaneous/dft/
// http://www.imagemagick.org/Usage/fourier/

class FastFourierTransform : public ImageFilter
{
public:
    FastFourierTransform();

    void process();

private:
    std::vector<comp> fft(std::vector<comp> in);
    int getNextPowerOfTwo(int number);
    void scaleCoefficients(std::vector<comp>& coefficients);

    bool m_isInverse = false;
};


#endif //SRCCMAKE_FASTFOURIERTRANSFORM_H
