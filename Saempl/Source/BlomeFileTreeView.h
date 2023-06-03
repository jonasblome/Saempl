/*
  ==============================================================================

    BlomeFileTreeView.h
    Created: 3 Jun 2023 4:59:52pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleDatabaseTableViewModel.h"

class BlomeFileTreeView
:   public FileTreeComponent,
    public ChangeListener
{
public:
    // Constructors
    BlomeFileTreeView(SampleDatabaseTableViewModel& inSampleDatabaseTableViewModel);
    ~BlomeFileTreeView();
    
    // Methods
    void filesDropped(const StringArray& files, int x, int y) override;
    bool isInterestedInFileDrag(const StringArray& files) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
private:
    // Fields
    SampleDatabaseTableViewModel& sampleDatabaseTableViewModel;
    
    // Methods
    
};
