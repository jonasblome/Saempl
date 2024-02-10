/*
 ==============================================================================
 
 BlomeSampleItemTileView.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeSampleItemTileView.h"

BlomeSampleItemTileView::BlomeSampleItemTileView(SampleItem* inSampleItem, SampleLibrary& inSampleLibrary, SampleItemPanel& inSampleItemPanel)
:
sampleItem(inSampleItem),
sampleLibrary(inSampleLibrary),
sampleItemPanel(inSampleItemPanel)
{
    sampleItemFilePath = sampleItem->getFilePath();
    isSelected = false;
}

BlomeSampleItemTileView::~BlomeSampleItemTileView()
{
    
}

String BlomeSampleItemTileView::getSampleItemFilePath()
{
    return sampleItemFilePath;
}

void BlomeSampleItemTileView::setSelected(bool inIsSelected)
{
    if (sampleItem->getTitle() == "")
    {
        return;
    }
    
    isSelected = inIsSelected;
}

bool BlomeSampleItemTileView::getIsSelected()
{
    return isSelected;
}

void BlomeSampleItemTileView::paint(Graphics& g)
{
    String title = sampleItem->getTitle();
    
    // Don't draw an empty tile
    if (title == "")
    {
        return;
    }
    
    Rectangle<float> bounds = getLocalBounds().toFloat().reduced(0.5);
    
    // Draw background
    if (isSelected)
    {
        g.setColour(style->COLOUR_ACCENT_LIGHT);
        g.fillRoundedRectangle(bounds, style->CORNER_SIZE_MEDIUM);
    }
    else
    {
        g.setColour(style->COLOUR_ACCENT_DARK);
        g.drawRoundedRectangle(bounds, style->CORNER_SIZE_MEDIUM, 1.0);
    }
    
    // Draw sample text
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_DARK);
    
    int maxTitleLength = 30;
    if (title.length() > maxTitleLength)
    {
        title = title.substring(0, maxTitleLength - 3) + "...";
    }
    
    g.drawFittedText(title,
                     bounds.reduced(style->PANEL_MARGIN).toNearestInt(),
                     Justification::topLeft,
                     5);
    
    // Make text dependent on tile size
    String other = "";
    int currentWidth = getWidth();
    
    if (currentWidth > 90)
    {
        other = other + "\n\n" + " - Key: " + KEY_INDEX_TO_KEY_NAME[sampleItem->getKey()];
    }
    if (currentWidth > 105)
    {
        other = other + "\n" + " - Tempo: " + std::to_string(sampleItem->getTempo());
    }
    if (currentWidth > 130)
    {
        other = other + "\n" + " - Length: " + std::to_string(sampleItem->getLength());
        other = other + "\n" + " - LUFS: " + std::to_string(sampleItem->getLoudnessLUFS());
        other = other + "\n" + " - dB: " + std::to_string(sampleItem->getLoudnessDecibel());
    }
    
    g.drawFittedText(other,
                     bounds.reduced(style->PANEL_MARGIN).removeFromBottom(getHeight() - 65).toNearestInt(),
                     Justification::topLeft,
                     5);
}

void BlomeSampleItemTileView::mouseDoubleClick(const MouseEvent& event)
{
    File inFile = sampleItem->getFilePath();
    
    if (sampleItem->getTitle() != "")
    {
        sampleItemPanel.tryShowAudioResource(inFile);
    }
}

void BlomeSampleItemTileView::mouseDrag(MouseEvent const & mouseEvent)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (mouseEvent.getLengthOfMousePress() > 100)
    {
        Point<int> mousePosition = mouseEvent.getEventRelativeTo(this).position.toInt();
        
        if (getLocalBounds().contains(mousePosition))
        {
            StringArray selectedFilePaths;
            selectedFilePaths.add(sampleItem->getFilePath());
            DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
            dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
        }
    }
}
