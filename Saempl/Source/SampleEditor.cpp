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

AudioFormatManager& SampleEditor::getAudioFormatManager()
{
    return mAudioPlayer->getFormatManager();
}

void SampleEditor::startOrStopAudio()
{
    mAudioPlayer->startOrStop();
}

void SampleEditor::setAudioReadheadPosition(double position)
{
    mAudioPlayer->setTransportSourcePosition(position);
}

void SampleEditor::startAudio()
{
    mAudioPlayer->start();
}

bool SampleEditor::isPlaying()
{
    return mAudioPlayer->isPlaying();
}

double SampleEditor::getCurrentReadheadPosition()
{
    return mAudioPlayer->getCurrentPosition();
}

void SampleEditor::stopAudio()
{
    mAudioPlayer->stop();
}


bool SampleEditor::loadURLIntoTransport(const URL& audioURL, TimeSliceThread& inThread)
{
    return mAudioPlayer->loadURLIntoTransport(audioURL, inThread);
}
