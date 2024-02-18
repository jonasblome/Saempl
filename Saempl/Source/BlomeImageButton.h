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

/**
 An ImageButton derivation that also draws a background if needed.
 */
class BlomeImageButton
:
public ImageButton
{
public:
    /**
     Creates the image button.
     
     @param name the name of the button object.
     @param inHasBackground whether to draw a background or not.
     */
    BlomeImageButton(String const & name, bool inHasBackground);
    ~BlomeImageButton();
    
private:
    bool hasBackground;
    BlomeStyling::Ptr style;
    
    void paintButton(Graphics&, bool, bool) override;
};
