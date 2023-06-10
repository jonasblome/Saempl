/*
  ==============================================================================

    SampleFileManager.h
    Created: 10 Jun 2023 6:16:05pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"

#define SAMPLE_FILE_EXTENSION ".bsf"

class SampleFileManager
{
public:
    // Constructors
    SampleFileManager();
    ~SampleFileManager();
    
    // Methods
    void createSampleFile(SampleItem& inSampleItem);
    SampleItem* loadSampleFile(String inFilePath);
    void getXmlForSampleItem(SampleItem& inSampleItem, juce::XmlElement* inElement);
    
private:
    // Fields
    
    // Methods
    
};
