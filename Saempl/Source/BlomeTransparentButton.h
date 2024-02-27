/*
  ==============================================================================

    BlomeTransparentButton.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class BlomeTransparentButton
:
public Button
{
public:
    BlomeTransparentButton(String const & buttonName);
    ~BlomeTransparentButton() override;
    
private:
    void paintButton(Graphics&, bool, bool) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeTransparentButton);
};
