/*
  ==============================================================================

    SampleLibrary.h
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#pragma once

#include "SampleItem.h"
#include "BlomeHelpers.h"
#include "SampleLibraryManager.h"
#include "SampleFileFilter.h"
#include "SampleFileFilterRuleTitle.h"
#include "SampleFileFilterRuleLength.h"

/**
 Manages storage and handling of \ref SampleItems.
 
 Implements:
 \ref juce::ChangeListener
 Stores \ref SampleItem objects in collections, adds or removes them, filters them.
 Can scan the file system for  files and add them to the library.
 Can filter through stored \ref SampleItem objects and files from the file system.
 */
class SampleLibrary
:   ChangeListener,
    public ChangeBroadcaster
{
public:
    /**
     The constructor of the sample library.
     
     @param inThread the thread for scanning through the file system.
     */
    SampleLibrary(TimeSliceThread& inThread);
    ~SampleLibrary();
    /**
     Creates sample item for given file and add it to the collection.
     
     @param inFile the file to add as a sample item.
     */
    void addToSampleItems(File const & inFile);
    /**
     Adds the file as a sample item and appends it to the palette collection.
     
     @param inFile the file to add to the palette.
     */
    void addToPalette(File const & inFile);
    /**
     Removes the sample item from all collections and deletes it.
     
     @param inFilePath the file to remove
     @param deletePermanently if true, deletes permanently, if false moves file to trash
     */
    void removeSampleItem(String const & inFilePath, bool deletePermanently);
    /**
     Removes the sample item from the palette collection.
     */
    void removeFromPalette(SampleItem& inSampleItem);
    /**
     @returns the directory list object.
     */
    DirectoryContentsList& getDirectoryList();
    /**
     @returns the file filter object.
     */
    SampleFileFilter& getFileFilter();
    void changeListenerCallback(ChangeBroadcaster* inSource) override;
    /**
     Deletes all sample items with missing files. Adds sample items for each new file.
     Applies filter and refreshes directory list.
     */
    void refresh();
    /**
     Sets the library directory to the given path.
     
     @param inDirectoryPath the path of the directory to set.
     */
    void setDirectory(String inDirectoryPath);
    /**
     Returns the sample item collection with the given scope.
     
     @param inCollectionType the type of collection to return (all, filtered or palette).
     
     @returns the sample item collection of given type.
     */
    OwnedArray<SampleItem>& getSampleItems(SampleItemCollectionScope inCollectionScope);
    /**
     Applies the filter to all sample items and adds filtered items to \ref mFilteredSampleItems collection.
     */
    void applyFilter();
    /**
     @returns the path of the current library directory.
     */
    String getCurrentLibraryPath();
    /**
     Deletes \ref SampleItem objects in all collections (all, filtered, palette).
     */
    void clearSampleItemCollections();
    /**
     @returns the progress of loading a new sample library in percentage.
     */
    double& getLoadingProgress();
    
private:
    std::unique_ptr<SampleFileFilter> mFileFilter;
    String mCurrentLibraryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryContent;
    OwnedArray<SampleItem> mAllSampleItems;
    OwnedArray<SampleItem> mFilteredSampleItems;
    OwnedArray<SampleItem> mPaletteSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleLibraryManager> mSampleLibraryManager;
    double loadingProgress;
};
