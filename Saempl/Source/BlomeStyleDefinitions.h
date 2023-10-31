/*
  ==============================================================================

    BlomeStyleDefinitions.h
    Created: 22 May 2023 6:54:17am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

// Other
float const CORNER_SIZE_MEDIUM = 6.0;
int const PANEL_MARGIN = 8;
int const BUTTON_SIZE_MEDIUM = 25;
int const BUTTON_SIZE_SMALL = 20;
int const COMBO_BOX_WIDTH_MEDIUM = 180;


// Panel dimensions
int const MAIN_PANEL_WIDTH = 1350;
int const MAIN_PANEL_HEIGHT = 900;

int const HEADER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
int const HEADER_PANEL_HEIGHT = 50;

int const FILTER_PANEL_WIDTH = 400;
int const FILTER_PANEL_HEIGHT = 50;

int const FILTER_RULE_WIDTH = FILTER_PANEL_WIDTH;
int const FILTER_RULE_HEIGHT = 35;

int const FILTER_RULE_TITLE_WIDTH = 80;

int const CENTER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
int const CENTER_PANEL_HEIGHT = MAIN_PANEL_HEIGHT - HEADER_PANEL_HEIGHT;

int const SAMPLE_PALETTE_PANEL_WIDTH = 250;
int const SAMPLE_PALETTE_PANEL_HEIGHT = CENTER_PANEL_HEIGHT;

int const SAMPLE_NAVIGATION_PANEL_WIDTH = CENTER_PANEL_WIDTH - SAMPLE_PALETTE_PANEL_WIDTH;
int const SAMPLE_NAVIGATION_PANEL_HEIGHT = 500;

int const SAMPLE_ITEM_PANEL_WIDTH = SAMPLE_NAVIGATION_PANEL_WIDTH;
int const SAMPLE_ITEM_PANEL_HEIGHT = CENTER_PANEL_HEIGHT - SAMPLE_NAVIGATION_PANEL_HEIGHT - BUTTON_SIZE_SMALL;

int const SAMPLE_CONTROL_WIDTH = 25;
int const SAMPLE_CONTROL_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;

int const SAMPLE_PREVIEW_WIDTH = SAMPLE_ITEM_PANEL_WIDTH - SAMPLE_CONTROL_WIDTH;
int const SAMPLE_PREVIEW_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;

int const SAMPLE_PREVIEW_TITLE_HEIGHT = 25;
int const PANEL_TITLE_HEIGHT = 42;


// Colour definitions
Colour const COLOUR_GREENISH_WHITE = Colour(235, 255, 235);

Colour const COLOUR_LIGHT_GRAY = Colour(155, 155, 175);
Colour const COLOUR_LIGHT_GRAY_LIGHT_TRANSPARENT = Colour(COLOUR_LIGHT_GRAY).withAlpha(0.5f);

Colour const COLOUR_GRAY = Colour(70, 70, 70);

Colour const COLOUR_DARK_GRAY = Colour(40, 40, 40);

Colour const COLOUR_BLACK = Colour(0, 0, 0);
Colour const COLOUR_BLACK_LIGHT_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.35f);
Colour const COLOUR_BLACK_MEDIUM_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.78f);
Colour const COLOUR_BLACK_STRONG_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.95f);

Colour const COLOUR_TRANSPARENT = Colour(0, 0, 0).withAlpha(0.0f);

Colour const COLOUR_GREENISH_BLACK = Colour(0, 25, 0);

Colour const COLOUR_ACCENT_LIGHT = Colour(216, 219, 167);
Colour const COLOUR_ACCENT_LIGHT_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_LIGHT).withAlpha(0.99f);

Colour const COLOUR_ACCENT_MEDIUM = Colour(19, 69, 43);
Colour const COLOUR_ACCENT_MEDIUM_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_MEDIUM).withAlpha(0.99f);

Colour const COLOUR_ACCENT_DARK = Colour(11, 41, 25);
Colour const COLOUR_ACCENT_DARK_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_DARK).withAlpha(0.99f);


// Fonts
Font const FONT_SMALL_BOLD("Helvetica Neue", 12.00f, Font::bold);
Font const FONT_SMALL_BOLD_ACCENTUATED("Helvetica Neue", 12.70f, Font::bold);
Font const FONT_MEDIUM_SMALL_BOLD("Helvetica Neue", 16.00f, Font::bold);
Font const FONT_MEDIUM("Helvetica Neue", 22.00f, Font::bold);
Font const FONT_LARGE("Helvetica Neue", 48.00f, Font::bold);
