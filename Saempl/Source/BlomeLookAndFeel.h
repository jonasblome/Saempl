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
    BlomeLookAndFeel(SampleLibrary& inSampleDatabase)
    :   sampleDatabase(inSampleDatabase)
    {
        // ComboBox Colours
        setColour(ComboBox::backgroundColourId, BlomeColour_BlackLightTransparent);
        setColour(ComboBox::outlineColourId, BlomeColour_Black);
        setColour(ComboBox::arrowColourId, BlomeColour_LightGray);
        setColour(ComboBox::textColourId, BlomeColour_LightGray);
        setColour(PopupMenu::backgroundColourId, BlomeColour_AccentColourLightStrongTransparent);
        
        // Button Text Colours
        setColour(TextButton::buttonColourId, BlomeColour_AccentColourLight);
        setColour(TextButton::textColourOnId, BlomeColour_AccentColourLight);
        setColour(TextButton::textColourOffId, BlomeColour_AccentColourLight);
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
        
        if(shouldDrawButtonAsDown)
        {
            fillColour = BlomeColour_BlackStrongTransparent;
        }
        else if(shouldDrawButtonAsHighlighted)
        {
            fillColour = BlomeColour_BlackMediumTransparent;
        }
        else
        {
            fillColour = BlomeColour_BlackLightTransparent;
        }
        
        const float cornerSize = 6.0f;
        const Rectangle<float> bounds = button.getLocalBounds().toFloat();
        g.setColour(fillColour);
        g.fillRoundedRectangle(bounds, cornerSize);
    }
    
    void drawToggleButton (Graphics& g,
                           ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override
     {
         Colour fillColour;
         float cornerSize = 6.0f;
         const Rectangle<float> bounds = button.getLocalBounds().toFloat();
         
         if(shouldDrawButtonAsHighlighted)
         {
             fillColour = BlomeColour_BlackMediumTransparent;
         }
         else
         {
             fillColour = BlomeColour_BlackLightTransparent;
         }

         if (!button.isEnabled())
         {
             g.setOpacity (0.5f);
         }
         
         g.setColour(fillColour);
         g.fillRoundedRectangle(bounds, cornerSize);
         
         if(button.getToggleState())
         {
             cornerSize = 4.0f;
             g.setColour(BlomeColour_AccentColourLight);
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
        
        if(isHighlighted)
        {
            g.setColour(BlomeColour_BlackLightTransparent);
            g.fillRoundedRectangle(r.reduced(1).toFloat(), 3.0);
        }
        
        Colour fontColour = isTicked ? BlomeColour_BlackStrongTransparent : BlomeColour_BlackMediumTransparent;
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
        
        g.setColour(BlomeColour_BlackLightTransparent);
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
    
    void drawLinearSlider (Graphics& g,
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const Slider::SliderStyle style,
                           Slider& slider) override
    {
        if (slider.isBar())
        {
            g.setColour(BlomeColour_BlackMediumTransparent);
            g.fillRect(slider.isHorizontal() ? Rectangle<float>(static_cast<float> (x), (float)y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                       : Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)));
        }
        else
        {
            auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
            auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);
            
            auto trackWidth = jmin (9.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);
            
            Point<float> startPoint (slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
                                     slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));
            
            Point<float> endPoint (slider.isHorizontal() ? (float)width + x : startPoint.x,
                                   slider.isHorizontal() ? startPoint.y : (float)y);
            
            Path backgroundTrack;
            backgroundTrack.startNewSubPath(startPoint);
            backgroundTrack.lineTo(endPoint);
            g.setColour(BlomeColour_BlackLightTransparent);
            g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
            
            Path valueTrack;
            Point<float> minPoint, maxPoint, thumbPoint;
            
            if (isTwoVal || isThreeVal)
            {
                minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                    slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };
                
                if (isThreeVal)
                    thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
                        slider.isHorizontal() ? (float) height * 0.5f : sliderPos };
                
                maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
                    slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
            }
            else
            {
                auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
                auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;
                
                minPoint = startPoint;
                maxPoint = { kx, ky };
            }
            
            auto thumbWidth = getSliderThumbRadius(slider);
            
            valueTrack.startNewSubPath (minPoint);
            valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
            g.setColour(BlomeColour_AccentColourLight);
            g.strokePath(valueTrack, { trackWidth - 4, PathStrokeType::curved, PathStrokeType::rounded });
            
            if (! isTwoVal)
            {
                g.setColour(BlomeColour_AccentColourLight);
                g.fillEllipse (Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (isThreeVal ? thumbPoint : maxPoint));
            }
            
            if (isTwoVal || isThreeVal)
            {
                auto sr = jmin (trackWidth, (slider.isHorizontal() ? (float) height : (float) width) * 0.4f);
                auto pointerColour = slider.findColour(Slider::thumbColourId);
                
                if (slider.isHorizontal())
                {
                    drawPointer (g, minSliderPos - sr,
                                 jmax (0.0f, (float) y + (float) height * 0.5f - trackWidth * 2.0f),
                                 trackWidth * 2.0f, pointerColour, 2);
                    
                    drawPointer (g, maxSliderPos - trackWidth,
                                 jmin ((float) (y + height) - trackWidth * 2.0f, (float) y + (float) height * 0.5f),
                                 trackWidth * 2.0f, pointerColour, 4);
                }
                else
                {
                    drawPointer (g, jmax (0.0f, (float) x + (float) width * 0.5f - trackWidth * 2.0f),
                                 minSliderPos - trackWidth,
                                 trackWidth * 2.0f, pointerColour, 1);
                    
                    drawPointer (g, jmin ((float) (x + width) - trackWidth * 2.0f, (float) x + (float) width * 0.5f), maxSliderPos - sr,
                                 trackWidth * 2.0f, pointerColour, 3);
                }
            }
        }
    }
    
    int getSliderThumbRadius(Slider& slider) override
    {
        return jmin(14, slider.isHorizontal() ? static_cast<int> ((float)slider.getHeight() * 0.5f)
                                               : static_cast<int> ((float)slider.getWidth() * 0.5f));
    }
    
    /** Scroll Bars **/
    void drawScrollbar(Graphics& g,
                       ScrollBar& scrollbar,
                       int x,
                       int y,
                       int width,
                       int height,
                       bool isScrollbarVertical,
                       int thumbStartPosition,
                       int thumbSize,
                       bool isMouseOver,
                       [[maybe_unused]] bool isMouseDown) override
    {
        Rectangle<int> thumbBounds;

        if (isScrollbarVertical)
            thumbBounds = { x, thumbStartPosition, width, thumbSize };
        else
            thumbBounds = { thumbStartPosition, y, thumbSize, height };

        g.setColour (isMouseOver ? BlomeColour_BlackLightTransparent : BlomeColour_AccentColourLight);
        g.fillRoundedRectangle (thumbBounds.reduced(1).toFloat(), 4.0f);
    }
    
    /** Tree Views **/
    void drawTreeviewPlusMinusBox(Graphics& g,
                                  const Rectangle<float>& area,
                                  Colour backgroundColour,
                                  bool isOpen,
                                  bool isMouseOver) override
    {
        Path p;
        p.addTriangle(0.0f, 0.0f, 1.0f, isOpen ? 0.0f : 0.5f, isOpen ? 0.5f : 0.0f, 1.0f);

        g.setColour(isMouseOver ? BlomeColour_BlackLightTransparent : BlomeColour_AccentColourLight);
        g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 3), true));
    }
    
    void drawFileBrowserRow(Graphics& g,
                            int width,
                            int height,
                            const File& file,
                            const String& filename,
                            Image* icon,
                            const String& fileSizeDescription,
                            const String& fileTimeDescription,
                            bool isDirectory,
                            bool isItemSelected,
                            int itemIndex,
                            DirectoryContentsDisplayComponent& directoryDisplayer) override
    {
        auto fileListComponent = dynamic_cast<Component*>(&directoryDisplayer);
        SampleItem* linkedSampleItem = sampleDatabase.getSampleItemWithFilePath(file.getFullPathName());
        
        // Markup if the row is selected
        if(isItemSelected)
        {
            g.setColour(fileListComponent != nullptr ? BlomeColour_BlackLightTransparent : BlomeColour_BlackMediumTransparent);
            g.fillRoundedRectangle(0, 0, width - 5, height, 3.0);
        }

        const int x = 5;
        
        g.setColour(BlomeColour_AccentColourLight);
        g.setFont (font_small_bold);

        if (width > 450 && !isDirectory)
        {
            int propertiesStartX = roundToInt((float)width * 0.8f);
            int propertiesWidth = roundToInt((float)width * 0.1f);

            // Draw file name
            g.drawFittedText(file.getFileNameWithoutExtension(),
                             x,
                             0,
                             propertiesStartX - x,
                             height,
                             Justification::centredLeft,
                             1);

            // Draw file properties
            g.drawFittedText(String(linkedSampleItem->getSampleTag(0)->getName()) + ": " + String(roundToInt(linkedSampleItem->getSampleTag(0)->getValue())) + "s",
                             propertiesStartX,
                             0,
                             propertiesWidth,
                             height,
                             Justification::centredLeft,
                             1);
            
            }
        else
        {
            g.drawFittedText(filename,
                             x,
                             0,
                             width - x,
                             height,
                             Justification::centredLeft,
                             1);

        }
    }
    
private:
    SampleLibrary& sampleDatabase;
    
};
