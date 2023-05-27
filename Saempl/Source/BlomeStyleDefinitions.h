/*
  ==============================================================================

    BlomeStyleDefinitions.h
    Created: 22 May 2023 6:54:17am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#define MAIN_PANEL_WIDTH                1050
#define MAIN_PANEL_HEIGHT               800
#define HEADER_PANEL_WIDTH              MAIN_PANEL_WIDTH
#define HEADER_PANEL_HEIGHT             50
#define CENTER_PANEL_WIDTH              MAIN_PANEL_WIDTH
#define CENTER_PANEL_HEIGHT             MAIN_PANEL_HEIGHT - HEADER_PANEL_HEIGHT
#define TABLE_PANEL_WIDTH               CENTER_PANEL_WIDTH
#define TABLE_PANEL_HEIGHT              550
#define SAMPLE_PANEL_WIDTH              CENTER_PANEL_WIDTH
#define SAMPLE_PANEL_HEIGHT             CENTER_PANEL_HEIGHT - TABLE_PANEL_HEIGHT
#define SAMPLE_PREVIEW_WIDTH            SAMPLE_PANEL_WIDTH
#define SAMPLE_PREVIEW_HEIGHT           SAMPLE_PANEL_HEIGHT - 75

// Light Gray
const Colour BlomeColour_LightGray = Colour(165, 165, 175);
const Colour BlomeColour_LightGrayLightTransparent = Colour(BlomeColour_LightGray).withAlpha(0.5f);

// Gray
const Colour BlomeColour_Gray = Colour(70, 70, 70);

// Dark Gray
const Colour BlomeColour_DarkGray = Colour(40, 40, 40);

// Black
const Colour BlomeColour_Black = Colour(0, 0, 0);
const Colour BlomeColour_FullTransparent = Colour(BlomeColour_Black).withAlpha(0.0f);
const Colour BlomeColour_BlackLightTransparent = Colour(BlomeColour_Black).withAlpha(0.45f);
const Colour BlomeColour_BlackMediumTransparent = Colour(BlomeColour_Black).withAlpha(0.78f);
const Colour BlomeColour_BlackStrongTransparent = Colour(BlomeColour_Black).withAlpha(0.95f);

// BlueBlack
const Colour BlomeColour_BlueBlack = Colour(0, 0, 20);

// Dark Yellow
const Colour BlomeColour_DarkRed = Colour(138, 39, 0);
const Colour BlomeColour_DarkRedStrongTransparent = Colour(BlomeColour_DarkRed).withAlpha(0.99f);

// Fonts
const Font font_small_bold("Helvetica Neue", 12.00f, Font::bold);
const Font font_small_accentuated("Helvetica Neue", 12.30f, Font::bold);
const Font font_medium("Helvetica Neue", 22.00f, Font::bold);
const Font font_large("Helvetica Neue", 48.00f, Font::bold);
