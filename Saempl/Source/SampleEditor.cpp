/*
  ==============================================================================

    SampleEditor.cpp
    Created: 21 May 2023 3:00:50pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleEditor.h"

SampleEditor::SampleEditor()
{
    mAudioPlayer = std::make_unique<AudioPlayer>();
}

SampleEditor::~SampleEditor()
{
    
}

AudioFormatManager& SampleEditor::getFormatManager()
{
    return mAudioPlayer->getFormatManager();
}

void SampleEditor::startOrStop()
{
    mAudioPlayer->startOrStop();
}

void SampleEditor::setPosition(double position)
{
    mAudioPlayer->setPosition(position);
}

void SampleEditor::start()
{
    mAudioPlayer->start();
}

bool SampleEditor::isPlaying()
{
    return mAudioPlayer->isPlaying();
}

double SampleEditor::getCurrentPosition()
{
    return mAudioPlayer->getCurrentPosition();
}

void SampleEditor::stop()
{
    mAudioPlayer->stop();
}

void SampleEditor::setSource(PositionableAudioSource* const newSource,
                             int readAheadSize,
                             TimeSliceThread* readAheadThread,
                             double sourceSampleRateToCorrectFor)
{
    mAudioPlayer->setSource(newSource, readAheadSize, readAheadThread, sourceSampleRateToCorrectFor);
}

bool SampleEditor::loadURLIntoTransport(const URL& audioURL, TimeSliceThread& inThread)
{
    return mAudioPlayer->loadURLIntoTransport(audioURL, inThread);
}
