/*
 ==============================================================================
 
 AudioPlayer.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    // Create thread for the audio player
    mThread = std::make_unique<TimeSliceThread>("AudioPlayerThread");
    mThread->startThread(Thread::Priority::normal);
    
    // Audio setup
    mFormatManager = std::make_unique<AudioFormatManager>();
    mTransportSource = std::make_unique<AudioTransportSource>();
    mAudioDeviceManager = std::make_unique<AudioDeviceManager>();
    mAudioSourcePlayer = std::make_unique<AudioSourcePlayer>();
    initialiseDefaultDevice();
    mFormatManager->registerBasicFormats();
    mAudioDeviceManager->addAudioCallback(&*mAudioSourcePlayer);
    mAudioSourcePlayer->setSource(&*mTransportSource);
    startTimerHz(5);
}

AudioPlayer::~AudioPlayer()
{
    mTransportSource->setSource(nullptr);
    mAudioSourcePlayer->setSource(nullptr);
    mAudioDeviceManager->removeAudioCallback(&*mAudioSourcePlayer);
    mThread->stopThread(10000);
    stopTimer();
}

AudioFormatManager& AudioPlayer::getFormatManager()
{
    return *mFormatManager;
}

void AudioPlayer::startOrStop()
{
    if(mTransportSource->isPlaying())
    {
        mTransportSource->stop();
    }
    else
    {
        mTransportSource->setPosition(0);
        mTransportSource->start();
    }
}

void AudioPlayer::setTransportSourcePosition(double inPosition)
{
    mTransportSource->setPosition(inPosition);
}

void AudioPlayer::start()
{
    mTransportSource->start();
}

bool AudioPlayer::isPlaying()
{
    return mTransportSource->isPlaying();
}

double AudioPlayer::getCurrentPosition()
{
    return mTransportSource->getCurrentPosition();
}

void AudioPlayer::stop()
{
    mTransportSource->stop();
}

bool AudioPlayer::loadURLIntoTransport(URL const & inURL)
{
    // Unload the previous file source and delete it
    mTransportSource->stop();
    mTransportSource->setSource(nullptr);
    mCurrentAudioFileSource.reset();
    
    auto const source = std::make_unique<URLInputSource>(inURL);
    
    if (source == nullptr)
    {
        return false;
    }
    
    auto stream = rawToUniquePtr(source->createInputStream());
    
    if (stream == nullptr)
    {
        return false;
    }
    
    auto reader = rawToUniquePtr(mFormatManager->createReaderFor(std::move(stream)));
    
    if (reader == nullptr)
    {
        return false;
    }
    
    mCurrentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader.release(), true);
    
    // Plug new audio source into our transport source
    mTransportSource->setSource(mCurrentAudioFileSource.get(),
                                32768,                   // Tells it to buffer this many samples ahead
                                &*mThread,                 // This is the background thread to use for reading-ahead
                                mCurrentAudioFileSource->getAudioFormatReader()->sampleRate);     // Allows for sample rate correction
    
    return true;
}

void AudioPlayer::emptyTransport()
{
    mTransportSource->stop();
    mTransportSource->setSource(nullptr);
    mCurrentAudioFileSource.reset();
}

void AudioPlayer::timerCallback()
{
    int defaultDeviceIndex = mAudioDeviceManager->getCurrentDeviceTypeObject()->getDefaultDeviceIndex(false);
    String defaultDeviceName = mAudioDeviceManager->getCurrentDeviceTypeObject()->getDeviceNames()[defaultDeviceIndex];
    
    if (defaultDeviceName != mAudioDeviceManager->getCurrentAudioDevice()->getName())
    {
        initialiseDefaultDevice();
    }
}

void AudioPlayer::initialiseDefaultDevice()
{
    int numInputChannels = 0;
    RuntimePermissions::request(RuntimePermissions::recordAudio,
                                [&numInputChannels] (bool granted)
                                { numInputChannels = granted ? 2 : 0; });
    mAudioDeviceManager->initialise(numInputChannels, 2, nullptr, true, {}, nullptr);
}

void AudioPlayer::setGain(float inGain)
{
    mAudioSourcePlayer->setGain(inGain);
}
