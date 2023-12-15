/*
 ==============================================================================
 
 MainPanel.cpp
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
    // Header panel drop shadow
    drawDropShadow(g,
                   Rectangle<int>(0, 
                                  - style->HEADER_PANEL_HEIGHT * 0.5,
                                  getWidth(),
                                  2),
                   0,
                   0,
                   80,
                   style);
    
    // Palette panel drop shadow
    drawDropShadow(g,
                   Rectangle<int>(0, 
                                  style->HEADER_PANEL_HEIGHT,
                                  style->SAMPLE_PALETTE_PANEL_WIDTH,
                                  getHeight()),
                   style->SAMPLE_NAVIGATION_PANEL_WIDTH,
                   0,
                   80,
                   style);
    
    // Navigation panel drop shadow
    drawDropShadow(g,
                   Rectangle<int>(0,
                                  style->HEADER_PANEL_HEIGHT,
                                  style->SAMPLE_NAVIGATION_PANEL_WIDTH,
                                  style->SAMPLE_NAVIGATION_PANEL_HEIGHT
                                  + (currentProcessor.getSampleItemPanelIsVisible()
                                  ? 0
                                  : style->SAMPLE_ITEM_PANEL_HEIGHT)),
                   0,
                   0,
                   80,
                   style);
    
    // Sample item panel drop shadow
    if (currentProcessor.getSampleItemPanelIsVisible())
    {
        drawDropShadow(g,
                       Rectangle<int>(0,
                                      style->HEADER_PANEL_HEIGHT
                                      + style->SAMPLE_NAVIGATION_PANEL_HEIGHT,
                                      style->SAMPLE_ITEM_PANEL_WIDTH,
                                      style->SAMPLE_ITEM_PANEL_HEIGHT),
                       0,
                       0,
                       80,
                       style);
    }
    
    // Sample item panel toggle drop shadow
    drawDropShadow(g,
                   Rectangle<int>(0,
                                  style->HEADER_PANEL_HEIGHT
                                  + style->CENTER_PANEL_HEIGHT - style->BUTTON_SIZE_SMALL,
                                  style->SAMPLE_ITEM_PANEL_WIDTH,
                                  style->SAMPLE_ITEM_PANEL_HEIGHT),
                   0,
                   0,
                   80,
                   style);
}

void MainPanel::setPanelComponents()
{
    // Add center panel
    mCenterPanel = std::make_unique<CenterPanel>(currentProcessor);
    mCenterPanel->setTopLeftPosition(0, style->HEADER_PANEL_HEIGHT);
    addAndMakeVisible(*mCenterPanel);
    
    // Add header panel
    mHeaderPanel = std::make_unique<HeaderPanel>(currentProcessor, *mCenterPanel);
    mHeaderPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(*mHeaderPanel);
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
