/*
  ==============================================================================

    CenterPanel.h
    Created: 27 May 2023 9:07:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "SampleDatabaseTablePanel.h"
#include "SampleItemPanel.h"

class CenterPanel
:   public PanelBase
{
public:
    // Constructors
    CenterPanel(TimeSliceThread& inThread);
    ~CenterPanel();
    
    // Methods
    void paint(Graphics& g) override;
    
private:
    // Fields
    std::unique_ptr<SampleDatabaseTablePanel> mSampleDatabaseTablePanel;
    std::unique_ptr<SampleItemPanel> mSampleItemPanel;
    
    // Methods
    
};
