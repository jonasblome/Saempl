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
StringArray const PROPERTY_NAMES = StringArray({ "Instrument", "Key", "Tempo", "Length", "Title" });
std::map<String, int> const PROPERTY_NAME_TYPES = // 0 = int, 1 = double, 2 = juce::String
{
    {"Instrument", 2},
    {"Key", 0},
    {"Tempo", 1},
    {"Length", 1},
    {"Title", 2}
};

// Filter rule compare operators
enum CompareOperators
{
    EqualTo,
    LessThan,
    GreaterThan,
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
