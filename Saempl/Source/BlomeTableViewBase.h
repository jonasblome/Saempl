/*
 ==============================================================================
 
 BlomeTableViewBase.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"
#include "SampleItemComparator.h"
#include "SampleItem.h"
#include "SampleItemPanel.h"

/**
 The base class for displaying SampleItem collections in a table.
 
 Handles drag and drop of files onto and from it.
 Retrieves its cell text information from SampleItem properties.
 */
class BlomeTableViewBase
:
public TableListBoxModel,
public TableListBox,
public FileDragAndDropTarget
{
public:
    /**
     Constructor for the table view base on the SampleLibrary.
     
     @param inProcessor the processor object of the current plugin instance.
     @param inSampleItemPanel the panel for previewing SampleItem objects.
     */
    BlomeTableViewBase(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewBase();
    int getNumRows() override;
    
protected:
    SaemplAudioProcessor& currentProcessor;
    SampleLibrary& sampleLibrary;
    SampleItemPanel& sampleItemPanel;
    int numRows;
    std::unique_ptr<SampleItemComparator> mComparator;
    SampleItemCollectionScope mSampleItemCollectionType;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void paintRowBackground(Graphics& g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;
    void paintCell(Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;
    /**
     Gets the text for the property of the SampleItem and the given column name.
     
     @param inSampleItem the SampleItem from which to get the property value.
     @param columnName the name of the property to retrieve.
     
     @returns the string that represents the property value of the given column.
     */
    String getCellText(SampleItem* inSampleItem, String columnName);
    int getColumnAutoSizeWidth(int columnId) override;
    /**
     Loads the file of the selected row into the audio player.
     */
    void loadSelectedRowIntoAudioPlayer(int rowNumber);
    void cellDoubleClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void mouseDrag(MouseEvent const & mouseEvent) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    void returnKeyPressed(int lastRowSelected) override;
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    /**
     Reanalyses all selected samples.
     */
    void reanalyseSamples();
};
