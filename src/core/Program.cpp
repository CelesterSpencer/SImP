#include <scene/widget/SpinnerWidget.h>
#include "Program.h"

Program::Program()
{
    //__________________________________________INIT_RENDERING_ENGINE_________________________________________________//

    WindowManager::getInstance().init(WINDOW_WIDTH, WINDOW_HEIGHT);
    GLFWwindow* window = WindowManager::getInstance().getWindow();
}
Program::~Program()
{

}

void Program::addFilter(ImageFilter* imagefilter)
{
    FilterManager::getInstance().registerImageFilter(imagefilter);
}
void Program::run()
{
    int mainWindowWidth = WindowManager::getInstance().getWidth();
    int mainWindowHeight = WindowManager::getInstance().getHeight();


    // setup menu widget manager
    WidgetManager widgetManager;



    //________________________________________________SETUP_WIDGETS___________________________________________________//

    SpinnerWidget spinnerWidget; // spinner should be above everything
    widgetManager.addWidget(&spinnerWidget, 100);

    LayerWidget layerWidget(mainWindowWidth, mainWindowHeight);
    widgetManager.addWidget(&layerWidget, 10);



    //_______________________________________________SETUP_FILE_MENU__________________________________________________//

    auto saveAction = [&]()
    {
        Image* image = layerWidget.getSelectedImage();
        std::string fileName = image->getFileName()+"_modified";
        std::string filePath = RESOURCES_PATH"/output/"+fileName+".png";
        ImageHandler::getInstance().saveImage(filePath, image->getRawData(),
                                              image->getWidth(), image->getHeight(), image->getChannelNumber());
        widgetManager.addWidget(new MessageWidget("Saved "+fileName+".", 10000.f), 60, true);
    };

    widgetManager.addMenuTab("File", 1);
    widgetManager.addMenuTabEntry("File", "Save",
        [&]
        {
            // get file name and create file path for saving
            Image* image = layerWidget.getSelectedImage();
            std::string fileName = image->getFileName()+"_modified";
            std::string filePath = RESOURCES_PATH"/output/"+fileName+".png";



            // show yes/no dialog when a file with the same name already exists
            if(FileHandler::getInstance().exists(filePath))
            {
                // show the dialog
                DialogWidget* p_fileOverwriteDialog = new DialogWidget("Do you want to overwrite "+fileName+"?",
                    [&](int savingStatus)
                    {
                        if(savingStatus == 1)
                        {
                            saveAction();
                        }
                    }
                );
                widgetManager.addWidget(p_fileOverwriteDialog, 50, true);
            }
            // if file doesn't exist simply save the image
            else
                saveAction();
        }
    );
    widgetManager.addMenuTabDivider("File");
    widgetManager.addMenuTabEntry("File", "Exit", [&]
    {
        WindowManager::getInstance().exit();
    });





    //______________________________________________SETUP_FILTER_MENU_________________________________________________//

    bool shouldSynchronizeImages = false;
    auto synchronizeImages = [&]()
    {
        // copy output images to the layers
        if (shouldSynchronizeImages)
        {
            // create new layers for every output image
            auto outputImages = FilterManager::getInstance().getOutputImages();
            for (int i = 0; i < outputImages.size(); i++)
            {
                LayerManager::getInstance().addLayer();
                LayerManager::getInstance().setImage(outputImages[i], LayerManager::getInstance().getNumberOfLayers()-1);
            }

            // reset temp images
            outputImages.clear();

            // reset input and output images
            FilterManager::getInstance().resetFilter();

            // finish transaction and unblock layers
            LayerManager::getInstance().unblockInteraction();
            spinnerWidget.hide();
            shouldSynchronizeImages = false;
        }
    };
    widgetManager.addMenuTab("Filter", 2);
    std::string previousFilterGroup = "";
    for(int i = 0; i < FilterManager::getInstance().getImageFilters().size(); i++)
    {
        ImageFilter* imageFilter = FilterManager::getInstance().getImageFilters()[i];
        // add filter groups above filters
        if(imageFilter->getFilterGroup() != previousFilterGroup)
        {
            previousFilterGroup = imageFilter->getFilterGroup();
            widgetManager.addMenuTabText("Filter", previousFilterGroup, 5, TEXT_HIGHLIGHTED_COLOR);
        }
        widgetManager.addMenuTabEntry("Filter", "    "+imageFilter->getName(), [&, i]
        {
            ImageFilter* imageFilter = FilterManager::getInstance().getImageFilters()[i];
            widgetManager.addWidget(new FilterSettingsWidget(imageFilter, [&, i](int status)
                {
                    if(status == 0) return; // user selected cancel

                    // start processing and block layers
                    ImageFilter* imageFilter = FilterManager::getInstance().getImageFilters()[i];
                    std::string filterType = imageFilter->getFilterType();


                    // execute gpu filter
                    if(filterType == "gpu filter")
                    {
                        // apply filter for the selected image
                        Image* inputImage = layerWidget.getSelectedImage();
                        FilterManager::getInstance().addImage(inputImage);
                        FilterManager::getInstance().addRequiredImages();
                        FilterManager::getInstance().applyFilter();

                        // filter execution is done
                        shouldSynchronizeImages = true;
                    }
                    // execute cpu filter
                    else if(filterType == "cpu filter")
                    {
                        LayerManager::getInstance().blockInteraction();
                        spinnerWidget.show();

                        // execute filter application in a different thread
                        m_imageProcessingThread = new std::thread(
                        [](Image* p_in,  bool* p_shouldSynchronizeImages)
                            {
                                // apply filter for the selected image
                                FilterManager::getInstance().addImage(p_in);
                                FilterManager::getInstance().addRequiredImages();
                                FilterManager::getInstance().applyFilter();

                                // reset variables to allow further filter applications
                                *p_shouldSynchronizeImages = true;
                            },LayerManager::getInstance().getImageOfActiveLayer(), &shouldSynchronizeImages
                        );
                    }
                }
            ), 30);
        });
    }



    //______________________________________________PROGRAM_LOGIC_____________________________________________________//

    auto blockUnblockFilterMenuTab = [&]
    {
        // check state of filter menu
        if(FilterManager::getInstance().isRunning() || layerWidget.getActiveLayerIndex() == -1)
            widgetManager.disableMenuTab("Filter");
        else
            widgetManager.enableMenuTab("Filter");

        if(layerWidget.getActiveLayerIndex() == -1)
            widgetManager.disableMenuTabEntry("File", "Save");
        else
            widgetManager.enableMenuTabEntry("File", "Save");
    };



    //_________________________________________________DRAWING________________________________________________________//

    // run until user closes the window or presses ALT+F4
    Timer timer;
    auto lambda = [&]()
    {
        // program logic
        blockUnblockFilterMenuTab();
        synchronizeImages();

        // rendering
        int width = WindowManager::getInstance().getWidth();
        int height = WindowManager::getInstance().getHeight();
        widgetManager.draw(width, height, timer.getDeltaTime());
    };
    WindowManager::getInstance().render(lambda);
}