/*
  ==============================================================================

    BlomeHelperFunctions.h
    Created: 22 May 2023 10:47:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "BlomeInterfaceDefines.h"

inline void paintComponentLabel(Graphics& g, Component* inComponent)
{
    const int x = inComponent->getX() - inComponent->getWidth() * 0.25;
    const int y = inComponent->getY() + inComponent->getHeight();
    const int w = inComponent->getWidth() * 1.5f;
    const int h = COMPONENT_LABEL_HEIGHT;
    
    const float cornerSize = 3.0f;
    
    const String label = inComponent->getName();
    
    g.setColour(BlomeColour_BlackMediumTransparent);
    g.fillRoundedRectangle(x, y, w, h, cornerSize);
    
    g.setColour(BlomeColour_LightGray);
    g.setFont(font_small_bold);
    
    g.drawFittedText(label, x, y, w, h, Justification::centred, 1);
}
