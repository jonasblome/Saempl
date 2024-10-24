/*
 ==============================================================================
 
 BlomeImageComboBox.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once
#include "JuceHeader.h"
#include "BlomeStyling.h"

class BlomeImageComboBox
:
public ComboBox
{
    public:
    /**
     Creates the image combo box.
     
     @param name the name of the combo box object.
     @param inImage the image to draw as the combo box.
     */
    BlomeImageComboBox(String const & name, Image const & inImage);
    ~BlomeImageComboBox();
    
    private:
    BlomeStyling::StylingPtr style;
    Image comboBoxImage;
    
    void paint(Graphics & g) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlomeImageComboBox);
};
