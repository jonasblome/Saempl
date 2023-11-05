/*
  ==============================================================================

    BlomeImageButton.h
    Created: 5 Nov 2023 3:39:31pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "BlomeStyleDefinitions.h"

class BlomeImageButton
:   public ImageButton
{
public:
    BlomeImageButton(const String& name, bool inHasBackground);
    ~BlomeImageButton();
    
private:
    bool hasBackground;
    
    void paintButton(Graphics&, bool, bool) override;
};
