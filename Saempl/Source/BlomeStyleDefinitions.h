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

const int SAMPLE_NAVIGATION_PANEL_WIDTH = CENTER_PANEL_WIDTH;
const int SAMPLE_NAVIGATION_PANEL_HEIGHT = 500;

const int SAMPLE_ITEM_PANEL_TOGGLE_HEIGHT = 20;

const int SAMPLE_ITEM_PANEL_WIDTH = CENTER_PANEL_WIDTH;
const int SAMPLE_ITEM_PANEL_HEIGHT = CENTER_PANEL_HEIGHT - SAMPLE_NAVIGATION_PANEL_HEIGHT - SAMPLE_ITEM_PANEL_TOGGLE_HEIGHT;

const int SAMPLE_CONTROL_WIDTH = 25;
const int SAMPLE_CONTROL_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;

const int SAMPLE_PREVIEW_WIDTH = SAMPLE_ITEM_PANEL_WIDTH - SAMPLE_CONTROL_WIDTH;
const int SAMPLE_PREVIEW_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;


// Colour definitions
const Colour COLOUR_GREENISH_WHITE = Colour(235, 255, 235);

const Colour COLOUR_LIGHT_GRAY = Colour(155, 155, 175);
const Colour COLOUR_LIGHT_GRAY_LIGHT_TRANSPARENT = Colour(COLOUR_LIGHT_GRAY).withAlpha(0.5f);

const Colour COLOUR_GRAY = Colour(70, 70, 70);

const Colour COLOUR_DARK_GRAY = Colour(40, 40, 40);

const Colour COLOUR_BLACK = Colour(0, 0, 0);
const Colour COLOUR_BLACK_LIGHT_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.35f);
const Colour COLOUR_BLACK_MEDIUM_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.78f);
const Colour COLOUR_BLACK_STRONG_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.95f);

const Colour COLOUR_TRANSPARENT = Colour(0, 0, 0).withAlpha(0.0f);

const Colour COLOUR_GREENISH_BLACK = Colour(0, 25, 0);

const Colour COLOUR_ACCENT_LIGHT = Colour(216, 219, 167);
const Colour COLOUR_ACCENT_LIGHT_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_LIGHT).withAlpha(0.99f);

const Colour COLOUR_ACCENT_MEDIUM = Colour(19, 69, 43);
const Colour COLOUR_ACCENT_MEDIUM_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_MEDIUM).withAlpha(0.99f);

const Colour COLOUR_ACCENT_DARK = Colour(11, 41, 25);
const Colour COLOUR_ACCENT_DARK_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_DARK).withAlpha(0.99f);


// Fonts
const Font FONT_SMALL_BOLD("Helvetica Neue", 12.00f, Font::bold);
const Font FONT_SMALL_ACCENTUATED("Helvetica Neue", 12.30f, Font::bold);
const Font FONT_MEDIUM_SMALL_BOLD("Helvetica Neue", 16.00f, Font::bold);
const Font FONT_MEDIUM("Helvetica Neue", 22.00f, Font::bold);
const Font FONT_LARGE("Helvetica Neue", 48.00f, Font::bold);


// Other
const float MEDIUM_CORNER_SIZE = 6.0;
const int PANEL_MARGIN = 8;
const int MEDIUM_BUTTON_HEIGHT = 25;
