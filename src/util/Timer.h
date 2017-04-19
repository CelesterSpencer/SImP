#ifndef SRCCMAKE_TIMER_H
#define SRCCMAKE_TIMER_H

// std
#include <chrono>

class Timer
{
public:
    Timer()
    {
        m_begin = std::chrono::high_resolution_clock::now();
        m_end = std::chrono::high_resolution_clock::now();
    }

    long getDeltaTime()
    {
        m_end = std::chrono::high_resolution_clock::now();
        long delta = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_begin).count();
        m_begin = m_end;
        return delta;
    }


private:
    std::chrono::high_resolution_clock::time_point m_begin;
    std::chrono::high_resolution_clock::time_point m_end;
};

#endif //SRCCMAKE_TIMER_H
