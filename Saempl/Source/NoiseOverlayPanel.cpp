/*
  ==============================================================================

    NoiseOverlayPanel.cpp
    Created: 29 May 2023 9:05:27am
    Author:  Jonas Blome

  ==============================================================================
*/

#include "NoiseOverlayPanel.h"

NoiseOverlayPanel::NoiseOverlayPanel()
{
    setInterceptsMouseClicks(false, true);
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    mNoiseOverlay = ImageCache::getFromMemory(BinaryData::noisetexture1050x800_png, BinaryData::noisetexture1050x800_pngSize);
    mNoiseOverlay.multiplyAllAlphas(0.055);
    setPanelComponents();
}

NoiseOverlayPanel::~NoiseOverlayPanel()
{
    
}

void NoiseOverlayPanel::paint(Graphics &g)
{
    g.drawImage(mNoiseOverlay, getLocalBounds().toFloat());
}

void NoiseOverlayPanel::setPanelComponents()
{
    
}
