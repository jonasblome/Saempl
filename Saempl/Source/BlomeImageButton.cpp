/*
 ==============================================================================
 
 BlomeImageButton.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeImageButton.h"

BlomeImageButton::BlomeImageButton(const String& name, bool inHasBackground)
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
        // drawDropShadow(g, Rectangle<int>(0, 0, 1, 1), getWidth() * 0.5, getHeight() * 0.5, getWidth() * 0.5, style);
    }
    
    ImageButton::paintButton(g, isOver, isDown);
}
