/*
 ==============================================================================
 
 SampleNavigationPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "SampleFolderPanel.h"
#include "SampleTablePanel.h"
#include "SampleGridPanel.h"
#include "SampleItemPanel.h"

/**
 Holds the panels for sample navigation.
 
 Includes SampleLibraryPanel, SampleTablePanel and SampleGridPanel.
 */
class SampleNavigationPanel
:
public PanelBase
{
public:
    /**
     The constructor for the navigation panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    SampleNavigationPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleNavigationPanel();
    /**
     Toggles between showing the library panel, the table panel and the grid panel.
     
     @param inPanelType the type of the navigation panel (library, table or grid).
     */
    void setActiveNavigationPanel(NavigationPanelType& inPanelType);
    /**
     Selects a random sample in the table view.
     */
    void selectRandomSample();
    
private:
    SampleItemPanel& sampleItemPanel;
    std::unique_ptr<SampleFolderPanel> mSampleLibraryPanel;
    std::unique_ptr<SampleTablePanel> mSampleTablePanel;
    std::unique_ptr<SampleGridPanel> mSampleGridPanel;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleNavigationPanel);
};
