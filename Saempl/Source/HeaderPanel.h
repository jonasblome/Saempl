/*
  ==============================================================================

    HeaderPanel.h
    Created: 27 May 2023 11:12:36am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "CenterPanel.h"
#include "FileFilterPanel.h"

/**
 Holds the logo, and components for global functionality.
 
 Implements:
 \ref PanelBase
 Encompasses the refresh button, the file chooser button,
 the navigation panel toggles and the filter settings.
 */
class HeaderPanel
:   public PanelBase
{
public:
    /**
     The constructor the plugins header panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     @param inCenterPanel the panel for the central plugin UI content.
     */
    HeaderPanel(SaemplAudioProcessor& inProcessor, CenterPanel& inCenterPanel);
    ~HeaderPanel();
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(HeaderPanel)
    
    SaemplAudioProcessor& currentProcessor;
    CenterPanel& linkedCenterPanel;
    std::unique_ptr<TextButton> mRefreshLibraryButton;
    std::unique_ptr<TextButton> mChooseLibraryFolderButton;
    std::unique_ptr<FileChooser> mFileChooser;
    std::unique_ptr<ToggleButton> mToggleLibraryPanelButton;
    std::unique_ptr<ToggleButton> mToggleSampleTablePanelButton;
    std::unique_ptr<TextButton> mChangeFilterButton;
    std::unique_ptr<ToggleButton> mActivateFilterButton;
    
    void showLibraryChooser();
    
};
