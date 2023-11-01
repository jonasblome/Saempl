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
    int x = 0;
    
    // Add refresh sample library button
    int textButtonWidth = 75;
    mRefreshLibraryButton = std::make_unique<TextButton>("Refresh");
    mRefreshLibraryButton->setBounds(x + PANEL_MARGIN / 2.0,
                                     PANEL_MARGIN / 2.0,
                                     textButtonWidth - PANEL_MARGIN / 2.0,
                                     getHeight() - PANEL_MARGIN);
    mRefreshLibraryButton->onClick = [this] { currentProcessor.getSampleLibrary().refresh(); };
    addAndMakeVisible(*mRefreshLibraryButton);
    x += textButtonWidth;
    
    // Add choose library directory button
    mChooseLibraryFolderButton = std::make_unique<TextButton>("Choose Dir.");
    mChooseLibraryFolderButton->setBounds(x + PANEL_MARGIN / 2.0,
                                          PANEL_MARGIN / 2.0,
                                          textButtonWidth - PANEL_MARGIN / 2.0,
                                          getHeight() - PANEL_MARGIN);
    mChooseLibraryFolderButton->onClick = [this] { showLibraryChooser(); };
    addAndMakeVisible(*mChooseLibraryFolderButton);
    x += textButtonWidth;
    
    // Add toggle for library panel
    int toggleButtonWidth = getHeight();
    mToggleLibraryPanelButton = std::make_unique<ToggleButton>("Toggle SampleLibraryPanel");
    mToggleLibraryPanelButton->setToggleState(true, NotificationType::dontSendNotification);
    mToggleLibraryPanelButton->setBounds(x + PANEL_MARGIN / 2.0,
                                         PANEL_MARGIN / 2.0,
                                         toggleButtonWidth - PANEL_MARGIN / 2.0,
                                         getHeight() - PANEL_MARGIN);
    mToggleLibraryPanelButton->onClick = [this]
    {
        if (!mToggleLibraryPanelButton->getToggleState())
        {
            mToggleLibraryPanelButton->setToggleState(true, NotificationType::dontSendNotification);
        }
        else
        {
            linkedCenterPanel.showNavigationPanel(PANELS_LIBRARY_PANEL);
            mToggleSampleTablePanelButton->setToggleState(false, NotificationType::dontSendNotification);
        }
    };
    addAndMakeVisible(*mToggleLibraryPanelButton);
    x += toggleButtonWidth;
    
    // Add toggle for sample table panel
    mToggleSampleTablePanelButton = std::make_unique<ToggleButton>("Toggle SampleTablePanel");
    mToggleSampleTablePanelButton->setToggleState(false, NotificationType::dontSendNotification);
    mToggleSampleTablePanelButton->setBounds(x + PANEL_MARGIN / 2.0,
                                             PANEL_MARGIN / 2.0,
                                             toggleButtonWidth - PANEL_MARGIN / 2.0,
                                             getHeight() - PANEL_MARGIN);
    mToggleSampleTablePanelButton->onClick = [this]
    {
        if (!mToggleSampleTablePanelButton->getToggleState())
        {
            mToggleSampleTablePanelButton->setToggleState(true, NotificationType::dontSendNotification);
        }
        else
        {
            linkedCenterPanel.showNavigationPanel(PANELS_TABLE_PANEL);
            mToggleLibraryPanelButton->setToggleState(false, NotificationType::dontSendNotification);
        }
    };
    addAndMakeVisible(*mToggleSampleTablePanelButton);
    x += toggleButtonWidth;
    
    // Add button for editing the file filter rules
    mChangeFilterButton = std::make_unique<TextButton>("Filter");
    mChangeFilterButton->setBounds(x + PANEL_MARGIN / 2.0,
                                   PANEL_MARGIN / 2.0,
                                   textButtonWidth - PANEL_MARGIN / 2.0,
                                   getHeight() - PANEL_MARGIN);
    mChangeFilterButton->onClick = [this]
    {
        std::unique_ptr<FileFilterPanel> fileFilterPanel = std::make_unique<FileFilterPanel>(currentProcessor
                                                                                             .getSampleLibrary());
        CallOutBox::launchAsynchronously(std::move(fileFilterPanel), mChangeFilterButton->getScreenBounds(), nullptr);
    };
    addAndMakeVisible(*mChangeFilterButton);
    x += textButtonWidth;
    
    // Add button for toggling filter on and off
    mActivateFilterButton = std::make_unique<ToggleButton>("ToggleFilterButton");
    mActivateFilterButton->setBounds(x + PANEL_MARGIN / 2.0,
                                     PANEL_MARGIN / 2.0,
                                     getHeight() - PANEL_MARGIN,
                                     getHeight() - PANEL_MARGIN);
    mActivateFilterButton->setToggleState(currentProcessor
                                          .getSampleLibrary()
                                          .getFileFilter()
                                          .getIsActive(),
                                          NotificationType::dontSendNotification);
    mActivateFilterButton->onClick = [this]
    {
        currentProcessor.getSampleLibrary().getFileFilter().setIsActive(mActivateFilterButton->getToggleState());
        currentProcessor.getSampleLibrary().refresh();
    };
    addAndMakeVisible(*mActivateFilterButton);
    x += getHeight();
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
            
            if (name != "") {
                currentProcessor.getSampleLibrary().setDirectory(name);
                
                // Show success popup message
                AlertWindow::showAsync(MessageBoxOptions()
                                       .withIconType(MessageBoxIconType::NoIcon)
                                       .withTitle("Sample Library Chooser")
                                       .withMessage("You picked: " + name)
                                       .withButton("OK"),
                                       nullptr);
            }
        }
    );
}
