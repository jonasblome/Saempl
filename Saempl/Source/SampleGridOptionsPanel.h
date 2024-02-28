/*
  ==============================================================================

    SampleGridOptionsPanel.h
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "PanelBase.h"

class SampleGridOptionsPanel
:
public PanelBase
{
public:
    /**
     The constructor of the panel for the library's file filter.
     
     @param inProcessor the processor object of the current plugin instance.
     */
    SampleGridOptionsPanel(SaemplAudioProcessor& inProcessor);
    ~SampleGridOptionsPanel();
    
private:
    std::unique_ptr<Slider> mLengthSlider;
    std::unique_ptr<Slider> mLoudnessSlider;
    std::unique_ptr<Slider> mDynamicRangeSlider;
    std::unique_ptr<Slider> mZeroCrossingRateSlider;
    std::unique_ptr<Slider> mTempoSlider;
    std::unique_ptr<Slider> mKeySlider;
    std::unique_ptr<Slider> mSpectralCentroidSlider;
    std::unique_ptr<Slider> mSpectralSpreadSlider;
    std::unique_ptr<Slider> mSpectralRolloffSlider;
    std::unique_ptr<Slider> mSpectralFluxSlider;
    std::unique_ptr<Slider> mChromaFluxSlider;
    std::unique_ptr<Slider> mSpectralDistributionSlider;
    std::unique_ptr<Slider> mChromaDistributionSlider;
    static int const labelWidth = 100;
    
    void paint(Graphics& g) override;
    void setPanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleGridOptionsPanel);
};
