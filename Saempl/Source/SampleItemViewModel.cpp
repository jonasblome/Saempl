/*
  ==============================================================================

    SampleItemViewModel.cpp
    Created: 22 May 2023 6:31:17am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleItemViewModel.h"

SampleItemViewModel::SampleItemViewModel()
{
    mSampleEditor = std::make_unique<SampleEditor>();
}

SampleItemViewModel::~SampleItemViewModel()
{
    
}

AudioFormatManager& SampleItemViewModel::getAudioFormatManager()
{
    return mSampleEditor->getAudioFormatManager();
}

void SampleItemViewModel::startOrStopAudio()
{
    mSampleEditor->startOrStopAudio();
}

void SampleItemViewModel::setAudioReadheadPosition(double position)
{
    mSampleEditor->setAudioReadheadPosition(position);
}

void SampleItemViewModel::startAudio()
{
    mSampleEditor->startAudio();
}

bool SampleItemViewModel::isPlayingAudio()
{
    return mSampleEditor->isPlaying();
}

double SampleItemViewModel::getCurrentPosition()
{
    return mSampleEditor->getCurrentReadheadPosition();
}

void SampleItemViewModel::stopAudio()
{
    mSampleEditor->stopAudio();
}

void SampleItemViewModel::setAudioSource(PositionableAudioSource* const newSource,
                                    int readAheadSize,
                                    TimeSliceThread* readAheadThread,
                                    double sourceSampleRateToCorrectFor)
{
    mSampleEditor->setAudioSource(newSource, readAheadSize, readAheadThread, sourceSampleRateToCorrectFor);
}

bool SampleItemViewModel::loadURLIntoTransport(const URL& audioURL, TimeSliceThread& inThread)
{
    return mSampleEditor->loadURLIntoTransport(audioURL, inThread);
}
