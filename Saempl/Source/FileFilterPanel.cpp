/*
 ==============================================================================
 
 FileFilterPanel.cpp
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
                .add(std::make_unique<BlomeFileFilterRuleViewTitle>(*dynamic_cast<SampleFileFilterRuleTitle*>(rule),
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
        {
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewLoudnessDecibel>(*dynamic_cast<SampleFileFilterRuleLoudnessDecibel*>(rule),
                                                                              sampleLibrary));
            break;
        }
        case 3:
        {
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewLoudnessLUFS>(*dynamic_cast<SampleFileFilterRuleLoudnessLUFS*>(rule),
                                                                           sampleLibrary));
            break;
        }
        case 4:
        {
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewTempo>(*dynamic_cast<SampleFileFilterRuleTempo*>(rule),
                                                                    sampleLibrary));
            break;
        }
        case 5:
        {
            newRuleView = mFilterRuleViews
                .add(std::make_unique<BlomeFileFilterRuleViewKey>(*dynamic_cast<SampleFileFilterRuleKey*>(rule),
                                                                  sampleLibrary));
            break;
        }
        default:
            jassertfalse;
            break;
    }
    
    newRuleView->addDeleteButtonListener(this);
}

void FileFilterPanel::addFilterRuleView()
{
    // Only add rule if type is selected, add maximum of 8 rules (reason: glitch in filter panel if more)
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
        {
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleTitle(PROPERTY_NAMES[0]));
            break;
        }
        case 1:
        {
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleLength(PROPERTY_NAMES[1]));
            break;
        }
        case 2:
        {
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleLoudnessDecibel(PROPERTY_NAMES[2]));
            break;
        }
        case 3:
        {
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleLoudnessLUFS(PROPERTY_NAMES[3]));
            break;
        }
        case 4:
        {
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleTempo(PROPERTY_NAMES[4]));
            break;
        }
        case 5:
        {
            newRule = libraryFileFilter.addFilterRule(new SampleFileFilterRuleKey(PROPERTY_NAMES[5]));
            break;
        }
        default:
        {
            jassertfalse;
            break;
        }
    }
    
    generateRuleView(newRule);
    setSize(style->FILTER_PANEL_WIDTH,
            style->FILTER_PANEL_HEIGHT
            + combinedFilterRuleViewHeight + style->FILTER_RULE_HEIGHT
            + style->PANEL_MARGIN / 2.0);
    repaint();
    
    if (libraryFileFilter.canHaveEffect())
    {
        sampleLibrary.refresh();
    }
}

void FileFilterPanel::removeFilterRule(SampleFileFilterRuleBase const & inFilterRule)
{
    libraryFileFilter.getFilterRules().removeObject(&inFilterRule);
}

void FileFilterPanel::setAddFilterRuleButton()
{
    mAddFilterRuleButton = std::make_unique<BlomeImageButton>("AddFilterRule", true);
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
}

void FileFilterPanel::setNewRuleTypeChooser() {
    mNewRuleTypeChooser = std::make_unique<ComboBox>("NewRuleTypeChooser");
    mNewRuleTypeChooser->setBounds(style->PANEL_MARGIN / 2.0,
                                   style->PANEL_MARGIN / 2.0,
                                   style->COMBO_BOX_WIDTH_MEDIUM - style->PANEL_MARGIN / 4.0,
                                   style->FILTER_RULE_HEIGHT - style->PANEL_MARGIN / 2.0);
    mNewRuleTypeChooser->addItem("New title rule", 1);
    mNewRuleTypeChooser->addItem("New length rule", 2);
    mNewRuleTypeChooser->addItem("New loudness(dB) rule", 3);
    mNewRuleTypeChooser->addItem("New loudness(LUFS) rule", 4);
    mNewRuleTypeChooser->addItem("New tempo rule", 5);
    mNewRuleTypeChooser->addItem("New key rule", 6);
    mNewRuleTypeChooser->setTextWhenNothingSelected("Choose new rule type");
    addAndMakeVisible(*mNewRuleTypeChooser);
}

void FileFilterPanel::setPanelComponents()
{
    // Add rule type combo box
    setNewRuleTypeChooser();
    
    // Add button for adding filter rules
    setAddFilterRuleButton();
    
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
        bool ruleCouldHaveEffect = ruleView->getFilterRule().canHaveEffect();
        bool filterCouldHaveEffect = libraryFileFilter.canHaveEffect();
        ruleView->removeDeleteButtonListener(this);
        removeFilterRule(ruleView->getFilterRule());
        mFilterRuleViews.removeObject(ruleView);
        
        if (ruleCouldHaveEffect && filterCouldHaveEffect)
        {
            sampleLibrary.refresh();
        }
        
        // Remove leftover space
        int combinedFilterRuleViewHeight = style->FILTER_RULE_HEIGHT * libraryFileFilter.getFilterRules().size();
        combinedFilterRuleViewHeight = combinedFilterRuleViewHeight == 0 ? style->FILTER_RULE_HEIGHT : combinedFilterRuleViewHeight;
        setSize(style->FILTER_PANEL_WIDTH, style->FILTER_PANEL_HEIGHT + combinedFilterRuleViewHeight + style->PANEL_MARGIN / 2.0);
    }
}
