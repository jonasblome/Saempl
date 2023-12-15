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
}

BlomeSampleItemTileView::~BlomeSampleItemTileView()
{
    
}

void BlomeSampleItemTileView::paint(Graphics& g)
{
    // Draw outline
    Rectangle<float> outline = getLocalBounds().toFloat().reduced(0.5);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawRoundedRectangle(outline, style->CORNER_SIZE_MEDIUM, 1.0);
    
    // Draw sample title
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawFittedText(sampleItem->getTitle(),
                     getLocalBounds().reduced(style->PANEL_MARGIN),
                     Justification::centred,
                     5);
}

void BlomeSampleItemTileView::mouseDoubleClick(const MouseEvent& event)
{
    File inFile = sampleItem->getFilePath();
    
    if (!sampleItemPanel.tryShowAudioResource(inFile))
    {
        showFileDeletedWarning();
    }
}

String BlomeSampleItemTileView::getSampleItemFilePath()
{
    return sampleItemFilePath;
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

void BlomeSampleItemTileView::mouseUp(MouseEvent const & mouseEvent)
{
    // Show options pop up menu
    if (mouseEvent.mods.isRightButtonDown())
    {
        PopupMenu popupMenu;
        popupMenu.addItem("Move File(s) to Trash", [this] { deleteFiles(false); });
        popupMenu.addItem("Add Sample(s) to Favorites", [this] { addToPalette(); });
        popupMenu.addItem("Re-analyse Sample(s)", [this] { reanalyseSamples(); });
        popupMenu.addItem("Delete File(s) Permanently", [this] { deleteFiles(true); });
        popupMenu.showMenuAsync(PopupMenu::Options{}.withMousePosition());
    }
}

void BlomeSampleItemTileView::deleteFiles(bool deletePermanently = false)
{
    sampleLibrary.removeSampleItem(sampleItem->getFilePath(), deletePermanently);
}

void BlomeSampleItemTileView::addToPalette()
{
    sampleLibrary.addAllToPalette(sampleItem->getFilePath());
}

void BlomeSampleItemTileView::reanalyseSamples()
{
    sampleLibrary.reanalyseSampleItem(sampleItem->getFilePath());
}
