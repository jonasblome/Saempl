/*
  ==============================================================================

    SampleEditorPanel.h
    Created: 10 Dec 2025 3:40:51pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class SampleEditorPanel
:
public PanelBase,
public TextEditor::Listener
{
public:
    SampleEditorPanel(SaemplAudioProcessor & inProcessor,
                      SampleItem * inSampleItem);
    ~SampleEditorPanel();

private:
    SampleItem * sampleItem;
    static int const infoTextWidth = 75;
    std::unique_ptr<TextEditor> mFileNameEditor;
    
    void paint(Graphics & g) override;
    void setPanelComponents() override;
    void textEditorReturnKeyPressed(TextEditor & textEditor) override;
    void textEditorEscapeKeyPressed(TextEditor & textEditor) override;
    void textEditorFocusLost(TextEditor&) override;
};
