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

class BlomeTableView
:   public Component,
    public TableListBoxModel
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
    int getColumnAutoSizeWidth(int columnId) override;
    
private:
    // Fields
    SampleLibraryViewModel& libraryViewModel;
    std::unique_ptr<TableListBox> mTable;
    int numRows;
    
    // Methods
    
};
