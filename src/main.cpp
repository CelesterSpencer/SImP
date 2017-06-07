// standard includes
#include <fstream>

// internal includes
#include "scene/Canvas.h"
#include "rendering/WindowManager.h"
#include "processing/FilterManager.h"
// linear
#include "processing/linear/MeanFilterGpu.h"
// non linear
#include "processing/nonlinear/MinFilterGpu.h"
#include "processing/nonlinear/MaxFilterGpu.h"
#include "processing/nonlinear/MedianFilter.h"
// edge detection
#include "processing/edgedetection/LaplaceFilter.h"
// color adjustment
#include "processing/coloradjustment/ConvertToGrayscale.h"
#include "processing/coloradjustment/SimplifyColors.h"
#include "processing/coloradjustment/Saturate.h"
#include "processing/coloradjustment/LinearSpread.h"
// distortion
#include "processing/distortion/Noise.h"
// restructure
#include "processing/restructure/SplitChannels.h"
#include "processing/restructure/MixImages.h"



// window
float WINDOW_WIDTH = 1024;
float WINDOW_HEIGHT = 768;



int main() {

    //__________________________________________INIT_RENDERING_ENGINE_________________________________________________//

    WindowManager::getInstance().init(WINDOW_WIDTH, WINDOW_HEIGHT);
    GLFWwindow* window = WindowManager::getInstance().getWindow();
    Canvas canvas;



    //________________________________________REGISTER_FILTERS_IN_PROGRAM_____________________________________________//

    // linear filter
    FilterManager::getInstance().registerImageFilter(new MeanFilterGpu);
    // non-linear filter
    FilterManager::getInstance().registerImageFilter(new MinFilterGpu);
    FilterManager::getInstance().registerImageFilter(new MaxFilterGpu);
    FilterManager::getInstance().registerImageFilter(new MedianFilter);
    // edge detection
    FilterManager::getInstance().registerImageFilter(new LaplaceFilter);
    // color adjustment
    FilterManager::getInstance().registerImageFilter(new ConvertToGrayscale);
    FilterManager::getInstance().registerImageFilter(new SimplifyColors);
    FilterManager::getInstance().registerImageFilter(new Saturate);
    FilterManager::getInstance().registerImageFilter(new LinearSpread);
    // distortion
    FilterManager::getInstance().registerImageFilter(new Noise);
    // restructure
    FilterManager::getInstance().registerImageFilter(new SplitChannels);
    FilterManager::getInstance().registerImageFilter(new MixImages);



    //_________________________________________________DRAWING________________________________________________________//

    // run until user closes the window or presses ALT+F4
    auto lambda = [&](){ canvas.draw(); };
    WindowManager::getInstance().render(lambda);

    return 0;
}