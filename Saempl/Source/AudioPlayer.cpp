/*
  ==============================================================================

    AudioPlayer.cpp
    Created: 25 May 2023 12:56:15am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    // Audio setup
    mAudioDeviceManager = std::make_unique<AudioDeviceManager>();
    mFormatManager.registerBasicFormats();
    RuntimePermissions::request(RuntimePermissions::recordAudio,
                                [this] (bool granted)
                                {
                                    int numInputChannels = granted ? 2 : 0;
                                    mAudioDeviceManager->initialise(numInputChannels, 2, nullptr, true, {}, nullptr);
                                });
    mAudioDeviceManager->addAudioCallback(&mAudioSourcePlayer);
    mAudioSourcePlayer.setSource(&mTransportSource);
}

AudioPlayer::~AudioPlayer()
{
    mTransportSource.setSource(nullptr);
    mAudioSourcePlayer.setSource(nullptr);
    mAudioDeviceManager->removeAudioCallback(&mAudioSourcePlayer);
}

AudioFormatManager& AudioPlayer::getFormatManager()
{
    return mFormatManager;
}

void AudioPlayer::startOrStop()
{
    if(mTransportSource.isPlaying())
    {
        mTransportSource.stop();
    }
    else
    {
        mTransportSource.setPosition(0);
        mTransportSource.start();
    }
}

void AudioPlayer::setPosition(double position)
{
    mTransportSource.setPosition(position);
}

void AudioPlayer::start()
{
    mTransportSource.start();
}

bool AudioPlayer::isPlaying()
{
    return mTransportSource.isPlaying();
}

double AudioPlayer::getCurrentPosition()
{
    return mTransportSource.getCurrentPosition();
}

void AudioPlayer::stop()
{
    mTransportSource.stop();
}

void AudioPlayer::setSource(PositionableAudioSource* const newSource,
                            int readAheadSize,
                            TimeSliceThread* readAheadThread,
                            double sourceSampleRateToCorrectFor)
{
    mTransportSource.setSource(newSource, readAheadSize, readAheadThread, sourceSampleRateToCorrectFor);
}

bool AudioPlayer::loadURLIntoTransport(const URL& audioURL, TimeSliceThread& inThread)
{
    // Unload the previous file source and delete it
    mTransportSource.stop();
    mTransportSource.setSource(nullptr);
    mCurrentAudioFileSource.reset();

    const auto source = std::make_unique<URLInputSource>(audioURL);

    if (source == nullptr)
        return false;

    auto stream = rawToUniquePtr(source->createInputStream());

    if (stream == nullptr)
        return false;

    auto reader = rawToUniquePtr(mFormatManager.createReaderFor(std::move(stream)));

    if (reader == nullptr)
        return false;

    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader.release(), true);

    // ..and plug it into our transport source
    mTransportSource.setSource(mCurrentAudioFileSource.get(),
                               32768,                   // Tells it to buffer this many samples ahead
                               &inThread,                 // This is the background thread to use for reading-ahead
                               mCurrentAudioFileSource->getAudioFormatReader()->sampleRate);     // Allows for sample rate correction

    return true;
}
