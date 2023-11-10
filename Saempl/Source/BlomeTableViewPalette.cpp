/*
 ==============================================================================
 
 BlomeTableViewPalette.cpp
 Created: 1 Nov 2023 10:34:25am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeTableViewPalette.h"

BlomeTableViewPalette::BlomeTableViewPalette(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
BlomeTableViewBase(inProcessor, inSampleItemPanel)
{
    mSampleItemCollectionType = PALETTE_SAMPLES;
    
    getHeader().addColumn(PROPERTY_NAMES[PROPERTY_NAMES.size() - 1],
                          1,
                          200,
                          50,
                          400,
                          TableHeaderComponent::defaultFlags,
                          0);
    getHeader().setSortColumnId(1, true);
    getHeader().reSortTable();
}

BlomeTableViewPalette::~BlomeTableViewPalette()
{
    
}

void BlomeTableViewPalette::cellClicked(int rowNumber, int columnId, MouseEvent const &mouseEvent)
{
    // Show options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Remove Sample(s) from Palette", [this] { removeSampleItemFromPalette(); } );
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

/**
 Handles what happens when files are dropped onto the tree view.
 */
void BlomeTableViewPalette::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the library and palette
    for (int f = 0; f < files.size(); f++)
    {
        sampleLibrary.addAllToPalette(files[f]);
    }
    
    sampleLibrary.refresh();
}

void BlomeTableViewPalette::removeSampleItemFromPalette()
{
    // Delete all selected items from palette
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        sampleLibrary.removeFromPalette(*sampleLibrary
                                        .getSampleItems(mSampleItemCollectionType)
                                        .getUnchecked(getSelectedRow(r)));
    }
    
    sampleLibrary.refresh();
}
