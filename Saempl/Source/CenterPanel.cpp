/*
  ==============================================================================

    CenterPanel.cpp
    Created: 27 May 2023 9:07:59pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "CenterPanel.h"

CenterPanel::CenterPanel(SaemplAudioProcessor& inProcessor)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor)
{
    setSize(CENTER_PANEL_WIDTH, CENTER_PANEL_HEIGHT);
    setPanelComponents();
}

CenterPanel::~CenterPanel()
{
    
}

void CenterPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
}

void CenterPanel::setPanelComponents()
{
    // Add panel for sample item view
    mSampleItemPanel = std::make_unique<SampleItemPanel>(currentProcessor);
    mSampleItemPanel->setTopLeftPosition(0, TABLE_PANEL_HEIGHT - Blome_PanelMargin / 2.0);
    addAndMakeVisible(*mSampleItemPanel);
    
    // Add panel for database views
    mSampleDatabaseTablePanel = std::make_unique<SampleDatabaseTablePanel>(currentProcessor, *mSampleItemPanel);
    mSampleDatabaseTablePanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mSampleDatabaseTablePanel);
}