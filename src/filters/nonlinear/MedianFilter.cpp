#include "MedianFilter.h"

MedianFilter::MedianFilter()
{
    init("Median Filter", "non linear");

    m_filterRadius = 2;
    m_minRadius = 0;
    m_maxRadius = 10;

    addUserInput("Filter radius", &m_filterRadius, &m_minRadius, &m_maxRadius);
};

void MedianFilter::process()
{
    Image* in = getInputImage(0);
    Image* out = new Image(in);

    int diameter = m_filterRadius*2 + 1;
    int median = (diameter*diameter-1)/2;

    out->parallel([=](Image* p_img, int x, int y, int c) -> float
      {
          std::vector<float> values;
          values.reserve(diameter*diameter);

          // iterate over filter
          for(int fx = -m_filterRadius; fx <= m_filterRadius; fx++)
          {
              for (int fy = -m_filterRadius; fy <= m_filterRadius; fy++)
              {
                  int posX = std::min(std::max(x+fx, 0), p_img->getWidth()-1);
                  int posY = std::min(std::max(y+fy, 0), p_img->getHeight()-1);
                  values.push_back(p_img->get(posX, posY, Image::Channel::RED));
              }
          }

          // sort the collected values
          std::sort(values.begin(), values.end());

          // return the median
          return values[median];
      });

    returnImage(out);
}