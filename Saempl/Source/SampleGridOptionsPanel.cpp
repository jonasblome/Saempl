/*
 ==============================================================================
 
 SampleGridOptionsPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleGridOptionsPanel.h"

SampleGridOptionsPanel::SampleGridOptionsPanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor)
{
    setSize(style->GRID_OPTIONS_PANEL_WIDTH, style->GRID_OPTIONS_PANEL_HEIGHT);
    setPanelComponents();
}

SampleGridOptionsPanel::~SampleGridOptionsPanel()
{
    std::vector<float> newWeights = std::vector<float>(NUM_FEATURES + 1);
    newWeights[0] = mLengthSlider->getValue();
    newWeights[1] = mLoudnessSlider->getValue();
    newWeights[2] = mDynamicRangeSlider->getValue();
    newWeights[3] = mZeroCrossingRateSlider->getValue();
    newWeights[4] = mTempoSlider->getValue();
    newWeights[5] = mKeySlider->getValue();
    newWeights[6] = mSpectralCentroidSlider->getValue();
    newWeights[7] = mSpectralSpreadSlider->getValue();
    newWeights[8] = mSpectralRolloffSlider->getValue();
    newWeights[9] = mSpectralFluxSlider->getValue();
    newWeights[10] = mChromaFluxSlider->getValue();
    newWeights[11] = mSpectralDistributionSlider->getValue();
    newWeights[12] = mChromaDistributionSlider->getValue();
    
    if (currentProcessor.getFeatureWeights() != newWeights)
    {
        currentProcessor.setFeatureWeights(newWeights);
    }
}

void SampleGridOptionsPanel::paint(Graphics& g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    
    // Draw explanation text
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    g.drawFittedText(String("Change how much the clustering focuses on each property or choose a preset:\n\n")
                     + String("(Double click sliders to reset to default value)"),
                     style->PANEL_MARGIN,
                     style->PANEL_MARGIN,
                     getWidth() - style->PANEL_MARGIN * 2.0,
                     introTextHeight,
                     Justification::topLeft, 10);
    
    // Draw separation line
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawLine(style->PANEL_MARGIN,
               introTextHeight + 5,
               getWidth() - style->PANEL_MARGIN,
               introTextHeight + 5);
    
    // Draw slider annotations
    g.setColour(style->COLOUR_ACCENT_LIGHT);
    int y = style->PANEL_MARGIN * 3.0 + introTextHeight + style->PANEL_TITLE_HEIGHT * 0.5;
    
    g.drawFittedText("Length:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Loudness:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Dynamic Range:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Zero Crossing Rate:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Tempo:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Key:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Avg. Frequency:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Frequency Spread:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Upper Freq. Rolloff:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Freq. Flux:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Harmonic Flux:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Freq. Distribution:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    g.drawFittedText("Harm. Distribution:",
                     style->PANEL_MARGIN,
                     y,
                     labelWidth - style->PANEL_MARGIN,
                     style->PANEL_TITLE_HEIGHT * 0.5,
                     Justification::centredLeft,
                     1);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
}

void SampleGridOptionsPanel::applyPresetToSliders(std::vector<float> inPresetValues)
{
    mLengthSlider->setValue(inPresetValues[0]);
    mLoudnessSlider->setValue(inPresetValues[1]);
    mDynamicRangeSlider->setValue(inPresetValues[2]);
    mZeroCrossingRateSlider->setValue(inPresetValues[3]);
    mTempoSlider->setValue(inPresetValues[4]);
    mKeySlider->setValue(inPresetValues[5]);
    mSpectralCentroidSlider->setValue(inPresetValues[6]);
    mSpectralSpreadSlider->setValue(inPresetValues[7]);
    mSpectralRolloffSlider->setValue(inPresetValues[8]);
    mSpectralFluxSlider->setValue(inPresetValues[9]);
    mChromaFluxSlider->setValue(inPresetValues[10]);
    mSpectralDistributionSlider->setValue(inPresetValues[11]);
    mChromaDistributionSlider->setValue(inPresetValues[12]);
}

void SampleGridOptionsPanel::setPanelComponents()
{
    std::vector<float> initialWeights = currentProcessor.getFeatureWeights();
    
    // Add preset buttons
    int buttonWidth = 60;
    int y = style->PANEL_MARGIN * 2.0 + introTextHeight;
    
    // Adds harmonic preset button
    mHarmonicPresetButton = std::make_unique<TextButton>("Harmonic");
    mHarmonicPresetButton->setBounds(style->PANEL_MARGIN * 0.5,
                                     y,
                                     buttonWidth,
                                     style->PANEL_TITLE_HEIGHT * 0.5);
    mHarmonicPresetButton->onClick = [this]
    {
        applyPresetToSliders(GRID_PRESET_HARMONIC);
    };
    mHarmonicPresetButton->setTooltip("A preset to optimise clustering for harmonic loops");
    addAndMakeVisible(*mHarmonicPresetButton);
    
    // Adds drums preset button
    mDrumsPresetButton = std::make_unique<TextButton>("Drums");
    mDrumsPresetButton->setBounds(style->PANEL_MARGIN * 1.0 + buttonWidth,
                                  y,
                                  buttonWidth,
                                  style->PANEL_TITLE_HEIGHT * 0.5);
    mDrumsPresetButton->onClick = [this]
    {
        applyPresetToSliders(GRID_PRESET_DRUMS);
    };
    mDrumsPresetButton->setTooltip("A preset to optimise clustering for drum one-shots");
    addAndMakeVisible(*mDrumsPresetButton);
    
    // Adds solo instrument preset button
    mMonophonicPresetButton = std::make_unique<TextButton>("Monoph.");
    mMonophonicPresetButton->setBounds(style->PANEL_MARGIN * 1.5 + buttonWidth * 2,
                                       y,
                                       buttonWidth,
                                       style->PANEL_TITLE_HEIGHT * 0.5);
    mMonophonicPresetButton->onClick = [this]
    {
        applyPresetToSliders(GRID_PRESET_MONOPHONIC);
    };
    mMonophonicPresetButton->setTooltip("A preset to optimise clustering for monophonic samples");
    addAndMakeVisible(*mMonophonicPresetButton);
    
    // Adds foley preset button
    mFoleyPresetButton = std::make_unique<TextButton>("Foley");
    mFoleyPresetButton->setBounds(style->PANEL_MARGIN * 2.0 + buttonWidth * 3,
                                  y,
                                  buttonWidth,
                                  style->PANEL_TITLE_HEIGHT * 0.5);
    mFoleyPresetButton->onClick = [this]
    {
        applyPresetToSliders(GRID_PRESET_FOLEY);
    };
    mFoleyPresetButton->setTooltip("A preset to optimise clustering for foley samples");
    addAndMakeVisible(*mFoleyPresetButton);
    
    // Add sliders
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds length slider
    mLengthSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mLengthSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                             y,
                             getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                             style->PANEL_TITLE_HEIGHT * 0.5);
    mLengthSlider->setRange(0, 10, 0);
    mLengthSlider->setDoubleClickReturnValue(true, 1.0);
    mLengthSlider->setSkewFactorFromMidPoint(1.0);
    mLengthSlider->setValue(initialWeights[0], NotificationType::dontSendNotification);
    mLengthSlider->setTooltip("Change focus on sample length");
    addAndMakeVisible(*mLengthSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds loudness slider
    mLoudnessSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mLoudnessSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                               y,
                               getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                               style->PANEL_TITLE_HEIGHT * 0.5);
    mLoudnessSlider->setRange(0, 10, 0);
    mLoudnessSlider->setDoubleClickReturnValue(true, 1.0);
    mLoudnessSlider->setSkewFactorFromMidPoint(1.0);
    mLoudnessSlider->setValue(initialWeights[1], NotificationType::dontSendNotification);
    mLoudnessSlider->setTooltip("Change focus on sample loudness");
    addAndMakeVisible(*mLoudnessSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds dynamic range slider
    mDynamicRangeSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mDynamicRangeSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                   y,
                                   getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                   style->PANEL_TITLE_HEIGHT * 0.5);
    mDynamicRangeSlider->setRange(0, 10, 0);
    mDynamicRangeSlider->setDoubleClickReturnValue(true, 1.0);
    mDynamicRangeSlider->setSkewFactorFromMidPoint(1.0);
    mDynamicRangeSlider->setValue(initialWeights[2], NotificationType::dontSendNotification);
    mDynamicRangeSlider->setTooltip("Change focus on dynamic range");
    addAndMakeVisible(*mDynamicRangeSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds zero crossing rate slider
    mZeroCrossingRateSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mZeroCrossingRateSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                       y,
                                       getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                       style->PANEL_TITLE_HEIGHT * 0.5);
    mZeroCrossingRateSlider->setRange(0, 10, 0);
    mZeroCrossingRateSlider->setDoubleClickReturnValue(true, 1.0);
    mZeroCrossingRateSlider->setSkewFactorFromMidPoint(1.0);
    mZeroCrossingRateSlider->setValue(initialWeights[3], NotificationType::dontSendNotification);
    mZeroCrossingRateSlider->setTooltip("Change focus on zero crossing rate");
    addAndMakeVisible(*mZeroCrossingRateSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds tempo slider
    mTempoSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mTempoSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                            y,
                            getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                            style->PANEL_TITLE_HEIGHT * 0.5);
    mTempoSlider->setRange(0, 10, 0);
    mTempoSlider->setDoubleClickReturnValue(true, 1.0);
    mTempoSlider->setSkewFactorFromMidPoint(1.0);
    mTempoSlider->setValue(initialWeights[4], NotificationType::dontSendNotification);
    mTempoSlider->setTooltip("Change focus on sample tempo");
    addAndMakeVisible(*mTempoSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds key slider
    mKeySlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mKeySlider->setBounds(style->PANEL_MARGIN + labelWidth,
                          y,
                          getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                          style->PANEL_TITLE_HEIGHT * 0.5);
    mKeySlider->setRange(0, 10, 0);
    mKeySlider->setDoubleClickReturnValue(true, 1.0);
    mKeySlider->setSkewFactorFromMidPoint(1.0);
    mKeySlider->setValue(initialWeights[5], NotificationType::dontSendNotification);
    mKeySlider->setTooltip("Change focus on sample key");
    addAndMakeVisible(*mKeySlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral centroid slider
    mSpectralCentroidSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mSpectralCentroidSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                       y,
                                       getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                       style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralCentroidSlider->setRange(0, 10, 0);
    mSpectralCentroidSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralCentroidSlider->setSkewFactorFromMidPoint(1.0);
    mSpectralCentroidSlider->setValue(initialWeights[6], NotificationType::dontSendNotification);
    mSpectralCentroidSlider->setTooltip("Change focus on avg. freq.");
    addAndMakeVisible(*mSpectralCentroidSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral spread slider
    mSpectralSpreadSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mSpectralSpreadSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                     y,
                                     getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                     style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralSpreadSlider->setRange(0, 10, 0);
    mSpectralSpreadSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralSpreadSlider->setSkewFactorFromMidPoint(1.0);
    mSpectralSpreadSlider->setValue(initialWeights[7], NotificationType::dontSendNotification);
    mSpectralSpreadSlider->setTooltip("Change focus on freq. spread");
    addAndMakeVisible(*mSpectralSpreadSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral rolloff slider
    mSpectralRolloffSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mSpectralRolloffSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                      y,
                                      getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                      style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralRolloffSlider->setRange(0, 10, 0);
    mSpectralRolloffSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralRolloffSlider->setSkewFactorFromMidPoint(1.0);
    mSpectralRolloffSlider->setValue(initialWeights[8], NotificationType::dontSendNotification);
    mSpectralRolloffSlider->setTooltip("Change focus on upper freq. rolloff");
    addAndMakeVisible(*mSpectralRolloffSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral flux slider
    mSpectralFluxSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mSpectralFluxSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                   y,
                                   getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                   style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralFluxSlider->setRange(0, 10, 0);
    mSpectralFluxSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralFluxSlider->setSkewFactorFromMidPoint(1.0);
    mSpectralFluxSlider->setValue(initialWeights[9], NotificationType::dontSendNotification);
    mSpectralFluxSlider->setTooltip("Change focus on freq. flux");
    addAndMakeVisible(*mSpectralFluxSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds chroma flux slider
    mChromaFluxSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mChromaFluxSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                 y,
                                 getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                 style->PANEL_TITLE_HEIGHT * 0.5);
    mChromaFluxSlider->setRange(0, 10, 0);
    mChromaFluxSlider->setDoubleClickReturnValue(true, 1.0);
    mChromaFluxSlider->setSkewFactorFromMidPoint(1.0);
    mChromaFluxSlider->setValue(initialWeights[10], NotificationType::dontSendNotification);
    mChromaFluxSlider->setTooltip("Change focus on harm. flux");
    addAndMakeVisible(*mChromaFluxSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral distribution slider
    mSpectralDistributionSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mSpectralDistributionSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                           y,
                                           getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                           style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralDistributionSlider->setRange(0, 10, 0);
    mSpectralDistributionSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralDistributionSlider->setSkewFactorFromMidPoint(1.0);
    mSpectralDistributionSlider->setValue(initialWeights[11], NotificationType::dontSendNotification);
    mSpectralDistributionSlider->setTooltip("Change focus on freq. distribution");
    addAndMakeVisible(*mSpectralDistributionSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds chroma distribution slider
    mChromaDistributionSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mChromaDistributionSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                         y,
                                         getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                         style->PANEL_TITLE_HEIGHT * 0.5);
    mChromaDistributionSlider->setRange(0, 10, 0);
    mChromaDistributionSlider->setDoubleClickReturnValue(true, 1.0);
    mChromaDistributionSlider->setSkewFactorFromMidPoint(1.0);
    mChromaDistributionSlider->setValue(initialWeights[12], NotificationType::dontSendNotification);
    mChromaDistributionSlider->setTooltip("Change focus on harm. distribution");
    addAndMakeVisible(*mChromaDistributionSlider);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
}
