/*
  ==============================================================================

    SampleDatabaseViewPanel.h
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleDatabaseTableViewModel.h"
#include "SampleItemPanel.h"

class SampleDatabaseTablePanel
:   public PanelBase,
    private FileBrowserListener,
    private ChangeListener
{
public:
    void extracted();
    
SampleDatabaseTablePanel(TimeSliceThread& inThread, SampleItemPanel& inSampleItemPanel);
    ~SampleDatabaseTablePanel();
    
    void paint(Graphics& g) override;
    
    void setPanelComponents() override;
    
private:
    // Fields
    TimeSliceThread& currentThread;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<SampleDatabaseTableViewModel> mSampleDatabaseTableViewModel;
    std::unique_ptr<Label> mExplanationLabel;
    std::unique_ptr<FileTreeComponent> mFileTree;
    
    // Methods
    void selectionChanged() override;
    void fileClicked (const File&, const MouseEvent&) override;
    void fileDoubleClicked (const File&) override;
    void browserRootChanged (const File&) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
};
