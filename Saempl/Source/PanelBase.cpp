/*
  ==============================================================================

    PanelBase.cpp
    Author:  Jonas Blome

  ==============================================================================
*/

#include "PanelBase.h"
#include "BlomeStyleDefinitions.h"

PanelBase::PanelBase(SaemplAudioProcessor& inProcessor)
:
currentProcessor(inProcessor)
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
