/*
 ==============================================================================
 
 SampleGridPanel.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleGridPanel.h"

SampleGridPanel::SampleGridPanel(SaemplAudioProcessor& inProcessor, SampleItemPanel& inSampleItemPanel, AudioPlayer& inAudioPlayer)
:
PanelBase(inProcessor),
sampleLibrary(currentProcessor.getSampleLibrary()),
sampleItemPanel(inSampleItemPanel),
audioPlayer(inAudioPlayer)
{
    setSize(style->SAMPLE_NAVIGATION_PANEL_WIDTH, style->SAMPLE_NAVIGATION_PANEL_HEIGHT);
    setPanelComponents();
    setWantsKeyboardFocus(true);
}

SampleGridPanel::~SampleGridPanel()
{
    
}

void SampleGridPanel::centrePositionInGridViewport(Point<int>& newPosition)
{
    newPosition.addXY(-mGridViewport->getWidth() / 2, -mGridViewport->getHeight() / 2);
    mGridViewport->setViewPosition(newPosition);
}

void SampleGridPanel::selectRandomSample()
{
    Point<int > newPosition = mSampleGrid->selectRandomTile();
    centrePositionInGridViewport(newPosition);
}

void SampleGridPanel::paint(Graphics& g)
{
    // Set background
    g.setColour(style->COLOUR_ACCENT_MEDIUM);
    g.fillRoundedRectangle(getLocalBounds()
                           .toFloat()
                           .withTrimmedLeft(style->PANEL_MARGIN * 0.5)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.25)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.25)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.25),
                           style->CORNER_SIZE_MEDIUM);
    
    // Draw Title
    drawDropShadow(g,
                   Rectangle<int>(0, 0, getWidth(), 20),
                   0,
                   0,
                   70,
                   style);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.fillRoundedRectangle(getLocalBounds()
                           .removeFromTop(style->PANEL_TITLE_HEIGHT)
                           .withTrimmedLeft(style->PANEL_MARGIN)
                           .withTrimmedTop(style->PANEL_MARGIN * 0.75)
                           .withTrimmedRight(style->PANEL_MARGIN * 0.25 + style->PANEL_TITLE_HEIGHT)
                           .withTrimmedBottom(style->PANEL_MARGIN * 0.25)
                           .toFloat(),
                           style->CORNER_SIZE_MEDIUM);
    g.setFont(style->FONT_MEDIUM_SMALL_BOLD);
    g.setColour(style->COLOUR_PANEL_TITLE_FONT);
    g.drawFittedText("Grid View - "
                     + sampleLibrary.getCurrentLibraryPath()
                     + " - " + std::to_string(sampleLibrary.getSampleItems(FILTERED_SAMPLES).size()) + " Samples",
                     style->PANEL_MARGIN * 1.5,
                     0,
                     getWidth() - style->PANEL_MARGIN * 4 - style->PANEL_TITLE_HEIGHT,
                     style->PANEL_TITLE_HEIGHT,
                     Justification::centred,
                     1);
}

void SampleGridPanel::setPanelComponents()
{
    mSampleGrid = std::make_unique<BlomeSampleGridView>(currentProcessor, sampleItemPanel, audioPlayer);
    mSampleGrid->setZoomFactor(currentProcessor.getSampleGridZoomFactor());
    
    if (sampleLibrary.getLibraryWasLoaded())
    {
        mSampleGrid->setReadyForClustering();
        
        if (isShowing())
        {
            mSampleGrid->clusterGrid();
        }
    }
    
    mChangeGridClusteringOptionsButton = std::make_unique<BlomeImageButton>("ChangeClusteringOptionsButton", true);
    mChangeGridClusteringOptionsButton->setImages(false,
                                                  true,
                                                  true,
                                                  ImageCache::getFromMemory(BinaryData::settings_FILL0_wght400_GRAD0_opsz24_png,
                                                                            BinaryData::settings_FILL0_wght400_GRAD0_opsz24_pngSize),
                                                  style->BUTTON_IS_DEFAULT_ALPHA,
                                                  style->COLOUR_SYMBOL_BUTTON,
                                                  Image(),
                                                  style->BUTTON_IS_OVER_ALPHA,
                                                  style->COLOUR_SYMBOL_BUTTON,
                                                  Image(),
                                                  style->BUTTON_IS_DOWN_ALPHA,
                                                  style->COLOUR_SYMBOL_BUTTON);
    mChangeGridClusteringOptionsButton->setTooltip("Change the clustering options here");
    mChangeGridClusteringOptionsButton->onClick = [this]
    {
        std::unique_ptr<SampleGridOptionsPanel> optionsPanel = std::make_unique<SampleGridOptionsPanel>(currentProcessor);
        CallOutBox::launchAsynchronously(std::move(optionsPanel), mChangeGridClusteringOptionsButton->getScreenBounds(), nullptr);
    };
    addAndMakeVisible(*mChangeGridClusteringOptionsButton);
    
    // Add zoom slider
    int sliderWidth = 150;
    mZoomSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    mZoomSlider->setBounds(style->PANEL_MARGIN,
                           style->PANEL_MARGIN * 0.25 + style->PANEL_TITLE_HEIGHT,
                           sliderWidth,
                           style->PANEL_TITLE_HEIGHT * 0.5);
    mZoomSlider->setRange(0, 1, 0.5);
    mZoomSlider->setValue(currentProcessor.getSampleGridZoomFactor(), NotificationType::dontSendNotification);
    mZoomSlider->onValueChange = [this]
    {
        // Perform zoom while maintaining view position
        float tileMinMaxRelation = mSampleGrid->getTileMinMaxRelation();
        float oldZoomFactor = currentProcessor.getSampleGridZoomFactor();
        
        // Set new zoom factor
        float newZoomFactor = mZoomSlider->getValue();
        currentProcessor.setSampleGridZoomFactor(newZoomFactor);
        mSampleGrid->setZoomFactor(newZoomFactor);
        float newOldZoomRatio =
        (1 + (newZoomFactor * (tileMinMaxRelation - 1)))
        / (1 + (oldZoomFactor * (tileMinMaxRelation - 1)));
        
        if (sampleLibrary.getSampleItems(FILTERED_SAMPLES).isEmpty())
        {
            return;
        }
        
        // Resize grid
        Point<float> viewPosition = mGridViewport->getViewPosition().toFloat();
        viewPosition.addXY(mGridViewport->getWidth() * 1.0 / 2, mGridViewport->getHeight() * 1.0 / 2);
        viewPosition.setXY(viewPosition.getX() * newOldZoomRatio,
                           viewPosition.getY() * newOldZoomRatio);
        mSampleGrid->performGridLayout();
        
        // Restore centre view position
        viewPosition.addXY(-mGridViewport->getWidth() * 1.0 / 2, -mGridViewport->getHeight() * 1.0 / 2);
        mGridViewport->setViewPosition(viewPosition.roundToInt());
    };
    mZoomSlider->setTooltip("Scales the size of the grid tiles");
    addAndMakeVisible(*mZoomSlider);
    
    // Add grid viewport
    mGridViewport = std::make_unique<Viewport>();
    mGridViewport->setViewedComponent(&*mSampleGrid, false);
    mGridViewport->setWantsKeyboardFocus(false);
    addAndMakeVisible(*mGridViewport);
    
    resizePanelComponents();
}

void SampleGridPanel::resizePanelComponents()
{
    if (mGridViewport != nullptr)
    {
        mGridViewport->setBounds(style->PANEL_MARGIN,
                                 style->PANEL_TITLE_HEIGHT * 1.5 + style->PANEL_MARGIN * 0.75,
                                 getWidth() - style->PANEL_MARGIN * 1.75,
                                 getHeight() - style->PANEL_TITLE_HEIGHT * 1.5 - style->PANEL_MARGIN * 1.5);
    }
    
    if (mChangeGridClusteringOptionsButton != nullptr)
    {
        mChangeGridClusteringOptionsButton->setBounds(getWidth() - style->PANEL_TITLE_HEIGHT + style->PANEL_MARGIN * 0.25,
                                                      style->PANEL_MARGIN * 0.75,
                                                      style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN,
                                                      style->PANEL_TITLE_HEIGHT - style->PANEL_MARGIN);
    }
}

void SampleGridPanel::visibilityChanged()
{
    if (isVisible())
    {
        mSampleGrid->clusterGrid();
    }
}

bool SampleGridPanel::keyPressed(KeyPress const & key)
{
    if (!isShowing())
    {
        return false;
    }
    
    int keyCode = key.getKeyCode();
    
    if (keyCode == KeyPress::returnKey)
    {
        mSampleGrid->loadSelectedTileIntoAudioPlayer();
        return true;
    }
    else if (keyCode == KeyPress::escapeKey)
    {
        mSampleGrid->deselectAll();
        return true;
    }
    else if (key.getModifiers().isCommandDown() && keyCode == 65) // Cmd + A
    {
        mSampleGrid->selectAll();
        return true;
    }
    else if (keyCode == KeyPress::leftKey)
    {
        Point<int > newPosition = mSampleGrid->selectLeft();
        centrePositionInGridViewport(newPosition);
        return true;
    }
    else if (keyCode == KeyPress::upKey)
    {
        Point<int > newPosition = mSampleGrid->selectUp();
        centrePositionInGridViewport(newPosition);
        return true;
    }
    else if (keyCode == KeyPress::rightKey)
    {
        Point<int > newPosition = mSampleGrid->selectRight();
        centrePositionInGridViewport(newPosition);
        return true;
    }
    else if (keyCode == KeyPress::downKey)
    {
        Point<int > newPosition = mSampleGrid->selectDown();
        centrePositionInGridViewport(newPosition);
        return true;
    }
    else if (key.getKeyCode() == 75) // K
    {
        mSampleGrid->playSelectedSample();
        return true;
    }
    
    return false;
}
