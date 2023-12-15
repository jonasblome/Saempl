/*
 ==============================================================================
 
 BlomeFileTreeView.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "BlomeFileTreeView.h"

BlomeFileTreeView::BlomeFileTreeView(SampleLibrary& inSampleLibrary)
:
FileTreeComponent(inSampleLibrary.getDirectoryList()),
sampleLibrary(inSampleLibrary)
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
    for(int f = 0; f < files.size(); f++)
    {
        sampleLibrary.addAllToSampleItems(files[f]);
    }
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
