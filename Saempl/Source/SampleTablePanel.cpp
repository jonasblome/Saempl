/*
 ==============================================================================
 
 SampleTablePanel.cpp
 Created: 25 Jun 2023 5:51:12pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleTablePanel.h"

SampleTablePanel::SampleTablePanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
linkedSampleItemPanel(inSampleItemPanel)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH - style->PANEL_MARGIN / 2.0,
            style->SAMPLE_NAVIGATION_PANEL_HEIGHT - style->PANEL_MARGIN / 2.0);
    setPanelComponents();
}

SampleTablePanel::~SampleTablePanel()
{
    
}

void SampleTablePanel::paint(Graphics& g)
{
    // Set background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    
    // Draw Title
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getLocalBounds().
                           removeFromTop(style->PANEL_TITLE_HEIGHT).
                           reduced(style->PANEL_MARGIN / 2.0).
                           toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.drawFittedText("All Samples - " + sampleLibrary.getCurrentLibraryPath(),
                     style->PANEL_MARGIN / 2.0,
                     style->PANEL_MARGIN / 2.0,
                     getWidth() - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN,
                     Justification::centred,
                     1);
    
    mSampleTable->updateContent();
}

void SampleTablePanel::setPanelComponents()
{
    // Set sample table component
    mSampleTable = std::make_unique<BlomeTableViewNavigation>(currentProcessor, linkedSampleItemPanel);
    mSampleTable->setBounds(style->PANEL_MARGIN / 2.0,
                            style->PANEL_TITLE_HEIGHT,
                            getWidth() - style->PANEL_MARGIN,
                            getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN / 2.0);
    addAndMakeVisible(*mSampleTable);
}

void SampleTablePanel::resizePanelComponents()
{
    if (mSampleTable != nullptr)
    {
        mSampleTable->setBounds(style->PANEL_MARGIN / 2.0,
                                style->PANEL_TITLE_HEIGHT,
                                getWidth() - style->PANEL_MARGIN,
                                getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN / 2.0);
    }
}

void SampleTablePanel::selectRandomSample()
{
    int numSamples = mSampleTable->getNumRows();
    int randomSampleIndex = Random::getSystemRandom().nextInt(numSamples);
    mSampleTable->selectRow(randomSampleIndex);
}
