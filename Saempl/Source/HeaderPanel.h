/*
  ==============================================================================

    HeaderPanel.h
    Created: 27 May 2023 11:12:36am
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class HeaderPanel
:   public PanelBase
{
public:
    HeaderPanel();
    ~HeaderPanel();
    
    void paint(Graphics& g) override;
    
private:
};
