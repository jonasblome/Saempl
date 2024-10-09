/*
 ==============================================================================
 
 BlomeHelpers.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "BlomeStyling.h"

#if JUCE_WINDOWS
static String const DIRECTORY_SEPARATOR = "\\";
#elif JUCE_MAC || JUCE_LINUX
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
static int const NUM_SPECTRAL_BANDS = 16;
static int const NUM_FEATURES = 12;
static int const LOWER_BPM_LIMIT = 70;
static int const UPPER_BPM_LIMIT = 180;

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
    { 0, "Gb major/Eb minor" },
    { 1, "Db major/Bb minor" },
    { 2, "Ab major/F minor" },
    { 3, "Eb major/C minor" },
    { 4, "Bb major/G minor" },
    { 5, "F major/D minor" },
    { 6, "C major/A minor" },
    { 7, "G maj/E minor" },
    { 8, "D major/B minor" },
    { 9, "A major/F# minor" },
    { 10, "E major/C# minor" },
    { 11, "B major/G# minor" },
    { NO_KEY_INDEX, "No key detected" },
    { SAMPLE_TOO_LONG_INDEX, "Too long, analyse separately!" },
};

static Array<Array<int>> const KEY_PATTERNS = Array<Array<int>>
({
    Array<int>({ 6, 8, 10, 11, 1, 3, 5 }), // Gb
    Array<int>({ 1, 3, 5, 6, 8, 10, 0 }),  // Db
    Array<int>({ 8, 10, 0, 1, 3, 5, 7 }),  // Ab
    Array<int>({ 3, 5, 7, 8, 10, 0, 2 }),  // Eb
    Array<int>({ 10, 0, 2, 3, 5, 7, 9 }),  // Bb
    Array<int>({ 5, 7, 9, 10, 0, 2, 4 }),  // F
    Array<int>({ 0, 2, 4, 5, 7, 9, 11 }),  // C
    Array<int>({ 7, 9, 11, 0, 2, 4, 6 }),  // G
    Array<int>({ 2, 4, 6, 7, 9, 11, 1 }),  // D
    Array<int>({ 9, 11, 1, 2, 4, 6, 8 }),  // A
    Array<int>({ 4, 6, 8, 9, 11, 1, 3 }),  // E
    Array<int>({ 11, 1, 3, 4, 6, 8, 10 }), // B
});

// Grid options preset vectors
static std::vector<float> const GRID_PRESET_HARMONIC = std::vector<float>
({
    1.0,  // Length
    0.0,  // Loudness
    0.0,  // Dynamic Range
    1.0,  // Zero Crossing Rate
    0.5,  // Tempo
    1.0,  // Key
    1.0,  // Avg. Frequency
    1.0,  // Freq. Spread
    1.0,  // Upper Freq. Rolloff
    1.0,  // Freq. Flux
    1.0,  // Harmonic Flux
    3.0,  // Freq. Distribution
    4.0,  // Harmonic Distribution
});

static std::vector<float> const GRID_PRESET_DRUMS = std::vector<float>
({
    1.0,  // Length
    0.0,  // Loudness
    1.0,  // Dynamic Range
    1.0,  // Zero Crossing Rate
    0.0,  // Tempo
    0.0,  // Key
    10.0, // Avg. Frequency,
    1.0,  // Freq. Spread
    1.0,  // Upper Freq. Rolloff
    1.0,  // Freq. Flux
    0.2,  // Harmonic Flux
    5.0,  // Freq. Distribution
    0.2,  // Harmonic Distribution
});

static std::vector<float> const GRID_PRESET_MONOPHONIC = std::vector<float>
({
    1.0,  // Length
    0.0,  // Loudness
    1.0,  // Dynamic Range
    1.0,  // Zero Crossing Rate
    0.5,  // Tempo
    1.0,  // Key
    1.0,  // Avg. Frequency
    1.0,  // Freq. Spread
    1.0,  // Upper Freq. Rolloff
    1.0,  // Freq. Flux
    1.0,  // Harmonic Flux
    1.0,  // Freq. Distribution
    1.0,  // Harmonic Distribution
});

static std::vector<float> const GRID_PRESET_FOLEY = std::vector<float>
({
    1.0,  // Length
    0.0,  // Loudness
    1.0,  // Dynamic Range
    1.0,  // Zero Crossing Rate
    0.0,  // Tempo
    0.0,  // Key
    1.0,  // Avg. Frequency
    1.0,  // Freq. Spread
    1.0,  // Upper Freq. Rolloff
    1.0,  // Freq. Flux
    0.0,  // Harmonic Flux
    4.0,  // Freq. Distribution
    0.0,  // Harmonic Distribution
});

// Sample library collection types
enum SampleItemCollectionScope
{
    ALL_SAMPLES,
    FILTERED_SAMPLES,
    FAVOURITE_SAMPLES,
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
