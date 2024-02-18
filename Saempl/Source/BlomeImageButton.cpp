/*
 ==============================================================================
 
 BlomeImageButton.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeImageButton.h"

BlomeImageButton::BlomeImageButton(String const & name, bool inHasBackground)
:
ImageButton(name),
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
        g.setColour(style->COLOUR_ACCENT_DARK);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    }
    
    ImageButton::paintButton(g, isOver, isDown);
}
