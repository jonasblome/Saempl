/*
  ==============================================================================

    SampleFileRenamingPanel.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleEditorPanel.h"

SampleEditorPanel::SampleEditorPanel(SaemplAudioProcessor& inProcessor,
                                     SampleItem * inSampleItem)
:
PanelBase(inProcessor),
sampleItem(inSampleItem),
oldTempo(inSampleItem->getTempo()),
oldKey(inSampleItem->getKey()),
oldComment(inSampleItem->getComment()),
oldPropertyLock(inSampleItem->getPropertyLock())
{
    setSize(style->SAMPLE_EDITOR_PANEL_WIDTH, style->SAMPLE_EDITOR_PANEL_HEIGHT);
    setPanelComponents();
}

SampleEditorPanel::~SampleEditorPanel()
{
    textEditorFocusLost(*mSampleTempoEditor);
    textEditorFocusLost(*mSampleCommentEditor);
    
    bool tempoChanged = sampleItem->getTempo() != oldTempo;
    bool keyChanged = sampleItem->getKey() != oldKey;
    bool commentChanged = sampleItem->getComment() != oldComment;
    bool propertyLockChanged = sampleItem->getPropertyLock() != oldPropertyLock;
    
    if (sampleItem->getPropertyLock())
    {
        if (tempoChanged)
        {
            sampleItem->addLockedProperty(PROPERTY_NAMES[4]);
        }
        
        if (keyChanged)
        {
            sampleItem->addLockedProperty(PROPERTY_NAMES[5]);
        }
        
        if (commentChanged)
        {
            sampleItem->addLockedProperty(PROPERTY_NAMES[13]);
        }
    }
    else
    {
        sampleItem->clearLockedProperties();
    }
    
    currentProcessor.getSampleLibrary().editSampleItem(sampleItem,
                                                       tempoChanged,
                                                       keyChanged,
                                                       commentChanged,
                                                       propertyLockChanged);
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
    
    g.drawFittedText("Comment:",
                     textBounds.removeFromTop(style->FILTER_RULE_HEIGHT),
                     Justification::centredRight,
                     1);
    
    g.drawFittedText("Lock properties:",
                     textBounds.removeFromTop(style->FILTER_RULE_HEIGHT),
                     Justification::centredRight,
                     1);
}

void SampleEditorPanel::setPanelComponents()
{
    // Add text editor for tempo
    mSampleTempoEditor = std::make_unique<TextEditor>("TempoEditor");
    mSampleTempoEditor->setFont(style->FONT_SMALL_BOLD);
    mSampleTempoEditor->setJustification(Justification::centredLeft);
    mSampleTempoEditor->setIndents(mSampleTempoEditor->getLeftIndent(), 0);
    mSampleTempoEditor->setText(std::to_string(sampleItem->getTempo()));
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
    for (auto const& key : KEY_INDEX_TO_KEY_NAME)
    {
        mSampleKeyComboBox->addItem(key.second, k++);
    }
    mSampleKeyComboBox->setSelectedItemIndex(sampleItem->getKey(), NotificationType::dontSendNotification);
    addAndMakeVisible(*mSampleKeyComboBox);
    
    // Add text editor for comment
    mSampleCommentEditor = std::make_unique<TextEditor>("CommentEditor");
    mSampleCommentEditor->setFont(style->FONT_SMALL_BOLD);
    mSampleCommentEditor->setJustification(Justification::centredLeft);
    mSampleCommentEditor->setIndents(mSampleCommentEditor->getLeftIndent(), 0);
    mSampleCommentEditor->setText(sampleItem->getComment());
    mSampleCommentEditor->setBounds(style->PANEL_MARGIN + infoTextWidth,
                                    style->PANEL_MARGIN * 0.5 + style->FILTER_RULE_HEIGHT * 2,
                                    getWidth() - infoTextWidth - style->PANEL_MARGIN * 1.5,
                                    style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN);
    mSampleCommentEditor->addListener(this);
    addAndMakeVisible(*mSampleCommentEditor);
    
    // Add button to lock sample properties
    mLockPropertiesButton = std::make_unique<ToggleButton>("ActivateRuleButton");
    mLockPropertiesButton->setToggleState(sampleItem->getPropertyLock(), NotificationType::dontSendNotification);
    mLockPropertiesButton->setTooltip("Lock/unlock the changed properties of this sample");
    mLockPropertiesButton->onClick = [this]
    {
        sampleItem->setPropertyLock(mLockPropertiesButton->getToggleState());
    };
    mLockPropertiesButton->setBounds(style->PANEL_MARGIN + infoTextWidth,
                                     style->PANEL_MARGIN * 0.5 + style->FILTER_RULE_HEIGHT * 3,
                                     style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN,
                                     style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN);
    addAndMakeVisible(*mLockPropertiesButton);
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
    textEditor.giveAwayKeyboardFocus();
    
    if (textEditor.getName() == "TempoEditor")
    {
        sampleItem->setTempo(textEditor.getText().getIntValue());
    }
    else if (textEditor.getName() == "CommentEditor")
    {
        sampleItem->setComment(textEditor.getText());
    }
}

void SampleEditorPanel::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    int newKey = comboBoxThatHasChanged->getSelectedId() - 1;
    int oldKey = sampleItem->getKey();
    
    if (newKey != oldKey)
    {
        sampleItem->setKey(newKey);
    }
}
