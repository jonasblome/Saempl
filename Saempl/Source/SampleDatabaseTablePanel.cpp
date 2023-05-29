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
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(TABLE_PANEL_WIDTH - Blome_PanelMargin, TABLE_PANEL_HEIGHT - Blome_PanelMargin);
    setPanelComponents();
}

SampleDatabaseTablePanel::~SampleDatabaseTablePanel()
{
    mFileTree->removeListener(this);
}

void SampleDatabaseTablePanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourMedium);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Blome_PanelCornerSize);
}

void SampleDatabaseTablePanel::setPanelComponents()
{
    // Setting view model
    mSampleDatabaseTableViewModel = std::make_unique<SampleDatabaseTableViewModel>(currentThread);
    
    // Set file tree component
    mFileTree = std::make_unique<FileTreeComponent>(*mSampleDatabaseTableViewModel->getDirectoryList());
    mFileTree->setBounds(Blome_PanelMargin / 2.0, Blome_PanelMargin / 2.0, getWidth() - Blome_PanelMargin, getHeight() - Blome_PanelMargin);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, BlomeColour_FullTransparent);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleDatabaseTablePanel::selectionChanged()
{
    
}

void SampleDatabaseTablePanel::fileClicked(const File&, const MouseEvent&)
{
    
}

void SampleDatabaseTablePanel::fileDoubleClicked(const File&)
{
    linkedSampleItemPanel.showAudioResource(URL(mFileTree->getSelectedFile()));
}

void SampleDatabaseTablePanel::browserRootChanged(const File&)
{
    
}

void SampleDatabaseTablePanel::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}
