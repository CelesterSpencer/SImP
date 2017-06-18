#include "KuwaharaFilter.h"

KuwaharaFilter::KuwaharaFilter()
{
    // init filter
    init("Kuwahara Filter", "non linear");

    // set initial values
    m_filterRadius = 3;
    m_radiusMin = 1;
    m_radiusMax = 10;

    // add user input
    addUserInput("Filter Radius", &m_filterRadius, &m_radiusMin, &m_radiusMax);
}

void KuwaharaFilter::process()
{
    // get input image
    Image* in = getInputImage(0);

    // create copy of input image
    Image* out = new Image(in);

    out->parallel([=](Image* p_img, int x, int y, int c)
        {
            int elementCount = (m_filterRadius+1)*(m_filterRadius+1);

            float mean1  = 0.f;
            float sigma1 = 0.f;
            std::vector<float> values1;

            float mean2  = 0.f;
            float sigma2 = 0.f;
            std::vector<float> values2;

            float mean3  = 0.f;
            float sigma3 = 0.f;
            std::vector<float> values3;

            float mean4  = 0.f;
            float sigma4 = 0.f;
            std::vector<float> values4;

            // collect all values of all 4 regions and sum up the mean values
            for(int fy = 0; fy <= m_filterRadius; fy++)
            {
                for(int fx = 0; fx <= m_filterRadius; fx++)
                {
                    int nx1 = p_img->getXInBounds(x-fx);
                    int ny1 = p_img->getYInBounds(y-fy);
                    float val1 = p_img->get(nx1, ny1, c);
                    mean1 += val1;
                    values1.push_back(val1);

                    int nx2 = p_img->getXInBounds(x+fx);
                    int ny2 = p_img->getYInBounds(y-fy);
                    float val2 = p_img->get(nx2, ny2, c);
                    mean2 += val2;
                    values2.push_back(val2);

                    int nx3 = p_img->getXInBounds(x-fx);
                    int ny3 = p_img->getYInBounds(y+fy);
                    float val3 = p_img->get(nx3, ny3, c);
                    mean3 += val3;
                    values3.push_back(val3);

                    int nx4 = p_img->getXInBounds(x+fx);
                    int ny4 = p_img->getYInBounds(y+fy);
                    float val4 = p_img->get(nx4, ny4, c);
                    mean4 += val4;
                    values4.push_back(val4);
                }
            }

            // divide mean values by the number of elements
            mean1 /= elementCount;
            mean2 /= elementCount;
            mean3 /= elementCount;
            mean4 /= elementCount;

            // calculate the standard deviations
            for(int i = 0; i < values1.size(); i++)
            {
                sigma1 += (values1[i] - mean1)*(values1[i] - mean1);
                sigma2 += (values2[i] - mean2)*(values2[i] - mean2);
                sigma3 += (values3[i] - mean3)*(values3[i] - mean3);
                sigma4 += (values4[i] - mean4)*(values4[i] - mean4);
            }

            // divide by the number of elements and calculate the root
            sigma1 = std::sqrt(sigma1 / elementCount);
            sigma2 = std::sqrt(sigma2 / elementCount);
            sigma3 = std::sqrt(sigma3 / elementCount);
            sigma4 = std::sqrt(sigma4 / elementCount);

            float result = 0.f;
            if(sigma1 < sigma2)
            {
                if(sigma3 < sigma4)
                {
                    result = (sigma1 < sigma3) ? mean1 : mean3;
                }
                else
                {
                    result = (sigma1 < sigma4) ? mean1 : mean4;
                }
            }
            else
            {
                if(sigma3 < sigma4)
                {
                    result = (sigma2 < sigma3) ? mean2 : mean3;
                }
                else
                {
                    result = (sigma2 < sigma4) ? mean2 : mean4;
                }
            }

            return result;
        });

    // return result
    returnImage(out);
}