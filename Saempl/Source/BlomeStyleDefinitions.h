/*
  ==============================================================================

    BlomeStyleDefinitions.h
    Created: 22 May 2023 6:54:17am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

// Panel dimensions
const int MAIN_PANEL_WIDTH = 1050;
const int MAIN_PANEL_HEIGHT = 800;

const int HEADER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
const int HEADER_PANEL_HEIGHT = 50;

const int CENTER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
const int CENTER_PANEL_HEIGHT = MAIN_PANEL_HEIGHT - HEADER_PANEL_HEIGHT;

const int TABLE_PANEL_WIDTH = CENTER_PANEL_WIDTH;
const int TABLE_PANEL_HEIGHT = 400;

const int SAMPLE_PANEL_WIDTH = CENTER_PANEL_WIDTH;
const int SAMPLE_PANEL_HEIGHT = CENTER_PANEL_HEIGHT - TABLE_PANEL_HEIGHT;

const int SAMPLE_PREVIEW_WIDTH = SAMPLE_PANEL_WIDTH;
const int SAMPLE_PREVIEW_HEIGHT = SAMPLE_PANEL_HEIGHT - 85;

// Light gray
const Colour BlomeColour_LightGray = Colour(155, 155, 175);
const Colour BlomeColour_LightGrayLightTransparent = Colour(BlomeColour_LightGray).withAlpha(0.5f);

// Gray
const Colour BlomeColour_Gray = Colour(70, 70, 70);

// Dark gray
const Colour BlomeColour_DarkGray = Colour(40, 40, 40);

// Black
const Colour BlomeColour_Black = Colour(0, 0, 0);
const Colour BlomeColour_Transparent = Colour(BlomeColour_Black).withAlpha(0.0f);
const Colour BlomeColour_BlackLightTransparent = Colour(BlomeColour_Black).withAlpha(0.25f);
const Colour BlomeColour_BlackMediumTransparent = Colour(BlomeColour_Black).withAlpha(0.78f);
const Colour BlomeColour_BlackStrongTransparent = Colour(BlomeColour_Black).withAlpha(0.95f);

// BlueBlack
const Colour BlomeColour_BlueishBlack = Colour(0, 0, 20);

// Accent colour palette
const Colour BlomeColour_AccentColourLight = Colour(184, 52, 0);
const Colour BlomeColour_AccentColourLightStrongTransparent = Colour(BlomeColour_AccentColourLight).withAlpha(0.99f);
const Colour BlomeColour_AccentColourMedium = Colour(138, 39, 0);
const Colour BlomeColour_AccentColourMediumStrongTransparent = Colour(BlomeColour_AccentColourMedium).withAlpha(0.99f);
const Colour BlomeColour_AccentColourDark = Colour(82, 23, 0);
const Colour BlomeColour_AccentColourDarkStrongTransparent = Colour(BlomeColour_AccentColourDark).withAlpha(0.99f);

// Fonts
const Font font_small_bold("Helvetica Neue", 12.00f, Font::bold);
const Font font_small_accentuated("Helvetica Neue", 12.30f, Font::bold);
const Font font_medium_small_bold("Helvetica Neue", 16.00f, Font::bold);
const Font font_medium("Helvetica Neue", 22.00f, Font::bold);
const Font font_large("Helvetica Neue", 48.00f, Font::bold);

// Other
const float Blome_PanelCornerSize = 6.0;
const int Blome_PanelMargin = 8;
