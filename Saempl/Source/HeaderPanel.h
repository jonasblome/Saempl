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

class HeaderPanel
:   public PanelBase
{
public:
    // Constructors
    HeaderPanel(SaemplAudioProcessor& inProcessor, CenterPanel& inCenterPanel);
    ~HeaderPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(HeaderPanel)
    
    // Fields
    SaemplAudioProcessor& currentProcessor;
    CenterPanel& linkedCenterPanel;
    std::unique_ptr<TextButton> mRefreshLibraryButton;
    std::unique_ptr<TextButton> mChooseLibraryFolderButton;
    std::unique_ptr<FileChooser> mFileChooser;
    std::unique_ptr<ToggleButton> mToggleLibraryPanelButton;
    std::unique_ptr<ToggleButton> mToggleSampleTablePanelButton;
    
    // Methods
    void showLibraryChooser();
    
};
