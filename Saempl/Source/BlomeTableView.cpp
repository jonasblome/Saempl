/*
  ==============================================================================

    BlomeTableView.cpp
    Created: 26 Jun 2023 12:54:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeTableView.h"

BlomeTableView::BlomeTableView(SampleLibraryViewModel& inSampleLibraryViewModel, SampleItemPanel& inSampleItemPanel)
:   libraryViewModel(inSampleLibraryViewModel),
    linkedSampleItemPanel(inSampleItemPanel)
{
    setModel(this);
    setColour(ListBox::outlineColourId, COLOUR_ACCENT_LIGHT);
    setOutlineThickness(0);
    setColour(backgroundColourId, COLOUR_TRANSPARENT);
    
    for (int c = 0; c < TAG_CATEGORIES.size(); c++)
    {
        getHeader().addColumn(TAG_CATEGORIES[c],
                              c + 1,
                              200,
                              50,
                              400,
                              TableHeaderComponent::defaultFlags,
                              0);
    }
    
    getHeader().addColumn("Title",
                          TAG_CATEGORIES.size() + 1,
                          200,
                          50,
                          400,
                          TableHeaderComponent::defaultFlags,
                          0);
    getHeader().setSortColumnId(5, true);
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
    numRows = libraryViewModel.getSampleItems()->size();
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
    if (rowNumber > libraryViewModel.getSampleItems()->size())
    {
        return;
    }
    
    if (SampleItem* rowSampleItem = libraryViewModel.getSampleItems()->getUnchecked(rowNumber))
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
    if (columnId == TAG_CATEGORIES.size() + 1)
    {
        return inSampleItem->getTitle();
    }
    else if (columnId <= TAG_CATEGORIES.size())
    {
        String tagName = TAG_CATEGORIES[columnId - 1];
        return std::to_string(inSampleItem->getSampleTag(tagName)->getValue());
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
        String categoryName = newSortColumnId <= TAG_CATEGORIES.size() ? TAG_CATEGORIES[newSortColumnId - 1] : "Title";
        mComparator.setCompareCategory(categoryName);
        mComparator.setSortingDirection(isForwards);
        libraryViewModel.getSampleItems()->sort(mComparator);
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
        if (SampleItem* sampleItem = libraryViewModel.getSampleItems()->getUnchecked(r))
        {
            String text = sampleItem->getFilePath();
            widest = jmax(widest, FONT_SMALL_BOLD.getStringWidth(text));
        }
    }

    return widest + 8;
}

void BlomeTableView::cellDoubleClicked (int rowNumber, int columnId, const MouseEvent&)
{
    File inFile = libraryViewModel.getSampleItems()->getUnchecked(rowNumber)->getFilePath();
    linkedSampleItemPanel.tryShowAudioResource(inFile);
}

/**
 Determines the components behaviour when the mouse is being dragged on it.
 */
void BlomeTableView::mouseDrag(const MouseEvent& e)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (e.getLengthOfMousePress() > 50) {
        Point<int> mousePosition = e.getEventRelativeTo(this).position.toInt();
        
        // Check if any of the selected rows was dragged
        for (int s = 0; s < getNumSelectedRows(); s++) {
            Rectangle<int> itemBounds = getRowPosition(getSelectedRow(s), false);
            
            if (itemBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                // Add all selected rows to external drag
                for (int r = 0; r < getNumSelectedRows(); r++)
                {
                    selectedFilePaths.add(libraryViewModel.getSampleItems()->getUnchecked(getSelectedRow(r))->getFilePath());
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}
