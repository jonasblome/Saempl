/*
  ==============================================================================

    FileFilterPanel.cpp
    Created: 29 Oct 2023 12:14:04pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "FileFilterPanel.h"

FileFilterPanel::FileFilterPanel(SampleLibrary& inSampleLibrary)
:   libraryFileFilter(inSampleLibrary.getFileFilter()),
    linkedLibrary(inSampleLibrary)
{
    setPanelComponents();
}

FileFilterPanel::~FileFilterPanel()
{
    
}

void FileFilterPanel::paint(Graphics &g)
{
    PanelBase::paint(g);
    
    // Draw background
    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    g.setColour(COLOUR_BLACK_LIGHT_TRANSPARENT);
    g.drawLine(PANEL_MARGIN / 2.0,
               FILTER_RULE_HEIGHT + 9,
               getWidth() - PANEL_MARGIN / 2.0,
               FILTER_RULE_HEIGHT + 9);
    
    for (BlomeFileFilterRuleViewBase* newRuleView : mFilterRuleViews)
    {
        addAndMakeVisible(*newRuleView);
    }
}

void FileFilterPanel::setPanelComponents()
{
    // New rule type combo box
    mNewRuleTypeChooser = std::make_unique<ComboBox>("NewRuleTypeChooser");
    mNewRuleTypeChooser->setBounds(PANEL_MARGIN / 2.0,
                                   PANEL_MARGIN / 2.0,
                                   COMBO_BOX_WIDTH_MEDIUM - PANEL_MARGIN / 4.0,
                                   FILTER_RULE_HEIGHT - PANEL_MARGIN / 2.0);
    mNewRuleTypeChooser->addItem("Title", 1);
    mNewRuleTypeChooser->addItem("Length", 2);
    mNewRuleTypeChooser->setTextWhenNothingSelected("Choose new rule type");
    addAndMakeVisible(*mNewRuleTypeChooser);
    
    // New rule button
    mAddFilterRuleButton = std::make_unique<TextButton>("Add filter");
    mAddFilterRuleButton->setBounds(COMBO_BOX_WIDTH_MEDIUM + PANEL_MARGIN * 0.75,
                                    PANEL_MARGIN / 2.0,
                                    80 - PANEL_MARGIN / 4.0,
                                    FILTER_RULE_HEIGHT - PANEL_MARGIN / 2.0);
    mAddFilterRuleButton->onClick = [this]
    {
        switch (mNewRuleTypeChooser->getSelectedItemIndex())
        {
            case 0:
            {
                SampleFileFilterRuleTitle* newTitleRule = static_cast<SampleFileFilterRuleTitle*>(libraryFileFilter.addFilterRule(new SampleFileFilterRuleTitle("Title")));
                newTitleRule->setCompareOperator(CompareOperators::GreaterThan);
                linkedLibrary.refresh();
                break;
            }
            case 1:
            {
                SampleFileFilterRuleLength* newLengthRule = static_cast<SampleFileFilterRuleLength*>(libraryFileFilter.addFilterRule(new SampleFileFilterRuleLength("Length")));
                newLengthRule->setCompareOperator(CompareOperators::GreaterThan);
                linkedLibrary.refresh();
                break;
            }
            default:
                break;
        }
        
        mNewRuleTypeChooser->setSelectedItemIndex(-1);
    };
    addAndMakeVisible(*mAddFilterRuleButton);
    
    // Filter rule components
    int combinedFilterRuleViewHeight = 0;
    
    for (SampleFileFilterRuleBase* rule : libraryFileFilter.getFilterRules())
    {
        BlomeFileFilterRuleViewBase* newRuleView = nullptr;
        
        switch (PROPERTY_NAMES.indexOf(rule->getRuleType()))
        {
            case 0:
            {
                newRuleView = mFilterRuleViews.add(std::make_unique<BlomeFileFilterRuleViewLength>(*dynamic_cast<SampleFileFilterRuleLength*>(rule), linkedLibrary));
                break;
            }
            case 1:
            {
                newRuleView = mFilterRuleViews.add(std::make_unique<BlomeFileFilterRuleViewTitle>(*dynamic_cast<SampleFileFilterRuleTitle*>(rule), linkedLibrary));
                break;
            }
            default:
                break;
        }
        
        if (newRuleView != nullptr)
        {
            newRuleView->setBounds(PANEL_MARGIN / 2.0,
                                   PANEL_MARGIN / 2.0 + FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight,
                                   FILTER_RULE_WIDTH - PANEL_MARGIN,
                                   FILTER_RULE_HEIGHT - PANEL_MARGIN / 2.0);
        }
        
        combinedFilterRuleViewHeight += FILTER_RULE_HEIGHT;
    }
    
    setSize(FILTER_PANEL_WIDTH, FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight + PANEL_MARGIN / 2.0);
}
