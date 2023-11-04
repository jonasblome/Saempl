/*
  ==============================================================================

    SampleLibraryManager.cpp
    Created: 10 Jun 2023 6:16:05pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleLibraryManager.h"


SampleLibraryManager::SampleLibraryManager()
{
    // Initialize sample analyser
    mSampleAnalyser = std::make_unique<SampleAnalyser>();
}

SampleLibraryManager::~SampleLibraryManager()
{
    
}

void SampleLibraryManager::updateSampleLibraryFile(File& inLibraryDirectory, OwnedArray<SampleItem>& inSampleItems)
{
    // Create sample library file as xml and store path to library
    XmlElement sampleLibraryXml("SampleLibrary");
    sampleLibraryXml.setAttribute("LibraryPath", inLibraryDirectory.getFullPathName());
    
    // Adding sample items xml to sample library xml
    XmlElement* sampleItemsXml = new XmlElement("SampleItems");
    
    for (SampleItem* sampleItem : inSampleItems)
    {
        XmlElement* sampleItemXml = new XmlElement("SampleItem");
        
        // Adding file path xml to sample item xml
        sampleItemXml->setAttribute("FilePath", sampleItem->getFilePath());
        
        // Adding sample properties xml to sample item xml
        XmlElement* samplePropertiesXml = new XmlElement("SampleProperties");
        
        // Adding length property
        XmlElement* samplePropertyXml = new XmlElement(PROPERTY_NAMES[0]);
        samplePropertyXml->setAttribute("PropertyValue", sampleItem->getLength());
        samplePropertiesXml->prependChildElement(samplePropertyXml);
        
        // Adding title property
        samplePropertyXml = new XmlElement(PROPERTY_NAMES[1]);
        samplePropertyXml->setAttribute("PropertyValue", sampleItem->getTitle());
        samplePropertiesXml->prependChildElement(samplePropertyXml);
        
        sampleItemXml->prependChildElement(samplePropertiesXml);
        
        sampleItemsXml->prependChildElement(sampleItemXml);
    }
    
    sampleLibraryXml.prependChildElement(sampleItemsXml);
    
    // Write sample library xml to external file
    File sampleLibraryFile = File(mLibraryFilesDirectoryPath
                                  + DIRECTORY_SEPARATOR
                                  + inLibraryDirectory.getFileNameWithoutExtension()
                                  + SAMPLE_LIBRARY_FILE_EXTENSION);
    
    if (!sampleLibraryFile.exists())
    {
        sampleLibraryFile.create();
    }
    else
    {
        sampleLibraryFile.deleteFile();
    }
    
    writeXmlToFile(sampleLibraryXml, sampleLibraryFile);
}

void SampleLibraryManager::loadSampleLibrary(File& inLibraryDirectory, OwnedArray<SampleItem>& inSampleItems)
{
    File libraryFile = File(mLibraryFilesDirectoryPath + DIRECTORY_SEPARATOR + inLibraryDirectory.getFileNameWithoutExtension() + SAMPLE_LIBRARY_FILE_EXTENSION);
    
    // Check if sample library file (.bslf) exists
    if (libraryFile.exists())
    {
        // Get data from library file
        XmlElement libraryXml = loadFileAsXml(libraryFile);
        XmlElement* libraryXmlPointer = &libraryXml;
        
        if(libraryXmlPointer)
        {
            XmlElement* sampleItemsXml = libraryXmlPointer->getChildByName("SampleItems");
            
            // Go over all sample items
            for (XmlElement* sampleItemXml : sampleItemsXml->getChildIterator())
            {
                // Create new sample item
                SampleItem* sampleItem = inSampleItems.add(new SampleItem());
                String filePath = sampleItemXml->getStringAttribute("FilePath");
                sampleItem->setFilePath(filePath);
                XmlElement* samplePropertiesXml = sampleItemXml->getChildByName("SampleProperties");
                
                // Add title property to item
                XmlElement* samplePropertyXml = samplePropertiesXml->getChildByName("Title");
                String title = samplePropertyXml->getStringAttribute("PropertyValue");
                sampleItem->setTitle(title);
                
                // Add length property to item
                samplePropertyXml = samplePropertiesXml->getChildByName("Length");
                double length = samplePropertyXml->getDoubleAttribute("PropertyValue");
                sampleItem->setLength(length);
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        // Go through all files in directory, check if a corresponding sample item
        // already exists in the sample item list, if not add it
        for (DirectoryEntry entry : RangedDirectoryIterator(inLibraryDirectory,
                                                            true,
                                                            SUPPORTED_AUDIO_FORMATS_WILDCARD,
                                                            File::findFiles))
        {
            createSampleItem(entry.getFile(), inSampleItems);
        }
    }
}

String SampleLibraryManager::getLastOpenedDirectory()
{
    File saemplDataFile = File(mSaemplDataFilePath);
    
    if (!saemplDataFile.exists())
    {
        return mDefaultLibraryDirectoryPath;
    }
    
    // Get data from library file
    XmlElement saemplDataXml = loadFileAsXml(saemplDataFile);
    XmlElement* saemplDataXmlPointer = &saemplDataXml;
    
    String lastOpenedDirectoryPath = "";
    
    if (saemplDataXmlPointer)
    {
        lastOpenedDirectoryPath = saemplDataXml.getStringAttribute("LastOpenedDirectory");
    }
    
    return lastOpenedDirectoryPath;
}

void SampleLibraryManager::storeLastOpenedDirectory(String& inDirectoryPath)
{
    File saemplDataFile = File(mSaemplDataFilePath);
    XmlElement saemplDataXml = XmlElement("SaemplData");
    
    if (!saemplDataFile.exists())
    {
        saemplDataFile.create();
    }
    else
    {
        saemplDataXml = loadFileAsXml(saemplDataFile);
        saemplDataFile.deleteFile();
    }
    
    // Get data from plugin data file
    saemplDataXml.setAttribute("LastOpenedDirectory", inDirectoryPath);
    writeXmlToFile(saemplDataXml, saemplDataFile);
}

XmlElement SampleLibraryManager::loadFileAsXml(File& inFile)
{
    MemoryBlock fileData;
    inFile.loadFileAsData(fileData);
    XmlElement fileXml = *AudioPluginInstance::getXmlFromBinary(fileData.getData(),
                                                                      (int) fileData.getSize());
    fileData.reset();
    
    return fileXml;
}

void SampleLibraryManager::writeXmlToFile(XmlElement& inXml, File& inFile)
{
    MemoryBlock destinationData;
    AudioPluginInstance::copyXmlToBinary(inXml, destinationData);
    inFile.appendData(destinationData.getData(), destinationData.getSize());
    destinationData.reset();
}

/**
 Creates a SampleItem for a file and adds it to the collection.
 If the corresponding SampleItem already exists, nothing happens.
 */
void SampleLibraryManager::createSampleItem(File inFile, OwnedArray<SampleItem>& inSampleItems)
{
    SampleItem* newItem = inSampleItems.add(new SampleItem());
    newItem->setFilePath(inFile.getFullPathName());
    newItem->setTitle(inFile.getFileNameWithoutExtension());
    newItem->setLength(mSampleAnalyser->analyseSampleLength(inFile));
}

SampleItem* SampleLibraryManager::getSampleItemWithFileName(String const & inFileName,
                                                            OwnedArray<SampleItem>& inSampleItems)
{
    for (SampleItem* sampleItem : inSampleItems)
    {
        if (File(sampleItem->getFilePath()).getFileName().compare(inFileName) == 0)
        {
            return sampleItem;
        }
    }
    
    return nullptr;
}
