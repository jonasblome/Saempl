/*
  ==============================================================================

    AudioPreviewPanel.cpp
    Created: 27 May 2023 4:20:03pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "AudioPreviewPanel.h"

AudioPreviewPanel::AudioPreviewPanel(TimeSliceThread& inThread,Slider& inSlider, SampleItemViewModel& inSampleItemViewModel)
:   PanelBase(),
    currentThread(inThread),
    sampleItemViewModel(inSampleItemViewModel),
    mZoomSlider(&inSlider),
    thumbnailCache(5),
    mAudioPreview(512, sampleItemViewModel.getFormatManager(), thumbnailCache),
    isFollowingTransport(false)
{
    setSize(SAMPLE_PREVIEW_WIDTH, SAMPLE_PREVIEW_HEIGHT);
    setPanelComponents();
}

AudioPreviewPanel::~AudioPreviewPanel()
{
    mAudioPreviewScrollbar->removeListener(this);
    mAudioPreview.removeChangeListener(this);
}

void AudioPreviewPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw background
    g.setColour(BlomeColour_BlackLightTransparent);
    g.fillRoundedRectangle(Rectangle<float>(getLocalBounds().toFloat()), PanelCornerSize);
    
    // Draw audio preview
    g.setColour(BlomeColour_LightGray);

    if(mAudioPreview.getTotalLength() > 0.0)
    {
        auto thumbArea = getLocalBounds();

        thumbArea.removeFromBottom(mAudioPreviewScrollbar->getHeight() + 4);
        mAudioPreview.drawChannels(g,
                                   thumbArea.reduced (2),
                                   visibleRange.getStart(),
                                   visibleRange.getEnd(),
                                   1.0f);
    }
    else
    {
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

    mPositionMarker.setFill(Colours::white.withAlpha(0.85f));
    addAndMakeVisible(mPositionMarker);
    
    repaint();
}

void AudioPreviewPanel::setURL(const URL& url)
{
    if(auto inputSource = std::make_unique<URLInputSource>(url))
    {
        mAudioPreview.setSource(inputSource.release());

        Range<double> newRange(0.0, mAudioPreview.getTotalLength());
        mAudioPreviewScrollbar->setRangeLimits(newRange);
        setRange(newRange);

        startTimerHz (40);
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

        setRange ({ timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5 });
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

void AudioPreviewPanel::changeListenerCallback (ChangeBroadcaster*)
{
    // This method is called by the thumbnail when it has changed, so we should repaint it..
    repaint();
}

bool AudioPreviewPanel::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void AudioPreviewPanel::filesDropped(const StringArray& files, int x, int y)
{
    lastFileDropped = URL(File(files[0]));
    sendChangeMessage();
}

void AudioPreviewPanel::mouseDown(const MouseEvent& e)
{
    mouseDrag (e);
}

void AudioPreviewPanel::mouseDrag(const MouseEvent& e)
{
    if(canMoveTransport())
    {
        sampleItemViewModel.setPosition(jmax(0.0, xToTime((float)e.x)));
    }
}

void AudioPreviewPanel::mouseUp(const MouseEvent&)
{
    sampleItemViewModel.start();
}

void AudioPreviewPanel::mouseWheelMove(const MouseEvent&, const MouseWheelDetails& wheel)
{
    if (mAudioPreview.getTotalLength() > 0.0)
    {
        auto newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
        newStart = jlimit (0.0, jmax (0.0, mAudioPreview.getTotalLength() - (visibleRange.getLength())), newStart);

        if (canMoveTransport())
            setRange({ newStart, newStart + visibleRange.getLength() });

        if (wheel.deltaY != 0.0f)
            mZoomSlider->setValue(mZoomSlider->getValue() - wheel.deltaY);

        repaint();
    }
}

float AudioPreviewPanel::timeToX(const double time) const
{
    if (visibleRange.getLength() <= 0)
        return 0;

    return (float) getWidth() * (float) ((time - visibleRange.getStart()) / visibleRange.getLength());
}

double AudioPreviewPanel::xToTime(const float x) const
{
    return (x / (float)getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
}

bool AudioPreviewPanel::canMoveTransport() const noexcept
{
    return !(isFollowingTransport && sampleItemViewModel.isPlaying());
}

void AudioPreviewPanel::scrollBarMoved(ScrollBar* scrollbar, double newRangeStart)
{
    if(scrollbar == &*mAudioPreviewScrollbar)
    {
        if(!(isFollowingTransport && sampleItemViewModel.isPlaying()))
        {
            setRange(visibleRange.movedToStartAt(newRangeStart));
        }
    }
}

void AudioPreviewPanel::timerCallback()
{
    if(canMoveTransport())
    {
        updateCursorPosition();
    }
    else
    {
        setRange (visibleRange.movedToStartAt(sampleItemViewModel.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
    }
}

void AudioPreviewPanel::updateCursorPosition()
{
    mPositionMarker.setVisible(sampleItemViewModel.isPlaying() || isMouseButtonDown());

    mPositionMarker.setRectangle(Rectangle<float>(timeToX(sampleItemViewModel.getCurrentPosition()) - 0.75f, 0,
                                                          1.5f, (float)(getHeight() - mAudioPreviewScrollbar->getHeight())));
}

void AudioPreviewPanel::showAudioResource()
{
    URL resource = getLastDroppedFile();
    showAudioResource(resource);
}

void AudioPreviewPanel::showAudioResource(URL inResource)
{
    if(loadURLIntoTransport(inResource))
    {
        mCurrentAudioFile = std::move(inResource);
    }

    mZoomSlider->setValue(0, dontSendNotification);
    setURL(mCurrentAudioFile);
}

void AudioPreviewPanel::startOrStop()
{
    sampleItemViewModel.startOrStop();
}

bool AudioPreviewPanel::loadURLIntoTransport(const URL& audioURL)
{
    return sampleItemViewModel.loadURLIntoTransport(audioURL, currentThread);
}
