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
    Rectangle<float> outline = getLocalBounds().toFloat().reduced(0.5);
    
    // Draw outline
    if (sampleItem->getTitle() != "")
    {
        g.setColour(style->COLOUR_ACCENT_DARK);
        g.drawRoundedRectangle(outline, style->CORNER_SIZE_MEDIUM, 1.0);
    }
    
    // Draw sample title
    g.setFont(style->FONT_SMALL_BOLD);
    g.setColour(style->COLOUR_ACCENT_DARK);
    g.drawFittedText(sampleItem->getTitle()
                     /*+ "\n" + "Length: " + std::to_string(sampleItem->getFeatureVector()[0])
                      + "\n" + "LUFS: " + std::to_string(sampleItem->getFeatureVector()[1])
                      + "\n" + "Start: " + std::to_string(sampleItem->getFeatureVector()[2])
                      + "\n" + "End: " + std::to_string(sampleItem->getFeatureVector()[3])
                      + "\n" + "ZCR: " + std::to_string(sampleItem->getFeatureVector()[4])
                      + "\n" + "Tempo: " + std::to_string(sampleItem->getFeatureVector()[5])
                      + "\n" + "Key: " + std::to_string(sampleItem->getFeatureVector()[6])
                      + "\n" + "Centroid: " + std::to_string(sampleItem->getFeatureVector()[7])
                      + "\n" + "Spread: " + std::to_string(sampleItem->getFeatureVector()[8])
                      + "\n" + "Rolloff: " + std::to_string(sampleItem->getFeatureVector()[9])
                      + "\n" + "Spec.flux: " + std::to_string(sampleItem->getFeatureVector()[10])
                      + "\n" + "Chrom.flux: " + std::to_string(sampleItem->getFeatureVector()[11])*/,
                     outline.reduced(style->PANEL_MARGIN).toNearestInt(),
                     Justification::centred,
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
    if (sampleItem->getTitle() != "" && mouseEvent.mods.isRightButtonDown())
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
