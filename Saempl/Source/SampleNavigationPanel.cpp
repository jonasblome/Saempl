/*
 ==============================================================================
 
 SampleNavigationPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleNavigationPanel.h"

SampleNavigationPanel::SampleNavigationPanel(SaemplAudioProcessor& inProcessor,
                                             SampleItemPanel& inSampleItemPanel,
                                             AudioPlayer& inAudioPlayer)
:
PanelBase(inProcessor),
audioPlayer(inAudioPlayer),
sampleItemPanel(inSampleItemPanel)
{
    // Set components
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    setPanelComponents();
}

SampleNavigationPanel::~SampleNavigationPanel()
{
    
}

void SampleNavigationPanel::paint(Graphics& g)
{
    
}

void SampleNavigationPanel::setPanelComponents()
{
    // Add library panel
    mSampleFolderPanel = std::make_unique<SampleFolderPanel>(currentProcessor, sampleItemPanel, audioPlayer);
    addChildComponent(*mSampleFolderPanel);
    
    // Add sample table panel
    mSampleTablePanel = std::make_unique<SampleTablePanel>(currentProcessor, sampleItemPanel, audioPlayer);
    addChildComponent(*mSampleTablePanel);
    
    // Add sample table panel
    mSampleGridPanel = std::make_unique<SampleGridPanel>(currentProcessor, sampleItemPanel, audioPlayer);
    addChildComponent(*mSampleGridPanel);
    
    // Set active panel
    setActiveNavigationPanel(currentProcessor.getActiveNavigationPanel());
    resizePanelComponents();
}

void SampleNavigationPanel::resizePanelComponents()
{
    if (mSampleFolderPanel != nullptr)
    {
        mSampleFolderPanel->setBounds(0, 0, getWidth(), getHeight());
    }
    
    if (mSampleTablePanel != nullptr)
    {
        mSampleTablePanel->setBounds(0, 0, getWidth(), getHeight());
    }
    
    if (mSampleGridPanel != nullptr)
    {
        mSampleGridPanel->setBounds(0, 0, getWidth(), getHeight());
    }
}

void SampleNavigationPanel::setActiveNavigationPanel(NavigationPanelType& inPanelType)
{
    currentProcessor.setActiveNavigationPanel(inPanelType);
    
    switch(currentProcessor.getActiveNavigationPanel())
    {
        case PANELS_LIBRARY_PANEL:
        {
            mSampleFolderPanel->setVisible(true);
            mSampleTablePanel->setVisible(false);
            mSampleGridPanel->setVisible(false);
            break;
        }
        case PANELS_TABLE_PANEL:
        {
            mSampleFolderPanel->setVisible(false);
            mSampleTablePanel->setVisible(true);
            mSampleGridPanel->setVisible(false);
            break;
        }
        case PANELS_GRID_PANEL:
        {
            mSampleFolderPanel->setVisible(false);
            mSampleTablePanel->setVisible(false);
            mSampleGridPanel->setVisible(true);
            break;
        }
        default:
        {
            jassertfalse;
            break;
        }
    }
}

void SampleNavigationPanel::selectRandomSample()
{
    switch(currentProcessor.getActiveNavigationPanel())
    {
        case PANELS_LIBRARY_PANEL:
        {
            mSampleTablePanel->selectRandomSample();
            break;
        }
        case PANELS_TABLE_PANEL:
        {
            mSampleTablePanel->selectRandomSample();
            break;
        }
        case PANELS_GRID_PANEL:
        {
            mSampleGridPanel->selectRandomSample();
            break;
        }
        default:
        {
            jassertfalse;
            break;
        }
    }
}

bool SampleNavigationPanel::keyPressed(KeyPress const & key)
{
    bool pressWasHandled = false;
    
    if (mSampleFolderPanel->keyPressed(key)
        || mSampleTablePanel->keyPressed(key)
        || mSampleGridPanel->keyPressed(key))
    {
        pressWasHandled = true;
    }
    
    return pressWasHandled;
}

void SampleNavigationPanel::showSample(String inFilePath)
{
    mSampleFolderPanel->showSample(inFilePath);
    mSampleTablePanel->showSample(inFilePath);
    mSampleGridPanel->showSample(inFilePath);
}
