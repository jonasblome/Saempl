/*
  ==============================================================================

    SampleItemLibraryListMenuPanel.cpp
    Created: 18 Jun 2023 6:29:42pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibrariesMenuPanel.h"

SampleLibrariesMenuPanel::SampleLibrariesMenuPanel(SaemplAudioProcessor& inProcessor, SampleLibrariesPanel& inSampleItemLibraryListPanel)
:   PanelBase(inProcessor),
    linkedSampleItemLibraryListPanel(inSampleItemLibraryListPanel)
{
    setSize(SAMPLE_ITEM_LIBRARY_LIST_MENU_PANEL_WIDTH - PANEL_MARGIN, SAMPLE_ITEM_LIBRARY_LIST_MENU_PANEL_HEIGHT);
    setPanelComponents();
}

SampleLibrariesMenuPanel::~SampleLibrariesMenuPanel()
{
    
}

void SampleLibrariesMenuPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourLight);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), PANEL_CORNER_SIZE);
}

void SampleLibrariesMenuPanel::setPanelComponents()
{
    // Add change directory button component
    mRefreshSampleDatabaseButton = std::make_unique<TextButton>("Refresh Sample Library");
    mRefreshSampleDatabaseButton->setBounds(PANEL_MARGIN / 2.0,
                                            PANEL_MARGIN / 2.0,
                                            getWidth() / 2.0 - PANEL_MARGIN,
                                            getHeight() - PANEL_MARGIN);
    mRefreshSampleDatabaseButton->onClick = [this] {
        linkedSampleItemLibraryListPanel.getViewModel().refreshSampleDatabase();
    };
    addAndMakeVisible(*mRefreshSampleDatabaseButton);
    
    // Repaint panel
    repaint();
}
