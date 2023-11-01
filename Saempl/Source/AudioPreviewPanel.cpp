/*
  ==============================================================================

    AudioPreviewPanel.cpp
    Created: 27 May 2023 4:20:03pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "AudioPreviewPanel.h"
#include "BlomeFileTreeView.h"

AudioPreviewPanel::AudioPreviewPanel(SaemplAudioProcessor& inProcessor, Slider& inSlider, SampleEditor& inSampleEditor)
:   PanelBase(),
    currentProcessor(inProcessor),
    sampleEditor(inSampleEditor),
    mZoomSlider(&inSlider),
    mThumbnailCache(5),
    mAudioPreview(512, sampleEditor.getAudioFormatManager(), mThumbnailCache),
    isFollowingTransport(false)
{
    setSize(SAMPLE_PREVIEW_WIDTH - PANEL_MARGIN * 1.5, SAMPLE_PREVIEW_HEIGHT - PANEL_MARGIN * 1.5);
    setPanelComponents();
}

AudioPreviewPanel::~AudioPreviewPanel()
{
    mAudioPreviewScrollbar->removeListener(this);
}

void AudioPreviewPanel::paint(Graphics& g)
{
    auto previewArea = getLocalBounds().removeFromBottom(getHeight() - SAMPLE_PREVIEW_TITLE_HEIGHT);

    // Draw background
    g.setColour(COLOUR_BLACK_LIGHT_TRANSPARENT);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    
    // Draw audio preview
    g.setColour(COLOUR_ACCENT_LIGHT);

    if (mAudioPreview.getTotalLength() > 0.0)
    {
        // Draw file name on title bar
        g.setFont(FONT_SMALL_BOLD);
        String fileName = restoreSpacesFromURLString(lastFileDropped.getFileName());
        g.drawFittedText(fileName,
                         getLocalBounds().removeFromRight(getWidth() - PANEL_MARGIN).removeFromTop(SAMPLE_PREVIEW_TITLE_HEIGHT),
                         Justification::centredLeft,
                         2);
        
        // Draw audio preview
        previewArea.removeFromBottom(mAudioPreviewScrollbar->getHeight() + 4);
        mAudioPreview.drawChannels(g,
                                   previewArea.reduced(PANEL_MARGIN / 2.0),
                                   visibleRange.getStart(),
                                   visibleRange.getEnd(),
                                   1.0f);
    }
    else
    {
        // Draw hint to drag in audio file
        g.setFont(FONT_SMALL_BOLD);
        g.drawFittedText("No audio file selected",
                         getLocalBounds().removeFromRight(getWidth() - PANEL_MARGIN).removeFromTop(SAMPLE_PREVIEW_TITLE_HEIGHT),
                         Justification::centredLeft,
                         1);
        g.setFont(FONT_MEDIUM);
        g.drawFittedText("No audio file selected", getLocalBounds(), Justification::centred, 2);
    }
}

void AudioPreviewPanel::setPanelComponents()
{
    // Add scrollbar for audio thumbnail
    mAudioPreviewScrollbar = std::make_unique<ScrollBar>(false);
    mAudioPreviewScrollbar->setBounds(getLocalBounds().removeFromBottom(14).reduced(2));
    mAudioPreviewScrollbar->setRangeLimits(visibleRange);
    mAudioPreviewScrollbar->setAutoHide(false);
    mAudioPreviewScrollbar->addListener(this);
    addAndMakeVisible(*mAudioPreviewScrollbar);

    // Add position marker
    mAudioPositionMarker.setFill(COLOUR_GREENISH_WHITE);
    addAndMakeVisible(mAudioPositionMarker);
    
    // Repaint panel components
    repaint();
}

void AudioPreviewPanel::setURL(URL const & url)
{
    if (auto inputSource = std::make_unique<URLInputSource>(url))
    {
        mAudioPreview.setSource(inputSource.release());

        Range<double> newRange(0.0, mAudioPreview.getTotalLength());
        mAudioPreviewScrollbar->setRangeLimits(newRange);
        setRange(newRange);

        startTimerHz(40);
    }
}

URL AudioPreviewPanel::getLastDroppedFile() const noexcept
{
    return lastFileDropped;
}

void AudioPreviewPanel::setZoomFactor(double amount)
{
    // Set preview and scrollbar zoom
    if (mAudioPreview.getTotalLength() > 0)
    {
        auto newScale = jmax(0.001, mAudioPreview.getTotalLength() * (1.0 - jlimit(0.0, 0.99, amount)));
        auto timeAtCentre = xToTime((float)getWidth() / 2.0f);

        setRange({ timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5 });
    }
}

void AudioPreviewPanel::setRange(Range<double> newRange)
{
    // Set range for audio preview and scrollbar
    visibleRange = newRange;
    mAudioPreviewScrollbar->setCurrentRange(visibleRange);
    updateCursorPosition();
    repaint();
}

void AudioPreviewPanel::setFollowsTransport(bool shouldFollow)
{
    isFollowingTransport = shouldFollow;
}

bool AudioPreviewPanel::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}

void AudioPreviewPanel::filesDropped(StringArray const & files, int x, int y)
{
    File file = File(files[0]);
    
    if (!file.isDirectory() && isSupportedAudioFileFormat(file.getFileExtension()))
    {
        lastFileDropped = URL(file);
        sendChangeMessage();
    }
}

bool AudioPreviewPanel::isInterestedInDragSource(SourceDetails const & dragSourceDetails)
{
    return true;
}

void AudioPreviewPanel::itemDropped(SourceDetails const & dragSourceDetails)
{
    // Set resource fom sample item drag source
    if (dragSourceDetails.description == "SampleItemFile")
    {
        Component* dragSourceComponent = dragSourceDetails.sourceComponent.get();
        if (BlomeFileTreeView* fileTreeView = dynamic_cast<BlomeFileTreeView*>(dragSourceComponent))
        {
            File sampleFile = fileTreeView->getSelectedFile();
            File linkedAudioFile = File(sampleFile.getParentDirectory().getFullPathName() +  DIRECTORY_SEPARATOR + sampleFile.getFileNameWithoutExtension());
            
            if (!sampleFile.isDirectory() && isSupportedAudioFileFormat(linkedAudioFile.getFileExtension()))
            {
                lastFileDropped = URL(sampleFile);
                sendChangeMessage();
            }
        }
    }
}

void AudioPreviewPanel::mouseDown(MouseEvent const & e)
{
    mouseDrag(e);
}

void AudioPreviewPanel::mouseDrag(MouseEvent const & e)
{
    if (canMoveTransport())
    {
        sampleEditor.setAudioReadheadPosition(jmax(0.0, xToTime((float)e.x)));
    }
}

void AudioPreviewPanel::mouseUp(MouseEvent const &)
{
    sampleEditor.startAudio();
}

void AudioPreviewPanel::mouseWheelMove(MouseEvent const &, MouseWheelDetails const & wheel)
{
    // Set zoom of audio preview if a file is loaded
    if (mAudioPreview.getTotalLength() > 0.0)
    {
        auto newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
        newStart = jlimit(0.0, jmax(0.0, mAudioPreview.getTotalLength() - (visibleRange.getLength())), newStart);

        if (canMoveTransport())
        {
            setRange({ newStart, newStart + visibleRange.getLength() });
        }

        if (wheel.deltaY != 0.0f)
        {
            mZoomSlider->setValue(mZoomSlider->getValue() - wheel.deltaY * 0.1);
        }

        repaint();
    }
}

float AudioPreviewPanel::timeToX(double const time) const
{
    if (visibleRange.getLength() <= 0)
    {
        return 0;
    }

    return (float) getWidth() * (float) ((time - visibleRange.getStart()) / visibleRange.getLength());
}

double AudioPreviewPanel::xToTime(float const x) const
{
    return (x / (float)getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
}

bool AudioPreviewPanel::canMoveTransport() const noexcept
{
    return !(isFollowingTransport && sampleEditor.isPlaying());
}

void AudioPreviewPanel::scrollBarMoved(ScrollBar* scrollbar, double newRangeStart)
{
    if (scrollbar == &*mAudioPreviewScrollbar)
    {
        if (!(isFollowingTransport && sampleEditor.isPlaying()))
        {
            setRange(visibleRange.movedToStartAt(newRangeStart));
        }
    }
}

void AudioPreviewPanel::timerCallback()
{
    // Update cursor position or audio preview range
    if (canMoveTransport())
    {
        updateCursorPosition();
    }
    else
    {
        setRange(visibleRange.movedToStartAt(sampleEditor.getCurrentReadheadPosition() - (visibleRange.getLength() / 2.0)));
    }
}

void AudioPreviewPanel::updateCursorPosition()
{
    mAudioPositionMarker.setVisible(sampleEditor.isPlaying() || isMouseButtonDown());
    mAudioPositionMarker.setRectangle(Rectangle<float>(timeToX(sampleEditor.getCurrentReadheadPosition())
                                                       - 0.75f,
                                                       SAMPLE_PREVIEW_TITLE_HEIGHT
                                                       + PANEL_MARGIN / 2.0,
                                                       1.5f,
                                                       (float) (getHeight()
                                                               - PANEL_MARGIN
                                                               - SAMPLE_PREVIEW_TITLE_HEIGHT
                                                               - mAudioPreviewScrollbar->getHeight())));
}

void AudioPreviewPanel::showAudioResource()
{
    URL resource = getLastDroppedFile();
    showAudioResource(resource);
}

/**
 Shows the given resource in the audio preview
 */
void AudioPreviewPanel::showAudioResource(URL inResource)
{
    // Load resource into preview and player
    lastFileDropped = inResource;
    
    if (loadURLIntoTransport(inResource))
    {
        mCurrentAudioFile = std::move(inResource);
    }

    mZoomSlider->setValue(0, dontSendNotification);
    setURL(mCurrentAudioFile);
}

void AudioPreviewPanel::startOrStop()
{
    sampleEditor.startOrStopAudio();
}

bool AudioPreviewPanel::loadURLIntoTransport(URL const & audioURL)
{
    return sampleEditor.loadURLIntoTransport(audioURL, currentProcessor.getThread());
}
