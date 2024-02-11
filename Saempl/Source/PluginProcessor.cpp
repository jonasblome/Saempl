/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaemplAudioProcessor::SaemplAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  )
#endif
{
    mSampleLibrary = std::make_unique<SampleLibrary>();
    mActiveNavigationPanelType = PANELS_LIBRARY_PANEL;
    mSortingColumnTitle = "Title";
    mSortingDirection = true;
    mSampleItemPanelIsVisible = true;
    mFollowAudioPlayhead = false;
    mSampleGridZoomFactor = 0.0;
}

SaemplAudioProcessor::~SaemplAudioProcessor()
{
    
}

//==============================================================================
const juce::String SaemplAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SaemplAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SaemplAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SaemplAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SaemplAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SaemplAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SaemplAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SaemplAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SaemplAudioProcessor::getProgramName (int index)
{
    return {};
}

void SaemplAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SaemplAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SaemplAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SaemplAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void SaemplAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // auto* channelData = buffer.getWritePointer (channel);
        // ..do something to the data...
    }
}

//==============================================================================
bool SaemplAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SaemplAudioProcessor::createEditor()
{
    return new SaemplAudioProcessorEditor(*this);
}

//==============================================================================
void SaemplAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    XmlElement stateInfo("Blome_StateInfo");
    XmlElement* stateInfoBody = new XmlElement("Blome_StateInfoBody");
    
    // Storing states
    stateInfoBody->setAttribute("ActiveNavigationPanel", NAVIGATION_PANEL_TYPE_TO_STRING[mActiveNavigationPanelType]);
    stateInfoBody->setAttribute("SortingColumnTitle", mSortingColumnTitle);
    stateInfoBody->setAttribute("SortingDirection", mSortingDirection);
    stateInfoBody->setAttribute("SampleItemPanelIsVisible", mSampleItemPanelIsVisible);
    stateInfoBody->setAttribute("FollowAudioPlayhead", mFollowAudioPlayhead);
    stateInfoBody->setAttribute("SampleGridZoomFactor", mSampleGridZoomFactor);
    stateInfoBody->setAttribute("LastOpenedLibrary", mLastOpenedLibrary);
    
    // Storing filter rules
    XmlElement* stateInfoFilter = new XmlElement("Blome_StateInfoFilter");
    stateInfoFilter->setAttribute("FilterIsActivated", mFilterIsActivated);
    
    int i = 0;
    for (SampleFileFilterRuleBase* rule : mSampleLibrary->getFileFilter().getFilterRules())
    {
        String stateRuleName = "Blome_StateInfoFilterRule" + std::to_string(i++);
        XmlElement* stateInfoFilterRule = new XmlElement(stateRuleName);
        
        // Storing rule parameters
        stateInfoFilterRule->setAttribute("PropertyName", rule->getRulePropertyName());
        stateInfoFilterRule->setAttribute("RuleIsActive", rule->canHaveEffect());
        stateInfoFilterRule->setAttribute("CompareOperator", COMPARE_OPERATOR_TO_STRING[rule->getCompareOperator()]);
        
        switch(PROPERTY_NAMES.indexOf(rule->getRulePropertyName()))
        {
            case 0:
            {
                stateInfoFilterRule->setAttribute("CompareValue", dynamic_cast<SampleFileFilterRuleTitle*>(rule)->getCompareValue());
                break;
            }
            case 1:
            {
                stateInfoFilterRule->setAttribute("CompareValue", dynamic_cast<SampleFileFilterRuleLength*>(rule)->getCompareValue());
                break;
            }
            case 2:
            {
                stateInfoFilterRule->setAttribute("CompareValue", dynamic_cast<SampleFileFilterRuleLoudnessDecibel*>(rule)->getCompareValue());
                break;
            }
            case 3:
            {
                stateInfoFilterRule->setAttribute("CompareValue", dynamic_cast<SampleFileFilterRuleLoudnessLUFS*>(rule)->getCompareValue());
                break;
            }
            case 4:
            {
                stateInfoFilterRule->setAttribute("CompareValue", dynamic_cast<SampleFileFilterRuleTempo*>(rule)->getCompareValue());
                break;
            }
            case 5:
            {
                stateInfoFilterRule->setAttribute("CompareValue", dynamic_cast<SampleFileFilterRuleKey*>(rule)->getCompareValue());
                break;
            }
            default:
            {
                jassertfalse;
                break;
            }
        }
        
        stateInfoFilter->addChildElement(stateInfoFilterRule);
    }
    
    stateInfo.addChildElement(stateInfoBody);
    stateInfo.addChildElement(stateInfoFilter);
    copyXmlToBinary(stateInfo, destData);
}

void SaemplAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::XmlElement xmlState = *getXmlFromBinary(data, sizeInBytes);
    juce::XmlElement* xmlStatePtr = &xmlState;
    
    if (xmlStatePtr)
    {
        XmlElement* stateInfoBody = xmlStatePtr->getChildByName("Blome_StateInfoBody");
        
        if (stateInfoBody)
        {
            mActiveNavigationPanelType = STRING_TO_NAVIGATION_PANEL_TYPE[stateInfoBody->getStringAttribute("ActiveNavigationPanel")];
            mSortingColumnTitle = stateInfoBody->getStringAttribute("SortingColumnTitle");
            mSortingDirection = stateInfoBody->getBoolAttribute("SortingDirection");
            mSampleItemPanelIsVisible = stateInfoBody->getBoolAttribute("SampleItemPanelIsVisible");
            mFollowAudioPlayhead = stateInfoBody->getBoolAttribute("FollowAudioPlayhead");
            mSampleGridZoomFactor = stateInfoBody->getDoubleAttribute("SampleGridZoomFactor");
            mLastOpenedLibrary = stateInfoBody->getStringAttribute("LastOpenedLibrary");
        }
        
        XmlElement* stateInfoFilter = xmlStatePtr->getChildByName("Blome_StateInfoFilter");
        
        if (stateInfoFilter)
        {
            mFilterIsActivated = stateInfoFilter->getBoolAttribute("FilterIsActivated");
            
            for (auto* filterRule: stateInfoFilter->getChildIterator())
            {
                SampleFileFilterRuleBase* newRule;
                String propertyName = filterRule->getStringAttribute("PropertyName");
                
                switch (PROPERTY_NAMES.indexOf(propertyName))
                {
                    case 0:
                    {
                        newRule = mSampleLibrary->getFileFilter().addFilterRule(new SampleFileFilterRuleTitle(propertyName));
                        dynamic_cast<SampleFileFilterRuleTitle*>(newRule)->setCompareValue(filterRule->getStringAttribute("CompareValue"));
                        break;
                    }
                    case 1:
                    {
                        newRule = mSampleLibrary->getFileFilter().addFilterRule(new SampleFileFilterRuleLength(propertyName));
                        dynamic_cast<SampleFileFilterRuleLength*>(newRule)->setCompareValue(filterRule->getDoubleAttribute("CompareValue"));
                        break;
                    }
                    case 2:
                    {
                        newRule = mSampleLibrary->getFileFilter().addFilterRule(new SampleFileFilterRuleLoudnessDecibel(propertyName));
                        dynamic_cast<SampleFileFilterRuleLoudnessDecibel*>(newRule)->setCompareValue(filterRule->getDoubleAttribute("CompareValue"));
                        break;
                    }
                    case 3:
                    {
                        newRule = mSampleLibrary->getFileFilter().addFilterRule(new SampleFileFilterRuleLoudnessLUFS(propertyName));
                        dynamic_cast<SampleFileFilterRuleLoudnessLUFS*>(newRule)->setCompareValue(filterRule->getDoubleAttribute("CompareValue"));
                        break;
                    }
                    case 4:
                    {
                        newRule = mSampleLibrary->getFileFilter().addFilterRule(new SampleFileFilterRuleTempo(propertyName));
                        dynamic_cast<SampleFileFilterRuleTempo*>(newRule)->setCompareValue(filterRule->getIntAttribute("CompareValue"));
                        break;
                    }
                    case 5:
                    {
                        newRule = mSampleLibrary->getFileFilter().addFilterRule(new SampleFileFilterRuleKey(propertyName));
                        dynamic_cast<SampleFileFilterRuleKey*>(newRule)->setCompareValue(filterRule->getIntAttribute("CompareValue"));
                        break;
                    }
                    default:
                    {
                        jassertfalse;
                        break;
                    }
                }
                
                newRule->setIsActive(filterRule->getBoolAttribute("RuleIsActive"));
                newRule->setCompareOperator(STRING_TO_COMPARE_OPERATORS[filterRule->getStringAttribute("CompareOperator")]);
            }
            
            if (mSampleLibrary->getFileFilter().canHaveEffect())
            {
                mSampleLibrary->refresh();
            }
        }
    }
    else
    {
        jassertfalse;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaemplAudioProcessor();
}


SampleLibrary& SaemplAudioProcessor::getSampleLibrary()
{
    return *mSampleLibrary;
}

NavigationPanelType& SaemplAudioProcessor::getActiveNavigationPanel()
{
    return mActiveNavigationPanelType;
}

void SaemplAudioProcessor::setActiveNavigationPanel(NavigationPanelType& inPanelType)
{
    mActiveNavigationPanelType = inPanelType;
}

String SaemplAudioProcessor::getSortingColumnTitle()
{
    return mSortingColumnTitle;
}

void SaemplAudioProcessor::setSortingColumnTitle(String inColumnTitle)
{
    mSortingColumnTitle = inColumnTitle;
}

String SaemplAudioProcessor::getLastOpenedLibrary()
{
    return mLastOpenedLibrary;
}

void SaemplAudioProcessor::setLastOpenedLibrary(String inLastOpenedLibraryPath)
{
    mLastOpenedLibrary = inLastOpenedLibraryPath;
}

bool SaemplAudioProcessor::getSortingDirection()
{
    return mSortingDirection;
}

void SaemplAudioProcessor::setSortingDirection(bool inDirection)
{
    mSortingDirection = inDirection;
}

bool SaemplAudioProcessor::getSampleItemPanelIsVisible()
{
    return mSampleItemPanelIsVisible;
}

void SaemplAudioProcessor::setSampleItemIsVisible(bool inPanelIsVisible)
{
    mSampleItemPanelIsVisible = inPanelIsVisible;
}

bool SaemplAudioProcessor::getFollowAudioPlayhead()
{
    return mFollowAudioPlayhead;
}

void SaemplAudioProcessor::setFollowAudioPlayhead(bool inFollowAudioPlayhead)
{
    mFollowAudioPlayhead = inFollowAudioPlayhead;
}

bool SaemplAudioProcessor::getFilterIsActivated()
{
    return mFilterIsActivated;
}

void SaemplAudioProcessor::setFilterIsActive(bool inFilterIsActivated)
{
    mFilterIsActivated = inFilterIsActivated;
}

float SaemplAudioProcessor::getSampleGridZoomFactor()
{
    return mSampleGridZoomFactor;
}

void SaemplAudioProcessor::setSampleGridZoomFactor(float inZoomFactor)
{
    mSampleGridZoomFactor = inZoomFactor;
}
