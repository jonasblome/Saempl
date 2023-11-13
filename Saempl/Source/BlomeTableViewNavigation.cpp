/*
 ==============================================================================
 
 BlomeTableViewNavigation.cpp
 Created: 1 Nov 2023 10:34:42am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeTableViewNavigation.h"

BlomeTableViewNavigation::BlomeTableViewNavigation(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
BlomeTableViewBase(inProcessor, inSampleItemPanel)
{
    sampleLibrary.addChangeListener(this);
    mSampleItemCollectionType = FILTERED_SAMPLES;
    
    // Add all sample item properties as table columns
    for (int c = 0; c < PROPERTY_NAMES.size(); c++)
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

void BlomeTableViewNavigation::cellClicked(int rowNumber, int columnId, MouseEvent const &mouseEvent)
{
    // Show delete options pop up menu
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

/**
 Handles what happens when files are dropped onto the tree view.
 */
void BlomeTableViewNavigation::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the database
    for (int f = 0; f < files.size(); f++)
    {
        sampleLibrary.addAllToSampleItems(files[f]);
    }
    
    sampleLibrary.refresh();
}

void BlomeTableViewNavigation::deleteFiles(bool deletePermanently = false)
{
    // Delete all selected files
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        sampleLibrary.removeSampleItem(sampleLibrary
                                       .getSampleItems(mSampleItemCollectionType)
                                       .getUnchecked(getSelectedRow(r))->getFilePath(),
                                       deletePermanently);
    }
    
    sampleLibrary.refresh();
}

void BlomeTableViewNavigation::addToPalette()
{
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        sampleLibrary.addAllToPalette(sampleLibrary
                                      .getSampleItems(mSampleItemCollectionType)
                                      .getUnchecked(getSelectedRow(r))->getFilePath());
    }
    
    sampleLibrary.refresh();
}

void BlomeTableViewNavigation::changeListenerCallback(ChangeBroadcaster *source)
{
    getHeader().reSortTable();
}
