/*
  ==============================================================================

    SampleItemLibraryListPanel.h
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "BlomeFileTreeView.h"
#include "SampleLibraryViewModel.h"
#include "SampleItemPanel.h"

class SampleLibrariesPanel
:   public PanelBase,
    private FileBrowserListener,
    private ChangeListener
{
public:
    // Constructors
    SampleLibrariesPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleLibrariesPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    SampleLibraryViewModel& getViewModel();
    
private:
    // Fields
    SaemplAudioProcessor& currentProcessor;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<SampleLibraryViewModel> mSampleItemLibraryViewModel;
    std::unique_ptr<BlomeFileTreeView> mFileTree;
    
    // Methods
    void selectionChanged() override;
    void fileClicked (const File& file, const MouseEvent& mouseEvent) override;
    void fileDoubleClicked (const File&) override;
    void browserRootChanged (const File&) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void deleteFile(bool deletePermanently);
};
