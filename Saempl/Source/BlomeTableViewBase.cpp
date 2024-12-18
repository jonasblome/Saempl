/*
 ==============================================================================
 
 BlomeTableViewBase.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeTableViewBase.h"

BlomeTableViewBase::BlomeTableViewBase(SaemplAudioProcessor& inProcessor,
                                       SampleItemPanel& inSampleItemPanel,
                                       AudioPlayer& inAudioPlayer)
:
currentProcessor(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
sampleItemPanel(inSampleItemPanel),
audioPlayer(inAudioPlayer)
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

int BlomeTableViewBase::getNumRows()
{
    numRows = sampleLibrary.getSampleItems(mSampleItemCollectionType).size();
    return numRows;
}

void BlomeTableViewBase::paintRowBackground(Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.setColour(style->COLOUR_ACCENT_LIGHT);
        g.fillRoundedRectangle(Rectangle<float>(width, height), style->CORNER_SIZE_MEDIUM);
    }
}

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
    if (columnName == PROPERTY_NAMES[0])
    {
        return inSampleItem->getTitle();
    }
    else if (columnName == PROPERTY_NAMES[1])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getLength(), 2) + "s";
    }
    else if (columnName == PROPERTY_NAMES[2])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getLoudnessDecibel(), 2) + "dB";
    }
    else if (columnName == PROPERTY_NAMES[3])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getLoudnessLUFS(), 2) + "LUFS";
    }
    else if (columnName == PROPERTY_NAMES[4])
    {
        return inSampleItem->getTempo() == 0 ? "No tempo detected" : std::to_string(inSampleItem->getTempo()) + "bpm";
    }
    else if (columnName == PROPERTY_NAMES[5])
    {
        return KEY_INDEX_TO_KEY_NAME[inSampleItem->getKey()];
    }
    else if (columnName == PROPERTY_NAMES[6])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getDynamicRange(), 2) + "LUFS";
    }
    else if (columnName == PROPERTY_NAMES[7])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getSpectralCentroid(), 2) + "Hz";
    }
    else if (columnName == PROPERTY_NAMES[8])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getSpectralRolloff(), 2) + "%";
    }
    else if (columnName == PROPERTY_NAMES[9])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getSpectralSpread(), 2) + "%";
    }
    else if (columnName == PROPERTY_NAMES[10])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getSpectralFlux(), 2) + "%";
    }
    else if (columnName == PROPERTY_NAMES[11])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getChromaFlux(), 2) + "%";
    }
    else if (columnName == PROPERTY_NAMES[12])
    {
        return String::toDecimalStringWithSignificantFigures(inSampleItem->getZeroCrossingRate(), 2) + "Hz";
    }
    else
    {
        return "unknown";
    }
}

int BlomeTableViewBase::getColumnAutoSizeWidth(int columnId)
{
    int widest = 32;
    
    // Find the widest bit of text in this column..
    for (int r = getNumRows(); --r >= 0;)
    {
        if (SampleItem* sampleItem = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(r))
        {
            String text = sampleItem->getCurrentFilePath();
            widest = jmax(widest, (int) TextLayout().getStringWidth(style->FONT_SMALL_BOLD, text));
        }
    }
    
    return widest + 8;
}

void BlomeTableViewBase::loadSelectedRowIntoAudioPlayer(int rowNumber)
{
    File inFile = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(rowNumber)->getCurrentFilePath();
    sampleItemPanel.tryShowAudioResource(inFile);
}

void BlomeTableViewBase::cellDoubleClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent)
{
    // Load linked sample item into audio resource
    loadSelectedRowIntoAudioPlayer(rowNumber);
}

void BlomeTableViewBase::mouseDrag(MouseEvent const & mouseEvent)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (mouseEvent.getLengthOfMousePress() > 100)
    {
        Point<int> mousePosition = mouseEvent.getEventRelativeTo(this).position.toInt();
        
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
                    selectedFilePaths.add(sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(getSelectedRow(r))->getCurrentFilePath());
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}

bool BlomeTableViewBase::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}

void BlomeTableViewBase::returnKeyPressed(int lastRowSelected)
{
    loadSelectedRowIntoAudioPlayer(lastRowSelected);
}

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
    StringArray filePaths;
    
    for (int r = getNumSelectedRows() - 1; r >= 0; r--)
    {
        filePaths.add(sampleLibrary
                      .getSampleItems(mSampleItemCollectionType)
                      .getUnchecked(getSelectedRow(r))->getCurrentFilePath());
    }
    
    sampleLibrary.reanalyseSampleItems(filePaths);
}

void BlomeTableViewBase::playSelectedSample()
{
    int selectedRowIndex = getLastRowSelected();
    
    if (selectedRowIndex == -1)
    {
        return;
    }
    
    File sampleFile = sampleLibrary.getSampleItems(mSampleItemCollectionType).getUnchecked(selectedRowIndex)->getCurrentFilePath();
    
    // Load file into source
    if (sampleFile.exists() && !sampleFile.isDirectory() && isSupportedAudioFileFormat(sampleFile.getFileExtension()))
    {
        audioPlayer.loadURLIntoTransport(URL(sampleFile));
        audioPlayer.setTransportSourcePosition(0.0);
        audioPlayer.start();
    }
    
    if (!sampleFile.exists())
    {
        audioPlayer.emptyTransport();
        sampleLibrary.refreshLibrary();
        AlertWindow::showAsync(MessageBoxOptions()
                               .withIconType(MessageBoxIconType::NoIcon)
                               .withTitle("File not available!")
                               .withMessage("This file has probably been externally deleted and was removed from the list of available samples.")
                               .withButton("OK"),
                               nullptr);
    }
}

void BlomeTableViewBase::showSampleInFinder()
{
    File(sampleLibrary
         .getSampleItems(mSampleItemCollectionType)
         .getUnchecked(getLastRowSelected())->getCurrentFilePath())
    .revealToUser();
}
