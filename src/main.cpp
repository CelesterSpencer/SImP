// include program
#include <core/Program.h>

//_________________________________________________//
//                 IMAGE FILTERS                   //
//_________________________________________________//

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
    // restructure
    program.addFilter(new SplitChannels);
    program.addFilter(new MixImages);



    //________________________________________________RUN_PROGRAM_____________________________________________________//

    program.run();



    return 0;
}