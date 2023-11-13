/*
 ==============================================================================
 
 BlomeTableView.cpp
 Created: 26 Jun 2023 12:54:09am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeTableViewBase.h"

BlomeTableViewBase::BlomeTableViewBase(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
currentProcessor(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
linkedSampleItemPanel(inSampleItemPanel)
{
    mComparator = std::make_unique<SampleItemComparator>();
    
    setModel(this);
    setOutlineThickness(0);
    getHeader().setStretchToFitActive(true);
    setMultipleSelectionEnabled(true);
    
    // Mouse listener catches events from the table's children
    addMouseListener(this, true);
}

BlomeTableViewBase::~BlomeTableViewBase()
{
    removeMouseListener(this);
}

void BlomeTableViewBase::paint(Graphics& g)
{
    drawDropShadow(g,
                   Rectangle<int>(0, 0, getWidth(), 0),
                   0,
                   0,
                   40,
                   style);
}

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int BlomeTableViewBase::getNumRows()
{
    numRows = sampleLibrary.getSampleItems(mSampleItemCollectionType).size();
    return numRows;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void BlomeTableViewBase::paintRowBackground(Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.setColour(style->COLOUR_ACCENT_DARK);
        g.fillRoundedRectangle(Rectangle<float>(width, height), style->CORNER_SIZE_MEDIUM);
    }
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void BlomeTableViewBase::paintCell(Graphics& g,
                                   int rowNumber,
                                   int columnId,
                                   int width,
                                   int height,
                                   bool rowIsSelected)
{
    // Dont draw more rows than items in collection
    if (rowNumber >= sampleLibrary.getSampleItems(mSampleItemCollectionType).size())
    {
        return;
    }
    
    if (SampleItem* rowSampleItem = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(rowNumber))
    {
        // Draw cell separators
        g.setColour(style->COLOUR_ACCENT_MEDIUM);
        g.fillRect(width - 1,
                   0,
                   1,
                   height);
        
        // Draw cell text
        g.setColour(style->COLOUR_ACCENT_DARK);
        g.setFont(style->FONT_SMALL_BOLD);
        g.drawText(getCellText(rowSampleItem, getHeader().getColumnName(columnId)),
                   2,
                   0,
                   width - 4,
                   height,
                   Justification::centredLeft,
                   true);
    }
}

String BlomeTableViewBase::getCellText(SampleItem* inSampleItem, String columnName)
{
    // Get text for sample item property
    if (columnName == "Title")
    {
        return inSampleItem->getTitle();
    }
    else if (columnName == "Length")
    {
        return std::to_string(inSampleItem->getLength());
    }
    else if (columnName == "Loudness")
    {
        return std::to_string(inSampleItem->getLoudness());
    }
    else
    {
        return "unknown";
    }
}

// This is overloaded from TableListBoxModel, and should choose the best width for the specified
// column.
int BlomeTableViewBase::getColumnAutoSizeWidth(int columnId)
{
    int widest = 32;
    
    // Find the widest bit of text in this column..
    for (int r = getNumRows(); --r >= 0;)
    {
        if (SampleItem* sampleItem = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(r))
        {
            String text = sampleItem->getFilePath();
            widest = jmax(widest, style->FONT_SMALL_BOLD.getStringWidth(text));
        }
    }
    
    return widest + 8;
}

void BlomeTableViewBase::loadSelectedRowIntoAudioPlayer(int rowNumber) {
    File inFile = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(rowNumber)->getFilePath();
    
    if (!linkedSampleItemPanel.tryShowAudioResource(inFile))
    {
        showFileDeletedWarning();
    }
}

void BlomeTableViewBase::cellDoubleClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent)
{
    // Load linked sample item into audio resource
    loadSelectedRowIntoAudioPlayer(rowNumber);
}

/**
 Determines the components behaviour when the mouse is being dragged on it.
 */
void BlomeTableViewBase::mouseDrag(MouseEvent const & e)
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
 Sets a flag if the tree view is interested in drag and drop of files.
 */
bool BlomeTableViewBase::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}

void BlomeTableViewBase::returnKeyPressed(int lastRowSelected)
{
    loadSelectedRowIntoAudioPlayer(lastRowSelected);
}

// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void BlomeTableViewBase::sortOrderChanged(int newSortColumnId, bool isForwards)
{
    currentProcessor.setSortingDirection(isForwards);
    
    // Sort items according to direction and property name
    if (newSortColumnId != 0)
    {
        String columnName = getHeader().getColumnName(newSortColumnId);
        currentProcessor.setSortingColumnTitle(columnName);
        mComparator->setCompareProperty(columnName);
        mComparator->setSortingDirection(isForwards);
        sampleLibrary.getSampleItems(mSampleItemCollectionType).sort(*mComparator);
        updateContent();
    }
}

void BlomeTableViewBase::reanalyseSamples()
{
    // Reanalyse all selected files
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        sampleLibrary.reanalyseSampleItem(sampleLibrary
                                          .getSampleItems(mSampleItemCollectionType)
                                          .getUnchecked(getSelectedRow(r))->getFilePath());
    }
    
    sampleLibrary.refresh();
}
