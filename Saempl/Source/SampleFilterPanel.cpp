/*
  ==============================================================================

    SampleItemFilterPanel.cpp
    Created: 17 Jun 2023 1:19:35pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFilterPanel.h"

SampleFilterPanel::SampleFilterPanel(SaemplAudioProcessor& inProcessor)
:   PanelBase(inProcessor),
    filterConditions(inProcessor.getSampleItemLibrary().getFilterConditions())
{
    // Set component size
    setSize(SAMPLE_ITEM_FILTER_PANEL_WIDTH, SAMPLE_ITEM_FILTER_PANEL_HEIGHT);
    setPanelComponents();
}

SampleFilterPanel::~SampleFilterPanel()
{
    
}

void SampleFilterPanel::setPanelComponents()
{
    // Add button for adding filter conditions
    mAddFilterConditionButton = std::make_unique<TextButton>("+");
    mAddFilterConditionButton->setBounds(PANEL_MARGIN / 2.0,
                                         PANEL_MARGIN / 2.0,
                                         NORMAL_BUTTON_HEIGHT - PANEL_MARGIN,
                                         NORMAL_BUTTON_HEIGHT - PANEL_MARGIN);
    mAddFilterConditionButton->onClick = [this] { addFilterCondition(); };
    addAndMakeVisible(*mAddFilterConditionButton);
    setFilterConditionViews();
    
    repaint();
}

void SampleFilterPanel::paint(Graphics& g)
{
    PanelBase::paint(g);
    
    // Set background
    g.setColour(BlomeColour_AccentColourLight);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), PANEL_CORNER_SIZE);
}

void SampleFilterPanel::setFilterConditionViews()
{
    mFilterConditionViews.clear();
    int y = NORMAL_BUTTON_HEIGHT;
    
    for (SampleFilterCondition* filterCondition : *filterConditions)
    {
        FilterConditionView* newConditionView = new FilterConditionView(filterCondition);
        newConditionView->setBounds(PANEL_MARGIN / 2.0, y + PANEL_MARGIN / 2.0, getWidth() - PANEL_MARGIN, FILTER_CONDITION_VIEW_HEIGHT - PANEL_MARGIN);
        addAndMakeVisible(*newConditionView);
        mFilterConditionViews.add(newConditionView);
        y += FILTER_CONDITION_VIEW_HEIGHT;
        setSize(getWidth(), getHeight() + FILTER_CONDITION_VIEW_HEIGHT);
    }
}

void SampleFilterPanel::addFilterCondition()
{
    filterConditions->add(new SampleFilterCondition(AnalysisTypes[0], 0, CompareOperator_Equals));
    setFilterConditionViews();
}
