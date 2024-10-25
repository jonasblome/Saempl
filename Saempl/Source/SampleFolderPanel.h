/*
 ==============================================================================
 
 SampleFolderPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "BlomeFileTreeView.h"
#include "SampleItemPanel.h"

/**
 Holds the BlomeFileTreeView and controls the SampleLibrary.
 
 Handles previewing and deleting of sample files.
 */
class SampleFolderPanel
:
public PanelBase,
private FileBrowserListener,
private ChangeListener
{
public:
    /**
     The constructor for the sample library panel.
     
     @param inProcessor the processor object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    SampleFolderPanel(SaemplAudioProcessor& inProcessor,
                      SampleItemPanel& inSampleItemPanel,
                      AudioPlayer& inAudioPlayer);
    ~SampleFolderPanel();
    bool keyPressed(const KeyPress& key) override;
    /**
     Show the sample in the folder view.
     
     @param inFilePath the path to the sample file.
     */
    void showSample(String inFilePath);
    
private:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    AudioPlayer& audioPlayer;
    std::unique_ptr<BlomeFileTreeView> mFileTree;
    SampleItemCollectionScope mSampleItemCollectionType;
    
    void selectionChanged() override;
    void fileClicked(File const & file, MouseEvent const & mouseEvent) override;
    void fileDoubleClicked(File const &) override;
    void browserRootChanged(File const &) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    /**
     Deletes the files selected in the file tree view.
     
     @param deletePermanently whether the file gets deleted permanently or move to the trash.
     */
    void deleteFiles(bool deletePermanently);
    /**
     Add all selected files to the sample item favourites collection.
     */
    void addToFavourites();
    /**
     Reanalyses all selected samples.
     */
    void reanalyseSamples();
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void resizePanelComponents() override;
    /**
     Opens the finder at the location of the chosen sample.
     */
    void showSampleInFinder();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleFolderPanel);
};
