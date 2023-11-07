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
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
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
    mCenterPanel->setTopLeftPosition(PANEL_MARGIN / 2.0, HEADER_PANEL_HEIGHT + PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mCenterPanel);
    
    // Add header panel
    mHeaderPanel = std::make_unique<HeaderPanel>(currentProcessor, *mCenterPanel);
    mHeaderPanel->setTopLeftPosition(PANEL_MARGIN / 2.0, PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mHeaderPanel);
    
    // Repaint panel
    repaint();
}

bool MainPanel::keyPressed(const KeyPress &key)
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
