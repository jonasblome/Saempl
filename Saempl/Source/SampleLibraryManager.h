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
#include "SampleManagerJob.h"

/**
 Handles updating and creating of directory meta-analysis files.
 
 Writes information on all files in a sample library directory to analysis files and reads them.
 */
class SampleLibraryManager
:
public ThreadWithProgressWindow,
public ChangeBroadcaster,
public ThreadPool
{
public:
    /**
     The constructor for the library manager.
     
     @params all sample item collections of the sample library.
     */
    SampleLibraryManager(OwnedArray<SampleItem>& inAllSampleItems,
                         OwnedArray<SampleItem>& inPaletteSampleItems,
                         OwnedArray<SampleItem>& inDeletedSampleItems,
                         OwnedArray<SampleItem>& inAddedSampleItems,
                         OwnedArray<SampleItem>& inAlteredSampleItems);
    ~SampleLibraryManager();
    /**
     Stores all properties of a sample item object in an xml element.
     */
    void writeSampleItemToXml(SampleItem* sampleItem, XmlElement* sampleItemXml);
    /**
     Adds meta-information of all sample items to an analysis file and updates their information if needed.
     */
    void updateSampleLibraryFile();
    /**
     Deletes all sample items where the files have been externally deleted
     and adds sample items for each new detected file.
     */
    void synchWithLibraryDirectory();
    /**
     Loads all library files of the given directory and then synchs with the library.
     
     @param inLibraryDirectory the file of the directory to load.
     */
    void loadSampleLibrary(File const & inLibraryDirectory);
    /**
     Creates a sample item from the stored properties in an  xml element.
     
     @param sampleItemXml the xml element belonging to the sample item.
     */
    void createSampleItemFromXml(XmlElement const * sampleItemXml);
    /**
     Loads meta-information of directory and all subdirectories as sample items collection.
     
     @param inLibraryDirectory the library directory file.
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
    void analyseSampleItem(SampleItem* inSampleItem, File const & inFile, bool forceAnalysis);
    
private:
    File libraryDirectory;
    OwnedArray<SampleItem>& allSampleItems;
    OwnedArray<SampleItem>& paletteSampleItems;
    OwnedArray<SampleItem>& deletedSampleItems;
    OwnedArray<SampleItem>& addedSampleItems;
    OwnedArray<SampleItem>& alteredSampleItems;
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
    int numProcessedItems;
    int numWithinTwoBPM;
    int numWithinFiveBPM;
    int numWithinTenBPM;
    int numFalseBPMDetected;
    int numCorrectKey;
    int numWithinOneKey;
    int numWithinThreeKey;
    int numFalseKeyDetected;
    
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
     Sets the threads progress bar and status message.
     */
    void setProgressAndStatus(int numItemsToProcess, int64 startTime);
    /**
     Runs the loading of a library while setting the progress for the progress bar.
     */
    void run() override;
    void threadComplete(bool userPressedCancel) override;
    /**
     Encodes a string to a format that is suitable for a JUCE XmlElement tag name.
     
     Adapted from a method at:
     https://stackoverflow.com/questions/5665231/most-efficient-way-to-escape-xml-html-in-c-string
     by Giovanni Funchal
     */
    String encodeForXml(String inString);
    /**
     Evaluates whether the detected tempo matches the actual tempo of the sample.
     
     @param tempo the detected tempo of the sample.
     @param title the sample's title containing the actual tempo.
     */
    void evaluateTempoDetection(int tempo, const String& title);
    /**
     Evaluates whether the detected key matches the actual key of the sample.
     
     @param key the detected key of the sample.
     @param title the sample's title containing the actual key.
     */
    void evaluateKeyDetection(int key, const String& title);
};
