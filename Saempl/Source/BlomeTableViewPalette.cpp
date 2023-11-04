/*
  ==============================================================================

    BlomeTableViewPalette.cpp
    Created: 1 Nov 2023 10:34:25am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeTableViewPalette.h"

BlomeTableViewPalette::BlomeTableViewPalette(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel)
:   BlomeTableViewBase(inSampleLibrary, inSampleItemPanel)
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
        popupMenu.addItem("Remove Sample from Palette", [this] { removeSampleItemFromPalette(); } );
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
        sampleLibrary.addToPalette(files[f]);
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

// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void BlomeTableViewPalette::sortOrderChanged(int newSortColumnId, bool isForwards)
{
    // Sort items by title and chosen direction
    if (newSortColumnId != 0)
    {
        mComparator.setCompareProperty("Title");
        mComparator.setSortingDirection(isForwards);
        sampleLibrary.getSampleItems(mSampleItemCollectionType).sort(mComparator);
        updateContent();
    }
}
