/*
 ==============================================================================
 
 SampleTablePanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleTablePanel.h"

SampleTablePanel::SampleTablePanel(SaemplAudioProcessor& inProcessor,
                                   SampleItemPanel& inSampleItemPanel,
                                   AudioPlayer& inAudioPlayer)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
sampleItemPanel(inSampleItemPanel),
audioPlayer(inAudioPlayer)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    setPanelComponents();
}

SampleTablePanel::~SampleTablePanel()
{
    
}

void SampleTablePanel::paint(Graphics& g)
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
    g.drawFittedText("Table View - "
                     + sampleLibrary.getCurrentLibraryPath()
                     + " - " + std::to_string(sampleLibrary.getSampleItems(FILTERED_SAMPLES).size()) + " Samples",
                     style->PANEL_MARGIN * 1.5,
                     0,
                     getWidth() - style->PANEL_MARGIN * 3,
                     style->PANEL_TITLE_HEIGHT,
                     Justification::centred,
                     1);
    
    mSampleTable->updateContent();
}

void SampleTablePanel::setPanelComponents()
{
    // Set sample table component
    mSampleTable = std::make_unique<BlomeTableViewNavigation>(currentProcessor, sampleItemPanel, audioPlayer);
    addAndMakeVisible(*mSampleTable);
    
    resizePanelComponents();
}

void SampleTablePanel::resizePanelComponents()
{
    if (mSampleTable != nullptr)
    {
        mSampleTable->setBounds(style->PANEL_MARGIN,
                                style->PANEL_TITLE_HEIGHT + style->PANEL_MARGIN * 0.25,
                                getWidth() - style->PANEL_MARGIN * 1.75,
                                getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN);
    }
}

void SampleTablePanel::selectRandomSample()
{
    int numSamples = mSampleTable->getNumRows();
    
    if (numSamples == 0)
    {
        return;
    }
    
    int randomSampleIndex = Random::getSystemRandom().nextInt(numSamples);
    mSampleTable->selectRow(randomSampleIndex);
}

void SampleTablePanel::visibilityChanged()
{
    if (isVisible())
    {
        mSampleTable->resortTable();
    }
}

bool SampleTablePanel::keyPressed(KeyPress const & key)
{
    if (!isShowing())
    {
        return false;
    }
    
    if (key.getKeyCode() == 75) // K
    {
        mSampleTable->playSelectedSample();
        return true;
    }
    
    return mSampleTable->keyPressed(key);
}
