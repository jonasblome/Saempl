/*
 ==============================================================================
 
 BlomeFileTreeView.h
 Created: 3 Jun 2023 4:59:52pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "SampleLibrary.h"

/**
 The view class for displaying the \ref SampleLibrary directory file tree.
 
 Implements:
 \ref juce::ChangeListener
 \ref juce::FileTreeComponent
 Can handle drag and drop of files onto and from it.
 */
class BlomeFileTreeView
:
public FileTreeComponent,
public ChangeListener
{
public:
    /**
     The constructor for the file tree view of the \ref SampleLibrary.
     
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
};
