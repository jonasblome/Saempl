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
    PanelBase::paint(g);
    
    // Draw background
    g.setColour(COLOUR_ACCENT_LIGHT);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), MEDIUM_CORNER_SIZE);
    
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
    int buttonWidth = 0;
    
    // Add refresh sample library button
    buttonWidth = 75;
    mRefreshLibraryButton = std::make_unique<TextButton>("Refresh");
    mRefreshLibraryButton->setBounds(x + PANEL_MARGIN / 2.0,
                                     PANEL_MARGIN / 2.0,
                                     buttonWidth - PANEL_MARGIN / 2.0,
                                     getHeight() - PANEL_MARGIN);
    mRefreshLibraryButton->onClick = [this] { currentProcessor.getSampleLibrary().refresh(); };
    addAndMakeVisible(*mRefreshLibraryButton);
    x += 75;
    
    // Add choose library directory button
    mChooseLibraryFolderButton = std::make_unique<TextButton>("Choose Dir");
    mChooseLibraryFolderButton->setBounds(x + PANEL_MARGIN / 2.0,
                                          PANEL_MARGIN / 2.0,
                                          buttonWidth - PANEL_MARGIN / 2.0,
                                          getHeight() - PANEL_MARGIN);
    mChooseLibraryFolderButton->onClick = [this] { showLibraryChooser(); };
    addAndMakeVisible(*mChooseLibraryFolderButton);
    x += 75;
    
    // Add toggle for library panel
    buttonWidth = getHeight();
    mToggleLibraryPanelButton = std::make_unique<ToggleButton>("Toggle SampleLibraryPanel");
    mToggleLibraryPanelButton->setToggleState(true, NotificationType::dontSendNotification);
    mToggleLibraryPanelButton->setBounds(x + PANEL_MARGIN / 2.0,
                                         PANEL_MARGIN / 2.0,
                                         buttonWidth - PANEL_MARGIN / 2.0,
                                         getHeight() - PANEL_MARGIN);
    mToggleLibraryPanelButton->onClick = [this] {
        if (!mToggleLibraryPanelButton->getToggleState())
        {
            mToggleLibraryPanelButton->setToggleState(true, NotificationType::dontSendNotification);
        }
        else
        {
            linkedCenterPanel.showNavigationPanel(0);
            mToggleSampleTablePanelButton->setToggleState(false, NotificationType::dontSendNotification);
        }
    };
    addAndMakeVisible(*mToggleLibraryPanelButton);
    x += getHeight();
    
    // Add toggle for sample table panel
    mToggleSampleTablePanelButton = std::make_unique<ToggleButton>("Toggle SampleTablePanel");
    mToggleSampleTablePanelButton->setToggleState(false, NotificationType::dontSendNotification);
    mToggleSampleTablePanelButton->setBounds(x + PANEL_MARGIN / 2.0,
                                         PANEL_MARGIN / 2.0,
                                         buttonWidth - PANEL_MARGIN / 2.0,
                                         getHeight() - PANEL_MARGIN);
    mToggleSampleTablePanelButton->onClick = [this] {
        if (!mToggleSampleTablePanelButton->getToggleState())
        {
            mToggleSampleTablePanelButton->setToggleState(true, NotificationType::dontSendNotification);
        }
        else
        {
            linkedCenterPanel.showNavigationPanel(1);
            mToggleLibraryPanelButton->setToggleState(false, NotificationType::dontSendNotification);
        }
    };
    addAndMakeVisible(*mToggleSampleTablePanelButton);
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
