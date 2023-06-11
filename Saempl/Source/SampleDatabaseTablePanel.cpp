/*
  ==============================================================================

    SampleDatabaseTablePanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleDatabaseTablePanel.h"

SampleDatabaseTablePanel::SampleDatabaseTablePanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(TABLE_PANEL_WIDTH - Blome_PanelMargin, TABLE_PANEL_HEIGHT - Blome_PanelMargin);
    setPanelComponents();
}

SampleDatabaseTablePanel::~SampleDatabaseTablePanel()
{
    mFileTree->removeListener(this);
    mSampleDatabaseTableViewModel->getDirectoryList()->removeChangeListener(&*mFileTree);
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
    mSampleDatabaseTableViewModel = std::make_unique<SampleDatabaseTableViewModel>(currentProcessor.getSampleDatabase());
    
    // Add change directory button component
    mChangeDirectoryButton = std::make_unique<TextButton>("Go One Folder Up");
    mChangeDirectoryButton->setBounds(Blome_PanelMargin / 2.0,
                                      Blome_PanelMargin / 2.0,
                                      150 - Blome_PanelMargin / 2.0,
                                      Blome_NormalButtonHeight - Blome_PanelMargin / 2.0);
    mChangeDirectoryButton->onClick = [this] { mSampleDatabaseTableViewModel->switchToParentDirectory(); };
    addAndMakeVisible(*mChangeDirectoryButton);
    
    // Add change directory button component
    mRefreshSampleDatabaseButton = std::make_unique<TextButton>("Refresh Sample Library");
    mRefreshSampleDatabaseButton->setBounds(150 + Blome_PanelMargin / 2.0,
                                            Blome_PanelMargin / 2.0,
                                            150 - Blome_PanelMargin / 2.0,
                                            Blome_NormalButtonHeight - Blome_PanelMargin / 2.0);
    mRefreshSampleDatabaseButton->onClick = [this] { mSampleDatabaseTableViewModel->refreshSampleDatabase(); };
    addAndMakeVisible(*mRefreshSampleDatabaseButton);
    
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(*mSampleDatabaseTableViewModel);
    mFileTree->setBounds(Blome_PanelMargin / 2.0, Blome_PanelMargin / 2.0 + Blome_NormalButtonHeight, getWidth() - Blome_PanelMargin, getHeight() - Blome_NormalButtonHeight - Blome_PanelMargin);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, BlomeColour_Transparent);
    mFileTree->setDragAndDropDescription("SampleItemFile"); // Currently blocked by the mouse drag in BlomeFileTreeView performing an external file drag
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    mSampleDatabaseTableViewModel->getDirectoryList()->addChangeListener(&*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleDatabaseTablePanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(Blome_PanelMargin / 2.0, Blome_PanelMargin / 2.0 + Blome_NormalButtonHeight, getWidth() - Blome_PanelMargin, getHeight() - Blome_PanelMargin);
    }
}

void SampleDatabaseTablePanel::selectionChanged()
{
    
}

void SampleDatabaseTablePanel::fileClicked(const File& file, const MouseEvent& mouseEvent)
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

void SampleDatabaseTablePanel::fileDoubleClicked(const File& inFile)
{
    File linkedAudioFile = File(inFile.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + inFile.getFileNameWithoutExtension());
    
    // Load file into source
    if (!inFile.isDirectory() && isSupportedAudioFileFormat(linkedAudioFile.getFileExtension()))
    {
        linkedSampleItemPanel.showAudioResource(URL(linkedAudioFile));
    }
    // Switch to selected directory
    else if (inFile.isDirectory())
    {
        mSampleDatabaseTableViewModel->setDirectory(inFile);
    }
}

void SampleDatabaseTablePanel::browserRootChanged(const File&)
{
    
}

void SampleDatabaseTablePanel::changeListenerCallback(ChangeBroadcaster* source)
{
    // When the file tree state changes
    repaint();
}

void SampleDatabaseTablePanel::deleteFile(bool deletePermanently = false)
{
    // Delete all selected files
    for (int f = 0; f < mFileTree->getNumSelectedItems(); f++)
    {
        mSampleDatabaseTableViewModel->removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
};


