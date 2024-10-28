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

void AboutPanel::paint(Graphics & g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    
    // Draw title bar
    int titleHeight = 40;
    String titleText = "Saempl - Sample Manager";
    g.setFont(style->FONT_MEDIUM_BOLD);
    int titleWidth = TextLayout().getStringWidth(style->FONT_MEDIUM_BOLD, titleText);
    drawDropShadow(g,
                   Rectangle<int>((getWidth() - titleWidth) / 2,
                                  style->PANEL_MARGIN * 0.5,
                                  titleWidth,
                                  titleHeight),
                   0,
                   titleHeight / 3.0,
                   titleHeight * 2,
                   style);
    Rectangle<int> titleBounds = getLocalBounds().reduced(style->PANEL_MARGIN * 0.5).removeFromTop(titleHeight);
//    g.setColour(style->COLOUR_ACCENT_LIGHT);
//    g.fillRoundedRectangle(titleBounds.toFloat(), style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_BOLD);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawFittedText(titleText, titleBounds.reduced(style->PANEL_MARGIN * 0.5), Justification::centred, 1);
    
    // Draw line
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawLine(style->PANEL_MARGIN,
               style->PANEL_MARGIN + titleHeight,
               getWidth() - style->PANEL_MARGIN,
               style->PANEL_MARGIN + titleHeight);
    
    // Draw info text
    Rectangle<int> infoTextBounds = getLocalBounds()
        .reduced(style->PANEL_MARGIN * 0.5)
        .removeFromBottom(getHeight() - titleHeight - style->PANEL_MARGIN * 2);
    drawDropShadow(g,
                   getLocalBounds().removeFromBottom(getHeight() - titleHeight - style->PANEL_MARGIN * 2),
                   0,
                   10,
                   15,
                   style);
    g.setColour(style->COLOUR_ACCENT_DARK_MEDIUM_STRONG_TRANSPARENT);
    g.fillRoundedRectangle(infoTextBounds.toFloat(), style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.drawFittedText("Developed by Jonas Blome"
                     + String("\n") + "Copyleft @Saempl 2024"
                     + String("\n") + "Email: jonas.blome@gmx.de"
                     + String("\n") + "GitHub: https://github.com/jonasblome"
                     + String("\n")
                     + String("\n") + "Short Intructions:"
                     + String("\n") + " - Choose a directory at the top left or press (D)"
                     + String("\n") + " - Hover over buttons and sliders to get explanations"
                     + String("\n") + " - Press (K) to start and (L) to stop quick pre-listen of a selected sample"
                     + String("\n") + "OR"
                     + String("\n") + " - Press Enter to load selected samples into the preview"
                     + String("\n") + " - Right click on samples to access a popup menu"
                     + String("\n") + " - Drag and drop samples into \"Favourites\" or into your DAW"
                     + String("\n") + " - Select a random sample with the shuffle button at the top or press (S)"
                     + String("\n") + " - Switch between folder, table and grid view with (1), (2), (3)"
                     + String("\n") + " - Filter through your samples by adding rules in the filter menu at the top",
                     infoTextBounds.reduced(style->PANEL_MARGIN * 1.0),
                     Justification::topLeft, 10);
}
