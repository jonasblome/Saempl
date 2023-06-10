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
    addMouseListener(this, true);
}

BlomeFileTreeView::~BlomeFileTreeView()
{
    removeMouseListener(this);
}

void BlomeFileTreeView::filesDropped(const StringArray& files, int x, int y)
{
    for(int f = 0; f < files.size(); f++)
    {
        sampleDatabaseTableViewModel.addSampleItem(files[f]);
    }
}

bool BlomeFileTreeView::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void BlomeFileTreeView::changeListenerCallback(ChangeBroadcaster* source)
{
    // When the directory list changes
    repaint();
}

void BlomeFileTreeView::mouseDrag(const MouseEvent& e)
{
    if (e.getLengthOfMousePress() > 50) {
        Point<int> mousePosition = e.getEventRelativeTo(this).position.toInt();
        
        for (int i = 0; i < getNumSelectedItems(); i++) {
            Rectangle<int> itemBounds = getSelectedItem(i)->getItemPosition(false);
            
            if (itemBounds.contains(mousePosition))
            {
                StringArray selectedFilePaths;
                
                for (int f = 0; f < getNumSelectedFiles(); f++) {
                    File selectedFile = getSelectedFile(f);
                    
                    if (!selectedFile.isDirectory()) {
                        selectedFilePaths.add(selectedFile.getFullPathName());
                    }
                }
                
                DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
                dragContainer->performExternalDragDropOfFiles(selectedFilePaths, false, this);
                
                return;
            }
        }
    }
}
