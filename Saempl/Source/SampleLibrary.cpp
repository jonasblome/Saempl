/*
  ==============================================================================

    SampleItemLibrary.cpp
    Created: 21 May 2023 2:45:47pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibrary.h"

SampleLibrary::SampleLibrary(TimeSliceThread& inThread)
{
    // Set directory path
    mSampleItemDirectoryPath =
    (File::getSpecialLocation(File::userMusicDirectory)).getFullPathName() + DIRECTORY_SEPARATOR + "Saempl" + DIRECTORY_SEPARATOR + "SampleFiles";
    
    if(!File(mSampleItemDirectoryPath).exists())
    {
        File(mSampleItemDirectoryPath).createDirectory();
    }
    
    // Initialize multi filter
    mDirectoryFilter = std::make_unique<SampleFilter>("DirectoryFilter");
    
    // Set directory with path and initial file filter
    mDirectory = std::make_unique<DirectoryContentsList>(&*mDirectoryFilter, inThread);
    mDirectory->addChangeListener(this);
    mDirectory->setDirectory(File(mSampleItemDirectoryPath), true, true);
    
    // Initialize sample analyser
    mSampleAnalyser = std::make_unique<SampleAnalyser>();
    
    // Load all sample files that are already in the current directory
    refreshSampleDatabase();
    updateDirectoryFilter();
}

SampleLibrary::~SampleLibrary()
{
    mDirectory->removeChangeListener(this);
}

void SampleLibrary::addSampleItem(File inFile)
{
    String fileName = inFile.getFileName();
    File newFile = File(mDirectoryPathToAddFilesTo + DIRECTORY_SEPARATOR + fileName);
    
    // Don't add files if they already exist
    if (!mDirectory->getDirectory().findChildFiles(File::findFiles, false, newFile.getFileName()).isEmpty()) {
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
    
    mDirectory->refresh();
}

void SampleLibrary::removeSampleItem(String inFilePath, bool deletePermanently = false)
{
    SampleItem* itemToDelete = getSampleItemWithFilePath(inFilePath);
    mSampleItems.removeObject(itemToDelete);
    
    File sampleFileToDelete = File(inFilePath);
    File linkedAudioFile = File(sampleFileToDelete.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + sampleFileToDelete.getFileNameWithoutExtension());
    
    deletePermanently ? sampleFileToDelete.deleteRecursively() : sampleFileToDelete.moveToTrash();;
        
    // Also delete linked audio file
    if (!sampleFileToDelete.isDirectory())
    {
        deletePermanently ? linkedAudioFile.deleteFile() : linkedAudioFile.moveToTrash();;
    }
    
    mDirectory->refresh();
}

DirectoryContentsList* SampleLibrary::getDirectoryList()
{
    return &*mDirectory;
}

void SampleLibrary::changeListenerCallback(ChangeBroadcaster* inSource)
{
    if (inSource == mDirectory.get())
    {
        mDirectoryPathToAddFilesTo = mDirectory->getDirectory().getFullPathName();
    }
}

void SampleLibrary::addFilterCondition(SampleFilterCondition* inFilterCondition)
{
    mFilterConditions.add(inFilterCondition);
    updateDirectoryFilter();
    // mDirectory->setFileFilter(&*mDirectoryFilter);
    mDirectory->refresh();
}

void SampleLibrary::removeFilterCondition(SampleFilterCondition* inFilterCondition)
{
    mFilterConditions.removeObject(inFilterCondition);
    updateDirectoryFilter();
    // mDirectory->setFileFilter(&*mDirectoryFilter);
    mDirectory->refresh();
}

OwnedArray<SampleFilterCondition>* SampleLibrary::getFilterConditions()
{
    return &mFilterConditions;
}

/**
 Updates all sample files in the current directory and updates the corresponding SampleItem collection
 */
void SampleLibrary::refreshSampleDatabase()
{
    // Loop over all sample files to either load them as sample items or delete them if their audio file doesn't exist
    for (DirectoryEntry entry : RangedDirectoryIterator(mDirectory->getDirectory(), true, "*.bsf", File::findFiles))
    {
        File sampleFile = entry.getFile();
        File linkedAudioFile = File(sampleFile.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + sampleFile.getFileNameWithoutExtension());
        
        File parentDirectory = sampleFile.getParentDirectory();
        bool linkedAudioFileNotExists = parentDirectory.findChildFiles(File::findFiles, false, linkedAudioFile.getFileName()).isEmpty();
        if (linkedAudioFileNotExists)
        {
            // Delete sample files without corresponding audio file
            sampleFile.deleteFile();
            
            // Delete linked SampleItem
            mSampleItems.removeObject(getSampleItemWithFilePath(sampleFile.getFullPathName()));
        }
        else
        {
            // Read in existing sample files
            if (getSampleItemWithFilePath(sampleFile.getFullPathName()) == nullptr)
            {
                mSampleItems.add(mSampleItemManager->loadSampleFile(sampleFile.getFullPathName()));
            }
        }
    }
    
    // Loop over all audio files to create sample files for each that don't have a corresponding one yet
    for (DirectoryEntry entry : RangedDirectoryIterator(mDirectory->getDirectory(), true, "*.wav;*.mp3;*.m4a;*.aiff"))
    {
        File audioFile = entry.getFile();
        File linkedSampleFile = File(audioFile.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + audioFile.getFileName() + SAMPLE_FILE_EXTENSION);
        
        // Create sample file for each audio file without corresponding sample file
        bool linkedSampleFileNotExists = audioFile.getParentDirectory().findChildFiles(File::findFiles, false, linkedSampleFile.getFileName()).isEmpty();
        if (linkedSampleFileNotExists)
        {
            createSampleItem(audioFile);
        }
    }
    
    mDirectory->refresh();
}

SampleItem* SampleLibrary::getSampleItemWithFilePath(String inFilePath)
{
    for(SampleItem* sampleItem : mSampleItems)
    {
        if (sampleItem->getFilePath() + SAMPLE_FILE_EXTENSION == inFilePath)
        {
            return sampleItem;
        }
    }
    
    return nullptr;
}

/**
 Creates a SampleItem for a file and adds it to the collection.
 If the corresponding SampleItem already exists,, nothing happens.
 */
void SampleLibrary::createSampleItem(File inFile)
{
    SampleItem* linkedSampleItem = getSampleItemWithFilePath(inFile.getFullPathName());
    
    if (linkedSampleItem == nullptr)
    {
        mSampleItems.add(new SampleItem());
        mSampleItems.getLast()->setFilePath(inFile.getFullPathName());
        
        for (int t = 0; t < AnalysisTypes.size(); t++)
        {
            mSampleItems.getLast()->addSampleTag(new SampleTag(AnalysisTypes[t], mSampleAnalyser->analyseType(inFile, t)));
        }
        
        mSampleItemManager->createSampleFile(*mSampleItems.getLast());
    }
    else
    {
        mSampleItemManager->createSampleFile(*linkedSampleItem);
    }
}

void SampleLibrary::updateDirectoryFilter()
{
    mDirectoryFilter->clear();
    
    for (DirectoryEntry entry : RangedDirectoryIterator(mDirectory->getDirectory(), true, "*.bsf", File::findFiles))
    {
        String filePath = entry.getFile().getFullPathName();
        SampleItem* sampleItem = getSampleItemWithFilePath(filePath);
        bool fileWasAccepted = true;
        
        for (SampleFilterCondition* condition : mFilterConditions) {
            if (!condition->acceptsSampleItem(sampleItem)) {
                fileWasAccepted = false;
            }
        }
        
        if (fileWasAccepted)
        {
            mDirectoryFilter->addFilteredFilePath(filePath);
        }
    }
}
