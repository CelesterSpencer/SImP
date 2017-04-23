// standard includes
#include <fstream>

// internal includes
#include "scene/Canvas.h"
#include "rendering/WindowManager.h"
#include "processing/FilterManager.h"

/*
 * linear
 */
#include "processing/linear/MeanFilter.h"
#include "processing/linear/MeanFilterGpu.h"
/*
 * non linear
 */
#include "processing/nonlinear/MinFilter.h"
#include "processing/nonlinear/MaxFilter.h"
#include "processing/nonlinear/MedianFilter.h"
/*
 * edge detection
 */
#include "processing/edgedetection/LaplaceFilter.h"
/*
 * color adjustment
 */
#include "processing/coloradjustment/ConvertToGrayscale.h"
#include "processing/coloradjustment/SimplifyColors.h"
#include "processing/coloradjustment/Saturate.h"
#include "processing/coloradjustment/LinearSpread.h"
/*
 * fft
 */
#include "processing/fft/FastFourierTransform.h"
/*
 * distortion
 */
#include "processing/distortion/Noise.h"
/*
 * restructure
 */
#include "processing/restructure/SplitChannels.h"
/*
 * test
 */
#include "processing/test/TestFilter.h"


// window
float WINDOW_WIDTH = 1024;
float WINDOW_HEIGHT = 768;



int main() {

    //_________________________________________________//
    //_________________Initalization___________________//
    //_________________________________________________//

    WindowManager::getInstance().init(WINDOW_WIDTH, WINDOW_HEIGHT);
    GLFWwindow* window = WindowManager::getInstance().getWindow();

    /*
     * create canvas
     */
    Canvas canvas;

    /*
     * register filters
     */
    FilterManager::getInstance().registerImageFilter(new MeanFilter);
    FilterManager::getInstance().registerImageFilter(new MeanFilterGpu);

    FilterManager::getInstance().registerImageFilter(new MinFilter);
    FilterManager::getInstance().registerImageFilter(new MaxFilter);
    FilterManager::getInstance().registerImageFilter(new MedianFilter);

    FilterManager::getInstance().registerImageFilter(new LaplaceFilter);

    FilterManager::getInstance().registerImageFilter(new ConvertToGrayscale);
    FilterManager::getInstance().registerImageFilter(new SimplifyColors);
    FilterManager::getInstance().registerImageFilter(new Saturate);
    FilterManager::getInstance().registerImageFilter(new LinearSpread);

    FilterManager::getInstance().registerImageFilter(new FastFourierTransform);

    FilterManager::getInstance().registerImageFilter(new Noise);

    FilterManager::getInstance().registerImageFilter(new SplitChannels);

    FilterManager::getInstance().registerImageFilter(new TestFilter);



    //_________________________________________________//
    //___________________Drawing_______________________//
    //_________________________________________________//

    // run until user closes the window or presses ALT+F4
    auto lambda = [&](){ canvas.draw(); };
    WindowManager::getInstance().render(lambda);

    return 0;
}