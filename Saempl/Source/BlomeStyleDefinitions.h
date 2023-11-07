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
static float const CORNER_SIZE_MEDIUM = 6.0;
static int const PANEL_MARGIN = 8;
static int const BUTTON_SIZE_MEDIUM = 25;
static int const BUTTON_SIZE_SMALL = 20;
static int const COMBO_BOX_WIDTH_MEDIUM = 180;


// Panel dimensions
static int const MAIN_PANEL_WIDTH = 900;
static int const MAIN_PANEL_HEIGHT = 600;

static int const HEADER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
static int const HEADER_PANEL_HEIGHT = 50;

static int const FILTER_PANEL_WIDTH = 400;
static int const FILTER_PANEL_HEIGHT = 50;

static int const FILTER_RULE_WIDTH = FILTER_PANEL_WIDTH;
static int const FILTER_RULE_HEIGHT = 35;

static int const FILTER_RULE_TITLE_WIDTH = 80;

static int const CENTER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
static int const CENTER_PANEL_HEIGHT = MAIN_PANEL_HEIGHT - HEADER_PANEL_HEIGHT;

static int const SAMPLE_PALETTE_PANEL_WIDTH = 200;
static int const SAMPLE_PALETTE_PANEL_HEIGHT = CENTER_PANEL_HEIGHT;

static int const SAMPLE_NAVIGATION_PANEL_WIDTH = CENTER_PANEL_WIDTH - SAMPLE_PALETTE_PANEL_WIDTH;
static int const SAMPLE_NAVIGATION_PANEL_HEIGHT = 350;

static int const SAMPLE_ITEM_PANEL_WIDTH = SAMPLE_NAVIGATION_PANEL_WIDTH;
static int const SAMPLE_ITEM_PANEL_HEIGHT = CENTER_PANEL_HEIGHT - SAMPLE_NAVIGATION_PANEL_HEIGHT - BUTTON_SIZE_SMALL - PANEL_MARGIN;

static int const SAMPLE_CONTROL_WIDTH = 45;
static int const SAMPLE_CONTROL_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;

static int const SAMPLE_PREVIEW_WIDTH = SAMPLE_ITEM_PANEL_WIDTH - SAMPLE_CONTROL_WIDTH;
static int const SAMPLE_PREVIEW_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;

static int const SAMPLE_PREVIEW_TITLE_HEIGHT = 25;
static int const PANEL_TITLE_HEIGHT = 42;


// Colour definitions
static Colour const COLOUR_GREENISH_WHITE = Colour(235, 255, 235);

static Colour const COLOUR_LIGHT_GRAY = Colour(155, 155, 175);
static Colour const COLOUR_LIGHT_GRAY_LIGHT_TRANSPARENT = Colour(COLOUR_LIGHT_GRAY).withAlpha(0.5f);

static Colour const COLOUR_GRAY = Colour(70, 70, 70);

static Colour const COLOUR_DARK_GRAY = Colour(40, 40, 40);

static Colour const COLOUR_BLACK = Colour(0, 0, 0);
static Colour const COLOUR_BLACK_SUPERLIGHT_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.15f);
static Colour const COLOUR_BLACK_LIGHT_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.35f);
static Colour const COLOUR_BLACK_MEDIUM_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.78f);
static Colour const COLOUR_BLACK_STRONG_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.95f);

static Colour const COLOUR_TRANSPARENT = Colour(0, 0, 0).withAlpha(0.0f);

static Colour const COLOUR_GREENISH_BLACK = Colour(0, 25, 0);

static Colour const COLOUR_ACCENT_LIGHT = Colour(207, 191, 163);
static Colour const COLOUR_ACCENT_LIGHT_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_LIGHT).withAlpha(0.99f);

static Colour const COLOUR_ACCENT_MEDIUM = Colour(71, 24, 19);
static Colour const COLOUR_ACCENT_MEDIUM_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_MEDIUM).withAlpha(0.99f);

static Colour const COLOUR_ACCENT_DARK = Colour(33, 16, 15);
static Colour const COLOUR_ACCENT_DARK_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_DARK).withAlpha(0.99f);


// Fonts
static Font const FONT_SMALL("Helvetica Neue", 12.00f, Font::plain);
static Font const FONT_SMALL_BOLD("Helvetica Neue", 12.00f, Font::bold);
static Font const FONT_SMALL_BOLD_ACCENTUATED("Helvetica Neue", 13.00f, Font::bold);

static Font const FONT_MEDIUM_SMALL("Helvetica Neue", 16.00f, Font::plain);
static Font const FONT_MEDIUM_SMALL_BOLD("Helvetica Neue", 16.00f, Font::bold);

static Font const FONT_MEDIUM("Helvetica Neue", 22.00f, Font::plain);
static Font const FONT_MEDIUM_BOLD("Helvetica Neue", 22.00f, Font::bold);

static Font const FONT_LARGE("Helvetica Neue", 48.00f, Font::plain);
static Font const FONT_LARGE_BOLD("Helvetica Neue", 48.00f, Font::bold);
