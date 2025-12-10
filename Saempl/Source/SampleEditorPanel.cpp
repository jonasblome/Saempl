/*
  ==============================================================================

    SampleFileRenamingPanel.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleEditorPanel.h"

SampleEditorPanel::SampleEditorPanel(SaemplAudioProcessor & inProcessor,
                                     SampleItem * inSampleItem)
:
PanelBase(inProcessor),
sampleItem(inSampleItem)
{
    setSize(style->RENAMING_PANEL_WIDTH, style->RENAMING_PANEL_HEIGHT);
    setPanelComponents();
}

SampleEditorPanel::~SampleEditorPanel()
{
    
}

void SampleEditorPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.fillRoundedRectangle(getLocalBounds().toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    
    // Draw info text
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.setFont(style->FONT_SMALL_BOLD);
    g.drawFittedText("New file name:",
                     getLocalBounds()
                     .reduced(style->PANEL_MARGIN * 0.5)
                     .removeFromLeft(infoTextWidth),
                     Justification::centredRight,
                     1);
}

void SampleEditorPanel::setPanelComponents()
{
    // Add text editor for compare value
    mFileNameEditor = std::make_unique<TextEditor>("FilePathEditor");
    mFileNameEditor->setFont(style->FONT_SMALL_BOLD);
    mFileNameEditor->setJustification(Justification::centredLeft);
    mFileNameEditor->setIndents(mFileNameEditor->getLeftIndent(), 0);
    mFileNameEditor->setText("TEST");
    mFileNameEditor->setBounds(style->PANEL_MARGIN + infoTextWidth,
                               style->PANEL_MARGIN * 0.5,
                               getWidth() - infoTextWidth - style->PANEL_MARGIN * 1.5,
                               getHeight() - style->PANEL_MARGIN);
    mFileNameEditor->addListener(this);
    addAndMakeVisible(*mFileNameEditor);
}

void SampleEditorPanel::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void SampleEditorPanel::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    mFileNameEditor->setText("RESET");
    mFileNameEditor->giveAwayKeyboardFocus();
}

void SampleEditorPanel::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mFileNameEditor->giveAwayKeyboardFocus();
}
