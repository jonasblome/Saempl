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
}

void SampleGridOptionsPanel::setPanelComponents()
{
    std::vector<float> initialWeights = currentProcessor.getFeatureWeights();
    
    // Add sliders
    int textBoxWidth = 30;
    int y = style->PANEL_MARGIN * 2.0 + introTextHeight;
    
    // Adds length slider
    mLengthSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mLengthSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                             y,
                             getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                             style->PANEL_TITLE_HEIGHT * 0.5);
    mLengthSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mLengthSlider->setRange(0, 10, 0.1);
    mLengthSlider->setDoubleClickReturnValue(true, 1.0);
    mLengthSlider->setValue(initialWeights[0], NotificationType::dontSendNotification);
    mLengthSlider->setTooltip("Change focus on sample length");
    addAndMakeVisible(*mLengthSlider);
    mLengthLabel = std::make_unique<Label>(String(), "Length:");
    mLengthLabel->attachToComponent(&*mLengthSlider, true);
    addAndMakeVisible(*mLengthLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds loudness slider
    mLoudnessSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mLoudnessSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                               y,
                               getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                               style->PANEL_TITLE_HEIGHT * 0.5);
    mLoudnessSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mLoudnessSlider->setRange(0, 10, 0.1);
    mLoudnessSlider->setDoubleClickReturnValue(true, 1.0);
    mLoudnessSlider->setValue(initialWeights[1], NotificationType::dontSendNotification);
    mLoudnessSlider->setTooltip("Change focus on sample loudness");
    addAndMakeVisible(*mLoudnessSlider);
    mLoudnessLabel = std::make_unique<Label>(String(), "Loudness:");
    mLoudnessLabel->attachToComponent(&*mLoudnessSlider, true);
    addAndMakeVisible(*mLoudnessLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds dynamic range slider
    mDynamicRangeSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mDynamicRangeSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                   y,
                                   getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                   style->PANEL_TITLE_HEIGHT * 0.5);
    mDynamicRangeSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mDynamicRangeSlider->setRange(0, 10, 0.1);
    mDynamicRangeSlider->setDoubleClickReturnValue(true, 1.0);
    mDynamicRangeSlider->setValue(initialWeights[2], NotificationType::dontSendNotification);
    mDynamicRangeSlider->setTooltip("Change focus on dynamic range");
    addAndMakeVisible(*mDynamicRangeSlider);
    mDynamicRangeLabel = std::make_unique<Label>(String(), "Dynamic Range:");
    mDynamicRangeLabel->attachToComponent(&*mDynamicRangeSlider, true);
    addAndMakeVisible(*mDynamicRangeLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds zero crossing rate slider
    mZeroCrossingRateSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mZeroCrossingRateSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                       y,
                                       getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                       style->PANEL_TITLE_HEIGHT * 0.5);
    mZeroCrossingRateSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mZeroCrossingRateSlider->setRange(0, 10, 0.1);
    mZeroCrossingRateSlider->setDoubleClickReturnValue(true, 1.0);
    mZeroCrossingRateSlider->setValue(initialWeights[3], NotificationType::dontSendNotification);
    mZeroCrossingRateSlider->setTooltip("Change focus on zero crossing rate");
    addAndMakeVisible(*mZeroCrossingRateSlider);
    mZeroCrossingRateLabel = std::make_unique<Label>(String(), "Zero Crossing Rate:");
    mZeroCrossingRateLabel->attachToComponent(&*mZeroCrossingRateSlider, true);
    addAndMakeVisible(*mZeroCrossingRateLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds tempo slider
    mTempoSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mTempoSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                            y,
                            getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                            style->PANEL_TITLE_HEIGHT * 0.5);
    mTempoSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mTempoSlider->setRange(0, 10, 0.1);
    mTempoSlider->setDoubleClickReturnValue(true, 1.0);
    mTempoSlider->setValue(initialWeights[4], NotificationType::dontSendNotification);
    mTempoSlider->setTooltip("Change focus on sample tempo");
    addAndMakeVisible(*mTempoSlider);
    mTempoLabel = std::make_unique<Label>(String(), "Tempo:");
    mTempoLabel->attachToComponent(&*mTempoSlider, true);
    addAndMakeVisible(*mTempoLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds key slider
    mKeySlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mKeySlider->setBounds(style->PANEL_MARGIN + labelWidth,
                          y,
                          getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                          style->PANEL_TITLE_HEIGHT * 0.5);
    mKeySlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mKeySlider->setRange(0, 10, 0.1);
    mKeySlider->setDoubleClickReturnValue(true, 1.0);
    mKeySlider->setValue(initialWeights[5], NotificationType::dontSendNotification);
    mKeySlider->setTooltip("Change focus on sample key");
    addAndMakeVisible(*mKeySlider);
    mKeyLabel = std::make_unique<Label>(String(), "Key:");
    mKeyLabel->attachToComponent(&*mKeySlider, true);
    addAndMakeVisible(*mKeyLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral centroid slider
    mSpectralCentroidSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mSpectralCentroidSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                       y,
                                       getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                       style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralCentroidSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralCentroidSlider->setRange(0, 10, 0.1);
    mSpectralCentroidSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralCentroidSlider->setValue(initialWeights[6], NotificationType::dontSendNotification);
    mSpectralCentroidSlider->setTooltip("Change focus on average freq.");
    addAndMakeVisible(*mSpectralCentroidSlider);
    mSpectralCentroidLabel = std::make_unique<Label>(String(), "Spectral Centroid:");
    mSpectralCentroidLabel->attachToComponent(&*mSpectralCentroidSlider, true);
    addAndMakeVisible(*mSpectralCentroidLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral spread slider
    mSpectralSpreadSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mSpectralSpreadSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                     y,
                                     getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                     style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralSpreadSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralSpreadSlider->setRange(0, 10, 0.1);
    mSpectralSpreadSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralSpreadSlider->setValue(initialWeights[7], NotificationType::dontSendNotification);
    mSpectralSpreadSlider->setTooltip("Change focus on freq. spread");
    addAndMakeVisible(*mSpectralSpreadSlider);
    mSpectralSpreadLabel = std::make_unique<Label>(String(), "Spectral Spread:");
    mSpectralSpreadLabel->attachToComponent(&*mSpectralSpreadSlider, true);
    addAndMakeVisible(*mSpectralSpreadLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral rolloff slider
    mSpectralRolloffSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mSpectralRolloffSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                      y,
                                      getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                      style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralRolloffSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralRolloffSlider->setRange(0, 10, 0.1);
    mSpectralRolloffSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralRolloffSlider->setValue(initialWeights[8], NotificationType::dontSendNotification);
    mSpectralRolloffSlider->setTooltip("Change focus on upper frequency rolloff");
    addAndMakeVisible(*mSpectralRolloffSlider);
    mSpectralRolloffLabel = std::make_unique<Label>(String(), "Spectral Rolloff:");
    mSpectralRolloffLabel->attachToComponent(&*mSpectralRolloffSlider, true);
    addAndMakeVisible(*mSpectralRolloffLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral flux slider
    mSpectralFluxSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mSpectralFluxSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                   y,
                                   getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                   style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralFluxSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralFluxSlider->setRange(0, 10, 0.1);
    mSpectralFluxSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralFluxSlider->setValue(initialWeights[9], NotificationType::dontSendNotification);
    mSpectralFluxSlider->setTooltip("Change focus on frequency fluctuation");
    addAndMakeVisible(*mSpectralFluxSlider);
    mSpectralFluxLabel = std::make_unique<Label>(String(), "Spectral Flux:");
    mSpectralFluxLabel->attachToComponent(&*mSpectralFluxSlider, true);
    addAndMakeVisible(*mSpectralFluxLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds chroma flux slider
    mChromaFluxSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mChromaFluxSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                 y,
                                 getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                 style->PANEL_TITLE_HEIGHT * 0.5);
    mChromaFluxSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mChromaFluxSlider->setRange(0, 10, 0.1);
    mChromaFluxSlider->setDoubleClickReturnValue(true, 1.0);
    mChromaFluxSlider->setValue(initialWeights[10], NotificationType::dontSendNotification);
    mChromaFluxSlider->setTooltip("Change focus on harmonic fluctuation");
    addAndMakeVisible(*mChromaFluxSlider);
    mChromaFluxLabel = std::make_unique<Label>(String(), "Chroma Flux:");
    mChromaFluxLabel->attachToComponent(&*mChromaFluxSlider, true);
    addAndMakeVisible(*mChromaFluxLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds spectral distribution slider
    mSpectralDistributionSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mSpectralDistributionSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                           y,
                                           getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                           style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralDistributionSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mSpectralDistributionSlider->setRange(0, 10, 0.1);
    mSpectralDistributionSlider->setDoubleClickReturnValue(true, 1.0);
    mSpectralDistributionSlider->setValue(initialWeights[11], NotificationType::dontSendNotification);
    mSpectralDistributionSlider->setTooltip("Change focus on frequency distribution");
    addAndMakeVisible(*mSpectralDistributionSlider);
    mSpectralDistributionLabel = std::make_unique<Label>(String(), "Spectral Distribution:");
    mSpectralDistributionLabel->attachToComponent(&*mSpectralDistributionSlider, true);
    addAndMakeVisible(*mSpectralDistributionLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
    
    // Adds chroma distribution slider
    mChromaDistributionSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::TextBoxLeft);
    mChromaDistributionSlider->setBounds(style->PANEL_MARGIN + labelWidth,
                                         y,
                                         getWidth() - style->PANEL_MARGIN * 2.0 - labelWidth,
                                         style->PANEL_TITLE_HEIGHT * 0.5);
    mChromaDistributionSlider->setTextBoxStyle(Slider::TextBoxLeft, false, textBoxWidth, style->PANEL_TITLE_HEIGHT * 0.5);
    mChromaDistributionSlider->setRange(0, 10, 0.1);
    mChromaDistributionSlider->setDoubleClickReturnValue(true, 1.0);
    mChromaDistributionSlider->setValue(initialWeights[12], NotificationType::dontSendNotification);
    mChromaDistributionSlider->setTooltip("Change focus on harmonic distribution");
    addAndMakeVisible(*mChromaDistributionSlider);
    mChromaDistributionLabel = std::make_unique<Label>(String(), "Chroma Distribution:");
    mChromaDistributionLabel->attachToComponent(&*mChromaDistributionSlider, true);
    addAndMakeVisible(*mChromaDistributionLabel);
    y += style->PANEL_TITLE_HEIGHT * 0.5 + style->PANEL_MARGIN;
}
