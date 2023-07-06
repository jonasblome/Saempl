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
    auto items = libraryViewModel.getSampleItems();
    numRows = items->size();
    
    mTable = std::make_unique<TableListBox>();
    mTable->setBounds(0, 0, getWidth(), getHeight());
    mTable->setModel(this);
    mTable->setColour(ListBox::outlineColourId, COLOUR_ACCENT_LIGHT);
    mTable->setOutlineThickness(1);
    mTable->getHeader().addColumn("Filename",
                                  1,
                                  200,
                                  50,
                                  400,
                                  TableHeaderComponent::defaultFlags);
    mTable->getHeader().setSortColumnId(1, true);
    mTable->getHeader().setStretchToFitActive(true);
    mTable->setMultipleSelectionEnabled(true);
    addAndMakeVisible(*mTable);
}

BlomeTableView::~BlomeTableView()
{
    
}

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int BlomeTableView::getNumRows()
{
    return numRows;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void BlomeTableView::paintRowBackground(Graphics& g,
                                         int rowNumber,
                                         int width,
                                         int height,
                                         bool rowIsSelected)
{
    auto alternateColour = getLookAndFeel()
        .findColour(ListBox::backgroundColourId)
        .interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
    if (rowIsSelected)
    {
        g.fillAll (Colours::lightblue);
    }
    else if (rowNumber % 2)
    {
        g.fillAll (alternateColour);
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

    if (auto* rowElement = libraryViewModel.getSampleItems()->getUnchecked(rowNumber))
    {
        auto text = rowElement->getFilePath();

        g.drawText(text,
                   2,
                   0,
                   width - 4,
                   height,
                   Justification::centredLeft,
                   true);
    }

    g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
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
    for (int i = getNumRows(); --i >= 0;)
    {
        if (SampleItem* sampleItem = libraryViewModel.getSampleItems()->getUnchecked(i))
        {
            auto text = sampleItem->getFilePath();

            widest = jmax(widest, FONT_SMALL_BOLD.getStringWidth(text));
        }
    }

    return widest + 8;
}
