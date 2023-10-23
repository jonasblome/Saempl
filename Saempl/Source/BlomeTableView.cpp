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
    setBounds(0, 0, getWidth(), getHeight());
    setColour(ListBox::outlineColourId, COLOUR_ACCENT_LIGHT);
    setOutlineThickness(1);
    getHeader().addColumn("Filename",
                                  1,
                                  200,
                                  50,
                                  400,
                                  TableHeaderComponent::defaultFlags,
                                  0);
    getHeader().setSortColumnId(1, true);
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
    if (rowNumber > libraryViewModel.getSampleItems()->size()) {
        return;
    }
    
    if (SampleItem* rowElement = libraryViewModel.getSampleItems()->getUnchecked(rowNumber))
    {
        g.setColour(COLOUR_ACCENT_MEDIUM);
        g.fillRect(width - 1,
                   0,
                   1,
                   height);
        
        g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
        g.setFont(FONT_SMALL_BOLD);

        g.drawText(rowElement->getFilePath(),
                   2,
                   0,
                   width - 4,
                   height,
                   Justification::centredLeft,
                   true);
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
