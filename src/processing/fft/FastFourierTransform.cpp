//
// Created by Windrian on 25.03.2017.
//

#include "FastFourierTransform.h"

Image* FastFourierTransform::fft(Image* in, int row, int col)
{
    int size = (row < 0) ? in->getHeight() : in->getWidth();
    std::vector<int> idxs;
    for(int i = 0; i < size; i++)
    {
        idxs.push_back(reverseBits(i));
    }
}

uint FastFourierTransform::reverseBits(uint i)
{
    uint mask = 0x55555555;                     // 0 1 0 1 ...
    i = ((i & mask) << 1) | ((i >> 1) & mask);
    mask = 0x33333333;                          // 00 11 ...
    i = ((i & mask) << 2) | ((i >> 2) & mask);
    mask = 0x0f0f0f0f;                          // 0000 1111 ...
    i = ((i & mask) << 4) | ((i >> 4) & mask);
    mask = 0x00ff00ff;                          // 0000 0000 1111 1111 ...
    i = ((i & mask) << 8) | ((i >> 8) & mask);
                                                // 0000 0000 0000 0000 1...
    i = (i << 16) | (i >> 16);
    return i;
}

bool FastFourierTransform::getNextPowerOfTwo(int number)
{

}