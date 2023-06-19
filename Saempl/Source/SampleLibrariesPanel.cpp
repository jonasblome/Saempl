/*
  ==============================================================================

    SampleItemLibraryListPanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibrariesPanel.h"

SampleLibrariesPanel::SampleLibrariesPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(SAMPLE_ITEM_LIBRARY_LIST_PANEL_WIDTH - PANEL_MARGIN, SAMPLE_ITEM_LIBRARY_LIST_PANEL_HEIGHT - PANEL_MARGIN);
    setPanelComponents();
}

SampleLibrariesPanel::~SampleLibrariesPanel()
{
    mFileTree->removeListener(this);
    mSampleItemLibraryViewModel->getDirectoryList()->removeChangeListener(&*mFileTree);
}

void SampleLibrariesPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourMedium);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), PANEL_CORNER_SIZE);
}

void SampleLibrariesPanel::setPanelComponents()
{
    // Setting view model
    mSampleItemLibraryViewModel = std::make_unique<SampleLibraryViewModel>(currentProcessor.getSampleItemLibrary());
    
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(*mSampleItemLibraryViewModel);
    mFileTree->setBounds(PANEL_MARGIN / 2.0,
                         PANEL_MARGIN / 2.0,
                         getWidth() - PANEL_MARGIN,
                         getHeight() - PANEL_MARGIN);
    mFileTree->setTitle("AudioFiles");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, BlomeColour_Transparent);
    mFileTree->setDragAndDropDescription("AudioFile"); // Currently blocked by the mouse drag in BlomeFileTreeView performing an external file drag
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    mSampleItemLibraryViewModel->getDirectoryList()->addChangeListener(&*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleLibrariesPanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(PANEL_MARGIN / 2.0, PANEL_MARGIN / 2.0 + NORMAL_BUTTON_HEIGHT, getWidth() - PANEL_MARGIN, getHeight() - PANEL_MARGIN);
    }
}

SampleLibraryViewModel& SampleLibrariesPanel::getViewModel()
{
    return *mSampleItemLibraryViewModel;
}

void SampleLibrariesPanel::selectionChanged()
{
    
}

void SampleLibrariesPanel::fileClicked(const File& file, const MouseEvent& mouseEvent)
{
    // Show options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Move File(s) to Trash", [&] { deleteFile(false); });
        popupMenu.addItem("Delete File(s) Permanently", [&] { deleteFile(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleLibrariesPanel::fileDoubleClicked(const File& inFile)
{
    // Load file into source
    if (!inFile.isDirectory() && isSupportedAudioFileFormat(inFile.getFileExtension()))
    {
        linkedSampleItemPanel.showAudioResource(URL(inFile));
    }
}

void SampleLibrariesPanel::browserRootChanged(const File&)
{
    
}

void SampleLibrariesPanel::changeListenerCallback(ChangeBroadcaster* source)
{
    // When the file tree state changes
    repaint();
}

void SampleLibrariesPanel::deleteFile(bool deletePermanently = false)
{
    // Delete all selected files
    for (int f = 0; f < mFileTree->getNumSelectedItems(); f++)
    {
        mSampleItemLibraryViewModel->removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
};


