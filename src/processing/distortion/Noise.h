//
// Created by Windrian on 21.03.2017.
//

#ifndef SRCCMAKE_NOISE_H
#define SRCCMAKE_NOISE_H

#include "processing/ImageFilter.h"

class Noise : public ImageFilter
{
public:
    Noise()
    {
        m_name = "Noise";
        m_noiseMin = -125;
        m_noiseMax = 125;

        m_userDataCollection.addUserData(
                UserData(
                        "Noise min",
                        UserData::DataType::DT_INT,
                        &m_noiseMin,
                        new int(-255),
                        &m_noiseMax
                )
        );
        m_userDataCollection.addUserData(
                UserData(
                        "Noise max",
                        UserData::DataType::DT_INT,
                        &m_noiseMax,
                        &m_noiseMin,
                        new int(255)
                )
        );
    };
    ~Noise() {}

    Image* process(Image* in);

private:
    int m_noiseMin;
    int m_noiseMax;
};


#endif //SRCCMAKE_NOISE_H
