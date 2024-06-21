# SImP

## About

This project is supposed to be an easy and lightweight image processing framework.
Fast and easy prototyping of filters is in its focus. Simple image loading and an
rudimentary Gui is used for interaction with images.

## Build

You should be able to build the project with the following commands
```bash
mkdir build
cd build
cmake ..
make
```
All dependencies are downloaded and built automatically.

## Usage

SImP provides two Classes that can be implemented. The first class is 
`ImageFilter` the other one `ImageFilterGpu`. An implementation of either 
of two has to overwrite the fields `m_name` and `m_filterGroup` in order
to be displayed properly in the menu.

###### ImageFilter (CPU)

Either pick a folder inside ***src/processing/*** or create a new folder
for your filter. 
Your filter needs to inherit from `ImageFilter`
```cpp
#include "processing/ImageFilter.h"

class MyFilter : public ImageFilter
{
    MyFilter() 
    {
        m_name = "My filter";
        m_filterGroup = "my filter group";
    }
}
```

###### ImageFilterGpu

In Addition to the fields `m_name` and `m_filterGroup` implementations of 
`ImageFilterGpu` need to call the functions
```cpp
setComputeShader("/path/to/MyComputeShaderFile");
addOutputImageDescription("Output image name", width, height, numberOfChannel);
```
inside their constructor.
To determine the output images dimensions from the size of the input image
just set `width`, `height` and `numberOfChannel` all to 0.

***TODO***

###### Using user input

***TODO***