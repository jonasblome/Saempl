/*
 ==============================================================================
 
 MainPanel.cpp
 Created: 22 May 2023 6:31:37am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "MainPanel.h"

MainPanel::MainPanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor)
{
    // Set panel size
    setSize(style->MAIN_PANEL_WIDTH, style->MAIN_PANEL_HEIGHT);
    setPanelComponents();
}

MainPanel::~MainPanel()
{
    
}

void MainPanel::paint(Graphics &g)
{
    
}

void MainPanel::setPanelComponents()
{
    // Add center panel
    mCenterPanel = std::make_unique<CenterPanel>(currentProcessor);
    mCenterPanel->setTopLeftPosition(style->PANEL_MARGIN / 2.0, style->HEADER_PANEL_HEIGHT + style->PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mCenterPanel);
    
    // Add header panel
    mHeaderPanel = std::make_unique<HeaderPanel>(currentProcessor, *mCenterPanel);
    mHeaderPanel->setTopLeftPosition(style->PANEL_MARGIN / 2.0, style->PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mHeaderPanel);
    
    // Repaint panel
    repaint();
}

bool MainPanel::keyPressed(KeyPress const & key)
{
    if (key.isKeyCode(KeyPress::spaceKey))
    {
        mCenterPanel->playCurrentAudio();
    }
    else
    {
        return false;
    }
    
    return true;
}
