/*
  ==============================================================================

    PanelBase.cpp
    Created: 22 May 2023 6:31:30am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "PanelBase.h"
#include "BlomeStyleDefinitions.h"

PanelBase::PanelBase(SaemplAudioProcessor& inProcessor)
{
    
}

PanelBase::~PanelBase()
{
    
}

void PanelBase::mouseEnter(const MouseEvent& event)
{
    repaint();
}

void PanelBase::mouseExit(const MouseEvent& event)
{
    repaint();
}

void PanelBase::paint(Graphics& g)
{
    
}
