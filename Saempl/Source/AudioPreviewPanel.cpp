/*
  ==============================================================================

    AudioPreviewPanel.cpp
    Created: 27 May 2023 4:20:03pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "AudioPreviewPanel.h"
#include "BlomeFileTreeView.h"

AudioPreviewPanel::AudioPreviewPanel(SaemplAudioProcessor& inProcessor, Slider& inSlider, SampleItemViewModel& inSampleItemViewModel)
:   PanelBase(inProcessor),
    currentProcessor(inProcessor),
    sampleItemViewModel(inSampleItemViewModel),
    mZoomSlider(&inSlider),
    thumbnailCache(5),
    mAudioPreview(512, sampleItemViewModel.getAudioFormatManager(), thumbnailCache),
    isFollowingTransport(false)
{
    setSize(SAMPLE_PREVIEW_WIDTH - PANEL_MARGIN * 2, SAMPLE_PREVIEW_HEIGHT - PANEL_MARGIN * 1.5);
    setPanelComponents();
}

AudioPreviewPanel::~AudioPreviewPanel()
{
    mAudioPreviewScrollbar->removeListener(this);
}

void AudioPreviewPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    auto previewArea = getLocalBounds().removeFromBottom(getHeight() - sampleItemTitleHeight);

    // Draw background
    g.setColour(BlomeColour_BlackLightTransparent);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Blome_PanelCornerSize);
    
    // Draw audio preview
    g.setColour(BlomeColour_AccentColourLight);

    if (mAudioPreview.getTotalLength() > 0.0)
    {
        // Draw file name on title bar
        g.setFont(font_small_bold);
        String fileName = restoreSpacesFromURLString(lastFileDropped.getFileName());
        g.drawFittedText(fileName,
                         getLocalBounds().removeFromRight(getWidth() - PANEL_MARGIN).removeFromTop(sampleItemTitleHeight),
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
        g.setFont(font_small_bold);
        g.drawFittedText("No audio file selected",
                         getLocalBounds().removeFromRight(getWidth() - PANEL_MARGIN).removeFromTop(sampleItemTitleHeight),
                         Justification::centredLeft,
                         1);
        g.setFont(font_medium);
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
    mAudioPositionMarker.setFill(BlomeColour_BlueishWhite);
    addAndMakeVisible(mAudioPositionMarker);
    
    // Repaint panel components
    repaint();
}

void AudioPreviewPanel::setURL(const URL& url)
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
    if (mAudioPreview.getTotalLength() > 0)
    {
        auto newScale = jmax(0.001, mAudioPreview.getTotalLength() * (1.0 - jlimit(0.0, 0.99, amount)));
        auto timeAtCentre = xToTime((float)getWidth() / 2.0f);

        setRange({ timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5 });
    }
}

void AudioPreviewPanel::setRange(Range<double> newRange)
{
    visibleRange = newRange;
    mAudioPreviewScrollbar->setCurrentRange(visibleRange);
    updateCursorPosition();
    repaint();
}

void AudioPreviewPanel::setFollowsTransport(bool shouldFollow)
{
    isFollowingTransport = shouldFollow;
}

bool AudioPreviewPanel::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void AudioPreviewPanel::filesDropped(const StringArray& files, int x, int y)
{
    File file = File(files[0]);
    
    if (!file.isDirectory() && isSupportedAudioFileFormat(file.getFileExtension()))
    {
        lastFileDropped = URL(file);
        sendChangeMessage();
    }
}

bool AudioPreviewPanel::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    return true;
}

void AudioPreviewPanel::itemDropped(const SourceDetails& dragSourceDetails)
{
    if (dragSourceDetails.description == "SampleItemFile")
    {
        Component* dragSourceComponent = dragSourceDetails.sourceComponent.get();
        if (BlomeFileTreeView* fileTreeView = static_cast<BlomeFileTreeView*>(dragSourceComponent))
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

void AudioPreviewPanel::mouseDown(const MouseEvent& e)
{
    mouseDrag(e);
}

void AudioPreviewPanel::mouseDrag(const MouseEvent& e)
{
    if (canMoveTransport())
    {
        sampleItemViewModel.setAudioReadheadPosition(jmax(0.0, xToTime((float)e.x)));
    }
}

void AudioPreviewPanel::mouseUp(const MouseEvent&)
{
    sampleItemViewModel.startAudio();
}

void AudioPreviewPanel::mouseWheelMove(const MouseEvent&, const MouseWheelDetails& wheel)
{
    // Set zoom of audio preview if a file is loaded
    if (mAudioPreview.getTotalLength() > 0.0)
    {
        auto newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
        newStart = jlimit (0.0, jmax (0.0, mAudioPreview.getTotalLength() - (visibleRange.getLength())), newStart);

        if (canMoveTransport())
        {
            setRange({ newStart, newStart + visibleRange.getLength() });
        }

        if (wheel.deltaY != 0.0f)
        {
            mZoomSlider->setValue(mZoomSlider->getValue() - wheel.deltaY);
        }

        repaint();
    }
}

float AudioPreviewPanel::timeToX(const double time) const
{
    if (visibleRange.getLength() <= 0)
    {
        return 0;
    }

    return (float) getWidth() * (float) ((time - visibleRange.getStart()) / visibleRange.getLength());
}

double AudioPreviewPanel::xToTime(const float x) const
{
    return (x / (float)getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
}

bool AudioPreviewPanel::canMoveTransport() const noexcept
{
    return !(isFollowingTransport && sampleItemViewModel.isPlayingAudio());
}

void AudioPreviewPanel::scrollBarMoved(ScrollBar* scrollbar, double newRangeStart)
{
    if (scrollbar == &*mAudioPreviewScrollbar)
    {
        if (!(isFollowingTransport && sampleItemViewModel.isPlayingAudio()))
        {
            setRange(visibleRange.movedToStartAt(newRangeStart));
        }
    }
}

void AudioPreviewPanel::timerCallback()
{
    if (canMoveTransport())
    {
        updateCursorPosition();
    }
    else
    {
        setRange(visibleRange.movedToStartAt(sampleItemViewModel.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
    }
}

void AudioPreviewPanel::updateCursorPosition()
{
    mAudioPositionMarker.setVisible(sampleItemViewModel.isPlayingAudio() || isMouseButtonDown());

    mAudioPositionMarker.setRectangle(Rectangle<float>(timeToX(sampleItemViewModel.getCurrentPosition()) - 0.75f, sampleItemTitleHeight + PANEL_MARGIN / 2.0,
                                                          1.5f, (float)(getHeight() - PANEL_MARGIN - sampleItemTitleHeight - mAudioPreviewScrollbar->getHeight())));
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
    sampleItemViewModel.startOrStopAudio();
}

bool AudioPreviewPanel::loadURLIntoTransport(const URL& audioURL)
{
    return sampleItemViewModel.loadURLIntoTransport(audioURL, currentProcessor.getThread());
}
