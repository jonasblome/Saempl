/*
 ==============================================================================
 
 HeaderPanel.cpp
 Created: 27 May 2023 11:12:36am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "HeaderPanel.h"

HeaderPanel::HeaderPanel(SaemplAudioProcessor& inProcessor, CenterPanel& inCenterPanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
linkedCenterPanel(inCenterPanel)
{
    setSize(HEADER_PANEL_WIDTH - PANEL_MARGIN, HEADER_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

HeaderPanel::~HeaderPanel()
{
    
}

void HeaderPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(COLOUR_ACCENT_LIGHT);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    
    g.setColour(COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(205, getHeight() / 2.0 - 1, 10, 2, 1.0);
    
    // Draw logo text
    int const logoWidth = 220;
    g.setColour(COLOUR_ACCENT_DARK);
    g.setFont(FONT_MEDIUM_BOLD);
    g.drawFittedText("Blome Audio",
                     HEADER_PANEL_WIDTH - logoWidth - 20 - PANEL_MARGIN,
                     PANEL_MARGIN / 2.0,
                     logoWidth,
                     getHeight() - PANEL_MARGIN,
                     Justification::centredRight,
                     1);
}

void HeaderPanel::setPanelComponents()
{
    int x = PANEL_MARGIN;
    int buttonWidth = getHeight() - PANEL_MARGIN * 2.0;
    int groupDistance = 10;
    
    // Add refresh sample library button
    mRefreshLibraryButton = std::make_unique<BlomeImageButton>("Refresh", false);
    mRefreshLibraryButton->setImages(false,
                                     true,
                                     true,
                                     ImageCache::getFromMemory(BinaryData::sync_FILL0_wght400_GRAD0_opsz24_png,
                                                               BinaryData::sync_FILL0_wght400_GRAD0_opsz24_pngSize),
                                     BUTTON_IS_DEFAULT_ALPHA,
                                     COLOUR_ACCENT_DARK,
                                     Image(),
                                     BUTTON_IS_OVER_ALPHA,
                                     COLOUR_ACCENT_DARK,
                                     Image(),
                                     BUTTON_IS_DOWN_ALPHA,
                                     COLOUR_ACCENT_DARK);
    mRefreshLibraryButton->setBounds(x,
                                     PANEL_MARGIN,
                                     buttonWidth,
                                     buttonWidth);
    mRefreshLibraryButton->setTooltip("Scans for new files and removes externally deleted files");
    mRefreshLibraryButton->onClick = [this] { sampleLibrary.synchWithLibraryDirectory(); };
    addAndMakeVisible(*mRefreshLibraryButton);
    x += buttonWidth + PANEL_MARGIN / 2.0;
    
    // Add choose library directory button
    mChooseLibraryFolderButton = std::make_unique<BlomeImageButton>("Choose Dir.", false);
    mChooseLibraryFolderButton->setImages(false,
                                          true,
                                          true,
                                          ImageCache::getFromMemory(BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_png,
                                                                    BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_pngSize),
                                          BUTTON_IS_DEFAULT_ALPHA,
                                          COLOUR_ACCENT_DARK,
                                          Image(),
                                          BUTTON_IS_OVER_ALPHA,
                                          COLOUR_ACCENT_DARK,
                                          Image(),
                                          BUTTON_IS_DOWN_ALPHA,
                                          COLOUR_ACCENT_DARK);
    mChooseLibraryFolderButton->setBounds(x,
                                          PANEL_MARGIN,
                                          buttonWidth,
                                          buttonWidth);
    mChooseLibraryFolderButton->setTooltip("Choose a new directory as your current sample library");
    mChooseLibraryFolderButton->onClick = [this] { showLibraryChooser(); };
    addAndMakeVisible(*mChooseLibraryFolderButton);
    x += buttonWidth + PANEL_MARGIN * 1.5 + groupDistance;
    
    // Add toggle for library panel
    mToggleLibraryPanelButton = std::make_unique<BlomeImageButton>("Toggle SampleLibraryPanel", false);
    mToggleLibraryPanelButton->setImages(false,
                                         true,
                                         true,
                                         ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                   BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                         currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL
                                         ? BUTTON_IS_DEFAULT_ALPHA
                                         : BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                         COLOUR_ACCENT_DARK,
                                         Image(),
                                         currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL
                                         ? BUTTON_IS_OVER_ALPHA
                                         : BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                         COLOUR_ACCENT_DARK,
                                         Image(),
                                         currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL
                                         ? BUTTON_IS_DOWN_ALPHA
                                         : BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                         COLOUR_ACCENT_DARK);
    mToggleLibraryPanelButton->setBounds(x,
                                         PANEL_MARGIN,
                                         buttonWidth,
                                         buttonWidth);
    mToggleLibraryPanelButton->setTooltip("Show the folder structure of your library directory");
    mToggleLibraryPanelButton->onClick = [this]
    {
        if (currentProcessor.getActiveNavigationPanel() == PANELS_TABLE_PANEL)
        {
            linkedCenterPanel.setActiveNavigationPanel(PANELS_LIBRARY_PANEL);
            mToggleLibraryPanelButton->setImages(false,
                                                 true,
                                                 true,
                                                 ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                           BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                 BUTTON_IS_DEFAULT_ALPHA,
                                                 COLOUR_ACCENT_DARK,
                                                 Image(),
                                                 BUTTON_IS_OVER_ALPHA,
                                                 COLOUR_ACCENT_DARK,
                                                 Image(),
                                                 BUTTON_IS_DOWN_ALPHA,
                                                 COLOUR_ACCENT_DARK);
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                     COLOUR_ACCENT_DARK,
                                                     Image(),
                                                     BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                     COLOUR_ACCENT_DARK,
                                                     Image(),
                                                     BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                     COLOUR_ACCENT_DARK);
        }
    };
    addAndMakeVisible(*mToggleLibraryPanelButton);
    x += buttonWidth + PANEL_MARGIN / 2.0;
    
    // Add toggle for sample table panel
    mToggleSampleTablePanelButton = std::make_unique<BlomeImageButton>("Toggle SampleTablePanel", false);
    mToggleSampleTablePanelButton->setImages(false,
                                             true,
                                             true,
                                             ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                       BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                             currentProcessor.getActiveNavigationPanel() == PANELS_TABLE_PANEL
                                             ? BUTTON_IS_DEFAULT_ALPHA
                                             : BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                             COLOUR_ACCENT_DARK,
                                             Image(),
                                             currentProcessor.getActiveNavigationPanel() == PANELS_TABLE_PANEL
                                             ? BUTTON_IS_OVER_ALPHA
                                             : BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                             COLOUR_ACCENT_DARK,
                                             Image(),
                                             currentProcessor.getActiveNavigationPanel() == PANELS_TABLE_PANEL
                                             ? BUTTON_IS_DOWN_ALPHA
                                             : BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                             COLOUR_ACCENT_DARK);
    mToggleSampleTablePanelButton->setBounds(x,
                                             PANEL_MARGIN,
                                             buttonWidth,
                                             buttonWidth);
    mToggleSampleTablePanelButton->setTooltip("Show table with all samples in your library");
    mToggleSampleTablePanelButton->onClick = [this]
    {
        if (currentProcessor.getActiveNavigationPanel() == PANELS_LIBRARY_PANEL)
        {
            linkedCenterPanel.setActiveNavigationPanel(PANELS_TABLE_PANEL);
            mToggleLibraryPanelButton->setImages(false,
                                                 true,
                                                 true,
                                                 ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                           BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                 BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                                 COLOUR_ACCENT_DARK,
                                                 Image(),
                                                 BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                                 COLOUR_ACCENT_DARK,
                                                 Image(),
                                                 BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                                 COLOUR_ACCENT_DARK);
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     BUTTON_IS_DEFAULT_ALPHA,
                                                     COLOUR_ACCENT_DARK,
                                                     Image(),
                                                     BUTTON_IS_OVER_ALPHA,
                                                     COLOUR_ACCENT_DARK,
                                                     Image(),
                                                     BUTTON_IS_DOWN_ALPHA,
                                                     COLOUR_ACCENT_DARK);
        }
    };
    addAndMakeVisible(*mToggleSampleTablePanelButton);
    x += buttonWidth + PANEL_MARGIN * 1.5 + groupDistance;
    
    // Add button for editing the file filter rules
    mChangeFilterButton = std::make_unique<BlomeImageButton>("Filter", false);
    mChangeFilterButton->setImages(false,
                                   true,
                                   false,
                                   ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                             BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   BUTTON_IS_DEFAULT_ALPHA,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   BUTTON_IS_OVER_ALPHA,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   BUTTON_IS_DOWN_ALPHA,
                                   COLOUR_ACCENT_DARK);
    mChangeFilterButton->setBounds(x,
                                   PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mChangeFilterButton->setTooltip("Change filter settings and add filter rules");
    mChangeFilterButton->onClick = [this]
    {
        std::unique_ptr<FileFilterPanel> fileFilterPanel = std::make_unique<FileFilterPanel>(currentProcessor);
        CallOutBox::launchAsynchronously(std::move(fileFilterPanel), mChangeFilterButton->getScreenBounds(), nullptr);
    };
    addAndMakeVisible(*mChangeFilterButton);
    x += buttonWidth + PANEL_MARGIN / 2.0;
    
    // Add button for toggling filter on and off
    mToggleFilterButton = std::make_unique<BlomeImageButton>("ToggleFilterButton", false);
    mToggleFilterButton->setImages(false,
                                   true,
                                   true,
                                   ImageCache::getFromMemory(BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_png,
                                                             BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   BUTTON_IS_DEFAULT_ALPHA,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   BUTTON_IS_OVER_ALPHA,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   BUTTON_IS_DOWN_ALPHA,
                                   COLOUR_ACCENT_DARK);
    mToggleFilterButton->setBounds(x,
                                   PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mToggleFilterButton->setTooltip("Activate/deactivate filter");
    mToggleFilterButton->onClick = [this]
    {
        bool filterIsActive = !sampleLibrary.getFileFilter().getIsActive();
        sampleLibrary.getFileFilter().setIsActive(filterIsActive);
        sampleLibrary.refresh();
        mChangeFilterButton->setImages(false,
                                       true,
                                       true,
                                       filterIsActive ?
                                       ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize) :
                                       ImageCache::getFromMemory(BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_pngSize),
                                       BUTTON_IS_DEFAULT_ALPHA,
                                       COLOUR_ACCENT_DARK,
                                       Image(),
                                       BUTTON_IS_OVER_ALPHA,
                                       COLOUR_ACCENT_DARK,
                                       Image(),
                                       BUTTON_IS_DOWN_ALPHA,
                                       COLOUR_ACCENT_DARK);
        mToggleFilterButton->setImages(false,
                                       true,
                                       true,
                                       ImageCache::getFromMemory(BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_pngSize),
                                       filterIsActive ? BUTTON_IS_DEFAULT_ALPHA : BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA,
                                       COLOUR_ACCENT_DARK,
                                       Image(),
                                       filterIsActive ? BUTTON_IS_OVER_ALPHA : BUTTON_IS_OVER_DEACTIVATED_ALPHA,
                                       COLOUR_ACCENT_DARK,
                                       Image(),
                                       filterIsActive ? BUTTON_IS_DOWN_ALPHA : BUTTON_IS_DOWN_DEACTIVATED_ALPHA,
                                       COLOUR_ACCENT_DARK);
    };
    addAndMakeVisible(*mToggleFilterButton);
    x += buttonWidth + PANEL_MARGIN * 1.5 + groupDistance;
    
    mRandomSampleButton = std::make_unique<BlomeImageButton>("RandomSampleButton", false);
    mRandomSampleButton->setImages(false,
                                   true,
                                   true,
                                   ImageCache::getFromMemory(BinaryData::ifl_FILL0_wght400_GRAD0_opsz24_png,
                                                             BinaryData::ifl_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   BUTTON_IS_DEFAULT_ALPHA,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   BUTTON_IS_OVER_ALPHA,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   BUTTON_IS_DOWN_ALPHA,
                                   COLOUR_ACCENT_DARK);
    mRandomSampleButton->setBounds(x,
                                   PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mRandomSampleButton->setTooltip("Opens the table view and selects a random sample");
    mRandomSampleButton->onClick = [this]
    {
        mToggleSampleTablePanelButton->triggerClick();
        linkedCenterPanel.selectRandomSample();
    };
    addAndMakeVisible(*mRandomSampleButton);
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
            
            // Show success popup message
            AlertWindow::showAsync(MessageBoxOptions()
                                   .withIconType(MessageBoxIconType::NoIcon)
                                   .withTitle("Sample Library Chooser")
                                   .withMessage("You picked: " + name)
                                   .withButton("OK"),
                                   nullptr);
        }
        
        mFileChooser.reset();
    });
}
