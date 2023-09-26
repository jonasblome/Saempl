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
    mTable = std::make_unique<TableListBox>();
    mTable->setBounds(0, 0, getWidth(), getHeight());
    mTable->setColour(ListBox::outlineColourId, COLOUR_ACCENT_LIGHT);
    mTable->setOutlineThickness(1);
    mTable->getHeader().addColumn("Filename",
                                  1,
                                  200,
                                  50,
                                  400,
                                  TableHeaderComponent::defaultFlags);
//    mTable->getHeader().setSortColumnId(1, true);
    mTable->getHeader().setStretchToFitActive(true);
    mTable->setMultipleSelectionEnabled(true);
    mTable->setModel(this);
    addAndMakeVisible(*mTable);
}

BlomeTableView::~BlomeTableView()
{
    
}

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int BlomeTableView::getNumRows()
{
    return libraryViewModel.getSampleItems()->size();
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
    g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
    g.setFont(FONT_SMALL_BOLD);

    if (SampleItem* rowElement = libraryViewModel.getSampleItems()->getUnchecked(rowNumber))
    {
        g.drawText(rowElement->getFilePath(),
                   2,
                   0,
                   width - 4,
                   height,
                   Justification::centredLeft,
                   true);
    }

    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRect(width - 1,
               0,
               1,
               height);
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
    mTable->updateContent();
}
