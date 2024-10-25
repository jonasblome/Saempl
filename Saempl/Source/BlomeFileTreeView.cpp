/*
 ==============================================================================
 
 BlomeFileTreeView.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileTreeView.h"

BlomeFileTreeView::BlomeFileTreeView(SampleLibrary& inSampleLibrary,
                                     AudioPlayer& inAudioPlayer)
:
FileTreeComponent(inSampleLibrary.getDirectoryList()),
sampleLibrary(inSampleLibrary),
audioPlayer(inAudioPlayer)
{
    // Mouse listener catches events from the tree view's children
    addMouseListener(this, true);
}

BlomeFileTreeView::~BlomeFileTreeView()
{
    removeMouseListener(this);
}

void BlomeFileTreeView::filesDropped(StringArray const & files, int x, int y)
{
    // Adding all the dropped files to the database
    sampleLibrary.addAllToSampleItems(files);
}

bool BlomeFileTreeView::isInterestedInFileDrag(StringArray const & files)
{
    return true;
}

void BlomeFileTreeView::changeListenerCallback(ChangeBroadcaster* source)
{
    // Repaint when the directory list changes
    repaint();
}

void BlomeFileTreeView::mouseDrag(MouseEvent const & e)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (e.getLengthOfMousePress() > 100)
    {
        Point<int> mousePosition = e.getEventRelativeTo(this).position.toInt();
        
        // Check if any of the selected items was dragged
        for (int i = 0; i < getNumSelectedItems(); i++)
        {
            Rectangle<int> itemBounds = getSelectedItem(i)->getItemPosition(true);
            
            if (itemBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                // Add all selected files to external drag
                for (int f = 0; f < getNumSelectedFiles(); f++)
                {
                    selectedFilePaths.add(getSelectedFile(f).getFullPathName());
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}

void BlomeFileTreeView::playSelectedSample()
{
    int selectedItemIndex = getNumSelectedFiles() - 1;
    
    if (selectedItemIndex == -1)
    {
        return;
    }
    
    File sampleFile = getSelectedFile(selectedItemIndex);
    
    // Load file into source
    if (sampleFile.exists() && !sampleFile.isDirectory() && isSupportedAudioFileFormat(sampleFile.getFileExtension()))
    {
        audioPlayer.loadURLIntoTransport(URL(sampleFile));
        audioPlayer.setTransportSourcePosition(0.0);
        audioPlayer.start();
    }
    
    if (!sampleFile.exists())
    {
        audioPlayer.emptyTransport();
        sampleLibrary.refreshLibrary();
        AlertWindow::showAsync(MessageBoxOptions()
                               .withIconType(MessageBoxIconType::NoIcon)
                               .withTitle("File not available!")
                               .withMessage("This file has probably been externally deleted and was removed from the list of available samples.")
                               .withButton("OK"),
                               nullptr);
    }
}
