/*
 ==============================================================================
 
 SampleLibraryManager.h
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
     */
    void updateSampleLibraryFile();
    /**
     Deletes all sample items where the files have been externally deleted
     and adds sample items for each new detected file.
     */
    void synchWithLibraryDirectory();
    void loadSampleLibrary(File const & inLibraryDirectory);
    /**
     Loads meta-information of analysis file as sample items collection.
     
     @param inLibraryDirectory the library directory file.
     @param inSampleItems the collection of sample items from the library.
     */
    void loadSampleLibraryFile(File const & inLibraryDirectory);
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
     @param forceAnalysis forces analysis even for files longer than one minute.
     */
    void analyseSampleItem(SampleItem& inSampleItem, File const & inFile, bool forceAnalysis);
    
private:
    File libraryDirectory;
    OwnedArray<SampleItem>& allSampleItems;
    OwnedArray<SampleItem>& paletteSampleItems;
    std::unique_ptr<SampleAnalyser> mSampleAnalyser;
    StringArray addedFilePaths;
    InterProcessLock mFileLock{"fileLock"};
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
    int currentVersion = 0;
    bool libraryHasOldVersion = false;
    
    /**
     Loads the given file as an XmlElement and returns a pointer to it.
     */
    XmlElement loadFileAsXml(File& inFile);
    /**
     Writes the contents of the given xml into the given file as binary data.
     
     @param inXml the XmlElement to store in the file.
     @param inFile the file to store the XmlElement in.
     */
    void writeXmlToFile(XmlElement& inXml, File& inFile);
    /**
     Runs the loading of a library while setting the progress for the progress bar.
     */
    void run() override;
    void threadComplete(bool userPressedCancel) override;
};
