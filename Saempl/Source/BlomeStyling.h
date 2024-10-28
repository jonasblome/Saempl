/*
 ==============================================================================
 
 BlomeStyleDefinitions.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"

struct BlomeStyling
{
    using StylingPtr=SharedResourcePointer<BlomeStyling>;
    
    // Other
    float const CORNER_SIZE_MEDIUM = 6.0;
    float const BUTTON_IS_DEFAULT_ALPHA = 1.0;
    float const BUTTON_IS_OVER_ALPHA = 0.7;
    float const BUTTON_IS_DOWN_ALPHA = 0.9;
    float const BUTTON_IS_DEFAULT_DEACTIVATED_ALPHA = 0.2;
    float const BUTTON_IS_OVER_DEACTIVATED_ALPHA = 0.05;
    float const BUTTON_IS_DOWN_DEACTIVATED_ALPHA = 0.1;
    int const PANEL_MARGIN = 8;
    int const PANEL_TITLE_HEIGHT = 42;
    int const SAMPLE_PREVIEW_TITLE_HEIGHT = 25;
    int const BUTTON_SIZE_MEDIUM = 25;
    int const BUTTON_SIZE_SMALL = 20;
    int const COMBO_BOX_WIDTH_LARGE = 180;
    int const COMBO_BOX_WIDTH_MEDIUM = 120;
    
    
    // Panel dimensions
    int const MIN_WINDOW_WIDTH = 700;
    int const MIN_WINDOW_HEIGHT = 500;
    
    int const MAX_WINDOW_WIDTH = 1600;
    int const MAX_WINDOW_HEIGHT = 1000;
    
    int const MAIN_PANEL_WIDTH = 900; // 1600;
    int const MAIN_PANEL_HEIGHT = 750; // 1000;
    
    int const HEADER_PANEL_WIDTH = MAIN_PANEL_WIDTH;
    int const HEADER_PANEL_HEIGHT = 50;
    
    int const FILTER_PANEL_WIDTH = 400;
    int const FILTER_PANEL_HEIGHT = 50;
    
    int const FILTER_RULE_WIDTH = FILTER_PANEL_WIDTH;
    int const FILTER_RULE_HEIGHT = 35;
    
    int const FILTER_RULE_TITLE_WIDTH = 80;
    
    int const ABOUT_PANEL_WIDTH = 300;
    int const ABOUT_PANEL_HEIGHT = 180;
    
    int const CENTRE_PANEL_WIDTH = MAIN_PANEL_WIDTH;
    int const CENTRE_PANEL_HEIGHT = MAIN_PANEL_HEIGHT - HEADER_PANEL_HEIGHT;
    
    int const SAMPLE_FAVOURITES_PANEL_WIDTH = 200;
    int const SAMPLE_FAVOURITES_PANEL_HEIGHT = CENTRE_PANEL_HEIGHT;
    
    int const SAMPLE_ITEM_PANEL_WIDTH = CENTRE_PANEL_WIDTH - SAMPLE_FAVOURITES_PANEL_WIDTH;
    int const SAMPLE_ITEM_PANEL_HEIGHT = 130 + BUTTON_SIZE_SMALL + PANEL_MARGIN;
    
    int const SAMPLE_NAVIGATION_PANEL_WIDTH = SAMPLE_ITEM_PANEL_WIDTH;
    int const SAMPLE_NAVIGATION_PANEL_HEIGHT = CENTRE_PANEL_HEIGHT - SAMPLE_ITEM_PANEL_HEIGHT;
    
    int const GRID_OPTIONS_PANEL_WIDTH = 300;
    int const GRID_OPTIONS_PANEL_HEIGHT = 465;
    
    int const SAMPLE_CONTROL_WIDTH = 45;
    int const SAMPLE_CONTROL_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;
    
    int const SAMPLE_PREVIEW_WIDTH = SAMPLE_ITEM_PANEL_WIDTH - SAMPLE_CONTROL_WIDTH;
    int const SAMPLE_PREVIEW_HEIGHT = SAMPLE_ITEM_PANEL_HEIGHT;
    
    
    // Colour definitions
    Colour const COLOUR_LIGHT_GRAY = Colour(155, 155, 175);
    Colour const COLOUR_LIGHT_GRAY_LIGHT_TRANSPARENT = Colour(COLOUR_LIGHT_GRAY).withAlpha(0.5f);
    
    Colour const COLOUR_GRAY = Colour(70, 70, 70);
    
    Colour const COLOUR_DARK_GRAY = Colour(40, 40, 40);
    
    Colour const COLOUR_BLACK = Colour(0, 0, 0);
    Colour const COLOUR_BLACK_SUPERLIGHT_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.20f);
    Colour const COLOUR_BLACK_LIGHT_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.35f);
    Colour const COLOUR_BLACK_MEDIUM_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.78f);
    Colour const COLOUR_BLACK_STRONG_TRANSPARENT = Colour(COLOUR_BLACK).withAlpha(0.95f);
    
    Colour const COLOUR_TRANSPARENT = Colour(0, 0, 0).withAlpha(0.0f);
    
    Colour const COLOUR_ACCENT_LIGHT = Colour(228, 97, 106); // 207, 191, 163 // 21, 149, 178 // 190, 215, 238 // 142, 158, 90
    Colour const COLOUR_ACCENT_LIGHT_MEDIUM_TRANSPARENT = Colour(COLOUR_ACCENT_LIGHT).withAlpha(0.78f);
    Colour const COLOUR_ACCENT_LIGHT_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_LIGHT).withAlpha(0.99f);
    
    Colour const COLOUR_ACCENT_MEDIUM = Colour(75, 107, 121); // 71, 24, 19 // 18, 44, 55 // 15, 71, 82 // 80, 97, 74
    Colour const COLOUR_ACCENT_MEDIUM_MEDIUM_TRANSPARENT = Colour(COLOUR_ACCENT_MEDIUM).withAlpha(0.78f);
    Colour const COLOUR_ACCENT_MEDIUM_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_MEDIUM).withAlpha(0.99f);
    
    Colour const COLOUR_ACCENT_DARK = Colour(14, 30, 35);  // 33, 16, 15 // 15, 25, 31 // 19, 41, 47 // 41, 43, 34
    Colour const COLOUR_ACCENT_DARK_STRONG_TRANSPARENT = Colour(COLOUR_ACCENT_DARK).withAlpha(0.99f);
    
    Colour const COLOUR_HEADER_BUTTONS = COLOUR_ACCENT_DARK;
    Colour const COLOUR_SYMBOL_BUTTON = COLOUR_ACCENT_LIGHT;
    Colour const COLOUR_PANEL_TITLE_FONT = COLOUR_ACCENT_MEDIUM;
    
    
    // Fonts
    Font const FONT_EXTRA_SMALL_BOLD = Font(FontOptions("Helvetica Neue", 8.00f, Font::bold));
    
    Font const FONT_SMALL = Font(FontOptions("Helvetica Neue", 12.00f, Font::plain));
    Font const FONT_SMALL_BOLD = Font(FontOptions("Helvetica Neue", 12.00f, Font::bold));
    Font const FONT_SMALL_BOLD_ACCENTUATED = Font(FontOptions("Helvetica Neue", 13.00f, Font::bold));
    
    Font const FONT_MEDIUM_SMALL = Font(FontOptions("Helvetica Neue", 16.00f, Font::plain));
    Font const FONT_MEDIUM_SMALL_BOLD = Font(FontOptions("Helvetica Neue", 16.00f, Font::bold));
    
    Font const FONT_MEDIUM = Font(FontOptions("Helvetica Neue", 22.00f, Font::plain));
    Font const FONT_MEDIUM_BOLD = Font(FontOptions("Helvetica Neue", 22.00f, Font::bold));
    
    Font const FONT_LARGE = Font(FontOptions("Helvetica Neue", 48.00f, Font::plain));
    Font const FONT_LARGE_BOLD = Font(FontOptions("Helvetica Neue", 48.00f, Font::bold));
};
