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

AudioFormatManager& SampleItemViewModel::getFormatManager()
{
    return mSampleEditor->getFormatManager();
}

void SampleItemViewModel::startOrStop()
{
    mSampleEditor->startOrStop();
}

void SampleItemViewModel::setPosition(double position)
{
    mSampleEditor->setPosition(position);
}

void SampleItemViewModel::start()
{
    mSampleEditor->start();
}

bool SampleItemViewModel::isPlaying()
{
    return mSampleEditor->isPlaying();
}

double SampleItemViewModel::getCurrentPosition()
{
    return mSampleEditor->getCurrentPosition();
}

void SampleItemViewModel::stop()
{
    mSampleEditor->stop();
}

void SampleItemViewModel::setSource(PositionableAudioSource* const newSource,
                                    int readAheadSize,
                                    TimeSliceThread* readAheadThread,
                                    double sourceSampleRateToCorrectFor)
{
    mSampleEditor->setSource(newSource, readAheadSize, readAheadThread, sourceSampleRateToCorrectFor);
}

bool SampleItemViewModel::loadURLIntoTransport(const URL& audioURL, TimeSliceThread& inThread)
{
    return mSampleEditor->loadURLIntoTransport(audioURL, inThread);
}
