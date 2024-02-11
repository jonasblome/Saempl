/*
 ==============================================================================
 
 SampleLibrary.h
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
 Manages storage and handling of SampleItems.
 
 Stores SampleItem objects in collections, adds or removes them, filters them.
 Can scan the file system for  files and add them to the library.
 Can filter through stored SampleItem objects and files from the file system.
 */
class SampleLibrary
:
public ChangeListener,
public ChangeBroadcaster
{
public:
    SampleLibrary();
    ~SampleLibrary();
    /**
     Adds the files and all their children to the sample item collection.
     
     @param inFilePaths the file paths to add to the collection.
     */
    void addAllToSampleItems(StringArray const & inFilePaths);
    /**
     Removes the given sample items from all collections and deletes it.
     
     @param inFilePaths the files to remove.
     @param deletePermanently if true, deletes permanently, if false moves file to trash.
     */
    void removeSampleItems(StringArray const & inFilePaths, bool deletePermanently);
    /**
     Adds the files and all their children to the palette collection.
     
     @param inFildPaths the files/directories to add to the palette.
     */
    void addAllToPalette(StringArray const & inFilePaths);
    /**
     Removes the sample items from the palette collection.
     */
    void removeAllFromPalette(Array<SampleItem*> inSampleItems);
    /**
     Reanalyses the sample and sets the properties to the newly analysed values.
     */
    void reanalyseSampleItems(StringArray const & inFiles);
    /**
     @returns the directory list object.
     */
    DirectoryContentsList& getDirectoryList();
    /**
     @returns the file filter object.
     */
    SampleFileFilter& getFileFilter();
    /**
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
     @returns the path of the current library directory.
     */
    String getCurrentLibraryPath();
    void changeListenerCallback(ChangeBroadcaster* inSource) override;
    /**
     Deletes all sample items where the files have been externally deleted
     and adds sample items for each new detected file.
     */
    void synchWithLibraryDirectory();
    /**
     @returns the array with the file paths of all filtered samples.
     */
    StringArray& getFilteredFilePaths();
    /**
     @returns whether the library manager has finished loading the library.
     */
    bool getLibraryWasLoaded();
    
private:
    std::unique_ptr<TimeSliceThread> mDirectoryScannerThread;
    std::unique_ptr<SampleFileFilter> mFileFilter;
    String mCurrentLibraryPath;
    std::unique_ptr<DirectoryContentsList> mDirectoryContentsList;
    OwnedArray<SampleItem> mAllSampleItems;
    OwnedArray<SampleItem> mFilteredSampleItems;
    StringArray mFilteredFilePaths;
    OwnedArray<SampleItem> mPaletteSampleItems;
    String mDirectoryPathToAddFilesTo;
    std::unique_ptr<SampleLibraryManager> mSampleLibraryManager;
    bool mLibraryWasLoaded;
    
    /**
     Deletes SampleItem objects in all collections (all, filtered, palette).
     */
    void clearSampleItemCollections();
    /**
     Applies the filter to all sample items and adds filtered items to mFilteredSampleItems collection.
     */
    void applyFilter();
    /**
     Adds the file and all its children to the sample item collection.
     
     @param inFile the file to add to the collection.
     */
    void addFileToSampleItems(File const & inFile);
    /**
     Creates sample item for given file and adds it to the collection.
     
     @param inFile the file to add as a sample item.
     
     @returns the newly added sample item.
     */
    SampleItem* addToSampleItems(File const & newFile);
    /**
     Adds the file and all its children to the palette collection.
     
     @param inFile the file/directory to add to the palette.
     
     @returns whether a new item was created for this file.
     */
    bool addFileToPalette(File const & inFile);
    /**
     Adds the file as a sample item and appends it to the palette collection.
     
     @param inFile the file to add to the palette.
     
     @returns whether a new item was created for this file.
     */
    bool addToPalette(const File &inFile);
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
     Reanalyses the sample and sets the properties to the newly analysed values.
     */
    void reanalyseSampleItem(File const & inFile);
};
