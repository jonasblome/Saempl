/*
  ==============================================================================

    SampleLibrariesPanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibraryPanel.h"

SampleLibraryPanel::SampleLibraryPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(SAMPLE_NAVIGATION_PANEL_WIDTH - PANEL_MARGIN, SAMPLE_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleLibraryPanel::~SampleLibraryPanel()
{
    mFileTree->removeListener(this);
    mSampleLibrariesViewModel->getDirectoryList()->removeChangeListener(&*mFileTree);
}

void SampleLibraryPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourMedium);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Blome_PanelCornerSize);
}

void SampleLibraryPanel::setPanelComponents()
{
    // Setting view model
    mSampleLibrariesViewModel = std::make_unique<SampleLibrariesViewModel>(currentProcessor.getSampleLibrary());
    
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(*mSampleLibrariesViewModel);
    mFileTree->setBounds(PANEL_MARGIN / 2.0,
                         PANEL_MARGIN / 2.0,
                         getWidth() - PANEL_MARGIN,
                         getHeight() - PANEL_MARGIN);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, BlomeColour_Transparent);
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    mSampleLibrariesViewModel->getDirectoryList()->addChangeListener(&*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleLibraryPanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(PANEL_MARGIN / 2.0, PANEL_MARGIN / 2.0 + Blome_NormalButtonHeight, getWidth() - PANEL_MARGIN, getHeight() - PANEL_MARGIN);
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
    if (!inFile.isDirectory() && isSupportedAudioFileFormat(inFile.getFileExtension()))
    {
        linkedSampleItemPanel.showAudioResource(URL(inFile));
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
        mSampleLibrariesViewModel->removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
};


