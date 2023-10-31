/*
  ==============================================================================

    BlomeTableView.cpp
    Created: 26 Jun 2023 12:54:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeTableView.h"

BlomeTableView::BlomeTableView(SampleLibrary& inSampleLibrary,
                               SampleItemPanel& inSampleItemPanel,
                               SampleItemCollectionType inSampleItemCollectionType)
:   sampleLibrary(inSampleLibrary),
    linkedSampleItemPanel(inSampleItemPanel),
    mSampleItemCollectionType(inSampleItemCollectionType)
{
    setModel(this);
    setColour(ListBox::outlineColourId, COLOUR_ACCENT_LIGHT);
    setOutlineThickness(0);
    setColour(backgroundColourId, COLOUR_TRANSPARENT);
    
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
    
    getHeader().setSortColumnId(PROPERTY_NAMES.size(), true);
    getHeader().setStretchToFitActive(true);
    setMultipleSelectionEnabled(true);
    
    // Mouse listener catches events from the table's children
    addMouseListener(this, true);
}

BlomeTableView::~BlomeTableView()
{
    removeMouseListener(this);
}

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int BlomeTableView::getNumRows()
{
    numRows = sampleLibrary.getSampleItems(mSampleItemCollectionType).size();
    return numRows;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void BlomeTableView::paintRowBackground(Graphics& g,
                                         int rowNumber,
                                         int width,
                                         int height,
                                         bool rowIsSelected)
{
    g.fillAll(COLOUR_ACCENT_MEDIUM);
    
    if (rowIsSelected)
    {
        g.fillAll(COLOUR_ACCENT_DARK);
    }
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void BlomeTableView::paintCell(Graphics& g,
                               int rowNumber,
                               int columnId,
                               int width,
                               int height,
                               bool rowIsSelected)
{
    if (rowNumber > sampleLibrary.getSampleItems(mSampleItemCollectionType).size())
    {
        return;
    }
    
    if (SampleItem* rowSampleItem = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(rowNumber))
    {
        g.setColour(COLOUR_ACCENT_MEDIUM);
        g.fillRect(width - 1,
                   0,
                   1,
                   height);
        
        g.setColour(COLOUR_ACCENT_LIGHT);
        g.setFont(FONT_SMALL_BOLD);

        g.drawText(getCellText(rowSampleItem, columnId),
                   2,
                   0,
                   width - 4,
                   height,
                   Justification::centredLeft,
                   true);
    }
}

String BlomeTableView::getCellText(SampleItem* inSampleItem, int columnId)
{
    if (PROPERTY_NAMES[columnId - 1] == "Title")
    {
        return inSampleItem->getTitle();
    }
    else if (PROPERTY_NAMES[columnId - 1] == "Length")
    {
        return std::to_string(inSampleItem->getLength());
    }
    else
    {
        return "unknown";
    }
}

// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void BlomeTableView::sortOrderChanged (int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
        String propertyName = newSortColumnId <= PROPERTY_NAMES.size() ? PROPERTY_NAMES[newSortColumnId - 1] : "Title";
        mComparator.setCompareProperty(propertyName);
        mComparator.setSortingDirection(isForwards);
        sampleLibrary.getSampleItems(mSampleItemCollectionType).sort(mComparator);
        updateContent();
    }
}

// This is overloaded from TableListBoxModel, and should choose the best width for the specified
// column.
int BlomeTableView::getColumnAutoSizeWidth(int columnId)
{
    int widest = 32;

    // Find the widest bit of text in this column..
    for (int r = getNumRows(); --r >= 0;)
    {
        if (SampleItem* sampleItem = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(r))
        {
            String text = sampleItem->getFilePath();
            widest = jmax(widest, FONT_SMALL_BOLD.getStringWidth(text));
        }
    }

    return widest + 8;
}

void BlomeTableView::cellClicked(int rowNumber, int columnId, MouseEvent const &mouseEvent)
{
    // Show options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        
        switch (mSampleItemCollectionType)
        {
            case ALL_SAMPLES:
            case FILTERED_SAMPLES:
            {
                popupMenu.addItem("Move File(s) to Trash", [this] { deleteFile(false); });
                popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFile(true); });
                break;
            }
            case PALETTE_SAMPLES:
            {
                popupMenu.addItem("Remove Sample from Palette", [this] { removeSampleItemFromPalette(); } );
                break;
            }
            default:
                break;
        }
        
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void BlomeTableView::cellDoubleClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent)
{
    File inFile = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(rowNumber)->getFilePath();
    linkedSampleItemPanel.tryShowAudioResource(inFile);
}

/**
 Determines the components behaviour when the mouse is being dragged on it.
 */
void BlomeTableView::mouseDrag(MouseEvent const & e)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (e.getLengthOfMousePress() > 50) {
        Point<int> mousePosition = e.getEventRelativeTo(this).position.toInt();
        
        // Check if any of the selected rows was dragged
        for (int s = 0; s < getNumSelectedRows(); s++)
        {
            Rectangle<int> rowBounds = getRowPosition(getSelectedRow(s), true);
            
            if (rowBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                // Add all selected rows to external drag
                for (int r = 0; r < getNumSelectedRows(); r++)
                {
                    selectedFilePaths.add(sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(getSelectedRow(r))->getFilePath());
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}

/**
 Handles what happens when files are dropped onto the tree view.
 */
void BlomeTableView::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the database
    switch (mSampleItemCollectionType)
    {
        case ALL_SAMPLES:
        case FILTERED_SAMPLES:
        {
            for (int f = 0; f < files.size(); f++)
            {
                sampleLibrary.addToSampleItems(files[f]);
            }
            break;
        }
        case PALETTE_SAMPLES:
        {
            for (int f = 0; f < files.size(); f++)
            {
                sampleLibrary.addToSampleItems(files[f]);
                sampleLibrary.addToPalette(files[f]);
            }
            break;
        }
        default:
            break;
    }
    
    sampleLibrary.refresh();
}

/**
 Sets a flag if the tree view is interested in drag and drop of files.
 */
bool BlomeTableView::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}

void BlomeTableView::deleteFile(bool deletePermanently = false)
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
};

void BlomeTableView::removeSampleItemFromPalette()
{
    // Delete all selected items from palette
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        sampleLibrary.removeSampleItemFromPalette(*sampleLibrary
                                       .getSampleItems(mSampleItemCollectionType)
                                       .getUnchecked(getSelectedRow(r)));
    }
    
    sampleLibrary.refresh();
}
