/*
 ==============================================================================
 
 SampleFolderPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFolderPanel.h"

SampleFolderPanel::SampleFolderPanel(SaemplAudioProcessor& inProcessor,
                                     SampleItemPanel& inSampleItemPanel,
                                     AudioPlayer& inAudioPlayer)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
sampleItemPanel(inSampleItemPanel),
audioPlayer(inAudioPlayer)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    mSampleItemCollectionType = FILTERED_SAMPLES;
    setPanelComponents();
}

SampleFolderPanel::~SampleFolderPanel()
{
    mFileTree->removeListener(this);
    sampleLibrary.getDirectoryList().removeChangeListener(&*mFileTree);
}

void SampleFolderPanel::paint(Graphics& g)
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
    g.drawFittedText("Folder View - "
                     + sampleLibrary.getCurrentLibraryPath()
                     + " - " + std::to_string(sampleLibrary.getSampleItems(mSampleItemCollectionType).size()) + " Samples",
                     style->PANEL_MARGIN * 1.5,
                     0,
                     getWidth() - style->PANEL_MARGIN * 3,
                     style->PANEL_TITLE_HEIGHT,
                     Justification::centred,
                     1);
}

void SampleFolderPanel::setPanelComponents()
{
    // Set file tree component
    mFileTree = std::make_unique<BlomeFileTreeView>(sampleLibrary, audioPlayer);
    mFileTree->setTitle("Files");
    mFileTree->setColour(FileTreeComponent::backgroundColourId, style->COLOUR_TRANSPARENT);
    mFileTree->setMultiSelectEnabled(true);
    mFileTree->addListener(this);
    sampleLibrary.getDirectoryList().addChangeListener(&*mFileTree);
    addAndMakeVisible(*mFileTree);
    
    resizePanelComponents();
}

void SampleFolderPanel::resizePanelComponents()
{
    if (mFileTree != nullptr)
    {
        mFileTree->setBounds(style->PANEL_MARGIN,
                             style->PANEL_TITLE_HEIGHT + style->PANEL_MARGIN * 0.25,
                             getWidth() - style->PANEL_MARGIN * 1.75,
                             getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN);
    }
}

void SampleFolderPanel::selectionChanged()
{
    
}

void SampleFolderPanel::fileClicked(File const & file, MouseEvent const & mouseEvent)
{
    // Show sample options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Move File(s) to Trash", [this] { deleteFiles(false); });
        popupMenu.addItem("Add Sample(s) to Favourites", [this] { addToFavourites(); });
        popupMenu.addItem("Show in Finder", [this] { showSampleInFinder(); });
        popupMenu.addItem("(Re-)analyse Sample(s)", [this] { reanalyseSamples(); });
        popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFiles(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void SampleFolderPanel::fileDoubleClicked(File const & inFile)
{
    if (!inFile.isDirectory())
    {
        sampleItemPanel.tryShowAudioResource(inFile);
    }
}

void SampleFolderPanel::browserRootChanged(File const &)
{
    // Not used...
}

void SampleFolderPanel::changeListenerCallback(ChangeBroadcaster* source)
{
    // When the file tree state changes
    repaint();
}

void SampleFolderPanel::deleteFiles(bool deletePermanently = false)
{
    // Delete all selected files
    StringArray filePaths;
    
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        filePaths.add(mFileTree->getSelectedFile(f).getFullPathName());
    }
    
    sampleLibrary.removeSampleItems(filePaths, deletePermanently);
}

void SampleFolderPanel::addToFavourites()
{
    StringArray filePaths;
    
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        filePaths.add(mFileTree->getSelectedFile(f).getFullPathName());
    }
    
    sampleLibrary.addAllToFavourites(filePaths);
}

void SampleFolderPanel::reanalyseSamples()
{
    // Reanalyse all selected files
    StringArray filePaths;
    
    for (int f = mFileTree->getNumSelectedItems() - 1; f >= 0 ; f--)
    {
        filePaths.add(mFileTree->getSelectedFile(f).getFullPathName());
    }
    
    sampleLibrary.reanalyseSampleItems(filePaths);
}

bool SampleFolderPanel::keyPressed(KeyPress const & key)
{
    if (!isShowing())
    {
        return false;
    }
    
    if (key.getKeyCode() == KeyPress::returnKey)
    {
        sampleItemPanel.tryShowAudioResource(mFileTree->getSelectedFile(0));
        return true;
    }
    else if (key.getKeyCode() == 75) // K
    {
        mFileTree->playSelectedSample();
        return true;
    }
    else
    {
        return mFileTree->keyPressed(key);
    }
}

void SampleFolderPanel::showSampleInFinder()
{
    mFileTree->getSelectedFile(mFileTree->getNumSelectedFiles() - 1).revealToUser();
}

void SampleFolderPanel::showSample(String inFilePath)
{
    mFileTree->setSelectedFile(inFilePath);
}
