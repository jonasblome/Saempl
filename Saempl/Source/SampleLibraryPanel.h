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
#include "SampleItemPanel.h"

/**
 Holds the \ref BlomeFileTreeView and controls the \ref SampleLibrary.
 
 Implements:
 \ref PanelBase
 Handles previewing and deleting of sample files.
 */
class SampleLibraryPanel
:   public PanelBase,
    private FileBrowserListener,
    private ChangeListener
{
public:
    /**
     The constructor for the sample library panel.
     
     @param inSampleLibrary the library object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    SampleLibraryPanel(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~SampleLibraryPanel();
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;

    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibraryPanel)
    
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    std::unique_ptr<BlomeFileTreeView> mFileTree;
    
    void selectionChanged() override;
    void fileClicked (File const & file, MouseEvent const & mouseEvent) override;
    void fileDoubleClicked (File const &) override;
    void browserRootChanged (File const &) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void deleteFile(bool deletePermanently);
};
