/*
  ==============================================================================

    MainPanel.h
    Created: 22 May 2023 6:31:37am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"
#include "HeaderPanel.h"
#include "CenterPanel.h"

class MainPanel
:   public PanelBase,
    public DragAndDropContainer
{
public:
    // Constructors
    MainPanel(SaemplAudioProcessor& inProcessor);
    ~MainPanel();
    
    // Methods
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(MainPanel)
    
    // Fields
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<HeaderPanel> mHeaderPanel;
    std::unique_ptr<CenterPanel> mCenterPanel;
    
    // Methods
    
};
