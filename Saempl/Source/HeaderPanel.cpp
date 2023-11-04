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
    
    // Draw library control button area background
    g.setColour(COLOUR_BLACK_LIGHT_TRANSPARENT);
    int backgroundWidth = (getHeight() - PANEL_MARGIN * 2.0) * 2.0 + PANEL_MARGIN * 1.5;
    g.fillRoundedRectangle(PANEL_MARGIN / 2.0,
                           PANEL_MARGIN / 2.0,
                           backgroundWidth,
                           getHeight() - PANEL_MARGIN ,
                           CORNER_SIZE_MEDIUM);
    
    // Draw navigation panel button area background
    g.fillRoundedRectangle(backgroundWidth + PANEL_MARGIN,
                           PANEL_MARGIN / 2.0,
                           backgroundWidth,
                           getHeight() - PANEL_MARGIN ,
                           CORNER_SIZE_MEDIUM);
    
    // Draw filter button area background
    g.fillRoundedRectangle(backgroundWidth * 2.0 + PANEL_MARGIN * 1.5,
                           PANEL_MARGIN / 2.0,
                           backgroundWidth,
                           getHeight() - PANEL_MARGIN ,
                           CORNER_SIZE_MEDIUM);
    
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
    mRefreshLibraryButton = std::make_unique<ImageButton>("Refresh");
    mRefreshLibraryButton->setImages(false,
                                     true,
                                     true,
                                     ImageCache::getFromMemory(BinaryData::sync_FILL0_wght400_GRAD0_opsz24_png,
                                                               BinaryData::sync_FILL0_wght400_GRAD0_opsz24_pngSize),
                                     1.0,
                                     COLOUR_ACCENT_LIGHT,
                                     Image(),
                                     0.35,
                                     Colour(),
                                     Image(),
                                     0.5,
                                     Colour());
    mRefreshLibraryButton->setBounds(x,
                                     PANEL_MARGIN,
                                     buttonWidth,
                                     buttonWidth);
    mRefreshLibraryButton->onClick = [this] { currentProcessor.getSampleLibrary().refresh(); };
    addAndMakeVisible(*mRefreshLibraryButton);
    x += buttonWidth + PANEL_MARGIN / 2.0;
    
    // Add choose library directory button
    mChooseLibraryFolderButton = std::make_unique<ImageButton>("Choose Dir.");
    mChooseLibraryFolderButton->setImages(false,
                                          true,
                                          true,
                                          ImageCache::getFromMemory(BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_png,
                                                                    BinaryData::folder_open_FILL0_wght400_GRAD0_opsz24_pngSize),
                                          1.0,
                                          COLOUR_ACCENT_LIGHT,
                                          Image(),
                                          0.35,
                                          Colour(),
                                          Image(),
                                          0.5,
                                          Colour());
    mChooseLibraryFolderButton->setBounds(x,
                                          PANEL_MARGIN,
                                          buttonWidth,
                                          buttonWidth);
    mChooseLibraryFolderButton->onClick = [this] { showLibraryChooser(); };
    addAndMakeVisible(*mChooseLibraryFolderButton);
    x += buttonWidth + PANEL_MARGIN * 1.5;
    
    // Add toggle for library panel
    mToggleLibraryPanelButton = std::make_unique<ImageButton>("Toggle SampleLibraryPanel");
    mToggleLibraryPanelButton->setImages(false,
                                         true,
                                         true,
                                         ImageCache::getFromMemory(BinaryData::segment_FILL0_wght400_GRAD0_opsz24_png,
                                                                   BinaryData::segment_FILL0_wght400_GRAD0_opsz24_pngSize),
                                         1.0,
                                         COLOUR_ACCENT_LIGHT,
                                         Image(),
                                         0.35,
                                         Colour(),
                                         Image(),
                                         0.5,
                                         Colour());
    mToggleLibraryPanelButton->setBounds(x,
                                         PANEL_MARGIN,
                                         buttonWidth,
                                         buttonWidth);
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
                                                 COLOUR_ACCENT_LIGHT,
                                                 Image(),
                                                 0.35,
                                                 Colour(),
                                                 Image(),
                                                 0.5,
                                                 Colour());
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     0.35,
                                                     COLOUR_BLACK,
                                                     Image(),
                                                     0.25,
                                                     Colour(),
                                                     Image(),
                                                     0.35,
                                                     Colour());
        }
    };
    addAndMakeVisible(*mToggleLibraryPanelButton);
    x += buttonWidth + PANEL_MARGIN / 2.0;
    
    // Add toggle for sample table panel
    mToggleSampleTablePanelButton = std::make_unique<ImageButton>("Toggle SampleTablePanel");
    mToggleSampleTablePanelButton->setImages(false,
                                             true,
                                             true,
                                             ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                       BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                             0.35,
                                             COLOUR_BLACK,
                                             Image(),
                                             0.25,
                                             Colour(),
                                             Image(),
                                             0.35,
                                             Colour());
    mToggleSampleTablePanelButton->setBounds(x,
                                             PANEL_MARGIN,
                                             buttonWidth,
                                             buttonWidth);
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
                                                 0.25,
                                                 Colour(),
                                                 Image(),
                                                 0.35,
                                                 Colour());
            mToggleSampleTablePanelButton->setImages(false,
                                                     true,
                                                     true,
                                                     ImageCache::getFromMemory(BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_png,
                                                                               BinaryData::table_rows_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                     1.0,
                                                     COLOUR_ACCENT_LIGHT,
                                                     Image(),
                                                     0.35,
                                                     Colour(),
                                                     Image(),
                                                     0.5,
                                                     Colour());
        }
    };
    addAndMakeVisible(*mToggleSampleTablePanelButton);
    x += buttonWidth + PANEL_MARGIN * 1.5;
    
    // Add button for editing the file filter rules
    mChangeFilterButton = std::make_unique<ImageButton>("Filter");
    mChangeFilterButton->setImages(false,
                                   true,
                                   false,
                                   ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                             BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize),
                                   1.0,
                                   COLOUR_ACCENT_LIGHT,
                                   Image(),
                                   0.35,
                                   Colour(),
                                   Image(),
                                   0.5,
                                   Colour());
    mChangeFilterButton->setBounds(x,
                                   PANEL_MARGIN,
                                   buttonWidth,
                                   buttonWidth);
    mChangeFilterButton->onClick = [this]
    {
        std::unique_ptr<FileFilterPanel> fileFilterPanel = std::make_unique<FileFilterPanel>(currentProcessor
                                                                                             .getSampleLibrary());
        CallOutBox::launchAsynchronously(std::move(fileFilterPanel), mChangeFilterButton->getScreenBounds(), nullptr);
    };
    addAndMakeVisible(*mChangeFilterButton);
    x += buttonWidth + PANEL_MARGIN / 2.0;
    
    // Add button for toggling filter on and off
    mActivateFilterButton = std::make_unique<ToggleButton>("ToggleFilterButton");
    mActivateFilterButton->setBounds(x,
                                     PANEL_MARGIN,
                                     buttonWidth,
                                     buttonWidth);
    mActivateFilterButton->setToggleState(currentProcessor
                                          .getSampleLibrary()
                                          .getFileFilter()
                                          .getIsActive(),
                                          NotificationType::dontSendNotification);
    mActivateFilterButton->onClick = [this]
    {
        currentProcessor.getSampleLibrary().getFileFilter().setIsActive(mActivateFilterButton->getToggleState());
        currentProcessor.getSampleLibrary().refresh();
        mChangeFilterButton->setImages(false,
                                       true,
                                       true,
                                       mActivateFilterButton->getToggleState() ?
                                       ImageCache::getFromMemory(BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::filter_alt_FILL0_wght400_GRAD0_opsz24_pngSize) :
                                       ImageCache::getFromMemory(BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_png,
                                                                 BinaryData::filter_alt_off_FILL0_wght400_GRAD0_opsz24_pngSize),
                                       1.0,
                                       COLOUR_ACCENT_LIGHT,
                                       Image(),
                                       0.35,
                                       Colour(),
                                       Image(),
                                       0.5,
                                       Colour());
    };
    addAndMakeVisible(*mActivateFilterButton);
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
