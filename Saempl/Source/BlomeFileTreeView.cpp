/*
  ==============================================================================

    BlomeFileTreeView.cpp
    Created: 3 Jun 2023 4:59:52pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "BlomeFileTreeView.h"

BlomeFileTreeView::BlomeFileTreeView(SampleDatabaseTableViewModel& inSampleDatabaseTableViewModel)
:   FileTreeComponent(*inSampleDatabaseTableViewModel.getDirectoryList()),
    sampleDatabaseTableViewModel(inSampleDatabaseTableViewModel)
{
    // Mouse listener catches events from the tree view's children
    addMouseListener(this, true);
}

BlomeFileTreeView::~BlomeFileTreeView()
{
    removeMouseListener(this);
}

/**
 Handles what happens when files are dropped onto the tree view.
 */
void BlomeFileTreeView::filesDropped(const StringArray& files, int x, int y)
{
    // Adding all the dropped files to the database
    for(int f = 0; f < files.size(); f++)
    {
        sampleDatabaseTableViewModel.addSampleItem(files[f]);
    }
}

/**
 Sets a flag if the tree view is interested in drag and drop of files.
 */
bool BlomeFileTreeView::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

/**
 Determines the components behaviour when reacting to a broadcasted change
 */
void BlomeFileTreeView::changeListenerCallback(ChangeBroadcaster* source)
{
    // Repaint when the directory list changes
    repaint();
}

/**
 Determines the components behaviour when the mouse is being dragged on it.
 */
void BlomeFileTreeView::mouseDrag(const MouseEvent& e)
{
    // If the drag was at least 50ms after the mouse was pressed
    if (e.getLengthOfMousePress() > 50) {
        Point<int> mousePosition = e.getEventRelativeTo(this).position.toInt();
        
        // Check if any of the selected items was dragged
        for (int i = 0; i < getNumSelectedItems(); i++) {
            Rectangle<int> itemBounds = getSelectedItem(i)->getItemPosition(false);
            
            if (itemBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                // Add all selected files to external drag
                for (int f = 0; f < getNumSelectedFiles(); f++) {
                    File selectedSampleFile = getSelectedFile(f);
                    String linkedAudioFilePath = selectedSampleFile.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + selectedSampleFile.getFileNameWithoutExtension();
                    selectedFilePaths.add(linkedAudioFilePath);
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}
