/*
  ==============================================================================

    CenterPanel.cpp
    Created: 27 May 2023 9:07:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "CenterPanel.h"

CenterPanel::CenterPanel(TimeSliceThread& inThread)
:   PanelBase()
{
    setSize(CENTER_PANEL_WIDTH, CENTER_PANEL_HEIGHT);
    
    // Add panel for sample item view
    mSampleItemPanel = std::make_unique<SampleItemPanel>(inThread);
    mSampleItemPanel->setTopLeftPosition(0, TABLE_PANEL_HEIGHT);
    addAndMakeVisible(*mSampleItemPanel);
    
    // Add panel for database views
    mSampleDatabaseTablePanel = std::make_unique<SampleDatabaseTablePanel>(inThread, *mSampleItemPanel);
    mSampleDatabaseTablePanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleDatabaseTablePanel);
}

CenterPanel::~CenterPanel()
{
    
}

void CenterPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw panel background
    g.fillAll(BlomeColour_DarkRed);
}
