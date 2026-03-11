/*
  ==============================================================================

    SampleEditorPanel.h
    Created: 10 Dec 2025 3:40:51pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "BlomeStyling.h"

class SampleEditorPanel
:
public PanelBase,
public TextEditor::Listener,
public ComboBox::Listener
{
public:
    SampleEditorPanel(SaemplAudioProcessor & inProcessor,
                      SampleItem * inSampleItem);
    ~SampleEditorPanel();

private:
    SampleItem * sampleItem;
    static int const infoTextWidth = 85;
    std::unique_ptr<TextEditor> mSampleTempoEditor;
    std::unique_ptr<ComboBox> mSampleKeyComboBox;
    std::unique_ptr<TextEditor> mSampleCommentEditor;
    int oldTempo;
    int oldKey;
    String oldComment;
    std::unique_ptr<ToggleButton> mLockPropertiesButton;
    bool sampleIsLocked = false;
    
    void paint(Graphics & g) override;
    void setPanelComponents() override;
    void textEditorReturnKeyPressed(TextEditor & textEditor) override;
    void textEditorEscapeKeyPressed(TextEditor & textEditor) override;
    void textEditorFocusLost(TextEditor&) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
};
