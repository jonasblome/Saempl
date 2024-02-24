/*
 ==============================================================================
 
 BlomeFileTreeView.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "SampleLibrary.h"

/**
 The view class for displaying the sample library directory file tree.
 
 Can handle drag and drop of files onto and from it.
 */
class BlomeFileTreeView
:
public FileTreeComponent,
public ChangeListener
{
public:
    /**
     The constructor for the file tree view of the SampleLibrary.
     
     @param inSampleLibrary the library object of the plugin instance.
     */
    BlomeFileTreeView(SampleLibrary& inSampleLibrary);
    ~BlomeFileTreeView();
    
private:
    SampleLibrary& sampleLibrary;
    
    void filesDropped(StringArray const & files, int x, int y) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void mouseDrag(MouseEvent const & mouseEvent) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeFileTreeView);
};
