/*
  ==============================================================================

    BlomeSampleItemTileView.cpp
    Created: 13 Nov 2023 5:31:43pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeSampleItemTileView.h"

BlomeSampleItemTileView::BlomeSampleItemTileView(SampleItem& inSampleItem, SampleItemPanel& inSampleItemPanel)
:
linkedSampleItem(inSampleItem),
linkedSampleItemPanel(inSampleItemPanel)
{
    
}

BlomeSampleItemTileView::~BlomeSampleItemTileView()
{
    
}

void BlomeSampleItemTileView::paint(Graphics& g)
{
    // Draw outline
    Rectangle<float> outline = getLocalBounds().toFloat().reduced(0.5);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawRoundedRectangle(outline, style->CORNER_SIZE_MEDIUM, 1.0);
    
    // Draw sample title
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawFittedText(linkedSampleItem.getTitle(),
                     getLocalBounds().reduced(style->PANEL_MARGIN),
                     Justification::centred,
                     5);
}

void BlomeSampleItemTileView::mouseDoubleClick (const MouseEvent& event)
{
    File inFile = linkedSampleItem.getFilePath();
    
    if (!linkedSampleItemPanel.tryShowAudioResource(inFile))
    {
        showFileDeletedWarning();
    }
}
