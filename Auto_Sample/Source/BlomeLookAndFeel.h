/*
  ==============================================================================

    BlomeLookAndFeel.h
    Created: 22 May 2023 6:53:58am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class BlomeLookAndFeel
:   public LookAndFeel_V4
{
public:
    BlomeLookAndFeel()
    {
        // ComboBox Colours
        setColour(ComboBox::backgroundColourId, BlomeColour_BlackLightTransparent);
        setColour(ComboBox::outlineColourId, BlomeColour_Black);
        setColour(ComboBox::arrowColourId, BlomeColour_LightGray);
        setColour(ComboBox::textColourId, BlomeColour_LightGray);
        setColour(PopupMenu::backgroundColourId, BlomeColour_DarkYellowStrongTransparent);
        
        // Button Text Colours
        setColour(TextButton::buttonColourId, BlomeColour_LightGray);
        setColour(TextButton::textColourOnId, BlomeColour_LightGray);
        setColour(TextButton::textColourOffId, BlomeColour_LightGray);
    }
    
    virtual ~BlomeLookAndFeel() {
        
    }
    
    /** Buttons */
    Font getTextButtonFont (TextButton&, int buttonHeight) override
    {
        return font_small_bold;
    }
    
    void drawButtonBackground (Graphics& g, Button& button,
                               const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override
    {
        Colour fillColour;
        
        if(shouldDrawButtonAsDown) {
            fillColour = BlomeColour_BlackStrongTransparent;
        } else if(shouldDrawButtonAsHighlighted) {
            fillColour = BlomeColour_BlackLightTransparent;
        } else {
            fillColour = BlomeColour_BlackMediumTransparent;
        }
        
        const float cornerSize = 6.0f;
        const Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        g.setColour(fillColour);
        g.fillRoundedRectangle(bounds.reduced(1), cornerSize);
    }
    
    void drawToggleButton (Graphics& g,
                           ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override
     {
         Colour fillColour;
         float cornerSize = 6.0f;
         const Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
         
         if(shouldDrawButtonAsHighlighted)
         {
             fillColour = BlomeColour_BlackLightTransparent;
         }
         else
         {
             fillColour = BlomeColour_BlackMediumTransparent;
         }

         if (!button.isEnabled())
         {
             g.setOpacity (0.5f);
         }
         
         g.setColour(fillColour);
         g.fillRoundedRectangle(bounds.reduced(1), cornerSize);
         
         if(button.getToggleState())
         {
             cornerSize = 4.0f;
             g.setColour(BlomeColour_DarkYellow);
             g.fillRoundedRectangle(bounds.reduced(3), cornerSize);
         }
     }
    
    /** ComboBoxes */
    Font getLabelFont(Label& label) override
    {
        return font_small_bold;
    }
    
    void drawPopupMenuBackgroundWithOptions(Graphics& g,
                                            int width,
                                            int height,
                                            const PopupMenu::Options& options) override
    {
        g.setColour(findColour(PopupMenu::backgroundColourId));
        g.fillRoundedRectangle(0, 2, width, height - 2, 3.0f);
    }
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override
    {
        Rectangle<int> r (area);
        
        Colour fontColour = isTicked ? BlomeColour_BlackStrongTransparent : BlomeColour_BlackLightTransparent;
        g.setColour(fontColour);
        g.setFont(isTicked ? font_small_accentuated : font_small_bold);
        
        r.setLeft(10);
        r.setY(1);
        g.drawFittedText(text, r, Justification::left, 1);
    }
    
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& box) override
    {
        const float cornerSize = 3.0f;
        const Rectangle<int> boxBounds (0, 0, width, height);
        
        g.setColour(BlomeColour_BlackMediumTransparent);
        g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);
        
        Rectangle<int> arrow (width - 30, 0, 20, height);
        
        Path path;
        
        path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
        path.lineTo(arrow.toFloat().getCentreX(), arrow.toFloat().getCentreY() + 3.0f);
        path.lineTo(arrow.toFloat().getRight() - 3.0f, arrow.toFloat().getCentreY() - 2.0f);
        
        Colour arrowColour = box.findColour(ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 0.9f : 0.2f);
        
        g.setColour(arrowColour);
        g.strokePath(path, PathStrokeType(2.0f));
    }
    
    void positionComboBoxText (ComboBox& box, Label& label) override
    {
        label.setBounds(6, 1, box.getWidth() - 30, box.getHeight() - 2);
        label.setFont (getComboBoxFont(box));
    }
    
    /** Sliders **/
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider& slider) override
    {
        g.setColour(BlomeColour_BlackMediumTransparent);
        g.fillEllipse(x + 6, y + 6, width - 14, height - 14);
        g.setColour(BlomeColour_LightGray);
        Line<float> sliderTick = Line<float>::fromStartAndAngle(Point<float>(width * 0.5 - 1, height * 0.5 - 1), (width - 19) * 0.5, sliderPosProportional * M_PI * 1.5 - (M_PI * 0.75));
        g.drawLine(sliderTick, 2.0);
    }
    
private:
    
};
