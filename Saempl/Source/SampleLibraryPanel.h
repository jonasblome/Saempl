/*
  ==============================================================================

    SampleLibrariesPanel.h
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "BlomeFileTreeView.h"
#include "SampleLibraryViewModel.h"
#include "SampleItemPanel.h"

class SampleLibraryPanel
:   public PanelBase,
    private FileBrowserListener,
    private ChangeListener
{
public:
    // Constructors
    SampleLibraryPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleLibraryPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;

    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibraryPanel)
    
    // Fields
    SaemplAudioProcessor& currentProcessor;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<SampleLibrariesViewModel> mSampleLibrariesViewModel;
    std::unique_ptr<BlomeFileTreeView> mFileTree;
    
    // Methods
    void selectionChanged() override;
    void fileClicked (const File& file, const MouseEvent& mouseEvent) override;
    void fileDoubleClicked (const File&) override;
    void browserRootChanged (const File&) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void deleteFile(bool deletePermanently);
};
