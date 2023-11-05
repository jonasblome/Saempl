/*
  ==============================================================================

    SampleNavigationPanel.h
    Created: 24 Jun 2023 4:28:16pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleLibraryPanel.h"
#include "SampleTablePanel.h"
#include "SampleItemPanel.h"

/**
 Holds the panels for sample navigation.
 
 Implements:
 \ref PanelBase
 Includes \ref SampleLibraryPanel, \ref SampleTablePanel and \ref SampleMapPanel.
 */
class SampleNavigationPanel
:   public PanelBase
{
public:
    /**
     The constructor for the navigation panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    SampleNavigationPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleNavigationPanel();
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
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<SampleLibraryPanel> mSampleLibraryPanel;
    std::unique_ptr<SampleTablePanel> mSampleTablePanel;
    SampleLibrary& sampleLibrary;
    NavigationPanelType mActiveNavigationPanelType;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
};
