/*
  ==============================================================================

    CenterPanel.h
    Created: 27 May 2023 9:07:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleNavigationPanel.h"
#include "SampleItemPanel.h"
#include "SamplePalettePanel.h"

/**
 Holds the central components of the UI.
 
 Implements:
 \ref PanelBase
 Includes the sample navigation panels and the \ref SampleItemPanel for audio previewing.
 */
class CenterPanel
:   public PanelBase
{
public:
    /**
     The constructor for the center panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     */
    CenterPanel(SaemplAudioProcessor& inProcessor);
    ~CenterPanel();
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    /**
     Toggles between showing the library panel, the table panel and the map panel.
     
     @param inPanelType the type of the navigation panel (library, table or map).
     */
    void showNavigationPanel(NavigationPanelType inPanelType);
    /**
     @returns the type of the currently shown navigation panel.
     */
    NavigationPanelType getActiveNavigationPanelType();
    
private:
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<SampleNavigationPanel> mSampleNavigationPanel;
    std::unique_ptr<SamplePalettePanel> mSamplePalettePanel;
    std::unique_ptr<SampleItemPanel> mSampleItemPanel;
    std::unique_ptr<ToggleButton> mToggleSampleItemPanelButton;
    
    /**
     Toggles the sample item's visibility on/off and rescales the navigation panels.
     */
    void toggleSampleItemPanel();
};
