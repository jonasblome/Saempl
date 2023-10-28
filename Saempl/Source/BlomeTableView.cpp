/*
  ==============================================================================

    BlomeTableView.cpp
    Created: 26 Jun 2023 12:54:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeTableView.h"

BlomeTableView::BlomeTableView(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel)
:   sampleLibrary(inSampleLibrary),
    linkedSampleItemPanel(inSampleItemPanel)
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
    numRows = sampleLibrary.getFilteredSampleItems().size();
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
    if (rowNumber > sampleLibrary.getFilteredSampleItems().size())
    {
        return;
    }
    
    if (SampleItem* rowSampleItem = sampleLibrary.getFilteredSampleItems().getUnchecked(rowNumber))
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
        sampleLibrary.getFilteredSampleItems().sort(mComparator);
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
        if (SampleItem* sampleItem = sampleLibrary.getFilteredSampleItems().getUnchecked(r))
        {
            String text = sampleItem->getFilePath();
            widest = jmax(widest, FONT_SMALL_BOLD.getStringWidth(text));
        }
    }

    return widest + 8;
}

void BlomeTableView::cellDoubleClicked (int rowNumber, int columnId, MouseEvent const &)
{
    File inFile = sampleLibrary.getFilteredSampleItems().getUnchecked(rowNumber)->getFilePath();
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
        for (int s = 0; s < getNumSelectedRows(); s++) {
            Rectangle<int> itemBounds = getRowPosition(getSelectedRow(s), false);
            
            if (itemBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                // Add all selected rows to external drag
                for (int r = 0; r < getNumSelectedRows(); r++)
                {
                    selectedFilePaths.add(sampleLibrary.getFilteredSampleItems().getUnchecked(getSelectedRow(r))->getFilePath());
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}
