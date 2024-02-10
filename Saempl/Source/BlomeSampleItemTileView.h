/*
  ==============================================================================

    BlomeSampleItemTileView.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "SampleItemPanel.h"

class BlomeSampleItemTileView
:
public Component
{
public:
    BlomeSampleItemTileView(SampleItem* inSampleItem, SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeSampleItemTileView();
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
    
private:
    SampleItem* sampleItem;
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    BlomeStyling::Ptr style;
    String sampleItemFilePath;
    bool isSelected;
    
    void paint(Graphics& g) override;
    void mouseDoubleClick (MouseEvent const & event) override;
};
