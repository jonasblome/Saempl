/*
 ==============================================================================
 
 SampleLibrariesPanel.cpp
 Created: 22 May 2023 6:32:52am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleLibraryPanel.h"

SampleLibraryPanel::SampleLibraryPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH - style->PANEL_MARGIN / 2.0,
            style->SAMPLE_NAVIGATION_PANEL_HEIGHT - style->PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleLibraryPanel::~SampleLibraryPanel()
{
    std::unique_ptr<XmlElement> treeOpennessState = mFileTree->getOpennessState(true);
    treeOpennessState->setTagName("TreeOpennessState");
    mFileTree->removeListener(this);
    sampleLibrary.getDirectoryList().removeChangeListener(&*mFileTree);
}

void SampleLibraryPanel::paint(Graphics& g)
{
    // Set background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    
    // Draw Title
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getLocalBounds().
                           removeFromTop(style->PANEL_TITLE_HEIGHT).
                           reduced(style->PANEL_MARGIN / 2.0).
                           toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.drawFittedText("Folders - " + sampleLibrary.getCurrentLibraryPath(),
                     style->PANEL_MARGIN / 2.0,
                     style->PANEL_MARGIN / 2.0,
                     getWidth() - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN,
                     Justification::centred,
                     1);
}

void SampleLibraryPanel::setPanelComponents()
{
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(sampleLibrary);
    mFileTree->setBounds(style->PANEL_MARGIN / 2.0,
                         style->PANEL_TITLE_HEIGHT,
                         getWidth() - style->PANEL_MARGIN,
                         getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN / 2.0);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, style->COLOUR_TRANSPARENT);
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
        mFileTree->setBounds(style->PANEL_MARGIN / 2.0,
                             style->PANEL_TITLE_HEIGHT,
                             getWidth() - style->PANEL_MARGIN,
                             getHeight()
                             - style->PANEL_TITLE_HEIGHT
                             - style->PANEL_MARGIN / 2.0);
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
        popupMenu.addItem("Move File(s) to Trash", [this] { deleteFiles(false); });
        popupMenu.addItem("Add Sample(s) to Favorites", [this] { addToPalette(); });
        popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFiles(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleLibraryPanel::fileDoubleClicked(File const & inFile)
{
    if (!inFile.isDirectory() && !linkedSampleItemPanel.tryShowAudioResource(inFile))
    {
        showFileDeletedWarning();
    }
}

void SampleLibraryPanel::browserRootChanged(File const&)
{
    // Not used...
}

void SampleLibraryPanel::changeListenerCallback(ChangeBroadcaster* source)
{
    // When the file tree state changes
    repaint();
}

void SampleLibraryPanel::deleteFiles(bool deletePermanently = false)
{
    // Delete all selected files
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        sampleLibrary.removeSampleItem(mFileTree->getSelectedFile(f).getFullPathName(), deletePermanently);
    }
    
    sampleLibrary.refresh();
}

void SampleLibraryPanel::addToPalette()
{
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        sampleLibrary.addAllToPalette(mFileTree->getSelectedFile(f).getFullPathName());
    }
    
    sampleLibrary.refresh();
}

bool SampleLibraryPanel::keyPressed(const KeyPress& key)
{
    if (key.getKeyCode() == KeyPress::returnKey)
    {
        fileDoubleClicked(mFileTree->getSelectedFile(0));
        return true;
    }
    
    return false;
}
