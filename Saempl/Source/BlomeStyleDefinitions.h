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
const int MAIN_PANEL_WIDTH = 1350;
const int MAIN_PANEL_HEIGHT = 900;

const int HEADER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
const int HEADER_PANEL_HEIGHT = 50;

const int CENTER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
const int CENTER_PANEL_HEIGHT = MAIN_PANEL_HEIGHT - HEADER_PANEL_HEIGHT;

const int SAMPLE_LIBRARIES_PANEL_WIDTH = CENTER_PANEL_WIDTH;
const int SAMPLE_LIBRARIES_PANEL_HEIGHT = 500;

const int SAMPLE_ITEM_PANEL_TOGGLE_HEIGHT = 20;

const int SAMPLE_ITEM_PANEL_WIDTH = CENTER_PANEL_WIDTH;
const int SAMPLE_ITEM_PANEL_HEIGHT = CENTER_PANEL_HEIGHT - SAMPLE_LIBRARIES_PANEL_HEIGHT - SAMPLE_ITEM_PANEL_TOGGLE_HEIGHT;

const int SAMPLE_CONTROL_WIDTH = 25;
const int SAMPLE_CONTROL_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;

const int SAMPLE_PREVIEW_WIDTH = SAMPLE_ITEM_PANEL_WIDTH - SAMPLE_CONTROL_WIDTH;
const int SAMPLE_PREVIEW_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;

// Colour definitions

// Greenish white
const Colour BlomeColour_BlueishWhite = Colour(235, 255, 235);

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
const Colour BlomeColour_BlackLightTransparent = Colour(BlomeColour_Black).withAlpha(0.35f);
const Colour BlomeColour_BlackMediumTransparent = Colour(BlomeColour_Black).withAlpha(0.78f);
const Colour BlomeColour_BlackStrongTransparent = Colour(BlomeColour_Black).withAlpha(0.95f);

// Blueish black
const Colour BlomeColour_BlueishBlack = Colour(0, 0, 20);

// Accent colour palette
const Colour BlomeColour_AccentColourLight = Colour(216, 219, 167);
const Colour BlomeColour_AccentColourLightStrongTransparent = Colour(BlomeColour_AccentColourLight).withAlpha(0.99f);
const Colour BlomeColour_AccentColourMedium = Colour(19, 69, 43);
const Colour BlomeColour_AccentColourMediumStrongTransparent = Colour(BlomeColour_AccentColourMedium).withAlpha(0.99f);
const Colour BlomeColour_AccentColourDark = Colour(11, 41, 25);
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
const int Blome_NormalButtonHeight = 25;
