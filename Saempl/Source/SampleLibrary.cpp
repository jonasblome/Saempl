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
    
    // Initialize library manager
    mSampleLibraryManager = std::make_unique<SampleLibraryManager>();
    
    // Set directory with path and file filter
    mDirectoryFilter = std::make_unique<WildcardFileFilter>(SUPPORTED_AUDIO_FORMATS_WILDCARD, "*", "AudioFormatsFilter");
    mDirectoryContent = std::make_unique<DirectoryContentsList>(&*mDirectoryFilter, inThread);
    mDirectoryContent->addChangeListener(this);
    setDirectory("");
}

SampleLibrary::~SampleLibrary()
{
    mDirectoryContent->removeChangeListener(this);
    mSampleLibraryManager->updateSampleLibraryFile(mCurrentLibraryPath, &mSampleItems);
}

void SampleLibrary::addSampleItem(File const & inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist in the current library
    if (!mDirectoryContent->getDirectory().findChildFiles(File::findFiles, true, newFile.getFileName()).isEmpty())
    {
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

void SampleLibrary::removeSampleItem(String const& inFilePath, bool deletePermanently = false)
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

/**
 Updates all sample files in the current directory and updates the corresponding SampleItem collection
 */
void SampleLibrary::refresh()
{
    // Go through all current sample items, check if corresponding audio file still exists and if not, delete sample item
    for (SampleItem* item : mSampleItems)
    {
        if (!File(item->getFilePath()).exists()) {
            mSampleItems.removeObject(item);
        }
    }
    
    // Go through all files in directory, check if a corresponding sample item now exists in the sample item list, if not add it
    for (DirectoryEntry entry : RangedDirectoryIterator(mDirectoryContent->getDirectory(), true, SUPPORTED_AUDIO_FORMATS_WILDCARD, File::findFiles))
    {
        bool linkedSampleItemExists = getSampleItemWithFilePath(entry.getFile().getFullPathName()) != nullptr;
        if (!linkedSampleItemExists)
        {
            createSampleItem(entry.getFile());
        }
    }
    
    mDirectoryContent->refresh();
}

SampleItem* SampleLibrary::getSampleItemWithFilePath(String const & inFilePath)
{
    for (SampleItem* sampleItem : mSampleItems)
    {
        if (sampleItem->getFilePath() == inFilePath)
        {
            return sampleItem;
        }
    }
    
    return nullptr;
}

void SampleLibrary::setDirectory(String inDirectoryPath)
{
    if (inDirectoryPath != "")
    {
        // Store current library
        mSampleLibraryManager->updateSampleLibraryFile(mCurrentLibraryPath, &mSampleItems);
        mCurrentLibraryPath = inDirectoryPath;
    }
    else
    {
        // Set library to default path
        mCurrentLibraryPath =
            (File::getSpecialLocation(File::userMusicDirectory)).getFullPathName()
            + DIRECTORY_SEPARATOR
            + "Plugins"
            + DIRECTORY_SEPARATOR
            + "Saempl"
            + DIRECTORY_SEPARATOR
            + "DefaultSampleLibrary";
        
        if (!File(mCurrentLibraryPath).exists())
        {
            File(mCurrentLibraryPath).createDirectory();
        }
    }
    
    // Load new library
    mSampleItems.clear();
    mSampleLibraryManager->loadSampleLibraryFile(mCurrentLibraryPath, &mSampleItems);
    mDirectoryContent->setDirectory(File(mCurrentLibraryPath), true, true);
    refresh();
}

OwnedArray<SampleItem>* SampleLibrary::getSampleItems()
{
    return &mSampleItems;
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
        SampleItem* newItem = mSampleItems.add(new SampleItem());
        newItem->setFilePath(inFile.getFullPathName());
        
        for (int c = 0; c < PROPERTY_NAMES.size(); c++)
        {
            switch (PROPERTY_NAME_TYPES.at(PROPERTY_NAMES[c]))
            {
                case 0:
                {
                    SamplePropertyInt* newProperty = dynamic_cast<SamplePropertyInt*>(newItem->addSampleProperty(new SamplePropertyInt()));
                    newProperty->setName(PROPERTY_NAMES[c]);
                    newProperty->setValue(mSampleAnalyser->analyseProperty(c, inFile));
                    break;
                }
                case 1:
                {
                    break;
                }
                case 2:
                {
                    break;
                }
                default:
                    break;
            }
        }
    }
}
