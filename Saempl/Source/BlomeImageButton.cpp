/*
  ==============================================================================

    BlomeImageButton.cpp
    Created: 5 Nov 2023 3:39:31pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeImageButton.h"

BlomeImageButton::BlomeImageButton(const String& name, bool inHasBackground)
:   ImageButton(name),
    hasBackground(inHasBackground)
{
    
}

BlomeImageButton::~BlomeImageButton()
{
    
}

void BlomeImageButton::paintButton(Graphics& g, bool isOver, bool isDown)
{
    if (hasBackground)
    {
        // Draw button background
        g.setColour(COLOUR_BLACK_LIGHT_TRANSPARENT);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    }
    
    ImageButton::paintButton(g, isOver, isDown);
}
