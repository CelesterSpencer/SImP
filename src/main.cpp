// include program
#include "core/Program.h"

//_________________________________________________//
//                 IMAGE FILTERS                   //
//_________________________________________________//

// linear
#include "filters/linear/MeanFilterGpu.h"
// non linear
#include "filters/nonlinear/MinFilterGpu.h"
#include "filters/nonlinear/MaxFilterGpu.h"
#include "filters/nonlinear/MedianFilter.h"
// edge detection
#include "filters/edgedetection/LaplaceFilter.h"
// color adjustment
#include "filters/coloradjustment/ConvertToGrayscale.h"
#include "filters/coloradjustment/SimplifyColors.h"
#include "filters/coloradjustment/Saturate.h"
#include "filters/coloradjustment/LinearSpread.h"
// distortion
#include "filters/distortion/Noise.h"
#include "filters/distortion/Whirl.h"
// restructure
#include "filters/restructure/SplitChannels.h"
#include "filters/restructure/MergeChannels.h"
#include "filters/restructure/MixImages.h"
// test
#include "filters/test/Multithreading.h"



int main() {
    //________________________________________________INIT_PROGRAM____________________________________________________//

    Program program;



    //___________________________________________ADD_FILTERS_TO_PROGRAM_______________________________________________//

    // linear filter
    program.addFilter(new MeanFilterGpu);
    // non-linear filter
    program.addFilter(new MinFilterGpu);
    program.addFilter(new MaxFilterGpu);
    program.addFilter(new MedianFilter);
    // edge detection
    program.addFilter(new LaplaceFilter);
    // color adjustment
    program.addFilter(new ConvertToGrayscale);
    program.addFilter(new SimplifyColors);
    program.addFilter(new Saturate);
    program.addFilter(new LinearSpread);
    // distortion
    program.addFilter(new Noise);
    program.addFilter(new Whirl);
    // restructure
    program.addFilter(new SplitChannels);
    program.addFilter(new MergeChannels);
    program.addFilter(new MixImages);
    // test
    program.addFilter(new Multithreading);



    //________________________________________________RUN_PROGRAM_____________________________________________________//

    program.run();



    return 0;
}