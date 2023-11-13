/*
 ==============================================================================
 
 SampleGridPanel.cpp
 Created: 13 Nov 2023 5:04:23pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleGridPanel.h"

SampleGridPanel::SampleGridPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    setPanelComponents();
}

SampleGridPanel::~SampleGridPanel()
{
    
}

void SampleGridPanel::paint(Graphics& g)
{
    // Set background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds()
                           .toFloat()
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.5)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.25)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.25)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.25),
                           style->CORNER_SIZE_MEDIUM);
    
    // Draw Title
    drawDropShadow(g,
                   Rectangle<int>(0, 0, getWidth(), 20),
                   0,
                   0,
                   70,
                   style);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getLocalBounds()
                           .removeFromTop(style->PANEL_TITLE_HEIGHT)
                           .withTrimmedLeft(style->PANEL_MARGIN)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.75)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.75)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.25)
                           .toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_SMALL_BOLD);
    g.setColour(style->COLOUR_PANEL_TITLE_FONT);
    g.drawFittedText("Grid View - " + sampleLibrary.getCurrentLibraryPath(),
                     0,
                     0,
                     getWidth(),
                     style->PANEL_TITLE_HEIGHT,
                     Justification::centred,
                     1);
}

void SampleGridPanel::setPanelComponents()
{
    for (SampleItem* sampleItem : sampleLibrary.getSampleItems(ALL_SAMPLES))
    {
        if (mSampleItemTiles.size() < 21)
        {
            addAndMakeVisible(mSampleItemTiles.add(new BlomeSampleItemTileView(*sampleItem, linkedSampleItemPanel)));
        }
    }
    
    // Setup tile grid
    mSampleTileGrid = std::make_unique<Grid>();
    mSampleTileGrid->setGap(Grid::Px(style->PANEL_MARGIN * 0.5f));
    using Track = Grid::TrackInfo;
    mSampleTileGrid->templateRows = { Track(1_fr), Track(1_fr), Track(1_fr) };
    mSampleTileGrid->templateColumns = { Track (1_fr), Track(1_fr), Track(1_fr) };
    mSampleTileGrid->autoRows = Track(1_fr);
    mSampleTileGrid->autoColumns = Track(1_fr);
    mSampleTileGrid->autoFlow = Grid::AutoFlow::column;
    mSampleTileGrid->items.addArray(mSampleItemTiles);
    mSampleTileGrid->performLayout(Rectangle<int>(style->PANEL_MARGIN,
                                                  style->PANEL_TITLE_HEIGHT + style->PANEL_MARGIN * 0.25,
                                                  getWidth() - style->PANEL_MARGIN * 1.75,
                                                  getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN));
}

void SampleGridPanel::resizePanelComponents()
{
    
}
