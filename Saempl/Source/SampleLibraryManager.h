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

/**
 Handles updating and creating of directory meta-analysis files.
 
 Writes information on all files in a sample library directory to analysis files and reads them.
 */
class SampleLibraryManager
{
public:
    // Constructors
    SampleLibraryManager();
    ~SampleLibraryManager();
    
    // Methods
    /**
     Adds meta-information of all sample items to an analysis file and updates their information if needed.
     
     @param inLibraryDirectory the library directory file.
     @param inSampleItems the collection of sample items from the library.
     */
    void updateSampleLibraryFile(File& inLibraryDirectory, OwnedArray<SampleItem>& inSampleItems);
    /**
     Loads meta-information of analysis file as sample items collection.
     
     @param inLibraryDirectory the library directory file.
     @param inSampleItems the collection of sample items from the library.
     */
    void loadSampleLibraryFile(File& inLibraryDirectory, OwnedArray<SampleItem>& inSampleItems);
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
     Loads the given file as an \ref XmlElement and returns a pointer to it.
     */
    XmlElement loadFileAsXml(File& inFile);
    /**
     Writes the contents of the given xml into the given file as binary data.
     
     @param inXml the \ref XmlElement to store in the file.
     @param inFile the file to store the \ref XmlElement in.
     */
    void writeXmlToFile(XmlElement& inXml, File& inFile);
    
private:
    // JUCE_HEAVYWEIGHT_LEAK_DETECTOR(SampleLibraryManager)
    
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
};
