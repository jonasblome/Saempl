/*
 ==============================================================================
 
 SampleLibraryManager.h
 Created: 10 Jun 2023 6:16:05pm
 Author:  Jonas Blome
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"
#include "SampleItem.h"
#include "BlomeHelpers.h"
#include "SampleAnalyser.h"

/**
 Handles updating and creating of directory meta-analysis files.
 
 Writes information on all files in a sample library directory to analysis files and reads them.
 */
class SampleLibraryManager
:
public ThreadWithProgressWindow,
public ChangeBroadcaster
{
public:
    SampleLibraryManager(OwnedArray<SampleItem>& inAllSampleItems, OwnedArray<SampleItem>& inPaletteSampleItems);
    ~SampleLibraryManager();
    /**
     Adds meta-information of all sample items to an analysis file and updates their information if needed.
     
     @param inLibraryDirectory the library directory file.
     @param inSampleItems the collection of sample items from the library.
     */
    void updateSampleLibraryFile(File& inLibraryDirectory);
    /**
     Deletes all sample items where the files have been externally deleted
     and adds sample items for each new detected file.
     */
    void synchWithLibraryDirectory();
    /**
     Runs the loading of a library while setting the progress for the progress bar.
     */
    void run() override;
    void threadComplete(bool userPressedCancel) override;
    /**
     Loads meta-information of analysis file as sample items collection.
     
     @param inLibraryDirectory the library directory file.
     @param inSampleItems the collection of sample items from the library.
     */
    void loadSampleLibrary(File& inLibraryDirectory);
    /**
     @returns the path of the last opened sample library directory or the default library directory.
     */
    String getLastOpenedDirectory();
    /**
     Writes the given path into the plugin data directory as the last used library directory path.
     
     @param inDirectoryPath the directory to set as last used.
     */
    void storeLastOpenedDirectory(String& inDirectoryPath);
    /**
     Creates a sample item for the given file and sets its properties.
     
     @param inFile the file for which to create the sample item.
     
     @returns the newly created sample item.
     */
    SampleItem* createSampleItem(File const & inFile);
    /**
     @param inFilePath the file path for which to get the corresponding sample item.
     
     @returns the sample item with that file path.
     */
    SampleItem* getSampleItemWithFilePath(String const & inFilePath);
    /**
     @param inFilePath the file path to check.
     
     @returns whether a file with the given file path has already been added to the library.
     */
    bool fileHasBeenAdded(String const & inFilePath);
    /**
     Analyses all analysis properties of the given sample file.
     
     @param inSampleItem the sample item to write the property values to.
     @param inFile the sample file to analyse.
     */
    void analyseSampleItem(SampleItem& inSampleItem, File const & inFile);
    
private:
    File libraryDirectory;
    OwnedArray<SampleItem>& allSampleItems;
    OwnedArray<SampleItem>& paletteSampleItems;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    StringArray addedFilePaths;
    InterProcessLock fileLock{"fileLock"};
    String mDefaultLibraryDirectoryPath =
    (File::getSpecialLocation(File::userMusicDirectory)).getFullPathName()
    + DIRECTORY_SEPARATOR
    + "Plugins"
    + DIRECTORY_SEPARATOR
    + "Saempl"
    + DIRECTORY_SEPARATOR
    + "DefaultSampleLibrary";
    String mLibraryFilesDirectoryPath =
    (File::getSpecialLocation(File::userMusicDirectory)).getFullPathName()
    + DIRECTORY_SEPARATOR
    + "Plugins"
    + DIRECTORY_SEPARATOR
    + "Saempl"
    + DIRECTORY_SEPARATOR
    + "SampleLibraryFiles";
    String mSaemplDataFilePath =
    (File::getSpecialLocation(File::userMusicDirectory)).getFullPathName()
    + DIRECTORY_SEPARATOR
    + "Plugins"
    + DIRECTORY_SEPARATOR
    + "Saempl"
    + DIRECTORY_SEPARATOR
    + "SaemplPluginData"
    + SAEMPL_DATA_FILE_EXTENSION;
    
    /**
     Loads the given file as an \ref XmlElement and returns a pointer to it.
     */
    XmlElement loadFileAsXml(File& inFile);
    /**
     Writes the contents of the given xml into the given file as binary data.
     
     @param inXml the \ref XmlElement to store in the file.
     @param inFile the file to store the \ref XmlElement in.
     */
    void writeXmlToFile(XmlElement& inXml, File& inFile);
};
