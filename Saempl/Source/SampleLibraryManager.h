/*
  ==============================================================================

    SampleLibraryManager.h
    Created: 10 Jun 2023 6:16:05pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelperFunctions.h"

class SampleLibraryManager
{
public:
    // Constructors
    SampleLibraryManager();
    ~SampleLibraryManager();
    
    // Methods
    void createSampleFile(SampleItem& inSampleItem);
    SampleItem* loadSampleFile(String inFilePath);
    void getXmlForSampleItem(SampleItem& inSampleItem, juce::XmlElement* inElement);
    
private:
    // Fields
    
    // Methods
    
};
