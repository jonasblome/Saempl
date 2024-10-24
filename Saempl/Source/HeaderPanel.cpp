/*
 ==============================================================================
 
 HeaderPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "HeaderPanel.h"

HeaderPanel::HeaderPanel(SaemplAudioProcessor& inProcessor, CentrePanel& inCentrePanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
centrePanel(inCentrePanel)
{
    setSize(style->HEADER_PANEL_WIDTH, style->HEADER_PANEL_HEIGHT);
    mAudioDeviceManager = std::make_unique<AudioDeviceManager>();
    setPanelComponents();
}

HeaderPanel::~HeaderPanel()
{
    
}

void HeaderPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.fillRoundedRectangle(getLocalBounds()
                           .toFloat()
                           .reduced(style->PANEL_MARGIN / 2.0, style->PANEL_MARGIN / 4.0)
                           .withTrimmedTop(style->PANEL_MARGIN / 4.0),
                           style->CORNER_SIZE_MEDIUM);
    
    // Draw filter on/off button line
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(263,
                           getHeight() / 2.0 - 1,
                           connectionLineWidth,
                           connectionLineHeight,
                           connectionLineRadius);
    
    // Draw volume normalisation on/off button line
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getWidth() - 295,
                           getHeight() / 2.0 - 1,
                           connectionLineWidth,
                           connectionLineHeight,
                           connectionLineRadius);
    
    // Draw logo text
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.setFont(style->FONT_MEDIUM_BOLD);
    g.drawFittedText("Blome Audio",
                     getWidth() - logoWidth - logoMarginRight - style->PANEL_MARGIN,
                     style->PANEL_MARGIN,
                     logoWidth,
                     getHeight() - style->PANEL_MARGIN * 1.75,
                     Justification::centredRight,
                     1);
}

void HeaderPanel::setRefreshLibraryButton(int buttonWidth, int x)
{
    mRefreshLibraryButton = std::make_unique<BlomeImageButton>("Refresh", false);
    mRefreshLibraryButton->setImages(false,
                                     true,
                                     true,
                                     ImageCache::getFromMemory(BinaryData::sync_FILL0_wght400_GRAD0_opsz24_png,
                                                               BinaryData::sync_FILL0_wght400_GRAD0_opsz24_pngSize),
                                     style->BUTTON_IS_DEFAULT_ALPHA,
                                     style->COLOUR_HEADER_BUTTONS,
                                     Image(),
                                     style->BUTTON_IS_OVER_ALPHA,
                                     style->COLOUR_HEADER_BUTTONS,
                                     Image(),
                                     style->BUTTON_IS_DOWN_ALPHA,
                                     style->COLOUR_HEADER_BUTTONS);
    mRefreshLibraryButton->setBounds(x,
                                     style->PANEL_MARGIN,
                                     buttonWidth,
                                     buttonWidth);
    mRefreshLibraryButton->setTooltip("Scans for new files and removes externally deleted files (R)");
    mRefreshLibraryButton->onClick = [this] { sampleLibrary.synchWithLibraryDirectory(); };
    addAndMakeVisible(*mRefreshLibraryButton);
}

void HeaderPanel::setChooseLibraryDirectoryButton(int buttonWidth, int x)
{
    mChooseLibraryDirectoryButton = std::make_unique<BlomeImageButton>("Choose Dir.", false);
    mChooseLibraryDirectoryButton->setImages(false,
                                             true,
                                             true,
                                             ImageCache::getFromMemory(BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_png,
                                                                       BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_pngSize),
                                             style->BUTTON_IS_DEFAULT_ALPHA,
                                             style->COLOUR_HEADER_BUTTONS,
                                             Image(),
                                             style->BUTTON_IS_OVER_ALPHA,
                                             style->COLOUR_HEADER_BUTTONS,
                                             Image(),
                                             style->BUTTON_IS_DOWN_ALPHA,
                                             style->COLOUR_HEADER_BUTTONS);
    mChooseLibraryDirectoryButton->setBounds(x,
                                             style->PANEL_MARGIN,
                                             buttonWidth,
                                             buttonWidth);
    mChooseLibraryDirectoryButton->setTooltip("Choose a new directory as your current sample library (D)");
    mChooseLibraryDirectoryButton->onClick = [this] { showLibraryChooser(); };
    addAndMakeVisible(*mChooseLibraryDirectoryButton);
}

void HeaderPanel::setToggleSampleFolderPanelButton(int buttonWidth, int x)
{
    mToggleSampleFolderPanelButton = std::make_unique<BlomeImageButton>("Toggle SampleFolderPanel", false);
    mToggleSampleFolderPanelButton->setImages(false,
                                              true,
                                              true,
                                              ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                        BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                              currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL
                                              ? style->BUTTON_IS_DEFAULT_ALPHA
                                              : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                              style->COLOUR_HEADER_BUTTONS,
                                              Image(),
                                              currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL
                                              ? style->BUTTON_IS_OVER_ALPHA
                                              : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                              style->COLOUR_HEADER_BUTTONS,
                                              Image(),
                                              currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL
                                              ? style->BUTTON_IS_DOWN_ALPHA
                                              : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                              style->COLOUR_HEADER_BUTTONS);
    mToggleSampleFolderPanelButton->setBounds(x,
                                              style->PANEL_MARGIN,
                                              buttonWidth,
                                              buttonWidth);
    mToggleSampleFolderPanelButton->setTooltip("Folder View - Show the folder structure of your library directory (1)");
    mToggleSampleFolderPanelButton->onClick = [this]
    {
        if (currentProcessor.getActiveNavigationPanel() != PANELS_LIBRARY_PANEL)
        {
            centrePanel.setActiveNavigationPanel(PANELS_LIBRARY_PANEL);
            mToggleSampleFolderPanelButton->setImages(false,
                                                      true,
                                                      true,
                                                      ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                                BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                      style->BUTTON_IS_DEFAULT_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS,
                                                      Image(),
                                                      style->BUTTON_IS_OVER_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS,
                                                      Image(),
                                                      style->BUTTON_IS_DOWN_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS);
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS,
                                                     Image(),
                                                     style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS,
                                                     Image(),
                                                     style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS);
            mToggleSampleGridPanelButton->setImages(false,
                                                    true,
                                                    true,
                                                    ImageCache::getFromMemory(BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_png,
                                                                              BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                    style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS,
                                                    Image(),
                                                    style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS,
                                                    Image(),
                                                    style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS);
        }
    };
    addAndMakeVisible(*mToggleSampleFolderPanelButton);
}

void HeaderPanel::setToggleSampleTablePanelButton(int buttonWidth, int x)
{
    mToggleSampleTablePanelButton = std::make_unique<BlomeImageButton>("Toggle SampleTablePanel", false);
    mToggleSampleTablePanelButton->setImages(false,
                                             true,
                                             true,
                                             ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                       BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                             currentProcessor.getActiveNavigationPanel() == PANELS_TABLE_PANEL
                                             ? style->BUTTON_IS_DEFAULT_ALPHA
                                             : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                             style->COLOUR_HEADER_BUTTONS,
                                             Image(),
                                             currentProcessor.getActiveNavigationPanel() == PANELS_TABLE_PANEL
                                             ? style->BUTTON_IS_OVER_ALPHA
                                             : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                             style->COLOUR_HEADER_BUTTONS,
                                             Image(),
                                             currentProcessor.getActiveNavigationPanel() == PANELS_TABLE_PANEL
                                             ? style->BUTTON_IS_DOWN_ALPHA
                                             : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                             style->COLOUR_HEADER_BUTTONS);
    mToggleSampleTablePanelButton->setBounds(x,
                                             style->PANEL_MARGIN,
                                             buttonWidth,
                                             buttonWidth);
    mToggleSampleTablePanelButton->setTooltip("Table View - Show a table with all samples in your library (2)");
    mToggleSampleTablePanelButton->onClick = [this]
    {
        if (currentProcessor.getActiveNavigationPanel() != PANELS_TABLE_PANEL)
        {
            centrePanel.setActiveNavigationPanel(PANELS_TABLE_PANEL);
            mToggleSampleFolderPanelButton->setImages(false,
                                                      true,
                                                      true,
                                                      ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                                BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                      style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS,
                                                      Image(),
                                                      style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS,
                                                      Image(),
                                                      style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS);
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     style->BUTTON_IS_DEFAULT_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS,
                                                     Image(),
                                                     style->BUTTON_IS_OVER_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS,
                                                     Image(),
                                                     style->BUTTON_IS_DOWN_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS);
            mToggleSampleGridPanelButton->setImages(false,
                                                    true,
                                                    true,
                                                    ImageCache::getFromMemory(BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_png,
                                                                              BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                    style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS,
                                                    Image(),
                                                    style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS,
                                                    Image(),
                                                    style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS);
        }
    };
    addAndMakeVisible(*mToggleSampleTablePanelButton);
}

void HeaderPanel::setToggleSampleGridPanelButton(int buttonWidth, int x)
{
    mToggleSampleGridPanelButton = std::make_unique<BlomeImageButton>("Toggle SampleTablePanel", false);
    mToggleSampleGridPanelButton->setImages(false,
                                            true,
                                            true,
                                            ImageCache::getFromMemory(BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_png,
                                                                      BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_pngSize),
                                            currentProcessor.getActiveNavigationPanel() == PANELS_GRID_PANEL
                                            ? style->BUTTON_IS_DEFAULT_ALPHA
                                            : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                            style->COLOUR_HEADER_BUTTONS,
                                            Image(),
                                            currentProcessor.getActiveNavigationPanel() == PANELS_GRID_PANEL
                                            ? style->BUTTON_IS_OVER_ALPHA
                                            : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                            style->COLOUR_HEADER_BUTTONS,
                                            Image(),
                                            currentProcessor.getActiveNavigationPanel() == PANELS_GRID_PANEL
                                            ? style->BUTTON_IS_DOWN_ALPHA
                                            : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                            style->COLOUR_HEADER_BUTTONS);
    mToggleSampleGridPanelButton->setBounds(x,
                                            style->PANEL_MARGIN,
                                            buttonWidth,
                                            buttonWidth);
    mToggleSampleGridPanelButton->setTooltip("Grid View - Show a grid with all samples clustered by similarity (3)");
    mToggleSampleGridPanelButton->onClick = [this]
    {
        if (currentProcessor.getActiveNavigationPanel() != PANELS_GRID_PANEL)
        {
            centrePanel.setActiveNavigationPanel(PANELS_GRID_PANEL);
            mToggleSampleFolderPanelButton->setImages(false,
                                                      true,
                                                      true,
                                                      ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                                BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                      style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS,
                                                      Image(),
                                                      style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS,
                                                      Image(),
                                                      style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                      style->COLOUR_HEADER_BUTTONS);
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS,
                                                     Image(),
                                                     style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS,
                                                     Image(),
                                                     style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                     style->COLOUR_HEADER_BUTTONS);
            mToggleSampleGridPanelButton->setImages(false,
                                                    true,
                                                    true,
                                                    ImageCache::getFromMemory(BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_png,
                                                                              BinaryData::grid_view_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                    style->BUTTON_IS_DEFAULT_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS,
                                                    Image(),
                                                    style->BUTTON_IS_OVER_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS,
                                                    Image(),
                                                    style->BUTTON_IS_DOWN_ALPHA,
                                                    style->COLOUR_HEADER_BUTTONS);
        }
    };
    addAndMakeVisible(*mToggleSampleGridPanelButton);
}

void HeaderPanel::setChangeFilterButton(int buttonWidth, int x)
{
    bool initialFilterIsActive = currentProcessor.getFilterIsActivated();
    mChangeFilterButton = std::make_unique<BlomeImageButton>("Filter", false);
    mChangeFilterButton->setImages(false,
                                   true,
                                   true,
                                   initialFilterIsActive
                                   ? ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                               BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize)
                                   : ImageCache::getFromMemory(BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_png,
                                                               BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   style->BUTTON_IS_DEFAULT_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS,
                                   Image(),
                                   style->BUTTON_IS_OVER_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS,
                                   Image(),
                                   style->BUTTON_IS_DOWN_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS);
    mChangeFilterButton->setBounds(x,
                                   style->PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mChangeFilterButton->setTooltip("Change filter settings and add filter rules");
    mChangeFilterButton->onClick = [this]
    {
        std::unique_ptr<FileFilterPanel> fileFilterPanel = std::make_unique<FileFilterPanel>(currentProcessor);
        CallOutBox::launchAsynchronously(std::move(fileFilterPanel), mChangeFilterButton->getScreenBounds(), nullptr);
    };
    addAndMakeVisible(*mChangeFilterButton);
}

void HeaderPanel::setToggleFilterButton(int buttonWidth, int x)
{
    bool initialFilterIsActive = currentProcessor.getFilterIsActivated();
    mToggleFilterButton = std::make_unique<BlomeImageButton>("ToggleFilterButton", false);
    mToggleFilterButton->setImages(false,
                                   true,
                                   true,
                                   ImageCache::getFromMemory(BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_png,
                                                             BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   initialFilterIsActive
                                   ? style->BUTTON_IS_DEFAULT_ALPHA
                                   : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS,
                                   Image(),
                                   initialFilterIsActive
                                   ? style->BUTTON_IS_OVER_ALPHA
                                   : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS,
                                   Image(),
                                   initialFilterIsActive ? style->BUTTON_IS_DOWN_ALPHA : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS);
    mToggleFilterButton->setBounds(x,
                                   style->PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mToggleFilterButton->setTooltip("Activate/deactivate filter (F)");
    mToggleFilterButton->onClick = [this]
    {
        bool couldHaveEffect = sampleLibrary.getFileFilter().canHaveEffect();
        bool filterIsActive = !sampleLibrary.getFileFilter().getIsActive();
        currentProcessor.setFilterIsActive(filterIsActive);
        sampleLibrary.getFileFilter().setIsActive(filterIsActive);
        
        if (sampleLibrary.getFileFilter().canHaveEffect() || couldHaveEffect)
        {
            sampleLibrary.refresh();
        }
        
        mChangeFilterButton->setImages(false,
                                       true,
                                       true,
                                       filterIsActive
                                       ? ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                                   BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize)
                                       : ImageCache::getFromMemory(BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_png,
                                                                   BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_pngSize),
                                       style->BUTTON_IS_DEFAULT_ALPHA,
                                       style->COLOUR_HEADER_BUTTONS,
                                       Image(),
                                       style->BUTTON_IS_OVER_ALPHA,
                                       style->COLOUR_HEADER_BUTTONS,
                                       Image(),
                                       style->BUTTON_IS_DOWN_ALPHA,
                                       style->COLOUR_HEADER_BUTTONS);
        mToggleFilterButton->setImages(false,
                                       true,
                                       true,
                                       ImageCache::getFromMemory(BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_pngSize),
                                       filterIsActive
                                       ? style->BUTTON_IS_DEFAULT_ALPHA
                                       : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                       style->COLOUR_HEADER_BUTTONS,
                                       Image(),
                                       filterIsActive
                                       ? style->BUTTON_IS_OVER_ALPHA
                                       : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                       style->COLOUR_HEADER_BUTTONS,
                                       Image(),
                                       filterIsActive ? style->BUTTON_IS_DOWN_ALPHA : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                       style->COLOUR_HEADER_BUTTONS);
    };
    addAndMakeVisible(*mToggleFilterButton);
    
    sampleLibrary.getFileFilter().setIsActive(initialFilterIsActive);
}

void HeaderPanel::setRandomSampleButton(int buttonWidth, int x)
{
    mRandomSampleButton = std::make_unique<BlomeImageButton>("RandomSampleButton", false);
    mRandomSampleButton->setImages(false,
                                   true,
                                   true,
                                   ImageCache::getFromMemory(BinaryData::ifl_FILL0_wght400_GRAD0_opsz24_png,
                                                             BinaryData::ifl_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   style->BUTTON_IS_DEFAULT_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS,
                                   Image(),
                                   style->BUTTON_IS_OVER_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS,
                                   Image(),
                                   style->BUTTON_IS_DOWN_ALPHA,
                                   style->COLOUR_HEADER_BUTTONS);
    mRandomSampleButton->setBounds(x,
                                   style->PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mRandomSampleButton->setTooltip("Selects a random sample in the table or grid view (S)");
    mRandomSampleButton->onClick = [this]
    {
        if (currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL)
        {
            mToggleSampleTablePanelButton->triggerClick();
        }
        
        centrePanel.selectRandomSample();
    };
    addAndMakeVisible(*mRandomSampleButton);
}

void HeaderPanel::setNormaliseVolumeButton(int buttonWidth)
{
    mNormaliseVolumeButton = std::make_unique<BlomeImageButton>("NormaliseVolumeButton", false);
    mNormaliseVolumeButton->setImages(false,
                                      true,
                                      true,
                                      ImageCache::getFromMemory(BinaryData::expand_24dp_000000_FILL0_wght400_GRAD0_opsz24_png,
                                                                BinaryData::expand_24dp_000000_FILL0_wght400_GRAD0_opsz24_pngSize),
                                      currentProcessor.getVolumeIsNormalised()
                                      ? style->BUTTON_IS_DEFAULT_ALPHA
                                      : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                      style->COLOUR_HEADER_BUTTONS,
                                      Image(),
                                      currentProcessor.getVolumeIsNormalised()
                                      ? style->BUTTON_IS_OVER_ALPHA
                                      : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                      style->COLOUR_HEADER_BUTTONS,
                                      Image(),
                                      currentProcessor.getVolumeIsNormalised()
                                      ? style->BUTTON_IS_DOWN_ALPHA
                                      : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                      style->COLOUR_HEADER_BUTTONS);
    mNormaliseVolumeButton->setSize(buttonWidth, buttonWidth);
    mNormaliseVolumeButton->setTooltip("Normalises the volume of the audio playback (N)");
    mNormaliseVolumeButton->onClick = [this]
    {
        centrePanel.toggleVolumeIsNormalised();
        
        mNormaliseVolumeButton->setImages(false,
                                          true,
                                          true,
                                          ImageCache::getFromMemory(BinaryData::expand_24dp_000000_FILL0_wght400_GRAD0_opsz24_png,
                                                                    BinaryData::expand_24dp_000000_FILL0_wght400_GRAD0_opsz24_pngSize),
                                          currentProcessor.getVolumeIsNormalised()
                                          ? style->BUTTON_IS_DEFAULT_ALPHA
                                          : style->BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                          style->COLOUR_HEADER_BUTTONS,
                                          Image(),
                                          currentProcessor.getVolumeIsNormalised()
                                          ? style->BUTTON_IS_OVER_ALPHA
                                          : style->BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                          style->COLOUR_HEADER_BUTTONS,
                                          Image(),
                                          currentProcessor.getVolumeIsNormalised()
                                          ? style->BUTTON_IS_DOWN_ALPHA
                                          : style->BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                          style->COLOUR_HEADER_BUTTONS);
    };
    addAndMakeVisible(*mNormaliseVolumeButton);
}

void HeaderPanel::setChooseOutputDeviceButton(int buttonWidth)
{
    mChooseOutputDeviceButton = std::make_unique<BlomeImageButton>("ChooseOutputDeviceButton", false);
    mChooseOutputDeviceButton->setImages(false,
                                         true,
                                         true,
                                         ImageCache::getFromMemory(BinaryData::volume_up_FILL0_wght400_GRAD0_opsz24_png,
                                                                   BinaryData::volume_up_FILL0_wght400_GRAD0_opsz24_pngSize),
                                         style->BUTTON_IS_DEFAULT_ALPHA,
                                         style->COLOUR_HEADER_BUTTONS,
                                         Image(),
                                         style->BUTTON_IS_OVER_ALPHA,
                                         style->COLOUR_HEADER_BUTTONS,
                                         Image(),
                                         style->BUTTON_IS_DOWN_ALPHA,
                                         style->COLOUR_HEADER_BUTTONS);
    mChooseOutputDeviceButton->setSize(buttonWidth, buttonWidth);
    mChooseOutputDeviceButton->setTooltip("Choose the output device for audio playback");
    mChooseOutputDeviceButton->onClick = [this]
    {
        PopupMenu popupMenu;
        
        // Add all output devices to popup menu
        for (AudioIODeviceType* deviceType : mAudioDeviceManager->getAvailableDeviceTypes())
        {
            for(String deviceName : deviceType->getDeviceNames())
            {
                popupMenu.addItem(deviceName, [this, deviceName] { selectOutputDevice(deviceName); });
            }
        }
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    };
    addAndMakeVisible(*mChooseOutputDeviceButton);
}

void HeaderPanel::setGainSlider()
{
    // Add zoom slider
    mGainSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mGainSlider->setRange(0, 1, 0);
    mGainSlider->setValue(currentProcessor.getOutputGain(), NotificationType::sendNotification);
    mGainSlider->onValueChange = [this]
    {
        centrePanel.setGain(mGainSlider->getValue());
    };
    mGainSlider->setSkewFactor(0.7);
    mGainSlider->setTooltip("Sets the gain of the audio playback (after the normalisation)");
    addAndMakeVisible(*mGainSlider);
}

void HeaderPanel::setShowAboutPanelButton()
{
    mShowAboutPanelButton = std::make_unique<BlomeTransparentButton>("ShowAboutPanelButton");
    mShowAboutPanelButton->setTooltip("Press to show the About section");
    mShowAboutPanelButton->onClick = [this]
    {
        std::unique_ptr<AboutPanel> aboutPanel = std::make_unique<AboutPanel>();
        CallOutBox::launchAsynchronously(std::move(aboutPanel), mShowAboutPanelButton->getScreenBounds(), nullptr);
    };
    addAndMakeVisible(*mShowAboutPanelButton);
}

void HeaderPanel::setPanelComponents()
{
    int x = style->PANEL_MARGIN;
    int buttonWidth = getHeight() - style->PANEL_MARGIN * 2.0;
    int groupDistance = 10;
    
    // Add refresh sample library button
    setRefreshLibraryButton(buttonWidth, x);
    x += buttonWidth + style->PANEL_MARGIN * 0.5;
    
    // Add choose library directory button
    setChooseLibraryDirectoryButton(buttonWidth, x);
    x += buttonWidth + style->PANEL_MARGIN * 1.5 + groupDistance;
    
    // Add toggle for library panel
    setToggleSampleFolderPanelButton(buttonWidth, x);
    x += buttonWidth + style->PANEL_MARGIN * 0.5;
    
    // Add toggle for sample table panel
    setToggleSampleTablePanelButton(buttonWidth, x);
    x += buttonWidth + style->PANEL_MARGIN * 0.5;
    
    // Add toggle for sample table panel
    setToggleSampleGridPanelButton(buttonWidth, x);
    x += buttonWidth + style->PANEL_MARGIN * 1.5 + groupDistance;
    
    // Add button for editing the file filter rules
    setChangeFilterButton(buttonWidth, x);
    x += buttonWidth + style->PANEL_MARGIN * 0.5;
    
    // Add button for toggling filter on and off
    setToggleFilterButton(buttonWidth, x);
    x += buttonWidth + style->PANEL_MARGIN * 1.5 + groupDistance;
    
    // Add button to select random sample on the table panel
    setRandomSampleButton(buttonWidth, x);
    
    // Add button to normalise playback volume
    setNormaliseVolumeButton(buttonWidth - 1);
    
    // Add combo box to choose output device
    setChooseOutputDeviceButton(buttonWidth);
    
    // Add gain slider
    setGainSlider();
    
    // Add button above logo text to show the "about" panel
    setShowAboutPanelButton();
    
    // Resize components
    resizePanelComponents();
}

void HeaderPanel::showLibraryChooser()
{
    mFileChooser.reset(new FileChooser("Choose a sample library directory",
                                       File::getCurrentWorkingDirectory(),
                                       "*",
                                       true));
    
    mFileChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
                              [&] (FileChooser const & chooser)
                              {
        URL result = chooser.getURLResult();
        String name = result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(true);
        
        if (name != "")
        {
            sampleLibrary.setDirectory(name);
            currentProcessor.setLastOpenedLibraryPath(name);
        }
        
        mFileChooser.reset();
    });
}

bool HeaderPanel::keyPressed(KeyPress const & key)
{
    int keyCode = key.getKeyCode();
    
    if (keyCode == 49) // 1
    {
        mToggleSampleFolderPanelButton->triggerClick();
        return true;
    }
    else if (keyCode == 50) // 2
    {
        mToggleSampleTablePanelButton->triggerClick();
        return true;
    }
    else if (keyCode == 51) // 3
    {
        mToggleSampleGridPanelButton->triggerClick();
        return true;
    }
    else if (keyCode == 68) // D
    {
        mChooseLibraryDirectoryButton->triggerClick();
        return true;
    }
    else if (keyCode == 70) // F
    {
        mToggleFilterButton->triggerClick();
        return true;
    }
    else if (keyCode == 78) // N
    {
        mNormaliseVolumeButton->triggerClick();
        return true;
    }
    else if (keyCode == 82) // R
    {
        mRefreshLibraryButton->triggerClick();
        return true;
    }
    else if (keyCode == 83) // S
    {
        mRandomSampleButton->triggerClick();
        return true;
    }
    
    return false;
}

void HeaderPanel::resizePanelComponents()
{
    int buttonWidth = getHeight() - style->PANEL_MARGIN * 2.0;
    
    if (mNormaliseVolumeButton != nullptr)
    {
        mNormaliseVolumeButton->setTopLeftPosition(getWidth()
                                                   - logoMarginRight
                                                   - logoWidth
                                                   - sliderWidth
                                                   - style->PANEL_MARGIN * 3.5
                                                   - buttonWidth * 2,
                                                   style->PANEL_MARGIN + 2);
    }
    
    if (mChooseOutputDeviceButton != nullptr)
    {
        mChooseOutputDeviceButton->setTopLeftPosition(getWidth()
                                                      - logoMarginRight
                                                      - logoWidth
                                                      - sliderWidth
                                                      - style->PANEL_MARGIN * 2.5
                                                      - buttonWidth,
                                                      style->PANEL_MARGIN);
    }
    
    if (mGainSlider != nullptr)
    {
        mGainSlider->setBounds(getWidth()
                               - logoMarginRight
                               - logoWidth
                               - sliderWidth
                               - style->PANEL_MARGIN * 3,
                               style->PANEL_MARGIN,
                               sliderWidth,
                               getHeight() - style->PANEL_MARGIN * 1.75);
    }
    
    if (mShowAboutPanelButton != nullptr)
    {
        mShowAboutPanelButton->setBounds(getWidth()
                                         - logoMarginRight
                                         - logoWidth
                                         - style->PANEL_MARGIN,
                                         style->PANEL_MARGIN,
                                         logoWidth,
                                         getHeight() - style->PANEL_MARGIN * 1.75);
    }
}

void HeaderPanel::selectOutputDevice(String inDeviceName)
{
    centrePanel.selectOutputDevice(inDeviceName);
}
