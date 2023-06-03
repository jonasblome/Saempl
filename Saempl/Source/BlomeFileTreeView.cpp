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
    
}

BlomeFileTreeView::~BlomeFileTreeView()
{
    
}

void BlomeFileTreeView::filesDropped(const StringArray& files, int x, int y)
{
    for(int f = 0; f < files.size(); f++)
    {
        sampleDatabaseTableViewModel.addFile(files[f]);
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
