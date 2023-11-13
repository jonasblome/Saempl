/*
 ==============================================================================
 
 FileFilterPanel.cpp
 Created: 29 Oct 2023 12:14:04pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "FileFilterPanel.h"

FileFilterPanel::FileFilterPanel(SaemplAudioProcessor& inProcessor)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
libraryFileFilter(sampleLibrary.getFileFilter())
{
    setPanelComponents();
}

FileFilterPanel::~FileFilterPanel()
{
    // Remove listeners for all rule views
    for (BlomeFileFilterRuleViewBase* ruleView : mFilterRuleViews)
    {
        ruleView->removeDeleteButtonListener(this);
    }
}

void FileFilterPanel::paint(Graphics &g)
{
    // Draw background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), style->CORNER_SIZE_MEDIUM);
    g.setColour(style->COLOUR_BLACK_LIGHT_TRANSPARENT);
    g.drawLine(style->PANEL_MARGIN / 2.0,
               style->FILTER_RULE_HEIGHT + 9,
               getWidth() - style->PANEL_MARGIN / 2.0,
               style->FILTER_RULE_HEIGHT + 9);
    
    // Draw hint message if rules are empty
    if (libraryFileFilter.getFilterRules().isEmpty())
    {
        g.setColour(style->COLOUR_ACCENT_LIGHT);
        g.setFont(style->FONT_SMALL_BOLD);
        g.drawFittedText("Add filter rules to see them here",
                         getLocalBounds().removeFromBottom(style->FILTER_RULE_HEIGHT),
                         Justification::centred,
                         2);
    }
    
    // Draw rule views
    int combinedFilterRuleViewHeight = 0;
    
    for (BlomeFileFilterRuleViewBase* newRuleView : mFilterRuleViews)
    {
        newRuleView->setBounds(style->PANEL_MARGIN / 2.0,
                               style->PANEL_MARGIN / 2.0 + style->FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight,
                               style->FILTER_RULE_WIDTH - style->PANEL_MARGIN,
                               style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN / 2.0);
        addAndMakeVisible(*newRuleView);
        combinedFilterRuleViewHeight += style->FILTER_RULE_HEIGHT;
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
                .add(std::make_unique<BlomeFileFilterRuleViewLoudness>(*dynamic_cast<SampleFileFilterRuleLoudness*>(rule),
                                                                       sampleLibrary));
            break;
        }
        case 1:
        {
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewLength>(*dynamic_cast<SampleFileFilterRuleLength*>(rule),
                                                                     sampleLibrary));
            break;
        }
        case 2:
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewTitle>(*dynamic_cast<SampleFileFilterRuleTitle*>(rule),
                                                                    sampleLibrary));
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
    if (mNewRuleTypeChooser->getSelectedItemIndex() == -1 || libraryFileFilter.getFilterRules().size() >= 8)
    {
        return;
    }
    
    // Add view for filter rule
    int combinedFilterRuleViewHeight = style->FILTER_RULE_HEIGHT * libraryFileFilter.getFilterRules().size();
    SampleFileFilterRuleBase* newRule;
    
    switch (mNewRuleTypeChooser->getSelectedItemIndex())
    {
        case 0:
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleTitle("Title"));
            break;
        case 1:
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleLength("Length"));
            break;
        case 2:
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleLoudness("Loudness"));
            break;
        default:
            jassertfalse;
            break;
    }
    
    generateRuleView(newRule);
    setSize(style->FILTER_PANEL_WIDTH,
            style->FILTER_PANEL_HEIGHT
            + combinedFilterRuleViewHeight + style->FILTER_RULE_HEIGHT
            + style->PANEL_MARGIN / 2.0);
    repaint();
    sampleLibrary.refresh();
}

void FileFilterPanel::removeFilterRule(SampleFileFilterRuleBase const & inFilterRule)
{
    libraryFileFilter.getFilterRules().removeObject(&inFilterRule);
}

void FileFilterPanel::setPanelComponents()
{
    // Add rule type combo box
    mNewRuleTypeChooser = std::make_unique<ComboBox>("NewRuleTypeChooser");
    mNewRuleTypeChooser->setBounds(style->PANEL_MARGIN / 2.0,
                                   style->PANEL_MARGIN / 2.0,
                                   style->COMBO_BOX_WIDTH_MEDIUM - style->PANEL_MARGIN / 4.0,
                                   style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN / 2.0);
    mNewRuleTypeChooser->addItem("New title rule", 1);
    mNewRuleTypeChooser->addItem("New length rule", 2);
    mNewRuleTypeChooser->addItem("New loudness rule", 3);
    mNewRuleTypeChooser->setTextWhenNothingSelected("Choose new rule type");
    addAndMakeVisible(*mNewRuleTypeChooser);
    
    // Add button for adding filter rules
    mAddFilterRuleButton = std::make_unique<BlomeImageButton>("Add filter", true);
    mAddFilterRuleButton->setImages(false,
                                    true,
                                    true,
                                    ImageCache::getFromMemory(BinaryData::add_FILL0_wght400_GRAD0_opsz24_png,
                                                              BinaryData::add_FILL0_wght400_GRAD0_opsz24_pngSize),
                                    style->BUTTON_IS_DEFAULT_ALPHA,
                                    style->COLOUR_SYMBOL_BUTTON,
                                    Image(),
                                    style->BUTTON_IS_OVER_ALPHA,
                                    style->COLOUR_SYMBOL_BUTTON,
                                    Image(),
                                    style->BUTTON_IS_DOWN_ALPHA,
                                    style->COLOUR_SYMBOL_BUTTON);
    mAddFilterRuleButton->setBounds(style->COMBO_BOX_WIDTH_MEDIUM + style->PANEL_MARGIN * 0.75,
                                    style->PANEL_MARGIN / 2.0,
                                    style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN / 2.0,
                                    style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN / 2.0);
    mAddFilterRuleButton->setTooltip("Add a new rule of the chosen type to the filter");
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
        combinedFilterRuleViewHeight += style->FILTER_RULE_HEIGHT;
    }
    
    // Add space for info text if there are no filter rules
    combinedFilterRuleViewHeight = combinedFilterRuleViewHeight == 0 ? style->FILTER_RULE_HEIGHT : combinedFilterRuleViewHeight;
    setSize(style->FILTER_PANEL_WIDTH, style->FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight + style->PANEL_MARGIN / 2.0);
}

void FileFilterPanel::buttonClicked(Button* button)
{
    // Delete rule and its view if its delete button was clicked
    if (BlomeFileFilterRuleViewBase* ruleView = dynamic_cast<BlomeFileFilterRuleViewBase*>(button->getParentComponent()))
    {
        ruleView->removeDeleteButtonListener(this);
        removeFilterRule(ruleView->getLinkedFilterRule());
        mFilterRuleViews.removeObject(ruleView);
        sampleLibrary.refresh();
        
        // Remove leftover space
        int combinedFilterRuleViewHeight = style->FILTER_RULE_HEIGHT * libraryFileFilter.getFilterRules().size();
        combinedFilterRuleViewHeight = combinedFilterRuleViewHeight == 0 ? style->FILTER_RULE_HEIGHT : combinedFilterRuleViewHeight;
        setSize(style->FILTER_PANEL_WIDTH, style->FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight + style->PANEL_MARGIN / 2.0);
    }
}
