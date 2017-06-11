#include "core/widget/SpinnerWidget.h"
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


    // setup layers
    LayerManager layerManager;



    // setup menu widget manager
    WidgetManager widgetManager;



    //________________________________________________SETUP_WIDGETS___________________________________________________//

    SpinnerWidget spinnerWidget; // spinner should be above everything
    widgetManager.addWidget(&spinnerWidget, 100);


    LayerWidget layerWidget(&layerManager, mainWindowWidth, mainWindowHeight);
    widgetManager.addWidget(&layerWidget, 10);



    //_______________________________________________SETUP_FILE_MENU__________________________________________________//

    auto saveAction = [&]()
    {
        Image* image = layerManager.getImageOfActiveLayer();
        std::string fileName = image->getFileName()+"_modified";
        image->save();
        widgetManager.addWidget(new MessageWidget("Saved "+fileName+".", 10000.f), 60, true);
    };

    widgetManager.addMenuTab("File", 1);
    widgetManager.addMenuTabEntry("File", "Save",
        [&]
        {
            // get file name and create file path for saving
            Image* image = layerManager.getImageOfActiveLayer();
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
                            Image* image = layerManager.getImageOfActiveLayer();
                            std::string fileName = image->getFileName()+"_modified";
                            image->save();
                            widgetManager.addWidget(new MessageWidget("Saved "+fileName+".", 10000.f), 60, true);
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
        if (FilterManager::getInstance().getFilterStatus() == FilterManager::FilterStatus::DONE)
        {
            std::cout << "copy images" << std::endl;
            // create new layers for every output image
            auto outputImages = FilterManager::getInstance().getOutputImages();
            for (int i = 0; i < outputImages.size(); i++)
            {
                layerManager.addLayer();
                layerManager.setImage(outputImages[i], layerManager.getNumberOfLayers()-1);
            }

            // reset temp images
            outputImages.clear();

            // cleanup image filters and set filtermanager to ready
            FilterManager::getInstance().cleanup();

            // finish transaction and unblock layers
            layerWidget.enable();
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
            widgetManager.addWidget(new FilterSettingsWidget(imageFilter, &widgetManager, &layerManager,
                [&, i](int status)
                {
                    if(status == 0) return; // user selected cancel

                    // if selected image filter is on cpu then block the layers and show spinner widget
                    ImageFilter* p_imageFilter = FilterManager::getInstance().getImageFilters()[i];
                    if(p_imageFilter->getFilterType() == "cpu filter")
                    {
                        layerWidget.disable();
                        spinnerWidget.show();
                    }

                    // execute image filter
                    Image* p_selectedImage = layerManager.getImageOfActiveLayer();
                    FilterManager::getInstance().execute(p_imageFilter, p_selectedImage);
                }
            ), 30, true);
        });
    }



    //______________________________________________PROGRAM_LOGIC_____________________________________________________//

    auto blockUnblockFilterMenuTab = [&]
    {
        // check state of filter menu
        if(FilterManager::getInstance().getFilterStatus() == FilterManager::FilterStatus::RUNNING ||
                !layerManager.isValidImageSelected())
            widgetManager.disableMenuTab("Filter");
        else
            widgetManager.enableMenuTab("Filter");

        // check state of save dialog
        if(!layerManager.isValidImageSelected())
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