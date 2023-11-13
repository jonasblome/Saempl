/*
  ==============================================================================

    BlomeSampleItemTileView.h
    Created: 13 Nov 2023 5:31:43pm
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
    BlomeSampleItemTileView(SampleItem& inSampleItem, SampleItemPanel& inSampleItemPanel);
    ~BlomeSampleItemTileView();
    
private:
    SampleItem& linkedSampleItem;
    SampleItemPanel& linkedSampleItemPanel;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void mouseDoubleClick (const MouseEvent& event) override;
};
