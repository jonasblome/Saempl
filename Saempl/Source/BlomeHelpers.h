/*
 ==============================================================================
 
 BlomeHelpers.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"

#if JUCE_WINDOWS
static String const DIRECTORY_SEPARATOR = "\\";
#elif JUCE_MAC
static String const DIRECTORY_SEPARATOR = "/";
#endif

static String const SAMPLE_LIBRARY_FILE_EXTENSION = ".bslf";
static String const SAEMPL_DATA_FILE_EXTENSION = ".saempl";
static String const EMPTY_TILE_PATH = "EMPTYTILE";
static StringArray const SUPPORTED_AUDIO_FORMATS = StringArray({ ".mp3", ".wav", ".aiff", ".m4a" });
static String const SUPPORTED_AUDIO_FORMATS_WILDCARD = "*.wav;*.mp3;*.aiff;*.m4a";
static String const SUPPORTED_AUDIO_FORMATS_EXTENSIONS = ".wav;.mp3;.aiff;.m4a";
static StringArray const PROPERTY_NAMES = StringArray({ "Title", "Length", "dB", "LUFS", "Tempo", "Key" });
static int const NUM_CHROMA = 12;
static int const NUM_SPECTRAL_BANDS = 12;
static int const NUM_FEATURES = 12;

enum CompareOperators
{
    LESS_THAN = 0,
    EQUAL_TO,
    GREATER_THAN,
    CONTAINS
};

enum NavigationPanelType
{
    PANELS_LIBRARY_PANEL = 0,
    PANELS_TABLE_PANEL,
    PANELS_GRID_PANEL,
};

static std::map<NavigationPanelType, String> NAVIGATION_PANEL_TYPE_TO_STRING
{
    { PANELS_LIBRARY_PANEL, "PANELS_LIBRARY_PANEL" },
    { PANELS_TABLE_PANEL, "PANELS_TABLE_PANEL" },
    { PANELS_GRID_PANEL, "PANELS_GRID_PANEL"}
};

static std::map<String, NavigationPanelType> STRING_TO_NAVIGATION_PANEL_TYPE
{
    { "PANELS_LIBRARY_PANEL", PANELS_LIBRARY_PANEL },
    { "PANELS_TABLE_PANEL", PANELS_TABLE_PANEL },
    { "PANELS_GRID_PANEL", PANELS_GRID_PANEL }
};

static std::map<CompareOperators, String> COMPARE_OPERATOR_TO_STRING
{
    { CompareOperators::CONTAINS, "CONTAINS" },
    { CompareOperators::EQUAL_TO, "EQUAL_TO" },
    { CompareOperators::GREATER_THAN, "GREATER_THAN" },
    { CompareOperators::LESS_THAN, "LESS_THAN" },
};

static std::map<String, CompareOperators> STRING_TO_COMPARE_OPERATORS
{
    { "CONTAINS", CompareOperators::CONTAINS },
    { "EQUAL_TO", CompareOperators::EQUAL_TO },
    { "GREATER_THAN", CompareOperators::GREATER_THAN },
    { "LESS_THAN", CompareOperators::LESS_THAN },
};

static int const NO_KEY_INDEX = 100;
static int const SAMPLE_TOO_LONG_INDEX = 200;

static std::map<int, String> KEY_INDEX_TO_KEY_NAME
{
    { 0, "Gb maj/Eb min" },
    { 1, "Db maj/Bb min" },
    { 2, "Ab maj/F min" },
    { 3, "Eb maj/C min" },
    { 4, "Bb maj/G min" },
    { 5, "F maj/D min" },
    { 6, "C maj/A min" },
    { 7, "G maj/E min" },
    { 8, "D maj/B min" },
    { 9, "A maj/F# min" },
    { 10, "E maj/C# min" },
    { 11, "B maj/G# min" },
    { NO_KEY_INDEX, "No key detected" },
    { SAMPLE_TOO_LONG_INDEX, "Too long, analyse separately!" },
};

static Array<Array<int>> const KEY_PATTERNS = Array<Array<int>>
({
    Array<int>({ 6, 8, 10, 11, 1, 3, 5 }), // Gb
    Array<int>({ 1, 3, 5, 6, 8, 10, 0 }), // Db
    Array<int>({ 8, 10, 0, 1, 3, 5, 7 }), // Ab
    Array<int>({ 3, 5, 7, 8, 10, 0, 2 }), // Eb
    Array<int>({ 10, 0, 2, 3, 5, 7, 9 }), // Bb
    Array<int>({ 5, 7, 9, 10, 0, 2, 4 }), // F
    Array<int>({ 0, 2, 4, 5, 7, 9, 11 }), // C
    Array<int>({ 7, 9, 11, 0, 2, 4, 6 }), // G
    Array<int>({ 2, 4, 6, 7, 9, 11, 1 }), // D
    Array<int>({ 9, 11, 1, 2, 4, 6, 8 }), // A
    Array<int>({ 4, 6, 8, 9, 11, 1, 3 }), // E
    Array<int>({ 11, 1, 3, 4, 6, 8, 10 }), // B
});

// Sample library collection types
enum SampleItemCollectionScope
{
    ALL_SAMPLES,
    FILTERED_SAMPLES,
    PALETTE_SAMPLES,
};

/**
 Checks if a file extension is a supported audio format.
 
 @param inExtension the string to check.
 
 @returns whether the extension is supported or not.
 */
inline bool isSupportedAudioFileFormat(String inExtension)
{
    for (String extension : SUPPORTED_AUDIO_FORMATS)
    {
        if (extension == inExtension)
        {
            return true;
        }
    }
    
    return false;
}

/**
 Draws a drop shadow for a given area and position.
 
 @param g the graphics object.
 @param dropShadowArea the area to draw the shadow in.
 @param offsetX the amount to offset the shadow in the x direction.
 @param offsetY the amount to offset the shadow in the y direction.
 @param radius the shadows gradient radius.
 @param style the styling object that provides global styling definitions.
 */
inline void drawDropShadow(Graphics& g,
                           Rectangle<int> dropShadowArea,
                           int offsetX,
                           int offsetY,
                           int radius,
                           BlomeStyling::Ptr const & style)
{
    Image dropShadowImage(Image::ARGB, g.getClipBounds().getWidth(), g.getClipBounds().getHeight(), true);
    Graphics g2(dropShadowImage);
    DropShadow(style->COLOUR_BLACK_SUPERLIGHT_TRANSPARENT, radius, { offsetX, offsetY }).drawForRectangle(g2, dropShadowArea);
    g.setColour(style->COLOUR_BLACK);
    g.drawImageAt(dropShadowImage, 0, 0);
}
