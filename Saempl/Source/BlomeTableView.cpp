/*
  ==============================================================================

    BlomeTableView.cpp
    Created: 26 Jun 2023 12:54:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeTableView.h"

BlomeTableView::BlomeTableView(SampleLibraryViewModel& inLibraryViewModel)
:   libraryViewModel(inLibraryViewModel)
{
    setModel(this);
    setBounds(PANEL_MARGIN / 2.0, PANEL_MARGIN / 2.0, getWidth() - PANEL_MARGIN, getHeight() - PANEL_MARGIN);
    setColour(ListBox::outlineColourId, COLOUR_ACCENT_LIGHT);
    setOutlineThickness(0);
    
    for (int c = 0; c < TAG_CATEGORIES.size(); c++)
    {
        getHeader().addColumn(TAG_CATEGORIES[c],
                              getHeader().getNumColumns(false) + 1,
                              200,
                              50,
                              400,
                              TableHeaderComponent::defaultFlags,
                              0);
    }
    
    getHeader().addColumn("Title",
                          getHeader().getNumColumns(false) + 1,
                          200,
                          50,
                          400,
                          TableHeaderComponent::defaultFlags,
                          0);
    getHeader().setSortColumnId(5, true);
    getHeader().setStretchToFitActive(true);
    setMultipleSelectionEnabled(true);
}

BlomeTableView::~BlomeTableView()
{
    
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
        return std::to_string(inSampleItem->getSampleTag(columnId - 1)->getValue());
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

void BlomeTableView::refresh()
{
    updateContent();
}
