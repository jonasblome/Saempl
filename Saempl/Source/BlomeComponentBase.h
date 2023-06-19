/*
  ==============================================================================

    BlomeComponentBase.h
    Created: 18 Jun 2023 1:27:07pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class BlomeComponentBase
:   public Component
{
public:
    // Constructors
    BlomeComponentBase();
    ~BlomeComponentBase();
    
    // Methods
    void mouseEnter(const MouseEvent& event) override;
    void mouseExit(const MouseEvent& event) override;
    void paint(Graphics& g) override;
    
private:
    // Fields
    
    // Methods
    
};
