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
    BlomeSampleItemTileView(SampleItem* inSampleItem, SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeSampleItemTileView();
    /**
     @returns the title of the sample item linked to the view.
     */
    String getSampleItemFilePath();
    
private:
    SampleItem* sampleItem;
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    BlomeStyling::Ptr style;
    String sampleItemFilePath;
    
    void paint(Graphics& g) override;
    void mouseDoubleClick (const MouseEvent& event) override;
    void mouseDrag(MouseEvent const & mouseEvent) override;
    void mouseUp(const MouseEvent & mouseEvent) override;
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
