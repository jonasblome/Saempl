/*
 ==============================================================================
 
 BlomeTableViewFavourites.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeTableViewFavourites.h"

BlomeTableViewFavourites::BlomeTableViewFavourites(SaemplAudioProcessor & inProcessor,
                                                   SampleNavigationPanel & inSampleNavigationPanel,
                                                   SampleItemPanel & inSampleItemPanel,
                                                   AudioPlayer & inAudioPlayer)
:
BlomeTableViewBase(inProcessor, inSampleItemPanel, inAudioPlayer),
sampleNavigationPanel(inSampleNavigationPanel)
{
    mSampleItemCollectionType = FAVOURITE_SAMPLES;
    
    getHeader().addColumn(PROPERTY_NAMES[0],
                          1,
                          200,
                          50,
                          400,
                          TableHeaderComponent::defaultFlags,
                          0);
    getHeader().setSortColumnId(1, true);
    getHeader().reSortTable();
}

BlomeTableViewFavourites::~BlomeTableViewFavourites()
{
    
}

void BlomeTableViewFavourites::cellClicked(int rowNumber, int columnId, MouseEvent const &mouseEvent)
{
    // Show sample options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Remove Sample(s) from Favourites", [this] { removeSampleItemFromFavourites(); } );
        popupMenu.addItem("Show Sample in Finder", [this] { showSampleInFinder(); });
        popupMenu.addItem("Show Sample in Navigation View", [this] { showSampleInNavigation(); });
        popupMenu.addItem("(Re-)analyse Sample(s)", [this] { reanalyseSamples(); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

/**
 Handles what happens when files are dropped onto the tree view.
 */
void BlomeTableViewFavourites::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the library and favourites
    sampleLibrary.addAllToFavourites(files);
}

void BlomeTableViewFavourites::removeSampleItemFromFavourites()
{
    // Delete all selected items from favourites
    Array<SampleItem*> sampleItems;
    
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        sampleItems.add(sampleLibrary
                        .getSampleItems(mSampleItemCollectionType)
                        .getUnchecked(getSelectedRow(r)));
    }
    
    sampleLibrary.removeAllFromFavourites(sampleItems);
}

void BlomeTableViewFavourites::showSampleInNavigation()
{
    sampleNavigationPanel.showSample(sampleLibrary
                                     .getSampleItems(mSampleItemCollectionType)
                                     .getUnchecked(getLastRowSelected())->getFilePath());
}
