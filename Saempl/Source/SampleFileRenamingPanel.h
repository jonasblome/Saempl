/*
  ==============================================================================

    SampleFileRenamingPanel.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class SampleFileRenamingPanel
:
public PanelBase,
public TextEditor::Listener
{
public:
    /**
     The constructor for the sample editor panel.
     
     @param inProcessor the processor object of the current plugin instance.
     @param inFilePath the file path of the sample.
     */
    SampleFileRenamingPanel(SaemplAudioProcessor& inProcessor,
                            String inFilePath);
    ~SampleFileRenamingPanel();

private:
    String originalFilePath;
    String newFilePath;
    static int const infoTextWidth = 75;
    std::unique_ptr<TextEditor> mFileNameEditor;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    void textEditorReturnKeyPressed(TextEditor& textEditor) override;
    void textEditorEscapeKeyPressed(TextEditor& textEditor) override;
    void textEditorFocusLost(TextEditor&) override;
    void setNewFileName();
};
