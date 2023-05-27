/*
  ==============================================================================

    SampleDatabaseTablePanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseTablePanel.h"

SampleDatabaseTablePanel::SampleDatabaseTablePanel(TimeSliceThread& inThread, SampleItemPanel& inSampleItemPanel)
:   PanelBase(),
    currentThread(inThread),
    connectedSampleItemPanel(inSampleItemPanel)
{
    // Set panel size
    setSize(TABLE_PANEL_WIDTH, TABLE_PANEL_HEIGHT);
    
    // Setting view model
    mSampleDatabaseTableViewModel = std::make_unique<SampleDatabaseTableViewModel>(inThread);
    
    setPanelStyle();
}

SampleDatabaseTablePanel::~SampleDatabaseTablePanel()
{
    mFileTree->removeListener(this);
}

void SampleDatabaseTablePanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw panel background
    g.fillAll(BlomeColour_DarkRed);
}

void SampleDatabaseTablePanel::setPanelStyle()
{
    // Set file tree component
    mFileTree = std::make_unique<FileTreeComponent>(*mSampleDatabaseTableViewModel->getDirectoryList());
    mFileTree->setBounds(0, 0, getWidth(), getHeight());
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, BlomeColour_FullTransparent);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleDatabaseTablePanel::selectionChanged()
{
    connectedSampleItemPanel.showAudioResource(URL(mFileTree->getSelectedFile()));
}

void SampleDatabaseTablePanel::fileClicked(const File&, const MouseEvent&)
{
    
}

void SampleDatabaseTablePanel::fileDoubleClicked(const File&)
{
    
}

void SampleDatabaseTablePanel::browserRootChanged(const File&)
{
    
}

void SampleDatabaseTablePanel::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}
