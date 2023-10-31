/*
  ==============================================================================

    SampleLibrariesPanel.cpp
    Created: 22 May 2023 6:32:52am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibraryPanel.h"

SampleLibraryPanel::SampleLibraryPanel(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel)
:   PanelBase(),
    sampleLibrary(inSampleLibrary),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(SAMPLE_NAVIGATION_PANEL_WIDTH - PANEL_MARGIN / 2.0, SAMPLE_NAVIGATION_PANEL_HEIGHT - PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleLibraryPanel::~SampleLibraryPanel()
{
    mFileTree->removeListener(this);
    sampleLibrary.getDirectoryList().removeChangeListener(&*mFileTree);
}

void SampleLibraryPanel::paint(Graphics& g)
{
    // Set background
    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    
    // Draw Title
    g.setColour(COLOUR_BLACK_LIGHT_TRANSPARENT);
    g.fillRoundedRectangle(getLocalBounds().
                           removeFromTop(PANEL_TITLE_HEIGHT).
                           reduced(PANEL_MARGIN / 2.0).
                           toFloat(),
                           CORNER_SIZE_MEDIUM);
    g.setFont(FONT_MEDIUM_SMALL_BOLD);
    g.setColour(COLOUR_ACCENT_LIGHT);
    g.drawFittedText("Sample Library - " + sampleLibrary.getCurrentLibraryPath(),
                     PANEL_MARGIN / 2.0,
                     PANEL_MARGIN / 2.0,
                     getWidth() - PANEL_MARGIN,
                     PANEL_TITLE_HEIGHT - PANEL_MARGIN,
                     Justification::centred,
                     1);
}

void SampleLibraryPanel::setPanelComponents()
{
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(sampleLibrary);
    mFileTree->setBounds(PANEL_MARGIN / 2.0,
                         PANEL_TITLE_HEIGHT,
                         getWidth() - PANEL_MARGIN,
                         getHeight() - PANEL_TITLE_HEIGHT - PANEL_MARGIN / 2.0);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, COLOUR_TRANSPARENT);
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    addAndMakeVisible(*mFileTree);
    sampleLibrary.getDirectoryList().addChangeListener(&*mFileTree);
    
    // Repaint panel
    repaint();
}

void SampleLibraryPanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(PANEL_MARGIN / 2.0,
                             PANEL_TITLE_HEIGHT,
                             getWidth() - PANEL_MARGIN,
                             getHeight() - PANEL_TITLE_HEIGHT - PANEL_MARGIN / 2.0);
    }
}

void SampleLibraryPanel::selectionChanged()
{
    
}

void SampleLibraryPanel::fileClicked(File const & file, MouseEvent const & mouseEvent)
{
    // Show options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Move File(s) to Trash", [this] { deleteFile(false); });
        popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFile(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleLibraryPanel::fileDoubleClicked(File const & inFile)
{
    linkedSampleItemPanel.tryShowAudioResource(inFile);
}

void SampleLibraryPanel::browserRootChanged(File const&)
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
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        sampleLibrary.removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
    
    sampleLibrary.refresh();
};


