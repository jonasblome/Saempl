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
    setSize(style->SAMPLE_EDITOR_PANEL_WIDTH, style->SAMPLE_EDITOR_PANEL_HEIGHT);
    setPanelComponents();
}

SampleEditorPanel::~SampleEditorPanel()
{
    // Trigger reload/refilter if necessary
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
    Rectangle<int> textBounds = getLocalBounds().reduced(style->PANEL_MARGIN * 0.5).removeFromLeft(infoTextWidth);
    
    g.drawFittedText("Tempo:",
                     textBounds.removeFromTop(style->FILTER_RULE_HEIGHT),
                     Justification::centredRight,
                     1);
    
    g.drawFittedText("Key:",
                     textBounds
                     .removeFromTop(style->FILTER_RULE_HEIGHT),
                     Justification::centredRight,
                     1);
}

void SampleEditorPanel::setPanelComponents()
{
    // Add text editor for compare value
    mSampleTempoEditor = std::make_unique<TextEditor>("TempoEditor");
    mSampleTempoEditor->setFont(style->FONT_SMALL_BOLD);
    mSampleTempoEditor->setJustification(Justification::centredLeft);
    mSampleTempoEditor->setIndents(mSampleTempoEditor->getLeftIndent(), 0);
    mSampleTempoEditor->setText(String::toDecimalStringWithSignificantFigures(sampleItem->getTempo(), 1));
    mSampleTempoEditor->setBounds(style->PANEL_MARGIN + infoTextWidth,
                                  style->PANEL_MARGIN * 0.5,
                                  getWidth() - infoTextWidth - style->PANEL_MARGIN * 1.5,
                                  style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN);
    mSampleTempoEditor->addListener(this);
    addAndMakeVisible(*mSampleTempoEditor);
    
    // Add combo box for key
    mSampleKeyComboBox = std::make_unique<ComboBox>("KeyComboBox");
    mSampleKeyComboBox->setBounds(style->PANEL_MARGIN + infoTextWidth,
                                  style->PANEL_MARGIN * 0.5 + style->FILTER_RULE_HEIGHT,
                                  getWidth() - infoTextWidth - style->PANEL_MARGIN * 1.5,
                                  style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN);
    mSampleKeyComboBox->addListener(this);
    int k = 1;
    for (auto const & key : KEY_INDEX_TO_KEY_NAME)
    {
        mSampleKeyComboBox->addItem(key.second, k++);
    }
    mSampleKeyComboBox->setSelectedItemIndex(sampleItem->getKey());
    addAndMakeVisible(*mSampleKeyComboBox);
}

void SampleEditorPanel::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void SampleEditorPanel::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void SampleEditorPanel::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mSampleTempoEditor->giveAwayKeyboardFocus();
    sampleItem->setTempo(textEditor.getText().getIntValue());
}

void SampleEditorPanel::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    int newKey = comboBoxThatHasChanged->getSelectedId() + 1;
    int oldKey = sampleItem->getKey();
    
    if (newKey != oldKey)
    {
        sampleItem->setKey(newKey);
    }
}
