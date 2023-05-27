/*
  ==============================================================================

    SampleItemPanel.cpp
    Created: 22 May 2023 9:46:19am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleItemPanel.h"

SampleItemPanel::SampleItemPanel(TimeSliceThread& inThread)
:   PanelBase(),
    audioReadaheadThread(inThread)
{
    // Set panel size
    setSize(SAMPLE_PANEL_WIDTH, SAMPLE_PANEL_HEIGHT);
    
    // Draw panel components
    setPanelStyle();
    
    // Audio setup
    mAudioDeviceManager = std::make_unique<AudioDeviceManager>();
    mFormatManager.registerBasicFormats();
    mAudioDeviceManager->addAudioCallback(&mAudioSourcePlayer);
    mAudioSourcePlayer.setSource(&mTransportSource);
}

SampleItemPanel::~SampleItemPanel()
{
    mTransportSource.setSource(nullptr);
    mAudioSourcePlayer.setSource(nullptr);
    mAudioDeviceManager->removeAudioCallback(&mAudioSourcePlayer);
    mAudioPreviewPanel->removeChangeListener(this);
}

void SampleItemPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Draw panel background
    g.fillAll(BlomeColour_DarkRed);
}

void SampleItemPanel::setPanelStyle()
{
    auto r = getLocalBounds();
    
    // Add sample editor component
    mSampleEditor = std::make_unique<SampleEditor>();
    
    // Add zoom slider
    mZoomSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mZoomSlider->setBounds(0, SAMPLE_PREVIEW_HEIGHT, getWidth(), 75 / 3);
    mZoomSlider->setRange(0, 1, 0);
    mZoomSlider->onValueChange = [this] { mAudioPreviewPanel->setZoomFactor(mZoomSlider->getValue()); };
    mZoomSlider->setSkewFactor(2);
    addAndMakeVisible(*mZoomSlider);

    // Add follow transport button
    mFollowTransportButton = std::make_unique<ToggleButton>("Follow Transport");
    mFollowTransportButton->setBounds(0, SAMPLE_PREVIEW_HEIGHT + 25, 25, 75 / 3);
    mFollowTransportButton->onClick = [this] { updateFollowTransportState(); };
    addAndMakeVisible(*mFollowTransportButton);
    
    // Add play button component
    mStartStopButton = std::make_unique<TextButton>("Play/Stop");
    mStartStopButton->setBounds(0, SAMPLE_PREVIEW_HEIGHT + 50, 75, 75 / 3);
    mStartStopButton->onClick = [this] { startOrStop(); };
    addAndMakeVisible(*mStartStopButton);
    
    // Add audio thumbnail component
    mAudioPreviewPanel = std::make_unique<AudioPreviewPanel>(mFormatManager, mTransportSource, *mZoomSlider);
    mAudioPreviewPanel->setBounds(r.removeFromTop(SAMPLE_PREVIEW_HEIGHT));
    addAndMakeVisible(mAudioPreviewPanel.get());
    mAudioPreviewPanel->addChangeListener(this);
    
    // Repaint panel
    repaint();
}

void SampleItemPanel::showAudioResource(URL resource)
{
    if (loadURLIntoTransport(resource))
        mCurrentAudioFile = std::move(resource);

    mZoomSlider->setValue(0, dontSendNotification);
    mAudioPreviewPanel->setURL(mCurrentAudioFile);
}

bool SampleItemPanel::loadURLIntoTransport(const URL& audioURL)
{
    // Unload the previous file source and delete it
    mTransportSource.stop();
    mTransportSource.setSource(nullptr);
    mCurrentAudioFileSource.reset();

    const auto source = std::make_unique<URLInputSource>(audioURL);

    if (source == nullptr)
        return false;

    auto stream = rawToUniquePtr (source->createInputStream());

    if (stream == nullptr)
        return false;

    auto reader = rawToUniquePtr (mFormatManager.createReaderFor(std::move (stream)));

    if (reader == nullptr)
        return false;

    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader.release(), true);

    // ..and plug it into our transport source
    mTransportSource.setSource (mCurrentAudioFileSource.get(),
                               32768,                   // Tells it to buffer this many samples ahead
                               &audioReadaheadThread,                 // This is the background thread to use for reading-ahead
                               mCurrentAudioFileSource->getAudioFormatReader()->sampleRate);     // Allows for sample rate correction

    return true;
}

void SampleItemPanel::startOrStop()
{
    if (mTransportSource.isPlaying())
    {
        mTransportSource.stop();
    }
    else
    {
        mTransportSource.setPosition(0);
        mTransportSource.start();
    }
}

void SampleItemPanel::updateFollowTransportState()
{
    mAudioPreviewPanel->setFollowsTransport(mFollowTransportButton->getToggleState());
}

void SampleItemPanel::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == mAudioPreviewPanel.get())
    {
        showAudioResource(URL(mAudioPreviewPanel->getLastDroppedFile()));
    }
}
