/*
  ==============================================================================

    AboutPanel.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "BlomeStyling.h"

class AboutPanel
:
public Component
{
public:
    AboutPanel();
    ~AboutPanel();
    
private:
    BlomeStyling::Ptr style;
    
    void mouseEnter(MouseEvent const & event) override;
    void mouseExit(MouseEvent const & event) override;
    void paint(Graphics& g) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AboutPanel);
};