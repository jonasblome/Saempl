/*
 ==============================================================================
 
 CentrePanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "SampleNavigationPanel.h"
#include "SampleItemPanel.h"
#include "SampleFavouritesPanel.h"

/**
 Holds the central components of the UI.
 
 Includes the sample navigation panels and the SampleItemPanel for audio previewing.
 */
class CentrePanel
:
public PanelBase
{
public:
    /**
     The constructor for the centre panel.
     
     @param inProcessor the audio processor of the current plugin instance.
     */
    CentrePanel(SaemplAudioProcessor& inProcessor);
    ~CentrePanel();
    /**
     Toggles between showing the library panel, the table panel and the grid panel.
     
     @param inPanelType the type of the navigation panel (library, table or grid).
     */
    void setActiveNavigationPanel(NavigationPanelType inPanelType);
    /**
     Selects a random sample in the table view.
     */
    void selectRandomSample();
    bool keyPressed(KeyPress const & key) override;
    
private:
    std::unique_ptr<AudioPlayer> mAudioPlayer;
    std::unique_ptr<SampleNavigationPanel> mSampleNavigationPanel;
    std::unique_ptr<SampleFavouritesPanel> mSampleFavouritesPanel;
    std::unique_ptr<SampleItemPanel> mSampleItemPanel;
    std::unique_ptr<ToggleButton> mToggleSampleItemPanelButton;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    /**
     Toggles the sample item's visibility on/off and rescales the navigation panels.
     
     @param inShouldBeVisible whether the panel should be visible or not.
     */
    void setSampleItemPanelVisibility(bool inShouldBeVisible);
    void resizePanelComponents() override;
    /**
     Stops playback of the currently playing sample.
     */
    void stopSelectedSample();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CentrePanel);
};
