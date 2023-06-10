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
    linkedSampleItemPanel(inSampleItemPanel),
    mButtonHeight(25)
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
    
    // Add play button component
    mChangeDirectoryButton = std::make_unique<TextButton>("^");
    mChangeDirectoryButton->setBounds(Blome_PanelMargin / 2.0,
                                Blome_PanelMargin / 2.0,
                                mButtonHeight - Blome_PanelMargin / 2.0,
                                mButtonHeight - Blome_PanelMargin / 2.0);
    mChangeDirectoryButton->onClick = [this] { mSampleDatabaseTableViewModel->switchToParentDirectory(); };
    addAndMakeVisible(*mChangeDirectoryButton);
    
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(*mSampleDatabaseTableViewModel);
    mFileTree->setBounds(Blome_PanelMargin / 2.0, Blome_PanelMargin / 2.0 + mButtonHeight, getWidth() - Blome_PanelMargin, getHeight() - mButtonHeight - Blome_PanelMargin);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, BlomeColour_Transparent);
    mFileTree->setDragAndDropDescription("SampleItemFile");
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    mSampleDatabaseTableViewModel->getDirectoryList()->addChangeListener(&*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleDatabaseTablePanel::resizePanelComponents()
{
    if(mFileTree != nullptr)
    {
        mFileTree->setBounds(Blome_PanelMargin / 2.0, Blome_PanelMargin / 2.0 + mButtonHeight, getWidth() - Blome_PanelMargin, getHeight() - Blome_PanelMargin);
    }
}

void SampleDatabaseTablePanel::selectionChanged()
{
    
}

void SampleDatabaseTablePanel::fileClicked(const File& file, const MouseEvent& mouseEvent)
{
    if(mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Move File(s) to Trash", [&] { deleteFile(false); });
        popupMenu.addItem("Placeholder", nullptr);
        popupMenu.addItem("Placeholder", nullptr);
        popupMenu.addItem("Delete File(s) Permanently", [&] { deleteFile(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleDatabaseTablePanel::fileDoubleClicked(const File& file)
{
    if (!file.isDirectory() && isSupportedAudioFileFormat(file.getFileExtension()))
    {
        linkedSampleItemPanel.showAudioResource(URL(file));
    }
    else if (file.isDirectory())
    {
        mSampleDatabaseTableViewModel->setDirectory(file);
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
    for (int f = 0; f < mFileTree->getNumSelectedItems(); f++)
    {
        mSampleDatabaseTableViewModel->removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
};


