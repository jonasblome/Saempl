/*
  ==============================================================================

    SampleLibrary.cpp
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibrary.h"

SampleLibrary::SampleLibrary(TimeSliceThread& inThread)
{
    // Initialize sample analyser
    mSampleAnalyser = std::make_unique<SampleAnalyser>();
    
    // Set directory path
    mLibraryFileDirectoryPath =
        (File::getSpecialLocation(juce::File::userMusicDirectory)).getFullPathName()
        + DIRECTORY_SEPARATOR
        + "Plugins"
        + DIRECTORY_SEPARATOR
        + "Saempl"
        + DIRECTORY_SEPARATOR
        + "SampleLibrary";
    
    if(!File(mLibraryFileDirectoryPath).exists())
    {
        File(mLibraryFileDirectoryPath).createDirectory();
    }
    
    mCurrentLibraryPath =
        (File::getSpecialLocation(juce::File::userMusicDirectory)).getFullPathName()
        + DIRECTORY_SEPARATOR
        + "Plugins"
        + DIRECTORY_SEPARATOR
        + "Saempl"
        + DIRECTORY_SEPARATOR
        + "DefaultSampleLibrary";
    
    // Load current library file
    File sampleLibraryFile = File(mLibraryFileDirectoryPath + DIRECTORY_SEPARATOR + "DefaultSampleLibrary" + SAMPLE_LIBRARY_FILE_EXTENSION);
    
    if (sampleLibraryFile.exists())
    {
        mSampleLibraryManager->loadSampleLibraryFile(sampleLibraryFile.getFullPathName(), &mSampleItems);
    }
    
    // Set directory with path and file filter
    mDirectoryFilter = std::make_unique<WildcardFileFilter>(supportedAudioFileFormatsWildcard, "*", "AudioFormatsFilter");
    mDirectoryContent = std::make_unique<DirectoryContentsList>(&*mDirectoryFilter, inThread);
    mDirectoryContent->addChangeListener(this);
    mDirectoryContent->setDirectory(File(mCurrentLibraryPath), true, true);
    
    refreshSampleLibrary();
}

SampleLibrary::~SampleLibrary()
{
    mDirectoryContent->removeChangeListener(this);
    mSampleLibraryManager->updateSampleLibraryFile(mLibraryFileDirectoryPath + DIRECTORY_SEPARATOR + "DefaultSampleLibrary.bslf", &mSampleItems);
}

void SampleLibrary::addSampleItem(File inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist in the current library
    if (!mDirectoryContent->getDirectory().findChildFiles(File::findFiles, true, newFile.getFileName()).isEmpty()) {
        return;
    }
    
    if (inFile.isDirectory())
    {
        // Check directory recursively for audio files and subdirectoryies
        mDirectoryPathToAddFilesTo = newFile.getFullPathName();
        bool newDirectoryWasCreated = newFile.createDirectory();
        
        if (newDirectoryWasCreated)
        {
            for (DirectoryEntry entry : RangedDirectoryIterator(inFile, false, "*", File::findFilesAndDirectories))
            {
                addSampleItem(entry.getFile());
            }
        }
        
        mDirectoryPathToAddFilesTo = newFile.getParentDirectory().getFullPathName();
    }
    else if (isSupportedAudioFileFormat(newFile.getFileExtension()))
    {
        // Add file to current directory and create SampleItem
        inFile.copyFileTo(newFile);
        createSampleItem(newFile);
    }
    else
    {
        return;
    }
    
    mDirectoryContent->refresh();
}

void SampleLibrary::removeSampleItem(String inFilePath, bool deletePermanently = false)
{
    // Delete sample item
    SampleItem* itemToDelete = getSampleItemWithFilePath(inFilePath);
    mSampleItems.removeObject(itemToDelete);
    
    // Delete audio file
    File fileToDelete = File(inFilePath);
    
    if (fileToDelete.exists())
    {
        deletePermanently ? fileToDelete.deleteRecursively() : fileToDelete.moveToTrash();
    }
    
    mDirectoryContent->refresh();
}

DirectoryContentsList* SampleLibrary::getDirectoryList()
{
    return &*mDirectoryContent;
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* inSource)
{
    if (inSource == mDirectoryContent.get())
    {
        mDirectoryPathToAddFilesTo = mDirectoryContent->getDirectory().getFullPathName();
    }
}

void SampleLibrary::setFileFilter()
{
    // Set current file filter to a chosen filter selection from the database table panel
}

/**
 Updates all sample files in the current directory and updates the corresponding SampleItem collection
 */
void SampleLibrary::refreshSampleLibrary()
{
    // Go through all current sample items, check if corresponding audio file still exists and if not, delete sample item
    for (SampleItem* item : mSampleItems)
    {
        if (!File(item->getFilePath()).exists()) {
            mSampleItems.removeObject(item);
        }
    }
    
    // Go through all files in directory, check if a corresponding sample item now exists in the sample item list, if not add it
    for (DirectoryEntry entry : RangedDirectoryIterator(mDirectoryContent->getDirectory(), true, supportedAudioFileFormatsWildcard, File::findFiles))
    {
        bool linkedSampleItemExists = getSampleItemWithFilePath(entry.getFile().getFullPathName()) == nullptr;
        if (linkedSampleItemExists)
        {
            createSampleItem(entry.getFile());
        }
    }
    
    mDirectoryContent->refresh();
}

SampleItem* SampleLibrary::getSampleItemWithFilePath(String inFilePath)
{
    for(SampleItem* sampleItem : mSampleItems)
    {
        if (sampleItem->getFilePath() == inFilePath)
        {
            return sampleItem;
        }
    }
    
    return nullptr;
}

/**
 Creates a SampleItem for a file and adds it to the collection.
 If the corresponding SampleItem already exists, nothing happens.
 */
void SampleLibrary::createSampleItem(File inFile)
{
    bool linkedSampleItemExists = getSampleItemWithFilePath(inFile.getFullPathName()) == nullptr;
    
    if (linkedSampleItemExists)
    {
        mSampleItems.add(new SampleItem());
        mSampleItems.getLast()->setFilePath(inFile.getFullPathName());
        mSampleItems.getLast()->addSampleTag(new SampleTag("Length", mSampleAnalyser->analyseSampleLength(inFile)));
    }
}
