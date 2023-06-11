/*
  ==============================================================================

    SampleDatabaseViewPanel.h
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "BlomeFileTreeView.h"
#include "SampleDatabaseTableViewModel.h"
#include "SampleItemPanel.h"

class SampleDatabaseTablePanel
:   public PanelBase,
    private FileBrowserListener,
    private ChangeListener
{
public:
    // Constructors
    SampleDatabaseTablePanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~SampleDatabaseTablePanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;

    
private:
    // Fields
    SaemplAudioProcessor& currentProcessor;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<SampleDatabaseTableViewModel> mSampleDatabaseTableViewModel;
    std::unique_ptr<BlomeFileTreeView> mFileTree;
    std::unique_ptr<TextButton> mChangeDirectoryButton;
    std::unique_ptr<TextButton> mRefreshSampleDatabaseButton;
    
    // Methods
    void selectionChanged() override;
    void fileClicked (const File& file, const MouseEvent& mouseEvent) override;
    void fileDoubleClicked (const File&) override;
    void browserRootChanged (const File&) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void deleteFile(bool deletePermanently);
};
