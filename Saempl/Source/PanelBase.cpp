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

void PanelBase::mouseEnter(MouseEvent const & event)
{
    repaint();
}

void PanelBase::mouseExit(MouseEvent const & event)
{
    repaint();
}

void PanelBase::paint(Graphics& g)
{
    
}

void PanelBase::resized()
{
    resizePanelComponents();
}
