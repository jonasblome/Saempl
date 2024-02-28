/*
  ==============================================================================

    AboutPanel.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "AboutPanel.h"

AboutPanel::AboutPanel()
{
    setSize(style->ABOUT_PANEL_WIDTH, style->ABOUT_PANEL_HEIGHT);
}

AboutPanel::~AboutPanel()
{
    
}

void AboutPanel::mouseEnter(MouseEvent const & event)
{
    repaint();
}

void AboutPanel::mouseExit(MouseEvent const & event)
{
    repaint();
}

void AboutPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    
    // Draw line
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawLine(style->PANEL_MARGIN,
               40,
               getWidth() - style->PANEL_MARGIN,
               40);
    
    // Draw text
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.setFont(style->FONT_MEDIUM_BOLD);
    Rectangle<int> textBounds = getLocalBounds().reduced(style->PANEL_MARGIN);
    g.drawFittedText("Saempl - Sample Manager", textBounds.removeFromTop(30), Justification::topLeft, 1);
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.setFont(style->FONT_SMALL_BOLD);
    g.drawFittedText(String("\n")
                     + "Developed by Jonas Blome"
                     + String("\n") + "Copyleft Saempl 2024"
                     + String("\n") + "Email: jonas.blome@gmx.de"
                     + String("\n") + "GitHub: https://github.com/jonasblome"
                     + String("\n")
                     + String("\n") + "Short Intructions:"
                     + String("\n") + " - Hover over buttons and sliders to get explanations"
                     + String("\n") + " - Press Enter to load selected samples into the preview"
                     + String("\n") + " - Drag and drop samples into Favourites or into your DAW",
                     textBounds.removeFromBottom(getHeight() - 60),
                     Justification::left, 10);
}
