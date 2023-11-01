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

class BlomeTableViewBase
:   public TableListBoxModel,
    public TableListBox,
    public FileDragAndDropTarget
{
public:
    // Constructors
    BlomeTableViewBase(SampleLibrary& inSampleLibrary,
                   SampleItemPanel& inSampleItemPanel);
    ~BlomeTableViewBase();
    
    // Methods
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
    String getCellText(SampleItem* inSampleItem, String columnName);
    int getColumnAutoSizeWidth(int columnId) override;
    void cellDoubleClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void mouseDrag(MouseEvent const & e) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeTableViewBase)
    
protected:
    // Fields
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    int numRows;
    SampleItemComparator mComparator;
    SampleItemCollectionType mSampleItemCollectionType;
    
    // Methods
    
};
