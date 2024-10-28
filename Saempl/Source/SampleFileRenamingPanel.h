/*
  ==============================================================================

    SampleFileRenamingPanel.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class SampleFileRenamingPanel
:
public PanelBase
{
public:
    SampleFileRenamingPanel(SaemplAudioProcessor & inProcessor,
                            String inFilePath);
    ~SampleFileRenamingPanel();

private:
    String originalFilePath;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
};
