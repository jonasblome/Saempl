/*
 ==============================================================================
 
 BlomeSampleTileView.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeSampleTileView.h"

BlomeSampleTileView::BlomeSampleTileView(SampleItem* inSampleItem,
                                                 SampleLibrary& inSampleLibrary,
                                                 SampleItemPanel& inSampleItemPanel,
                                                 AudioPlayer& inAudioPlayer)
:
sampleItem(inSampleItem),
sampleLibrary(inSampleLibrary),
sampleItemPanel(inSampleItemPanel),
audioPlayer(inAudioPlayer)
{
    sampleItemFilePath = sampleItem->getCurrentFilePath();
    isSelected = false;
    
    if (sampleItemFilePath == EMPTY_TILE_PATH)
    {
        return;
    }
    
    // Add play button component
    setPlayButton();
}

BlomeSampleTileView::~BlomeSampleTileView()
{
    
}

void BlomeSampleTileView::setPlayButton()
{
    mStartStopButton = std::make_unique<BlomeImageButton>("Play/Stop", false);
    mStartStopButton->setImages(false,
                                true,
                                true,
                                ImageCache::getFromMemory(BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_png,
                                                          BinaryData::play_pause_FILL0_wght400_GRAD0_opsz24_pngSize),
                                style->BUTTON_IS_DEFAULT_ALPHA,
                                style->COLOUR_HEADER_BUTTONS,
                                Image(),
                                style->BUTTON_IS_OVER_ALPHA,
                                style->COLOUR_HEADER_BUTTONS,
                                Image(),
                                style->BUTTON_IS_DOWN_ALPHA,
                                style->COLOUR_HEADER_BUTTONS);
    mStartStopButton->setTooltip("Start (K) or stop (L) playback of the selected sample");
    mStartStopButton->setWantsKeyboardFocus(false);
    mStartStopButton->onClick = [this]
    {
        startPlayback();
    };
    addAndMakeVisible(*mStartStopButton);
}

void BlomeSampleTileView::startPlayback()
{
    if (sampleItemFilePath == EMPTY_TILE_PATH)
    {
        return;
    }
    
    File sampleFile = File(sampleItemFilePath);
    
    // Load file into source
    if (sampleFile.exists() && !sampleFile.isDirectory() && isSupportedAudioFileFormat(sampleFile.getFileExtension()))
    {
        audioPlayer.loadURLIntoTransport(URL(sampleFile));
        audioPlayer.setTransportSourcePosition(0.0);
        audioPlayer.start();
    }
    
    if (!sampleFile.exists())
    {
        audioPlayer.emptyTransport();
        sampleLibrary.refreshLibrary();
        AlertWindow::showAsync(MessageBoxOptions()
                               .withIconType(MessageBoxIconType::NoIcon)
                               .withTitle("File not available!")
                               .withMessage("This file has probably been externally deleted and was removed from the list of available samples.")
                               .withButton("OK"),
                               nullptr);
    }
}

String BlomeSampleTileView::getSampleItemFilePath()
{
    if (sampleItemFilePath == EMPTY_TILE_PATH)
    {
        return sampleItemFilePath;
    }
    else
    {
        return sampleItem->getCurrentFilePath();
    }
}

void BlomeSampleTileView::setSelected(bool inIsSelected)
{
    isSelected = inIsSelected;
}

bool BlomeSampleTileView::getIsSelected()
{
    return isSelected;
}

void BlomeSampleTileView::loadIntoAudioPlayer()
{
    if (sampleItemFilePath != EMPTY_TILE_PATH)
    {
        File inFile = sampleItem->getCurrentFilePath();
        
        sampleItemPanel.tryShowAudioResource(inFile);
    }
}

void BlomeSampleTileView::paint(Graphics& g)
{
    // Don't draw an empty tile
    if (sampleItemFilePath == EMPTY_TILE_PATH)
    {
        return;
    }
    
    Rectangle<float> bounds = getLocalBounds().toFloat().reduced(0.5);
    
    // Draw background
    if (isSelected)
    {
        g.setColour(style->COLOUR_ACCENT_LIGHT);
        g.fillRoundedRectangle(bounds, style->CORNER_SIZE_MEDIUM);
    }
    else
    {
        g.setColour(style->COLOUR_ACCENT_DARK);
        g.drawRoundedRectangle(bounds, style->CORNER_SIZE_MEDIUM, 1.0);
    }
    
    // Draw sample text
    String title = sampleItem->getTitle();
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_DARK);
    
    int maxTitleLength = 25;
    if (title.length() > maxTitleLength)
    {
        title = title.substring(0, maxTitleLength - 3) + "...";
    }
    
    g.drawFittedText(title,
                     bounds.reduced(style->PANEL_MARGIN).toNearestInt(),
                     Justification::topLeft,
                     5);
    
    // Make text dependent on tile size
    String other = "";
    int currentWidth = getWidth();
    
    if (currentWidth > 90)
    {
        other = other
        + "\n\n"
        + " - Key: "
        + KEY_INDEX_TO_KEY_NAME[sampleItem->getKey()];
        other = other
        + "\n"
        + " - Tempo: "
        + std::to_string(sampleItem->getTempo())
        + "bpm";
        other = other
        + "\n"
        + " - Dyn. Range: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getDynamicRange(), 2)
        + "LUFS";
        other = other
        + "\n"
        + " - Avg. Freq: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getSpectralCentroid(), 2)
        + "Hz";
        other = other
        + "\n"
        + " - Rolloff: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getSpectralRolloff(), 2)
        + "%";
        other = other
        + "\n"
        + " - Freq. Spread: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getSpectralSpread(), 2)
        + "%";
    }
    if (currentWidth > 150)
    {
        other = other
        + "\n"
        + " - Freq. Flux: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getSpectralFlux(), 2)
        + "%";
        other = other
        + "\n"
        + " - Harm. Flux: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getChromaFlux(), 2)
        + "%";
        other = other
        + "\n"
        + " - Length: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getLength(), 2)
        + "s";
        other = other
        + "\n"
        + " - Loudness: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getLoudnessLUFS(), 2)
        + "LUFS";
        other = other
        + "\n"
        + " - ZCR: "
        + String::toDecimalStringWithSignificantFigures(sampleItem->getZeroCrossingRate(), 2)
        + "Hz";
    }
    
    g.drawFittedText(other,
                     bounds.reduced(style->PANEL_MARGIN).removeFromBottom(getHeight() - 25).removeFromTop(getHeight() - 25 - style->BUTTON_SIZE_MEDIUM - style->PANEL_MARGIN).toNearestInt(),
                     Justification::topLeft,
                     5);
}

void BlomeSampleTileView::mouseDoubleClick(MouseEvent const & event)
{
    loadIntoAudioPlayer();
}

void BlomeSampleTileView::resized()
{
    if (mStartStopButton != nullptr)
    {
        Rectangle<float> buttonBounds = getLocalBounds().toFloat().reduced(style->PANEL_MARGIN * 0.5);
        buttonBounds = buttonBounds.removeFromBottom(style->BUTTON_SIZE_MEDIUM).removeFromRight(style->BUTTON_SIZE_MEDIUM);
        mStartStopButton->setBounds(buttonBounds.toNearestInt());
    }
}
