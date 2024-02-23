/*
 ==============================================================================
 
 MainPanel.h
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "PanelBase.h"
#include "HeaderPanel.h"
#include "CentrePanel.h"

/**
 Holds all the panels and components of the plugin editor window.
 */
class MainPanel
:
public PanelBase,
public DragAndDropContainer
{
public:
    /**
     The constructor of the main window panel.
     
     @param inProcessor the audio processor of the plugin instance.
     */
    MainPanel(SaemplAudioProcessor& inProcessor);
    ~MainPanel();
    
private:
    std::unique_ptr<HeaderPanel> mHeaderPanel;
    std::unique_ptr<CentrePanel> mCentrePanel;
    BlomeStyling::Ptr style;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    bool keyPressed(KeyPress const & key) override;
    void resizePanelComponents() override;
};
