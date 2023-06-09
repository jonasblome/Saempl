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
    static const juce::String DIRECTORY_SEPARATOR = "\\";
#elif JUCE_MAC
    static const juce::String DIRECTORY_SEPARATOR = "/";
#endif

#define SAMPLE_LIBRARY_FILE_EXTENSION ".bslf"

const StringArray SUPPORTED_AUDIO_FORMATS = StringArray({ ".mp3", ".wav", ".aiff", ".m4a" });
const String SUPPORTED_AUDIO_FORMATS_WILDCARD = "*.wav;*.mp3;*.aiff;*.m4a";

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
