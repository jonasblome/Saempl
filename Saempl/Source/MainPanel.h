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

/**
 Holds all the panels and components of the plugin editor window.
 
 Implements:
 \ref PanelBase
 \ref juce::DragAndDropContainer
 */
class MainPanel
:   public PanelBase,
    public DragAndDropContainer
{
public:
    /**
     The constructor of the main window panel.
     
     @param inProcessor the audio processor of the plugin instance.
     */
    MainPanel(SaemplAudioProcessor& inProcessor);
    ~MainPanel();
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
private:
    JUCE_HEAVYWEIGHT_LEAK_DETECTOR(MainPanel)
    
    SaemplAudioProcessor& currentProcessor;
    std::unique_ptr<HeaderPanel> mHeaderPanel;
    std::unique_ptr<CenterPanel> mCenterPanel;
};
