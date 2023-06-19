/*
  ==============================================================================

    SampleLibrariesPanel.cpp
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
    setSize(SAMPLE_LIBRARIES_PANEL_WIDTH - Blome_PanelMargin, SAMPLE_LIBRARIES_PANEL_HEIGHT - Blome_PanelMargin);
    setPanelComponents();
}

SampleLibrariesPanel::~SampleLibrariesPanel()
{
    mFileTree->removeListener(this);
    mSampleLibrariesViewModel->getDirectoryList()->removeChangeListener(&*mFileTree);
}

void SampleLibrariesPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourMedium);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Blome_PanelCornerSize);
}

void SampleLibrariesPanel::setPanelComponents()
{
    // Setting view model
    mSampleLibrariesViewModel = std::make_unique<SampleLibrariesViewModel>(currentProcessor.getSampleDatabase());
    
    // Add change directory button component
    mRefreshSampleLibrariesButton = std::make_unique<TextButton>("Refresh Library");
    mRefreshSampleLibrariesButton->setBounds(Blome_PanelMargin / 2.0,
                                            Blome_PanelMargin / 2.0,
                                            100 - Blome_PanelMargin / 2.0,
                                            Blome_NormalButtonHeight - Blome_PanelMargin / 2.0);
    mRefreshSampleLibrariesButton->onClick = [this] { mSampleLibrariesViewModel->refreshSampleDatabase(); };
    addAndMakeVisible(*mRefreshSampleLibrariesButton);
    
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(*mSampleLibrariesViewModel);
    mFileTree->setBounds(Blome_PanelMargin / 2.0,
                         Blome_PanelMargin / 2.0 + Blome_NormalButtonHeight,
                         getWidth() - Blome_PanelMargin,
                         getHeight() - Blome_NormalButtonHeight - Blome_PanelMargin);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, BlomeColour_Transparent);
    mFileTree->setDragAndDropDescription("SampleItemFile"); // Currently blocked by the mouse drag in BlomeFileTreeView performing an external file drag
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    mSampleLibrariesViewModel->getDirectoryList()->addChangeListener(&*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleLibrariesPanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(Blome_PanelMargin / 2.0, Blome_PanelMargin / 2.0 + Blome_NormalButtonHeight, getWidth() - Blome_PanelMargin, getHeight() - Blome_PanelMargin);
    }
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
        popupMenu.addItem("Placeholder", nullptr);
        popupMenu.addItem("Placeholder", nullptr);
        popupMenu.addItem("Delete File(s) Permanently", [&] { deleteFile(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleLibrariesPanel::fileDoubleClicked(const File& inFile)
{
    File linkedAudioFile = File(inFile.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + inFile.getFileNameWithoutExtension());
    
    // Load file into source
    if (!inFile.isDirectory() && isSupportedAudioFileFormat(linkedAudioFile.getFileExtension()))
    {
        linkedSampleItemPanel.showAudioResource(URL(linkedAudioFile));
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
        mSampleLibrariesViewModel->removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
};


