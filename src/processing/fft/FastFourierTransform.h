#ifndef SRCCMAKE_FASTFOURIERTRANSFORM_H
#define SRCCMAKE_FASTFOURIERTRANSFORM_H

#include "processing/ImageFilter.h"
#include "util/Types.h"

class FastFourierTransform : public ImageFilter
{
public:
    FastFourierTransform() {
        m_name = "Fast Fourier Transform";
        m_filterGroup = "fft";
    };
    ~FastFourierTransform() {};

    Image* process(Image* in);

private:
    std::vector<comp> fft(std::vector<comp> in);
    int getNextPowerOfTwo(int number);
};


#endif //SRCCMAKE_FASTFOURIERTRANSFORM_H
