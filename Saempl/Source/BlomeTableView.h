/*
  ==============================================================================

    BlomeTableView.h
    Created: 26 Jun 2023 12:54:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleLibraryViewModel.h"
#include "BlomeStyleDefinitions.h"
#include "SampleItemComparator.h"
#include "SampleItem.h"

class BlomeTableView
:   public TableListBoxModel,
    public TableListBox
{
public:
    // Constructors
    BlomeTableView(SampleLibraryViewModel& inLibraryViewModel);
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
    void sortOrderChanged (int newSortColumnId, bool isForwards) override;
    int getColumnAutoSizeWidth(int columnId) override;
    void refresh();
    
private:
    // Fields
    SampleLibraryViewModel& libraryViewModel;
    int numRows;
    SampleItemComparator mComparator;
    
    // Methods
    
};
