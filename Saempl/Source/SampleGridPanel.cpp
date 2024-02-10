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
    setWantsKeyboardFocus(true);
}

SampleGridPanel::~SampleGridPanel()
{
    
}

void SampleGridPanel::centerPositionInGridViewport(Point<int>& newPosition)
{
    newPosition.addXY(-mGridViewport->getWidth() / 2, -mGridViewport->getHeight() / 2);
    mGridViewport->setViewPosition(newPosition);
}

void SampleGridPanel::selectRandomSample()
{
    Point<int > newPosition = mSampleTileGrid->selectRandomTile();
    centerPositionInGridViewport(newPosition);
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
    mZoomSlider->setValue(0.0);
    mZoomSlider->onValueChange = [this] { mSampleTileGrid->performGridLayout(mZoomSlider->getValue()); };
    addAndMakeVisible(*mZoomSlider);
    
    // Add grid viewport
    mGridViewport = std::make_unique<Viewport>();
    mGridViewport->setViewedComponent(&*mSampleTileGrid, false);
    mGridViewport->setBounds(style->PANEL_MARGIN,
                             style->PANEL_TITLE_HEIGHT * 1.5 + style->PANEL_MARGIN * 0.75,
                             getWidth() - style->PANEL_MARGIN * 1.75,
                             getHeight() - style->PANEL_TITLE_HEIGHT * 1.5 - style->PANEL_MARGIN * 1.5);
    mGridViewport->setWantsKeyboardFocus(false);
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

bool SampleGridPanel::keyPressed(const KeyPress& key)
{
    int keyCode = key.getKeyCode();
    
    if (keyCode == KeyPress::returnKey)
    {
        mSampleTileGrid->loadSelectedTileIntoAudioPlayer();
        return true;
    }
    else if (keyCode == KeyPress::escapeKey)
    {
        mSampleTileGrid->deselectAll();
        return true;
    }
    else if (key.getModifiers().isCommandDown() && keyCode == 65)
    {
        mSampleTileGrid->selectAll();
        return true;
    }
    else if (keyCode == KeyPress::leftKey)
    {
        Point<int > newPosition = mSampleTileGrid->selectLeft();
        centerPositionInGridViewport(newPosition);
        return true;
    }
    else if (keyCode == KeyPress::upKey)
    {
        Point<int > newPosition = mSampleTileGrid->selectUp();
        centerPositionInGridViewport(newPosition);
        return true;
    }
    else if (keyCode == KeyPress::rightKey)
    {
        Point<int > newPosition = mSampleTileGrid->selectRight();
        centerPositionInGridViewport(newPosition);
        return true;
    }
    else if (keyCode == KeyPress::downKey)
    {
        Point<int > newPosition = mSampleTileGrid->selectDown();
        centerPositionInGridViewport(newPosition);
        return true;
    }
    
    return false;
}
