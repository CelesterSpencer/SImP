// standard includes
#include <fstream>

// internal includes
#include "scene/Canvas.h"
#include "rendering/WindowManager.h"
#include "processing/FilterManager.h"

#include "processing/linear/MeanFilter.h"
#include "processing/nonlinear/MinFilter.h"
#include "processing/nonlinear/MaxFilter.h"
#include "processing/nonlinear/MedianFilter.h"
#include "processing/edgedetection/LaplaceFilter.h"
#include "processing/coloradjustment/ConvertToGrayscale.h"
#include "processing/distortion/Noise.h"



// window
float WINDOW_WIDTH = 1024;
float WINDOW_HEIGHT = 768;



int main() {

    //_________________________________________________//
    //_________________Initalization___________________//
    //_________________________________________________//
    WindowManager::getInstance().init(WINDOW_WIDTH, WINDOW_HEIGHT);
    GLFWwindow* window = WindowManager::getInstance().getWindow();

    // register filters
    FilterManager::getInstance().registerImageFilter(new MeanFilter);
    FilterManager::getInstance().registerImageFilter(new MinFilter);
    FilterManager::getInstance().registerImageFilter(new MaxFilter);
    FilterManager::getInstance().registerImageFilter(new MedianFilter);
    FilterManager::getInstance().registerImageFilter(new LaplaceFilter);
    FilterManager::getInstance().registerImageFilter(new ConvertToGrayscale);
    FilterManager::getInstance().registerImageFilter(new Noise);

    // create canvas
    Canvas canvas;



    //_________________________________________________//
    //___________________Drawing_______________________//
    //_________________________________________________//

    // run until user closes the window or presses ALT+F4
    auto lambda = [&](){ canvas.draw(); };
    WindowManager::getInstance().render(lambda);

    return 0;
}