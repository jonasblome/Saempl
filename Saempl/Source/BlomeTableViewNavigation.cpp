/*
 ==============================================================================
 
 BlomeTableViewNavigation.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeTableViewNavigation.h"

BlomeTableViewNavigation::BlomeTableViewNavigation(SaemplAudioProcessor& inProcessor,
                                                   SampleItemPanel& inSampleItemPanel,
                                                   AudioPlayer& inAudioPlayer)
:
BlomeTableViewBase(inProcessor, inSampleItemPanel, inAudioPlayer)
{
    sampleLibrary.addChangeListener(this);
    mSampleItemCollectionType = FILTERED_SAMPLES;
    
    // Add all sample item properties as table columns
    for (int c = PROPERTY_NAMES.size() - 1; c >= 0 ; c--)
    {
        getHeader().addColumn(PROPERTY_NAMES[c],
                              c + 1,
                              200,
                              50,
                              400,
                              TableHeaderComponent::defaultFlags,
                              0);
    }
    
    getHeader().setSortColumnId(PROPERTY_NAMES.indexOf(currentProcessor.getSortingColumnTitle()) + 1,
                                currentProcessor.getSortingDirection());
    getHeader().reSortTable();
}

BlomeTableViewNavigation::~BlomeTableViewNavigation()
{
    sampleLibrary.removeChangeListener(this);
}

void BlomeTableViewNavigation::cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent)
{
    // Show delete options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Move File(s) to Trash", [this] { deleteFiles(false); });
        popupMenu.addItem("Add Sample(s) to Favourites", [this] { addToFavourites(); });
        popupMenu.addItem("(Re-)analyse Sample(s)", [this] { reanalyseSamples(); });
        popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFiles(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void BlomeTableViewNavigation::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the database
    sampleLibrary.addAllToSampleItems(files);
}

void BlomeTableViewNavigation::deleteFiles(bool deletePermanently = false)
{
    // Delete all selected files
    StringArray filePaths;
    
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        filePaths.add(sampleLibrary
                      .getSampleItems(mSampleItemCollectionType)
                      .getUnchecked(getSelectedRow(r))->getFilePath());
    }
    
    sampleLibrary.removeSampleItems(filePaths, deletePermanently);
}

void BlomeTableViewNavigation::addToFavourites()
{
    StringArray filePaths;
    
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        filePaths.add(sampleLibrary
                      .getSampleItems(mSampleItemCollectionType)
                      .getUnchecked(getSelectedRow(r))->getFilePath());
    }
    
    sampleLibrary.addAllToFavourites(filePaths);
}

void BlomeTableViewNavigation::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &sampleLibrary && isShowing())
    {
        resortTable();
    }
}

void BlomeTableViewNavigation::resortTable()
{
    getHeader().reSortTable();
}
