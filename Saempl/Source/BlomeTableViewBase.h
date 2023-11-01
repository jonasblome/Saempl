/*
  ==============================================================================

    BlomeTableView.h
    Created: 26 Jun 2023 12:54:09am
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
 The base class for displaying \ref SampleItem collections in a table.
 
 Implements
 \ref juce::TableListBoxModel
 \ref juce::TableListBox
 \ref juce::FileDragAndDropTarget
 Handles drag and drop of files onto and from it.
 Retrieves its cell text information from \ref SampleItem properties.
 */
class BlomeTableViewBase
:   public TableListBoxModel,
    public TableListBox,
    public FileDragAndDropTarget
{
public:
    /**
     Constructor for the table view base on the \ref SampleLibrary.
     
     @param inSampleLibrary the sample library of the current plugin instance.
     @param inSampleItemPanel the panel for previewing \ref SampleItem objects.
     */
    BlomeTableViewBase(SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewBase();
    int getNumRows() override;
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
     Gets the text for the property of the \ref SampleItem and the given column name.
     
     @param inSampleItem the \ref SampleItem from which to get the property value.
     @param columnName the name of the property to retrieve.
     
     @returns the string that represents the property value of the given column.
     */
    String getCellText(SampleItem* inSampleItem, String columnName);
    int getColumnAutoSizeWidth(int columnId) override;
    void cellDoubleClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void mouseDrag(MouseEvent const & e) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeTableViewBase)
    
protected:
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    int numRows;
    SampleItemComparator mComparator;
    SampleItemCollectionScope mSampleItemCollectionType;
};
