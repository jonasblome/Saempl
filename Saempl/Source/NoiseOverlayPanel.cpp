/*
 ==============================================================================
 
 NoiseOverlayPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "NoiseOverlayPanel.h"

NoiseOverlayPanel::NoiseOverlayPanel()
{
    setInterceptsMouseClicks(false, true);
    setSize(getParentWidth(), getParentHeight());
    setPanelComponents();
    setAlwaysOnTop(true);
}

NoiseOverlayPanel::~NoiseOverlayPanel()
{
    
}

void NoiseOverlayPanel::paint(Graphics &g)
{
    g.drawImage(*mNoiseOverlay, getLocalBounds().toFloat());
}

void NoiseOverlayPanel::setPanelComponents()
{
    // Draw grain overlay
    mNoiseOverlay = std::make_unique<Image>(ImageCache::getFromMemory(BinaryData::noisetexture1050x800_png,
                                                                      BinaryData::noisetexture1050x800_pngSize));
    mNoiseOverlay->multiplyAllAlphas(0.055);
}
