/*
 ==============================================================================
 
 BlomeHelperFunctions.h
 Created: 22 May 2023 10:47:09am
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

// Custom file extensions
static String const SAMPLE_LIBRARY_FILE_EXTENSION = ".bslf";
static String const SAEMPL_DATA_FILE_EXTENSION = ".saempl";

// Audio formats
static StringArray const SUPPORTED_AUDIO_FORMATS = StringArray({ ".mp3", ".wav", ".aiff", ".m4a" });
static String const SUPPORTED_AUDIO_FORMATS_WILDCARD = "*.wav;*.mp3;*.aiff;*.m4a";
static String const SUPPORTED_AUDIO_FORMATS_EXTENSIONS = ".wav;.mp3;.aiff;.m4a";

// Property names
static StringArray const PROPERTY_NAMES = StringArray({ "Length", "Title" });

// Filter rule compare operators
enum CompareOperators
{
    LESS_THAN = 0,
    EQUAL_TO,
    GREATER_THAN,
    CONTAINS
};

// Navigation panel names
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

// Sample library collection types
enum SampleItemCollectionScope
{
    ALL_SAMPLES,
    FILTERED_SAMPLES,
    PALETTE_SAMPLES,
};

// Helper functions
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

inline void showFileDeletedWarning()
{
    // Show warning popup message
    AlertWindow::showAsync(MessageBoxOptions()
                           .withIconType(MessageBoxIconType::NoIcon)
                           .withTitle("File not available!")
                           .withMessage("This file has probably been externally deleted and was removed from the list of available samples.")
                           .withButton("OK"),
                           nullptr);
}
