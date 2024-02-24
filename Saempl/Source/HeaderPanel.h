/*
 ==============================================================================
 
 HeaderPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "CentrePanel.h"
#include "FileFilterPanel.h"
#include "BlomeImageButton.h"

/**
 Holds the logo, and components for global functionality.
 
 Encompasses the refresh button, the file chooser button,
 the navigation panel toggles and the filter settings.
 */
class HeaderPanel
:
public PanelBase
{
public:
    /**
     The constructor the plugins header panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     @param inCentrePanel the panel for the central plugin UI content.
     */
    HeaderPanel(SaemplAudioProcessor& inProcessor, CentrePanel& inCentrePanel);
    ~HeaderPanel();
    bool keyPressed(KeyPress const & key) override;
    
private:
    SampleLibrary& sampleLibrary;
    CentrePanel& centrePanel;
    std::unique_ptr<BlomeImageButton> mRefreshLibraryButton;
    std::unique_ptr<BlomeImageButton> mChooseLibraryDirectoryButton;
    std::unique_ptr<FileChooser> mFileChooser;
    std::unique_ptr<BlomeImageButton> mToggleSampleLibraryPanelButton;
    std::unique_ptr<BlomeImageButton> mToggleSampleTablePanelButton;
    std::unique_ptr<BlomeImageButton> mToggleSampleGridPanelButton;
    std::unique_ptr<BlomeImageButton> mChangeFilterButton;
    std::unique_ptr<BlomeImageButton> mToggleFilterButton;
    std::unique_ptr<BlomeImageButton> mRandomSampleButton;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void setRefreshLibraryButton(int buttonWidth, int x);
    void setChooseLibraryDirectoryButton(int buttonWidth, int x);
    void setToggleSampleLibraryPanelButton(int buttonWidth, int x);
    void setToggleSampleTablePanelButton(int buttonWidth, int x);
    void setToggleSampleGridPanelButton(int buttonWidth, int x);
    void setChangeFilterButton(int buttonWidth, int x);
    void setToggleFilterButton(int buttonWidth, int x);
    void setRandomSampleButton(int buttonWidth, int x);
    void setPanelComponents() override;
    /**
     Shows the dialogue for choosing the sample library directory.
     */
    void showLibraryChooser();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel);
};
