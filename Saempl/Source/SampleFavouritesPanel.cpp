/*
 ==============================================================================
 
 SampleFavouritesPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleFavouritesPanel.h"

SampleFavouritesPanel::SampleFavouritesPanel(SaemplAudioProcessor& inProcessor, 
                                             SampleItemPanel& inSampleItemPanel,
                                             AudioPlayer& inAudioPlayer)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
sampleItemPanel(inSampleItemPanel),
audioPlayer(inAudioPlayer)
{
    setSize(style->SAMPLE_FAVOURITES_PANEL_WIDTH, style->SAMPLE_FAVOURITES_PANEL_HEIGHT);
    setPanelComponents();
}

SampleFavouritesPanel::~SampleFavouritesPanel()
{
    
}

void SampleFavouritesPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds()
                           .toFloat()
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.25)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.25)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.5)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.5),
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
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.75)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.75)
                           .withTrimmedRight(style->PANEL_MARGIN)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.25)
                           .toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_SMALL_BOLD);
    g.setColour(style->COLOUR_PANEL_TITLE_FONT);
    g.drawFittedText("Favourites",
                     0,
                     0,
                     getWidth(),
                     style->PANEL_TITLE_HEIGHT,
                     Justification::centred,
                     1);
    
    mSampleTable->updateContent();
}

void SampleFavouritesPanel::setPanelComponents()
{
    // Set sample table component
    mSampleTable = std::make_unique<BlomeTableViewFavourites>(currentProcessor, sampleItemPanel, audioPlayer);
    addAndMakeVisible(*mSampleTable);
    
    resizePanelComponents();
}

void SampleFavouritesPanel::resizePanelComponents()
{
    if (mSampleTable != nullptr)
    {
        mSampleTable->setBounds(style->PANEL_MARGIN * 0.75,
                                style->PANEL_TITLE_HEIGHT + style->PANEL_MARGIN * 0.25,
                                getWidth() - style->PANEL_MARGIN * 1.75,
                                getHeight() - style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN * 1.25);
    }
}

bool SampleFavouritesPanel::keyPressed(KeyPress const & key)
{
    if (key.getKeyCode() == 75) // K
    {
        mSampleTable->playSelectedSample();
        return true;
    }
    
    return mSampleTable->keyPressed(key);
}
