#include "FastFourierTransform.h"

FastFourierTransform::FastFourierTransform()
{
    // init filter
    init("Fast Fourier Transform", "fft");

    // add user input
    addUserInput("Inverse", &m_isInverse);
    addUserInput("Phase image", [=]() -> bool { return m_isInverse; });
};

void FastFourierTransform::process()
{
    Image* in = getInputImage(0);
    Image* in2;
    if(m_isInverse) in2 = getInputImage(1);

    // get power of two for the image dimensions
    int sizeX = getNextPowerOfTwo(in->getWidth());
    int sizeY = getNextPowerOfTwo(in->getHeight());



    // fit image into vector with power of two sizes
    std::vector<comp> input(sizeX*sizeY, comp(0, 0));

    // matrix holding the results of the first 1D fft
    std::vector<comp> complexMat(sizeX*sizeY, comp(0, 0));

    // init the result image
    Image* MagnitudeImage = new Image(sizeX, sizeY, 1); // gray scale image
    Image* PhaseImage     = new Image(sizeX, sizeY, 1); // gray scale image


    // fill input image
    for(int y = 0; y < in->getHeight(); y++)
    {
        for(int x = 0; x < in->getWidth(); x++)
        {
            if(m_isInverse)
            {
                input[y*sizeX+x] = comp(in->get(x,y,Image::Channel::RED), in2->get(x,y,Image::Channel::RED));
            }
            else
            {
                input[y*sizeX+x] = comp(in->get(x,y,Image::Channel::RED), 0);
            }
        }
    }



    // execute fft for all rows
    for(int y = 0; y < sizeY; y++)
    {
        // fill row
        std::vector<comp> row(sizeX, comp(0, 0));
        for(int x = 0; x < sizeX; x++)
        {
            row[x] = input[y*sizeX+x];
        }

        // calculate fft for the rows
        row = fft(row);
        if(m_isInverse) scaleCoefficients(row);

        // write data in a temporary vector
        complexMat.insert(complexMat.begin()+y*sizeX, row.begin(), row.end());
    }



    // execute fft for all columns
    for(int x = 0; x < sizeX; x++)
    {
        // fill column
        std::vector<comp> column(sizeY, comp(0, 0));
        for(int y = 0; y < sizeY; y++)
        {
            column[y] = complexMat[y*sizeX+x];
        }

        // fft for columns
        column = fft(column);
        if(m_isInverse) scaleCoefficients(column);

        for(int y = 0; y < sizeY; y++)
        {
            MagnitudeImage->set(std::norm(column[y]), x, y, Image::Channel::RED);
            PhaseImage->set((std::atan2(column[y].imag(), column[y].real())+PI) / (2*PI), x, y,Image::Channel::RED);
        }
    }



    // return images
    returnImage(MagnitudeImage);
    returnImage(PhaseImage);
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
        std::vector<comp> result(n, comp(0, 0));

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
        float sign = (m_isInverse) ? 1.0 : -1.0;
        for (int k = 0; k < n/2; k++)
        {
            result[k]     = even[k] + odd[k] * exp(Im*comp(sign*2*PI*k/n,0));
            result[k+n/2] = even[k] - odd[k] * exp(Im*comp(sign*2*PI*k/n,0));
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

void FastFourierTransform::scaleCoefficients(std::vector<comp>& coefficients)
{
    float N = coefficients.size();
    for (int i = 0; i < coefficients.size(); i++)
    {
        coefficients[i] / N;
    }
}