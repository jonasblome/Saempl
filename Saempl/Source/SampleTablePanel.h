/*
 ==============================================================================
 
 SampleTablePanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "SampleItemPanel.h"
#include "BlomeTableViewNavigation.h"

/**
 Holds the table for displaying all SampleLibrary::mAllSampleItems.
 */
class SampleTablePanel
:
public PanelBase
{
public:
    /**
     The constructor for the sample table panel.
     
     @param inSampleLibrary the library object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    SampleTablePanel(SaemplAudioProcessor& inProcessor,
                     SampleItemPanel& inSampleItemPanel,
                     AudioPlayer& inAudioPlayer);
    ~SampleTablePanel();
    /**
     Selects a random sample in the table view.
     */
    void selectRandomSample();
    bool keyPressed(KeyPress const & key) override;
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    AudioPlayer& audioPlayer;
    std::unique_ptr<BlomeTableViewNavigation> mSampleTable;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    void visibilityChanged() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleTablePanel);
};
