#ifndef SRCCMAKE_FASTFOURIERTRANSFORM_H
#define SRCCMAKE_FASTFOURIERTRANSFORM_H

#include "util/Types.h"
#include "processing/ImageFilter.h"

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
    Image* fft(Image* in, int row, int col);
    /**
     * reverses the bits that usually would
     * be reverted by the recursive fft
     */
    uint reverseBits(uint i);
    bool getNextPowerOfTwo(int number);
};


#endif //SRCCMAKE_FASTFOURIERTRANSFORM_H
