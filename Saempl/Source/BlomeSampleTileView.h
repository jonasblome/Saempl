/*
 ==============================================================================
 
 BlomeSampleTileView.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "SampleItemPanel.h"

/**
 The view class for tiles on the grid that each represent a sample item.
 */
class BlomeSampleTileView
:
public Component
{
public:
    BlomeSampleTileView(SampleItem* inSampleItem, 
                            SampleLibrary& inSampleLibrary,
                            SampleItemPanel& inSampleItemPanel,
                            AudioPlayer& inAudioPlayer);
    ~BlomeSampleTileView();
    /**
     @returns the title of the sample item linked to the view.
     */
    String getSampleItemFilePath();
    /**
     Sets the selection toggle of the tile.
     
     @param inIsSelected whether the tile is now selected or not.
     */
    void setSelected(bool inIsSelected);
    /**
     @returns whether the tile is selected or not.
     */
    bool getIsSelected();
    /**
     Tries to load the item's file into the audio player.
     */
    void loadIntoAudioPlayer();
    /**
     Loads the sample into the grid's audio player and starts playback.
     */
    void startPlayback();
    
private:
    SampleItem* sampleItem;
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    AudioPlayer& audioPlayer;
    std::unique_ptr<BlomeImageButton> mStartStopButton;
    BlomeStyling::Ptr style;
    String sampleItemFilePath;
    bool isSelected;
    
    void paint(Graphics& g) override;
    void mouseDoubleClick (MouseEvent const & event) override;
    void resized() override;
    void setPlayButton();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeSampleTileView);
};
