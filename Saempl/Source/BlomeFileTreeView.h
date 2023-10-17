/*
  ==============================================================================

    BlomeFileTreeView.h
    Created: 3 Jun 2023 4:59:52pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleLibraryViewModel.h"

class BlomeFileTreeView
:   public FileTreeComponent,
    public ChangeListener
{
public:
    // Constructors
    BlomeFileTreeView(SampleLibraryViewModel& inSampleLibraryViewModel);
    ~BlomeFileTreeView();
    
    // Methods
    void filesDropped(const StringArray& files, int x, int y) override;
    bool isInterestedInFileDrag(const StringArray& files) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void mouseDrag(const MouseEvent& e) override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeFileTreeView)
    
    // Fields
    SampleLibraryViewModel& sampleLibraryViewModel;
    
    // Methods
    
};
