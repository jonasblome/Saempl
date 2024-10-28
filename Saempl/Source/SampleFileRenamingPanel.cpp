/*
  ==============================================================================

    SampleFileRenamingPanel.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileRenamingPanel.h"

SampleFileRenamingPanel::SampleFileRenamingPanel(SaemplAudioProcessor & inProcessor,
                                                 String inFilePath)
:
PanelBase(inProcessor),
originalFilePath(inFilePath),
newFilePath(inFilePath)
{
    setSize(style->RENAMING_PANEL_WIDTH, style->RENAMING_PANEL_HEIGHT);
    setPanelComponents();
}

SampleFileRenamingPanel::~SampleFileRenamingPanel()
{
    setNewFileName();
    
    if (originalFilePath == newFilePath)
    {
        return;
    }
    
    if (std::rename(originalFilePath.toUTF8(), newFilePath.toUTF8()))
    {
        AlertWindow::showAsync(MessageBoxOptions()
                               .withIconType(MessageBoxIconType::NoIcon)
                               .withTitle("Renaming not successful!")
                               .withMessage("Maybe this file was deleted or is currently in use. Please try again.")
                               .withButton("OK"),
                               nullptr);
    }
    else
    {
        currentProcessor.getSampleLibrary().renameSampleItem(originalFilePath, newFilePath);
    }
}

void SampleFileRenamingPanel::paint(Graphics& g)
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

void SampleFileRenamingPanel::setPanelComponents()
{
    // Add text editor for compare value
    mFileNameEditor = std::make_unique<TextEditor>("FilePathEditor");
    mFileNameEditor->setFont(style->FONT_SMALL_BOLD);
    mFileNameEditor->setJustification(Justification::centredLeft);
    mFileNameEditor->setIndents(mFileNameEditor->getLeftIndent(), 0);
    mFileNameEditor->setText(File(originalFilePath).getFileNameWithoutExtension());
    mFileNameEditor->setBounds(style->PANEL_MARGIN + infoTextWidth,
                               style->PANEL_MARGIN * 0.5,
                               getWidth() - infoTextWidth - style->PANEL_MARGIN * 1.5,
                               getHeight() - style->PANEL_MARGIN);
    mFileNameEditor->addListener(this);
    addAndMakeVisible(*mFileNameEditor);
}

void SampleFileRenamingPanel::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    textEditorFocusLost(textEditor);
}

void SampleFileRenamingPanel::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    mFileNameEditor->setText(File(originalFilePath).getFileNameWithoutExtension());
    mFileNameEditor->giveAwayKeyboardFocus();
}

void SampleFileRenamingPanel::setNewFileName()
{
    File file = File(originalFilePath);
    String text = mFileNameEditor->getText();
    
    if (text == "")
    {
        mFileNameEditor->setText(File(originalFilePath).getFileNameWithoutExtension());
        AlertWindow::showAsync(MessageBoxOptions()
                               .withIconType(MessageBoxIconType::NoIcon)
                               .withTitle("Renaming not successful!")
                               .withMessage("You cannot rename a file to an empty file name.")
                               .withButton("OK"),
                               nullptr);
        return;
    }
    else
    {
        newFilePath = file.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + text + file.getFileExtension();
    }
}

void SampleFileRenamingPanel::textEditorFocusLost(TextEditor& textEditor)
{
    // Lose focus, set compare value and refresh library
    mFileNameEditor->giveAwayKeyboardFocus();
    setNewFileName();
}
