/*
 ==============================================================================
 
 BlomeLookAndFeel.h
 Created: 22 May 2023 6:53:58am
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"

/**
 Custom look and feel class for the Saempl audio plugin.
 */
class BlomeLookAndFeel
:   public LookAndFeel_V4
{
public:
    BlomeLookAndFeel(SampleLibrary& inSampleDatabase)
    :   sampleLibrary(inSampleDatabase)
    {
        // ComboBox colours
        setColour(ComboBox::backgroundColourId, COLOUR_ACCENT_DARK);
        setColour(ComboBox::outlineColourId, COLOUR_BLACK);
        setColour(ComboBox::arrowColourId, COLOUR_LIGHT_GRAY);
        setColour(ComboBox::textColourId, COLOUR_LIGHT_GRAY);
        setColour(PopupMenu::backgroundColourId, COLOUR_ACCENT_LIGHT_STRONG_TRANSPARENT);
        setColour(AlertWindow::textColourId, COLOUR_ACCENT_DARK);
        
        // Button text colours
        setColour(TextButton::buttonColourId, COLOUR_ACCENT_LIGHT);
        setColour(TextButton::textColourOnId, COLOUR_ACCENT_LIGHT);
        setColour(TextButton::textColourOffId, COLOUR_ACCENT_LIGHT);
        
        // Text editor colours
        setColour(TextEditor::textColourId, COLOUR_ACCENT_LIGHT);
    }
    
    virtual ~BlomeLookAndFeel()
    {
        
    }
    
    Font getTextButtonFont(TextButton&, int buttonHeight) override
    {
        return FONT_SMALL_BOLD;
    }
    
    void drawButtonBackground(Graphics& g,
                              Button& button,
                              Colour const & backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override
    {
        Colour fillColour;
        
        if(shouldDrawButtonAsDown)
        {
            fillColour = COLOUR_ACCENT_DARK.withAlpha(0.9f);
        }
        else if(shouldDrawButtonAsHighlighted)
        {
            fillColour = COLOUR_ACCENT_DARK.withAlpha(0.7f);
        }
        else
        {
            fillColour = COLOUR_ACCENT_DARK;
        }
        
        float const cornerSize = 6.0f;
        Rectangle<float> const bounds = button.getLocalBounds().toFloat();
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
        Rectangle<float> const bounds = button.getLocalBounds().toFloat();
        
        if(shouldDrawButtonAsHighlighted)
        {
            fillColour = COLOUR_ACCENT_DARK.withAlpha(0.7f);
        }
        else
        {
            fillColour = COLOUR_ACCENT_DARK;
        }
        
        if (!button.isEnabled())
        {
            g.setOpacity(0.5f);
        }
        
        g.setColour(fillColour);
        g.fillRoundedRectangle(bounds, cornerSize);
        
        if(button.getToggleState())
        {
            cornerSize = 4.0f;
            g.setColour(COLOUR_ACCENT_LIGHT);
            g.fillRoundedRectangle(bounds.reduced(3), cornerSize);
        }
    }
    
    Font getLabelFont(Label& label) override
    {
        return FONT_SMALL_BOLD;
    }
    
    void drawPopupMenuBackgroundWithOptions(Graphics& g,
                                            int width,
                                            int height,
                                            PopupMenu::Options const & options) override
    {
        g.setColour(findColour(PopupMenu::backgroundColourId));
        g.fillRoundedRectangle(0,
                               2,
                               width,
                               height - 4,
                               CORNER_SIZE_MEDIUM);
    }
    
    void drawPopupMenuItem (Graphics& g,
                            Rectangle<int> const & area,
                            bool isSeparator,
                            bool isActive,
                            bool isHighlighted,
                            bool isTicked,
                            bool hasSubMenu,
                            String const & text,
                            String const & shortcutKeyText,
                            Drawable const * icon,
                            Colour const * textColour) override
    {
        Rectangle<int> r(area);
        
        if (isHighlighted || isTicked)
        {
            g.setColour(isHighlighted ? COLOUR_BLACK_LIGHT_TRANSPARENT : COLOUR_ACCENT_DARK);
            g.fillRoundedRectangle(r.reduced(1).toFloat(), CORNER_SIZE_MEDIUM);
        }
        
        Colour fontColour = isTicked ? COLOUR_ACCENT_LIGHT : COLOUR_ACCENT_DARK;
        g.setColour(fontColour);
        g.setFont(FONT_SMALL_BOLD);
        
        r.setLeft(10);
        r.setY(1);
        g.drawFittedText(text, r, Justification::left, 1);
    }
    
    void drawComboBox (Graphics& g,
                       int width,
                       int height,
                       bool isButtonDown,
                       int buttonX,
                       int buttonY,
                       int buttonW,
                       int buttonH,
                       ComboBox& box) override
    {
        Rectangle<int> const boxBounds (0, 0, width, height);
        
        g.setColour(COLOUR_ACCENT_DARK);
        g.fillRoundedRectangle(boxBounds.toFloat(), CORNER_SIZE_MEDIUM);
        
        Rectangle<int> arrow (width - 30, 0, 20, height);
        
        Path path;
        
        path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
        path.lineTo(arrow.toFloat().getCentreX(), arrow.toFloat().getCentreY() + 3.0f);
        path.lineTo(arrow.toFloat().getRight() - 3.0f, arrow.toFloat().getCentreY() - 2.0f);
        
        Colour arrowColour = COLOUR_ACCENT_LIGHT.withAlpha(box.isEnabled() ? 0.9f : 0.2f);
        
        g.setColour(arrowColour);
        g.strokePath(path, PathStrokeType(2.0f));
    }
    
    void positionComboBoxText(ComboBox& box, Label& label) override
    {
        label.setBounds(6, 1, box.getWidth() - 30, box.getHeight() - 2);
        label.setFont(FONT_SMALL_BOLD);
        label.setColour(Label::ColourIds::textColourId, COLOUR_ACCENT_LIGHT);
    }
    
    void drawComboBoxTextWhenNothingSelected(Graphics& g,
                                             ComboBox& box,
                                             Label& label) override
    {
        g.setColour(COLOUR_ACCENT_LIGHT.withAlpha(0.5f));
        g.setFont(FONT_SMALL_BOLD);
        
        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds()).removeFromRight(label.getWidth() - 15);
        
        g.drawFittedText(box.getTextWhenNothingSelected(),
                         textArea,
                         label.getJustificationType(),
                         jmax(1, (int) ((float) textArea.getHeight() / g.getCurrentFont().getHeight())),
                         label.getMinimumHorizontalScale());
    }
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider& slider) override
    {
        g.setColour(COLOUR_ACCENT_DARK);
        g.fillEllipse(x + 6, y + 6, width - 14, height - 14);
        g.setColour(COLOUR_ACCENT_LIGHT);
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
                           Slider::SliderStyle const style,
                           Slider& slider) override
    {
        if (slider.isBar())
        {
            g.setColour(COLOUR_ACCENT_DARK);
            g.fillRect(slider.isHorizontal() ? Rectangle<float>(static_cast<float> (x), (float)y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                       : Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)));
        }
        else
        {
            auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
            auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);
            
            auto trackWidth = jmin(6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);
            
            Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                     slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));
            
            Point<float> endPoint (slider.isHorizontal() ? (float) width + x : startPoint.x,
                                   slider.isHorizontal() ? startPoint.y : (float) y);
            
            Path backgroundTrack;
            backgroundTrack.startNewSubPath(startPoint);
            backgroundTrack.lineTo(endPoint);
            g.setColour(COLOUR_ACCENT_DARK);
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
            g.setColour(COLOUR_ACCENT_LIGHT);
            g.strokePath(valueTrack, { trackWidth - 4, PathStrokeType::curved, PathStrokeType::rounded });
            
            if (! isTwoVal)
            {
                g.setColour(COLOUR_ACCENT_LIGHT);
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
        return jmin(10, slider.isHorizontal() ? static_cast<int>((float) slider.getHeight() * 0.5f)
                    : static_cast<int>((float) slider.getWidth() * 0.5f));
    }
    
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
        
        g.setColour (isMouseOver ? COLOUR_ACCENT_LIGHT.withAlpha(0.7f) : COLOUR_ACCENT_LIGHT);
        g.fillRoundedRectangle (thumbBounds.reduced(1).toFloat(), 4.0f);
    }
    
    void drawTreeviewPlusMinusBox(Graphics& g,
                                  Rectangle<float> const & area,
                                  Colour backgroundColour,
                                  bool isOpen,
                                  bool isMouseOver) override
    {
        Path p;
        p.addTriangle(0.0f, 0.0f, 1.0f, isOpen ? 0.0f : 0.5f, isOpen ? 0.5f : 0.0f, 1.0f);
        
        g.setColour(isMouseOver ? COLOUR_ACCENT_LIGHT.withAlpha(0.7f) : COLOUR_ACCENT_LIGHT);
        g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 3), true));
    }
    
    void drawFileBrowserRow(Graphics& g,
                            int width,
                            int height,
                            File const & file,
                            String const & filename,
                            Image* icon,
                            String const & fileSizeDescription,
                            String const & fileTimeDescription,
                            bool isDirectory,
                            bool isItemSelected,
                            int itemIndex,
                            DirectoryContentsDisplayComponent& directoryDisplayer) override
    {
        auto fileListComponent = dynamic_cast<Component*>(&directoryDisplayer);
        
        // Markup if the row is selected
        if (isItemSelected)
        {
            g.setColour(fileListComponent != nullptr ? COLOUR_ACCENT_DARK : COLOUR_ACCENT_DARK);
            g.fillRoundedRectangle(0, 0, width - 5, height, 3.0);
        }
        
        int const x = 5;
        
        g.setColour(COLOUR_ACCENT_LIGHT);
        g.setFont(FONT_SMALL_BOLD);
        
        
        g.drawFittedText(filename,
                         x,
                         0,
                         width - x,
                         height,
                         Justification::centredLeft,
                         1);
    }
    
    void drawTableHeaderBackground (Graphics& g, TableHeaderComponent& header) override
    {
        Rectangle<int> r = header.getLocalBounds();
        g.setColour(COLOUR_ACCENT_LIGHT);
        g.fillRoundedRectangle(r.toFloat(), CORNER_SIZE_MEDIUM);
        g.setColour(COLOUR_ACCENT_DARK);
        
        for (int i = header.getNumColumns(true); --i >= 0;)
        {
            g.fillRect(header.getColumnPosition(i).removeFromRight(1));
        }
    }
    
    void drawTableHeaderColumn(Graphics& g,
                               TableHeaderComponent& header,
                               String const & columnName,
                               int columnId,
                               int width,
                               int height,
                               bool isMouseOver,
                               bool isMouseDown,
                               int columnFlags) override
    {
        auto highlightColour = COLOUR_ACCENT_MEDIUM.withAlpha(0.7f);
        
        if (isMouseDown)
        {
            g.fillAll(highlightColour);
        }
        else if (isMouseOver)
        {
            g.fillAll(highlightColour.withMultipliedAlpha (0.625f));
        }
        
        Rectangle<int> area (width, height);
        area.reduce (4, 0);
        
        if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0)
        {
            Path sortArrow;
            sortArrow.addTriangle (0.0f, 0.0f,
                                   0.5f, (columnFlags & TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
                                   1.0f, 0.0f);
            
            g.setColour(COLOUR_ACCENT_DARK);
            g.fillPath (sortArrow, sortArrow.getTransformToScaleToFit (area.removeFromRight (height / 2).reduced (2).toFloat(), true));
        }
        
        g.setColour(COLOUR_ACCENT_DARK);
        g.setFont(FONT_MEDIUM_SMALL_BOLD);
        g.drawFittedText(columnName, area, Justification::centredLeft, 1);
    }
    
    void drawCallOutBoxBackground(CallOutBox& box,
                                  Graphics& g,
                                  Path const & path,
                                  Image& cachedImage) override
    {
        if (cachedImage.isNull())
        {
            cachedImage = { Image::ARGB, box.getWidth(), box.getHeight(), true };
            Graphics g2(cachedImage);
            
            DropShadow(COLOUR_BLACK_MEDIUM_TRANSPARENT, 8, { 2, 2 }).drawForPath(g2, path);
        }
        
        g.setColour(COLOUR_BLACK);
        g.drawImageAt(cachedImage, 0, 0);
        
        g.setColour(COLOUR_ACCENT_DARK.withAlpha(0.8f));
        g.fillPath(path);
        
        // g.setColour(COLOUR_ACCENT_LIGHT.withAlpha(0.8f));
        // g.strokePath(path, PathStrokeType(2.0));
    }
    
    int getCallOutBoxBorderSize(CallOutBox const &) override
    {
        return 20;
    }
    
    float getCallOutBoxCornerSize(CallOutBox const &) override
    {
        return CORNER_SIZE_MEDIUM;
    }
    
    void drawAlertBox(Graphics& g,
                      AlertWindow& alert,
                      Rectangle<int> const & textArea,
                      TextLayout& textLayout) override
    {
        // Draw alert box background
        auto cornerSize = CORNER_SIZE_MEDIUM;
        g.setColour(COLOUR_ACCENT_LIGHT_STRONG_TRANSPARENT);
        g.fillRoundedRectangle(alert.getLocalBounds().toFloat(), cornerSize);
        
        // Draw alert text
        Rectangle<int> alertBounds(alert.getLocalBounds().getX(),
                                   30,
                                   alert.getLocalBounds().getWidth(),
                                   alert.getLocalBounds().getHeight() - getAlertWindowButtonHeight() - 20);
        textLayout.draw(g, alertBounds.toFloat());
    }
    
    Font getAlertWindowTitleFont() override
    {
        return FONT_MEDIUM_SMALL_BOLD;
    }
    
    Font getAlertWindowMessageFont() override
    {
        return FONT_MEDIUM_SMALL_BOLD;
    }
    
    Font getAlertWindowFont() override
    {
        return FONT_MEDIUM_SMALL_BOLD;
    }
    
    void drawTextEditorOutline(Graphics& g,
                               int width,
                               int height,
                               TextEditor& textEditor) override
    {
        if (dynamic_cast<AlertWindow*>(textEditor.getParentComponent()) == nullptr)
        {
            if (textEditor.isEnabled())
            {
                if (textEditor.hasKeyboardFocus(true) && !textEditor.isReadOnly())
                {
                    g.setColour(COLOUR_ACCENT_DARK);
                    g.drawRoundedRectangle(1,
                                           1,
                                           width - 2,
                                           height - 2,
                                           CORNER_SIZE_MEDIUM - 1,
                                           2.0);
                }
            }
        }
    }
    
    void fillTextEditorBackground(Graphics& g,
                                  int width,
                                  int height,
                                  TextEditor& textEditor) override
    {
        g.setColour(COLOUR_ACCENT_DARK);
        g.fillRoundedRectangle(0, 0, width, height, CORNER_SIZE_MEDIUM);
    }
    
    void drawImageButton(Graphics& g,
                         Image* image,
                         int imageX,
                         int imageY,
                         int imageW,
                         int imageH,
                         const Colour& overlayColour,
                         float imageOpacity,
                         ImageButton& button) override
    {
        if (!button.isEnabled())
        {
            imageOpacity *= 0.3f;
        }
        
        AffineTransform transfromImageToButtonBounds =
        RectanglePlacement(RectanglePlacement::stretchToFit)
            .getTransformToFit(image->getBounds().toFloat(),
                               Rectangle<float>(std::nearbyint(jmin(image->getWidth(),
                                                                    button.getWidth() - PANEL_MARGIN) * 0.5f) * 2.0f - 1,
                                                std::nearbyint(jmin(image->getHeight(),
                                                                    button.getHeight() - PANEL_MARGIN) * 0.5f) * 2.0f - 1)
                               .withCentre(button.getLocalBounds().getCentre().toFloat()).toFloat());
        
        g.setColour(overlayColour.withAlpha(imageOpacity));
        g.drawImageTransformed(*image, transfromImageToButtonBounds, true);
    }
    
    void drawTooltip(Graphics& g,
                     String const & text,
                     int width,
                     int height) override
    {
        Rectangle<int> bounds (width, height);
        auto cornerSize = 5.0f;
        
        // Draw border
        g.setColour(COLOUR_ACCENT_MEDIUM);
        g.fillRoundedRectangle(bounds.toFloat(), cornerSize);
        
        // Draw background
        g.setColour(COLOUR_ACCENT_DARK);
        g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 1.0f);
        
        // Draw text
        const int maxToolTipWidth = 400;
        
        AttributedString tooltipText;
        tooltipText.setJustification(Justification::centred);
        tooltipText.append(text, FONT_SMALL_BOLD, COLOUR_ACCENT_LIGHT);
        
        TextLayout layout;
        layout.createLayoutWithBalancedLineLengths(tooltipText, (float) maxToolTipWidth);
        layout.draw(g, { static_cast<float>(width), static_cast<float>(height) });
    }
    
    void drawProgressBar(Graphics& g,
                         ProgressBar& progressBar,
                         int width,
                         int height,
                         double progress,
                         String const & textToShow) override
    {
        Colour background = COLOUR_ACCENT_DARK;
        Colour foreground = COLOUR_ACCENT_MEDIUM;
        
        Rectangle<float> barBounds = progressBar.getLocalBounds().toFloat();
        
        g.setColour(background);
        g.fillRoundedRectangle(barBounds, (float) progressBar.getHeight() * 0.5f);
        
        if (progress >= 0.0f && progress <= 1.0f)
        {
            Path p;
            p.addRoundedRectangle(barBounds, (float) progressBar.getHeight() * 0.5f);
            g.reduceClipRegion(p);
            
            barBounds.setWidth(barBounds.getWidth() * (float) progress);
            g.setColour(foreground);
            g.fillRoundedRectangle(barBounds, (float) progressBar.getHeight() * 0.5f);
        }
        else
        {
            // Spinning bar..
            g.setColour(background);
            
            auto stripeWidth = height * 2;
            auto position = static_cast<int> (Time::getMillisecondCounter() / 15) % stripeWidth;
            
            Path p;
            
            for (auto x = static_cast<float> (-position); x < (float) (width + stripeWidth); x += (float) stripeWidth)
                p.addQuadrilateral(x, 0.0f,
                                   x + (float) stripeWidth * 0.5f, 0.0f,
                                   x, static_cast<float> (height),
                                   x - (float) stripeWidth * 0.5f, static_cast<float> (height));
            
            Image im(Image::ARGB, width, height, true);
            
            {
                Graphics g2(im);
                g2.setColour(foreground);
                g2.fillRoundedRectangle(barBounds, (float) progressBar.getHeight() * 0.5f);
            }
            
            g.setTiledImageFill(im, 0, 0, 0.85f);
            g.fillPath(p);
        }
        
        if (textToShow.isNotEmpty())
        {
            g.setColour(COLOUR_ACCENT_LIGHT);
            g.setFont(FONT_SMALL_BOLD);
            
            g.drawText(textToShow, 0, 0, width, height, Justification::centred, false);
        }
    }
    
private:
    SampleLibrary& sampleLibrary;
};
