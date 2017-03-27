//
// Created by Windrian on 25.03.2017.
//

#include "FastFourierTransform.h"

Image* FastFourierTransform::process(Image* in)
{
    Image* frequencyImage = new Image;
    frequencyImage->reserve(in->getWidth(), in->getHeight(), 1); // gray scale image

    // create two vectors that are power of two
    int sizeX = getNextPowerOfTwo(in->getWidth());
    int sizeY = getNextPowerOfTwo(in->getHeight());

    std::vector<comp> complexMat(sizeX*sizeY, 0);

    // execute fft for all rows
    for(int y = 0; y < in->getHeight(); y++)
    {
        // fill row
        std::vector<comp> row(sizeX, 0);
        for(int x = 0; x < in->getWidth(); x++)
        {
            row[x] = in->get(x,y,Image::Channel::RED);
        }

        // calculate fft for the rows
        row = fft(row);

        // write data in temporary vector
        complexMat.insert(std::end(complexMat), std::begin(row), std::end(row));
    }

    // execute fft for all columns
    for(int x = 0; x < in->getWidth(); x++)
    {
        // fill column
        std::vector<comp> column(sizeY);
        for(int y = 0; y < in->getHeight(); y++)
        {
            column[y] = complexMat[y*sizeX+x];
        }

        // fft for columns
        column = fft(column);

        // write results in two images
        for(int y = 0; y < in->getHeight(); y++)
        {
            frequencyImage->set(std::norm(column[y]),x,y,Image::Channel::RED);
        }
    }

    // return images
    return frequencyImage;
}

std::vector<comp> FastFourierTransform::fft(std::vector<comp> in)
{
    int n = in.size();
    if (n == 1)
    {
        return in;
    }
    else
    {
        std::vector<comp> result(n,0);

        // fill even and odd
        std::vector<comp> even;
        std::vector<comp> odd;
        for (int i = 0; i < n; i+=2)
        {
            even.push_back(in[i]);
            odd.push_back(in[i+1]);
        }

        // recursion
        even = fft(even);
        odd = fft(odd);

        // merge results
        for (int k = 0; k < n/2; k++)
        {
            result[k]     = even[k] + odd[k] * exp(I*comp(-2*PI*k/n,0));
            result[k+n/2] = even[k] - odd[k] * exp(I*comp(-2*PI*k/n,0));
        }

        return result;
    }
}

int FastFourierTransform::getNextPowerOfTwo(int number)
{
    int powerOfTwo = 2;
    while(powerOfTwo < number)
    {
        powerOfTwo *= 2;
    }
    return powerOfTwo;
}