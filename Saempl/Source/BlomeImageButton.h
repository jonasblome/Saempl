/*
 ==============================================================================
 
 BlomeImageButton.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once
#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"
#include "BlomeHelpers.h"

class BlomeImageButton
:
public ImageButton
{
public:
    BlomeImageButton(const String& name, bool inHasBackground);
    ~BlomeImageButton();
    
private:
    bool hasBackground;
    BlomeStyling::Ptr style;
    
    void paintButton(Graphics&, bool, bool) override;
};
