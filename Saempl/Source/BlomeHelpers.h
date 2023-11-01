/*
  ==============================================================================

    BlomeHelperFunctions.h
    Created: 22 May 2023 10:47:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#if JUCE_WINDOWS
    static String const DIRECTORY_SEPARATOR = "\\";
#elif JUCE_MAC
    static String const DIRECTORY_SEPARATOR = "/";
#endif

// Custom file extensions
String const SAMPLE_LIBRARY_FILE_EXTENSION = ".bslf";

// Audio formats
StringArray const SUPPORTED_AUDIO_FORMATS = StringArray({ ".mp3", ".wav", ".aiff", ".m4a" });
String const SUPPORTED_AUDIO_FORMATS_WILDCARD = "*.wav;*.mp3;*.aiff;*.m4a";
String const SUPPORTED_AUDIO_FORMATS_EXTENSIONS = ".wav;.mp3;.aiff;.m4a";

// Property names
StringArray const PROPERTY_NAMES = StringArray({ "Length", "Title" });

// Filter rule compare operators
enum CompareOperators
{
    LESS_THAN,
    EQUAL_TO,
    GREATER_THAN,
    CONTAINS
};

// Navigation panel names
enum NavigationPanelType
{
    PANELS_LIBRARY_PANEL,
    PANELS_TABLE_PANEL,
    PANELS_MAP_PANEL,
};

// Sample library collection types
enum SampleItemCollectionType
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
        if (extension == inExtension) {
            return true;
        }
    }
    
    return false;
}

inline String restoreSpacesFromURLString(String urlString)
{
    return urlString.replace("%20", " ");
}
