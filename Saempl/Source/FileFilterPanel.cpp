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
    // Remove listeners for all rule views
    for (BlomeFileFilterRuleViewBase* ruleView : mFilterRuleViews)
    {
        if (BlomeFileFilterRuleViewTitle* ruleViewTitle = dynamic_cast<BlomeFileFilterRuleViewTitle*>(ruleView))
        {
            ruleViewTitle->removeDeleteButtonListener(this);
        }
        else if (BlomeFileFilterRuleViewLength* ruleViewLength = dynamic_cast<BlomeFileFilterRuleViewLength*>(ruleView))
        {
            ruleViewLength->removeDeleteButtonListener(this);
        }
    }
}

void FileFilterPanel::paint(Graphics &g)
{
    // Draw background
    g.setColour(COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), CORNER_SIZE_MEDIUM);
    g.setColour(COLOUR_BLACK_LIGHT_TRANSPARENT);
    g.drawLine(PANEL_MARGIN / 2.0,
               FILTER_RULE_HEIGHT + 9,
               getWidth() - PANEL_MARGIN / 2.0,
               FILTER_RULE_HEIGHT + 9);
    
    // Draw hint message if rules are empty
    if (libraryFileFilter.getFilterRules().isEmpty())
    {
        g.setColour(COLOUR_ACCENT_LIGHT);
        g.setFont(FONT_SMALL_BOLD_ACCENTUATED);
        g.drawFittedText("Add filter rules to see them here",
                         getLocalBounds().removeFromBottom(FILTER_RULE_HEIGHT),
                         Justification::centred,
                         2);
    }
    
    // Draw rule views
    int combinedFilterRuleViewHeight = 0;
    
    for (BlomeFileFilterRuleViewBase* newRuleView : mFilterRuleViews)
    {
        newRuleView->setBounds(PANEL_MARGIN / 2.0,
                               PANEL_MARGIN / 2.0 + FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight,
                               FILTER_RULE_WIDTH - PANEL_MARGIN,
                               FILTER_RULE_HEIGHT - PANEL_MARGIN / 2.0);
        addAndMakeVisible(*newRuleView);
        combinedFilterRuleViewHeight += FILTER_RULE_HEIGHT;
    }
}

void FileFilterPanel::generateRuleView(SampleFileFilterRuleBase *rule)
{
    BlomeFileFilterRuleViewBase* newRuleView = nullptr;
    
    switch (PROPERTY_NAMES.indexOf(rule->getRulePropertyName()))
    {
        case 0:
        {
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewLength>(*dynamic_cast<SampleFileFilterRuleLength*>(rule),
                                                                     linkedLibrary));
            break;
        }
        case 1:
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewTitle>(*dynamic_cast<SampleFileFilterRuleTitle*>(rule),
                                                                    linkedLibrary));
            break;
        default:
            jassertfalse;
            break;
    }
    
    newRuleView->addDeleteButtonListener(this);
}

void FileFilterPanel::addFilterRuleView()
{
    // Only add rule if type is selected, add maximum of 12 rules (reason: glitch in filter panel if more)
    if (mNewRuleTypeChooser->getSelectedItemIndex() == -1 || libraryFileFilter.getFilterRules().size() >= 12)
    {
        return;
    }
    
    // Add view for filter rule
    int combinedFilterRuleViewHeight = FILTER_RULE_HEIGHT * libraryFileFilter.getFilterRules().size();
    SampleFileFilterRuleBase* newRule;
    
    switch (mNewRuleTypeChooser->getSelectedItemIndex())
    {
        case 0:
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleTitle("Title"));
            break;
        case 1:
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleLength("Length"));
            break;
        default:
            jassertfalse;
            break;
    }
    
    generateRuleView(newRule);
    setSize(FILTER_PANEL_WIDTH,
            FILTER_PANEL_HEIGHT
            + combinedFilterRuleViewHeight + FILTER_RULE_HEIGHT
            + PANEL_MARGIN / 2.0);
    repaint();
    linkedLibrary.refresh();
}

void FileFilterPanel::removeFilterRule(SampleFileFilterRuleBase const & inFilterRule)
{
    libraryFileFilter.getFilterRules().removeObject(&inFilterRule);
}

void FileFilterPanel::setPanelComponents()
{
    // Add rule type combo box
    mNewRuleTypeChooser = std::make_unique<ComboBox>("NewRuleTypeChooser");
    mNewRuleTypeChooser->setBounds(PANEL_MARGIN / 2.0,
                                   PANEL_MARGIN / 2.0,
                                   COMBO_BOX_WIDTH_MEDIUM - PANEL_MARGIN / 4.0,
                                   FILTER_RULE_HEIGHT - PANEL_MARGIN / 2.0);
    mNewRuleTypeChooser->addItem("New title rule:", 1);
    mNewRuleTypeChooser->addItem("New length rule:", 2);
    mNewRuleTypeChooser->setTextWhenNothingSelected("Choose new rule type");
    addAndMakeVisible(*mNewRuleTypeChooser);
    
    // Add button for adding filter rules
    mAddFilterRuleButton = std::make_unique<ImageButton>("Add filter");
    mAddFilterRuleButton->setImages(false,
                                    true,
                                    true,
                                    ImageCache::getFromMemory(BinaryData::add_FILL0_wght400_GRAD0_opsz24_png,
                                                              BinaryData::add_FILL0_wght400_GRAD0_opsz24_pngSize),
                                    1.0,
                                    COLOUR_ACCENT_LIGHT,
                                    Image(),
                                    0.35,
                                    COLOUR_ACCENT_LIGHT,
                                    Image(),
                                    0.5,
                                    COLOUR_ACCENT_LIGHT);
    mAddFilterRuleButton->setBounds(COMBO_BOX_WIDTH_MEDIUM + PANEL_MARGIN * 0.75,
                                    PANEL_MARGIN / 2.0,
                                    FILTER_RULE_HEIGHT - PANEL_MARGIN / 2.0,
                                    FILTER_RULE_HEIGHT - PANEL_MARGIN / 2.0);
    mAddFilterRuleButton->onClick = [this]
    {
        addFilterRuleView();
    };
    addAndMakeVisible(*mAddFilterRuleButton);
    
    // Filter rule components
    int combinedFilterRuleViewHeight = 0;
    
    for (SampleFileFilterRuleBase* rule : libraryFileFilter.getFilterRules())
    {
        generateRuleView(rule);
        combinedFilterRuleViewHeight += FILTER_RULE_HEIGHT;
    }
    
    // Add space for info text if there are no filter rules
    combinedFilterRuleViewHeight = combinedFilterRuleViewHeight == 0 ? FILTER_RULE_HEIGHT : combinedFilterRuleViewHeight;
    setSize(FILTER_PANEL_WIDTH, FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight + PANEL_MARGIN / 2.0);
}

void FileFilterPanel::buttonClicked(Button* button)
{
    // Delete rule and its view if its delete button was clicked
    if (BlomeFileFilterRuleViewBase* ruleView = dynamic_cast<BlomeFileFilterRuleViewBase*>(button->getParentComponent()))
    {
        ruleView->removeDeleteButtonListener(this);
        removeFilterRule(ruleView->getLinkedFilterRule());
        mFilterRuleViews.removeObject(ruleView);
        linkedLibrary.refresh();
    }
    
    // Remove leftover space
    int combinedFilterRuleViewHeight = FILTER_RULE_HEIGHT * libraryFileFilter.getFilterRules().size();
    combinedFilterRuleViewHeight = combinedFilterRuleViewHeight == 0 ? FILTER_RULE_HEIGHT : combinedFilterRuleViewHeight;
    setSize(FILTER_PANEL_WIDTH, FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight + PANEL_MARGIN / 2.0);
}
