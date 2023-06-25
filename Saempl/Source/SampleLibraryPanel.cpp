/*
  ==============================================================================

    SampleLibrariesPanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibraryPanel.h"

SampleLibraryPanel::SampleLibraryPanel(SaemplAudioProcessor& inProcessor, SampleLibraryViewModel& inSampleLibraryViewModel, SampleItemPanel& inSampleItemPanel)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor),
    sampleLibraryViewModel(inSampleLibraryViewModel),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(SAMPLE_NAVIGATION_PANEL_WIDTH - PANEL_MARGIN, SAMPLE_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleLibraryPanel::~SampleLibraryPanel()
{
    mFileTree->removeListener(this);
    sampleLibraryViewModel.getDirectoryList()->removeChangeListener(&*mFileTree);
}

void SampleLibraryPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), MEDIUM_CORNER_SIZE);
}

void SampleLibraryPanel::setPanelComponents()
{
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(sampleLibraryViewModel);
    mFileTree->setBounds(PANEL_MARGIN / 2.0,
                         PANEL_MARGIN / 2.0,
                         getWidth() - PANEL_MARGIN,
                         getHeight() - PANEL_MARGIN);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, COLOUR_TRANSPARENT);
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    sampleLibraryViewModel.getDirectoryList()->addChangeListener(&*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleLibraryPanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(PANEL_MARGIN / 2.0, PANEL_MARGIN / 2.0, getWidth() - PANEL_MARGIN, getHeight() - PANEL_MARGIN);
    }
}

void SampleLibraryPanel::selectionChanged()
{
    
}

void SampleLibraryPanel::fileClicked(const File& file, const MouseEvent& mouseEvent)
{
    // Show options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Move File(s) to Trash", [&] { deleteFile(false); });
        popupMenu.addItem("Placeholder", nullptr);
        popupMenu.addItem("Placeholder", nullptr);
        popupMenu.addItem("Delete File(s) Permanently", [&] { deleteFile(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleLibraryPanel::fileDoubleClicked(const File& inFile)
{
    // Load file into source
    if (inFile.exists() && !inFile.isDirectory() && isSupportedAudioFileFormat(inFile.getFileExtension()))
    {
        linkedSampleItemPanel.showAudioResource(URL(inFile));
    }
    else if (!inFile.exists())
    {
        linkedSampleItemPanel.showAudioResource(URL());
    }
}

void SampleLibraryPanel::browserRootChanged(const File&)
{
    
}

void SampleLibraryPanel::changeListenerCallback(ChangeBroadcaster* source)
{
    // When the file tree state changes
    repaint();
}

void SampleLibraryPanel::deleteFile(bool deletePermanently = false)
{
    // Delete all selected files
    for (int f = 0; f < mFileTree->getNumSelectedItems(); f++)
    {
        sampleLibraryViewModel.removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
};


