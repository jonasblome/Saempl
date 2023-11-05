/*
 ==============================================================================
 
 HeaderPanel.cpp
 Created: 27 May 2023 11:12:36am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "HeaderPanel.h"

HeaderPanel::HeaderPanel(SaemplAudioProcessor& inProcessor, CenterPanel& inCenterPanel)
:   PanelBase(),
currentProcessor(inProcessor),
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
    // g.drawLine(235, getHeight() / 2.0, 245, getHeight() / 2.0, 2.0);
    g.fillRoundedRectangle(235, getHeight() / 2.0 - 1, 10, 2, 1.0);
    
    // Draw logo text
    int const logoWidth = 220;
    g.setColour(COLOUR_ACCENT_DARK);
    g.setFont(FONT_MEDIUM);
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
    
    // Add refresh sample library button
    mRefreshLibraryButton = std::make_unique<BlomeImageButton>("Refresh", false);
    mRefreshLibraryButton->setImages(false,
                                     true,
                                     true,
                                     ImageCache::getFromMemory(BinaryData::sync_FILL0_wght400_GRAD0_opsz24_png,
                                                               BinaryData::sync_FILL0_wght400_GRAD0_opsz24_pngSize),
                                     1.0,
                                     COLOUR_ACCENT_DARK,
                                     Image(),
                                     0.35,
                                     COLOUR_ACCENT_DARK,
                                     Image(),
                                     0.5,
                                     COLOUR_ACCENT_DARK);
    mRefreshLibraryButton->setBounds(x,
                                     PANEL_MARGIN,
                                     buttonWidth,
                                     buttonWidth);
    mRefreshLibraryButton->setTooltip("Scans for new files and removes externally deleted files");
    mRefreshLibraryButton->onClick = [this] { currentProcessor.getSampleLibrary().refresh(); };
    addAndMakeVisible(*mRefreshLibraryButton);
    x += buttonWidth + PANEL_MARGIN / 2.0;
    
    // Add choose library directory button
    mChooseLibraryFolderButton = std::make_unique<BlomeImageButton>("Choose Dir.", false);
    mChooseLibraryFolderButton->setImages(false,
                                          true,
                                          true,
                                          ImageCache::getFromMemory(BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_png,
                                                                    BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_pngSize),
                                          1.0,
                                          COLOUR_ACCENT_DARK,
                                          Image(),
                                          0.35,
                                          COLOUR_ACCENT_DARK,
                                          Image(),
                                          0.5,
                                          COLOUR_ACCENT_DARK);
    mChooseLibraryFolderButton->setBounds(x,
                                          PANEL_MARGIN,
                                          buttonWidth,
                                          buttonWidth);
    mChooseLibraryFolderButton->setTooltip("Choose a new directory as your current sample library");
    mChooseLibraryFolderButton->onClick = [this] { showLibraryChooser(); };
    addAndMakeVisible(*mChooseLibraryFolderButton);
    x += buttonWidth + PANEL_MARGIN * 1.5 + 25;
    
    // Add toggle for library panel
    mToggleLibraryPanelButton = std::make_unique<BlomeImageButton>("Toggle SampleLibraryPanel", false);
    mToggleLibraryPanelButton->setImages(false,
                                         true,
                                         true,
                                         ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                   BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                         1.0,
                                         COLOUR_ACCENT_DARK,
                                         Image(),
                                         0.35,
                                         COLOUR_ACCENT_DARK,
                                         Image(),
                                         0.5,
                                         COLOUR_ACCENT_DARK);
    mToggleLibraryPanelButton->setBounds(x,
                                         PANEL_MARGIN,
                                         buttonWidth,
                                         buttonWidth);
    mToggleLibraryPanelButton->setTooltip("Show the file tree view of your library directory");
    mToggleLibraryPanelButton->onClick = [this]
    {
        if (linkedCenterPanel.getActiveNavigationPanelType() == PANELS_TABLE_PANEL)
        {
            linkedCenterPanel.showNavigationPanel(PANELS_LIBRARY_PANEL);
            mToggleLibraryPanelButton->setImages(false,
                                                 true,
                                                 true,
                                                 ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                           BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                 1.0,
                                                 COLOUR_ACCENT_DARK,
                                                 Image(),
                                                 0.35,
                                                 COLOUR_ACCENT_DARK,
                                                 Image(),
                                                 0.5,
                                                 COLOUR_ACCENT_DARK);
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     0.35,
                                                     COLOUR_BLACK,
                                                     Image(),
                                                     0.15,
                                                     COLOUR_BLACK,
                                                     Image(),
                                                     0.35,
                                                     COLOUR_BLACK);
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
                                             0.35,
                                             COLOUR_BLACK,
                                             Image(),
                                             0.15,
                                             COLOUR_BLACK,
                                             Image(),
                                             0.35,
                                             COLOUR_BLACK);
    mToggleSampleTablePanelButton->setBounds(x,
                                             PANEL_MARGIN,
                                             buttonWidth,
                                             buttonWidth);
    mToggleSampleTablePanelButton->setTooltip("Show table with all samples in your library");
    mToggleSampleTablePanelButton->onClick = [this]
    {
        if (linkedCenterPanel.getActiveNavigationPanelType() == PANELS_LIBRARY_PANEL)
        {
            linkedCenterPanel.showNavigationPanel(PANELS_TABLE_PANEL);
            mToggleLibraryPanelButton->setImages(false,
                                                 true,
                                                 true,
                                                 ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                           BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                 0.35,
                                                 COLOUR_BLACK,
                                                 Image(),
                                                 0.15,
                                                 COLOUR_BLACK,
                                                 Image(),
                                                 0.35,
                                                 COLOUR_BLACK);
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     1.0,
                                                     COLOUR_ACCENT_DARK,
                                                     Image(),
                                                     0.35,
                                                     COLOUR_ACCENT_DARK,
                                                     Image(),
                                                     0.5,
                                                     COLOUR_ACCENT_DARK);
        }
    };
    addAndMakeVisible(*mToggleSampleTablePanelButton);
    x += buttonWidth + PANEL_MARGIN * 1.5 + 25;
    
    // Add button for editing the file filter rules
    mChangeFilterButton = std::make_unique<BlomeImageButton>("Filter", false);
    mChangeFilterButton->setImages(false,
                                   true,
                                   false,
                                   ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                             BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   1.0,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   0.35,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   0.5,
                                   COLOUR_ACCENT_DARK);
    mChangeFilterButton->setBounds(x,
                                   PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mChangeFilterButton->setTooltip("Change filter settings and add filter rules");
    mChangeFilterButton->onClick = [this]
    {
        std::unique_ptr<FileFilterPanel> fileFilterPanel = std::make_unique<FileFilterPanel>(currentProcessor
                                                                                             .getSampleLibrary());
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
                                   1.0,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   0.35,
                                   COLOUR_ACCENT_DARK,
                                   Image(),
                                   0.5,
                                   COLOUR_ACCENT_DARK);
    mToggleFilterButton->setBounds(x,
                                   PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mToggleFilterButton->setTooltip("Activate/deactivate filter");
    mToggleFilterButton->onClick = [this]
    {
        bool filterIsActive = !currentProcessor.getSampleLibrary().getFileFilter().getIsActive();
        currentProcessor.getSampleLibrary().getFileFilter().setIsActive(filterIsActive);
        currentProcessor.getSampleLibrary().refresh();
        mChangeFilterButton->setImages(false,
                                       true,
                                       true,
                                       filterIsActive ?
                                       ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize) :
                                       ImageCache::getFromMemory(BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_pngSize),
                                       1.0,
                                       COLOUR_ACCENT_DARK,
                                       Image(),
                                       0.35,
                                       COLOUR_ACCENT_DARK,
                                       Image(),
                                       0.5,
                                       COLOUR_ACCENT_DARK);
        mToggleFilterButton->setImages(false,
                                       true,
                                       true,
                                       ImageCache::getFromMemory(BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::power_settings_new_FILL0_wght400_GRAD0_opsz24_pngSize),
                                       filterIsActive ? 1.0 : 0.35,
                                       filterIsActive
                                       ? COLOUR_ACCENT_DARK
                                       : COLOUR_BLACK,
                                       Image(),
                                       filterIsActive ? 0.35 : 0.15,
                                       filterIsActive
                                       ? COLOUR_ACCENT_DARK
                                       : COLOUR_BLACK,
                                       Image(),
                                       filterIsActive ? 0.5 : 0.35,
                                       filterIsActive
                                       ? COLOUR_ACCENT_DARK
                                       : COLOUR_BLACK);
    };
    addAndMakeVisible(*mToggleFilterButton);
}

void HeaderPanel::showLibraryChooser()
{
    mFileChooser.reset(new FileChooser("Choose a sample library directory",
                                       File::getCurrentWorkingDirectory(),
                                       "*",
                                       true));
    
    mFileChooser->launchAsync
    (
     FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
     [&] (FileChooser const & chooser)
     {
         URL result = chooser.getURLResult();
         String name = result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(true);
         
         if (name != "")
         {
             currentProcessor.getSampleLibrary().setDirectory(name);
             
             // Show success popup message
             AlertWindow::showAsync(MessageBoxOptions()
                                    .withIconType(MessageBoxIconType::NoIcon)
                                    .withTitle("Sample Library Chooser")
                                    .withMessage("You picked: " + name)
                                    .withButton("OK"),
                                    nullptr);
         }
         
         mFileChooser.reset();
     }
     );
}
