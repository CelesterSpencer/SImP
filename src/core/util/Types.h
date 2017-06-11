#ifndef SRCCMAKE_TYPES_H
#define SRCCMAKE_TYPES_H

#include <complex>

// types
using uint = unsigned int;
using comp = std::complex<float>;

// variables
namespace CONSTS
{
    #define PI 3.141592653589
    #define Im comp(0,1)
}


#endif //SRCCMAKE_TYPES_H
