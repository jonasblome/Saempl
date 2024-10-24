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
    std::unique_ptr<TextButton> mHarmonicPresetButton;
    std::unique_ptr<TextButton> mDrumsPresetButton;
    std::unique_ptr<TextButton> mMonophonicPresetButton;
    std::unique_ptr<TextButton> mFoleyPresetButton;
    std::unique_ptr<Slider> mLengthSlider;
    std::unique_ptr<Label> mLengthLabel;
    std::unique_ptr<Slider> mLoudnessSlider;
    std::unique_ptr<Label> mLoudnessLabel;
    std::unique_ptr<Slider> mDynamicRangeSlider;
    std::unique_ptr<Label> mDynamicRangeLabel;
    std::unique_ptr<Slider> mZeroCrossingRateSlider;
    std::unique_ptr<Label> mZeroCrossingRateLabel;
    std::unique_ptr<Slider> mTempoSlider;
    std::unique_ptr<Label> mTempoLabel;
    std::unique_ptr<Slider> mKeySlider;
    std::unique_ptr<Label> mKeyLabel;
    std::unique_ptr<Slider> mSpectralCentroidSlider;
    std::unique_ptr<Label> mSpectralCentroidLabel;
    std::unique_ptr<Slider> mSpectralSpreadSlider;
    std::unique_ptr<Label> mSpectralSpreadLabel;
    std::unique_ptr<Slider> mSpectralRolloffSlider;
    std::unique_ptr<Label> mSpectralRolloffLabel;
    std::unique_ptr<Slider> mSpectralFluxSlider;
    std::unique_ptr<Label> mSpectralFluxLabel;
    std::unique_ptr<Slider> mChromaFluxSlider;
    std::unique_ptr<Label> mChromaFluxLabel;
    std::unique_ptr<Slider> mSpectralDistributionSlider;
    std::unique_ptr<Label> mSpectralDistributionLabel;
    std::unique_ptr<Slider> mChromaDistributionSlider;
    std::unique_ptr<Label> mChromaDistributionLabel;
    static int const labelWidth = 70;
    static int const introTextHeight = 70;
    
    void paint(Graphics& g) override;
    void applyPresetToSliders(std::vector<float> inPresetValues);
    
void setPanelComponents() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleGridOptionsPanel);
};
