/*
 ==============================================================================
 
 SampleGridPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleGridPanel.h"

SampleGridPanel::SampleGridPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
sampleItemPanel(inSampleItemPanel)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    setPanelComponents();
}

SampleGridPanel::~SampleGridPanel()
{
    
}

void SampleGridPanel::selectRandomSample()
{
    mGridViewport->setViewPosition(mSampleTileGrid->selectRandomTile());
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
    mSampleTileGrid = std::make_unique<BlomeSampleTileGridView>(sampleLibrary, sampleItemPanel);
    
    // Add zoom slider
    int sliderWidth = 150;
    mZoomSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mZoomSlider->setBounds(style->PANEL_MARGIN,
                           style->PANEL_MARGIN * 0.25 + style->PANEL_TITLE_HEIGHT,
                           sliderWidth,
                           style->PANEL_TITLE_HEIGHT * 0.5);
    mZoomSlider->setRange(0, 1, 0);
    mZoomSlider->setValue(0.43);
    mZoomSlider->onValueChange = [this] { mSampleTileGrid->performGridLayout(mZoomSlider->getValue()); };
    addAndMakeVisible(*mZoomSlider);
    
    // Add grid viewport
    mGridViewport = std::make_unique<Viewport>();
    mGridViewport->setViewedComponent(&*mSampleTileGrid, false);
    mGridViewport->setBounds(style->PANEL_MARGIN,
                             style->PANEL_TITLE_HEIGHT * 1.5 + style->PANEL_MARGIN * 0.75,
                             getWidth() - style->PANEL_MARGIN * 1.75,
                             getHeight() - style->PANEL_TITLE_HEIGHT * 1.5 - style->PANEL_MARGIN * 1.5);
    addAndMakeVisible(*mGridViewport);
}

void SampleGridPanel::resizePanelComponents()
{
    if (mGridViewport != nullptr)
    {
        mGridViewport->setBounds(style->PANEL_MARGIN,
                                 style->PANEL_TITLE_HEIGHT * 1.5 + style->PANEL_MARGIN * 0.75,
                                 getWidth() - style->PANEL_MARGIN * 1.75,
                                 getHeight() - style->PANEL_TITLE_HEIGHT * 1.5 - style->PANEL_MARGIN * 1.5);
    }
}

void SampleGridPanel::visibilityChanged()
{
    if (isVisible())
    {
        mSampleTileGrid->sortGrid();
    }
}
