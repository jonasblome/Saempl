/*
  ==============================================================================

    BlomeStyleDefinitions.h
    Created: 22 May 2023 6:54:17am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#define MAIN_PANEL_WIDTH                900
#define MAIN_PANEL_HEIGHT               650

// Light Gray
const Colour BlomeColour_LightGray = Colour(150, 150, 150);
const Colour BlomeColour_LightGrayLightTransparent = Colour(BlomeColour_LightGray).withAlpha(0.5f);

// Gray
const Colour BlomeColour_Gray = Colour(70, 70, 70);

// Dark Gray
const Colour BlomeColour_DarkGray = Colour(40, 40, 40);

// Black
const Colour BlomeColour_Black = Colour(0, 0, 0);
const Colour BlomeColour_BlackFullTransparent = Colour(BlomeColour_Black).withAlpha(0.0f);
const Colour BlomeColour_BlackLightTransparent = Colour(BlomeColour_Black).withAlpha(0.63f);
const Colour BlomeColour_BlackMediumTransparent = Colour(BlomeColour_Black).withAlpha(0.78f);
const Colour BlomeColour_BlackStrongTransparent = Colour(BlomeColour_Black).withAlpha(0.95f);

// Dark Yellow
const Colour BlomeColour_DarkYellow = Colour(138, 39, 0);
const Colour BlomeColour_DarkYellowStrongTransparent = Colour(BlomeColour_DarkYellow).withAlpha(0.99f);

// Fonts
const Font font_small_bold("Helvetica Neue", 12.00f, Font::bold);
const Font font_small_accentuated("Helvetica Neue", 12.30f, Font::bold);
const Font font_medium("Helvetica Neue", 22.00f, Font::bold);
const Font font_large("Helvetica Neue", 48.00f, Font::bold);
