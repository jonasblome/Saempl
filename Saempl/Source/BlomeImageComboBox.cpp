/*
  ==============================================================================

    BlomeImageComboBox.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeImageComboBox.h"

BlomeImageComboBox::BlomeImageComboBox(String const & name, Image const & inImage)
:
ComboBox(name)
{
    comboBoxImage = inImage;
}

BlomeImageComboBox::~BlomeImageComboBox()
{
    
}

void BlomeImageComboBox::paint(Graphics& g)
{
    Rectangle<int> imageBounds = getBounds();
    
    if (comboBoxImage.isValid())
    {
        const int iw = comboBoxImage.getWidth();
        const int ih = comboBoxImage.getHeight();
        int w = getWidth();
        int h = getHeight();
        int x = (w - iw) / 2;
        int y = (h - ih) / 2;

        int newW, newH;
        const float imRatio = (float) ih / (float) iw;
        const float destRatio = (float) h / (float) w;

        if (imRatio > destRatio)
        {
            newW = roundToInt ((float) h / imRatio);
            newH = h;
        }
        else
        {
            newW = w;
            newH = roundToInt ((float) w * imRatio);
        }

        x = (w - newW) / 2;
        y = (h - newH) / 2;
        w = newW;
        h = newH;

        imageBounds.setBounds(x, y, w, h);
        
        float imageOpacity = isMouseOver() ? style->BUTTON_IS_OVER_ALPHA : 1.0;
        
        AffineTransform transfromImageToBoxBounds =
        RectanglePlacement(RectanglePlacement::stretchToFit)
            .getTransformToFit(comboBoxImage.getBounds().toFloat(),
                               Rectangle<float>(std::nearbyint(jmin(imageBounds.getWidth(),
                                                                    getWidth() - style->PANEL_MARGIN)
                                                               * 0.5f)
                                                * 2.0f - 1,
                                                std::nearbyint(jmin(imageBounds.getHeight(),
                                                                    getHeight() - style->PANEL_MARGIN)
                                                               * 0.5f)
                                                * 2.0f - 1)
                               .withCentre(getLocalBounds().getCentre().toFloat()).toFloat());
        g.setColour(style->COLOUR_ACCENT_DARK.withAlpha(imageOpacity));
        g.drawImageTransformed(comboBoxImage, transfromImageToBoxBounds, true);
    }
}
