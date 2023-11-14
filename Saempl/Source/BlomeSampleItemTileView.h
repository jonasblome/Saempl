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
    BlomeSampleItemTileView(SampleItem& inSampleItem, SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeSampleItemTileView();
    /**
     @returns the \ref SampleItem linked to the view.
     */
    SampleItem& getLinkedSampleItem();
    
private:
    SampleItem& linkedSampleItem;
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void mouseDoubleClick (const MouseEvent& event) override;
    void mouseDrag(MouseEvent const & e) override;
    void mouseUp (const MouseEvent& event) override;
    /**
     Deletes the files selected in the file tree view.
     
     @param deletePermanently whether the file gets deleted permanently or move to the trash.
     */
    void deleteFiles(bool deletePermanently);
    /**
     Add all selected files to the sample item palette collection.
     */
    void addToPalette();
    /**
     Reanalyses all selected samples.
     */
    void reanalyseSamples();
};
