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
    setPanelComponents();
    setWantsKeyboardFocus(true);
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
    
    // Favourites panel drop shadow
    drawDropShadow(g,
                   Rectangle<int>(0,
                                  style->HEADER_PANEL_HEIGHT,
                                  style->SAMPLE_FAVOURITES_PANEL_WIDTH,
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
                                  + style->CENTRE_PANEL_HEIGHT - style->BUTTON_SIZE_SMALL,
                                  style->SAMPLE_ITEM_PANEL_WIDTH,
                                  style->SAMPLE_ITEM_PANEL_HEIGHT),
                   0,
                   0,
                   80,
                   style);
}

void MainPanel::setPanelComponents()
{
    // Add centre panel
    mCentrePanel = std::make_unique<CentrePanel>(currentProcessor);
    addAndMakeVisible(*mCentrePanel);
    
    // Add header panel
    mHeaderPanel = std::make_unique<HeaderPanel>(currentProcessor, *mCentrePanel);
    addAndMakeVisible(*mHeaderPanel);
    
    resizePanelComponents();
}

bool MainPanel::keyPressed(KeyPress const & key)
{
    bool pressWasHandled = false;
    
    if (mCentrePanel->keyPressed(key) || mHeaderPanel->keyPressed(key))
    {
        pressWasHandled = true;
    }
    
    return pressWasHandled;
}

void MainPanel::resizePanelComponents()
{
    if (mCentrePanel != nullptr)
    {
        mCentrePanel->setBounds(0, style->HEADER_PANEL_HEIGHT, getWidth(), getHeight() - style->HEADER_PANEL_HEIGHT);
    }
    
    if (mHeaderPanel != nullptr)
    {
        mHeaderPanel->setBounds(0, 0, getWidth(), style->HEADER_PANEL_HEIGHT);
    }
}
