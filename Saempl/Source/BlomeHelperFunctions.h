/*
  ==============================================================================

    BlomeHelperFunctions.h
    Created: 22 May 2023 10:47:09am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

const StringArray supportedAudioFileFormat = StringArray({ ".mp3", ".wav" });

inline bool isSupportedAudioFileFormat(String inExtension)
{
    for(String extension : supportedAudioFileFormat)
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
