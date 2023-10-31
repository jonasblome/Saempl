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

class BlomeTableView
:   public TableListBoxModel,
    public TableListBox,
    public FileDragAndDropTarget
{
public:
    // Constructors
    BlomeTableView(SampleLibrary& inSampleLibrary,
                   SampleItemPanel& inSampleItemPanel,
                   SampleItemCollectionType inSampleItemCollectionType);
    ~BlomeTableView();
    
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
    String getCellText(SampleItem* inSampleItem, int columnId);
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    int getColumnAutoSizeWidth(int columnId) override;
    void cellClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void cellDoubleClicked(int rowNumber, int columnId, MouseEvent const & mouseEvent) override;
    void mouseDrag(MouseEvent const & e) override;
    void filesDropped(StringArray const & files, int x, int y) override;
    bool isInterestedInFileDrag(StringArray const & files) override;
    void deleteFile(bool deletePermanently);
    void removeSampleItemFromPalette();
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(BlomeTableView)
    
    // Fields
    SampleLibrary& sampleLibrary;
    SampleItemPanel& linkedSampleItemPanel;
    int numRows;
    SampleItemComparator mComparator;
    SampleItemCollectionType mSampleItemCollectionType;
    
    // Methods
    
};
