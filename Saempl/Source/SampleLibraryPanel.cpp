/*
 ==============================================================================
 
 SampleLibraryPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleLibraryPanel.h"

SampleLibraryPanel::SampleLibraryPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
sampleItemPanel(inSampleItemPanel)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
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
    g.fillRoundedRectangle(getLocalBounds()
                           .toFloat()
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.5)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.25)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.25)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.25),
                           style->CORNER_SIZE_MEDIUM);
    
    // Draw Title
    drawDropShadow(g,
                   Rectangle<int>(0, 0, getWidth(), 20),
                   0,
                   0,
                   70,
                   style);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getLocalBounds()
                           .removeFromTop(style->PANEL_TITLE_HEIGHT)
                           .withTrimmedLeft(style->PANEL_MARGIN)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.75)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.75)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.25)
                           .toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_SMALL_BOLD);
    g.setColour(style->COLOUR_PANEL_TITLE_FONT);
    g.drawFittedText("Folder View - " + sampleLibrary.getCurrentLibraryPath(),
                     0,
                     0,
                     getWidth(),
                     style->PANEL_TITLE_HEIGHT,
                     Justification::centred,
                     1);
}

void SampleLibraryPanel::setPanelComponents()
{
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(sampleLibrary);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, style->COLOUR_TRANSPARENT);
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    resizePanelComponents();
    addAndMakeVisible(*mFileTree);
    sampleLibrary.getDirectoryList().addChangeListener(&*mFileTree);
}

void SampleLibraryPanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(style->PANEL_MARGIN,
                             style->PANEL_TITLE_HEIGHT + style->PANEL_MARGIN * 0.25,
                             getWidth() - style->PANEL_MARGIN * 1.75,
                             getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN);
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
        popupMenu.addItem("Re-analyse Sample(s)", [this] { reanalyseSamples(); });
        popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFiles(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleLibraryPanel::fileDoubleClicked(File const & inFile)
{
    if (!inFile.isDirectory() && !sampleItemPanel.tryShowAudioResource(inFile))
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
}

void SampleLibraryPanel::addToPalette()
{
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        sampleLibrary.addAllToPalette(mFileTree->getSelectedFile(f).getFullPathName());
    }
}

void SampleLibraryPanel::reanalyseSamples()
{
    // Reanalyse all selected files
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        sampleLibrary.reanalyseSampleItem(mFileTree->getSelectedFile(f).getFullPathName());
    }
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
