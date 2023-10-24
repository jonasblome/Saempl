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
    mLibraryFilesDirectoryPath = "";
}

SampleLibraryManager::~SampleLibraryManager()
{
    
}

void SampleLibraryManager::updateSampleLibraryFile(String& inLibraryPath, OwnedArray<SampleItem>* inSampleItems)
{
    // Set library file directory path
    mLibraryFilesDirectoryPath = getLibraryFilesDirectoryPath();
    File librayDirectory = File(inLibraryPath);
    
    if(!librayDirectory.exists())
    {
        librayDirectory.createDirectory();
    }
    
    // Create sample library file as xml and store path to library
    XmlElement sampleLibraryXml("SampleLibrary");
    sampleLibraryXml.setAttribute("LibraryPath", inLibraryPath);
    
    // Adding sample items xml to sample library xml
    XmlElement* sampleItemsXml = new XmlElement("SampleItems");
    
    for (SampleItem* sampleItem : *inSampleItems)
    {
        XmlElement* sampleItemXml = new XmlElement("SampleItem");
        
        // Adding file path xml to sample item xml
        sampleItemXml->setAttribute("FilePath", sampleItem->getFilePath());
        
        // Adding sample tags xml to sample item xml
        XmlElement* sampleTagsXml = new XmlElement("SampleTags");
        
        for (SampleTag* sampleTag : *sampleItem->getSampleTags())
        {
            XmlElement* sampleTagXml = new XmlElement(sampleTag->getName());
            sampleTagXml->setAttribute("TagValue", sampleTag->getValue());
            sampleTagsXml->prependChildElement(sampleTagXml);
        }
        
        sampleItemXml->addChildElement(sampleTagsXml);
        
        sampleItemsXml->prependChildElement(sampleItemXml);
    }
    
    sampleLibraryXml.addChildElement(sampleItemsXml);
    
    // Write sample library xml to external file
    File sampleLibraryFile = File(mLibraryFilesDirectoryPath + DIRECTORY_SEPARATOR + librayDirectory.getFileNameWithoutExtension() + SAMPLE_LIBRARY_FILE_EXTENSION);
    
    if(!sampleLibraryFile.exists()) {
        sampleLibraryFile.create();
    }
    else {
        sampleLibraryFile.deleteFile();
    }
    
    MemoryBlock destinationData;
    AudioPluginInstance::copyXmlToBinary(sampleLibraryXml, destinationData);
    sampleLibraryFile.appendData(destinationData.getData(),
                          destinationData.getSize());
    destinationData.reset();
}

void SampleLibraryManager::loadSampleLibraryFile(String& inLibraryPath, OwnedArray<SampleItem>* inSampleItems)
{
    File libraryDirectory = File(inLibraryPath);
    File libraryFile = File(getLibraryFilesDirectoryPath() + DIRECTORY_SEPARATOR + libraryDirectory.getFileNameWithoutExtension() + SAMPLE_LIBRARY_FILE_EXTENSION);
    
    if (libraryFile.exists())
    {
        MemoryBlock libraryFileData;
        libraryFile.loadFileAsData(libraryFileData);
        XmlElement sampleLibraryXml = *AudioPluginInstance::getXmlFromBinary(libraryFileData.getData(), (int) libraryFileData.getSize());
        libraryFileData.reset();
        XmlElement* libraryXmlPointer = &sampleLibraryXml;
        
        if(libraryXmlPointer)
        {
            XmlElement* sampleItemsXml = libraryXmlPointer->getChildByName("SampleItems");
            
            for (XmlElement* sampleItemXml : sampleItemsXml->getChildIterator())
            {
                SampleItem* sampleItem = new SampleItem();
                String filePath = sampleItemXml->getStringAttribute("FilePath");
                sampleItem->setFilePath(filePath);
                XmlElement* sampleTagsXml = sampleItemXml->getChildByName("SampleTags");
                
                for (XmlElement* sampleTagXml : sampleTagsXml->getChildIterator())
                {
                    String tagName = sampleTagXml->getTagName();
                    double tagValue = sampleTagXml->getDoubleAttribute("TagValue");
                    
                    sampleItem->addSampleTag(new SampleTag(tagName, tagValue));
                }
                
                inSampleItems->add(sampleItem);
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}

String SampleLibraryManager::getLibraryFilesDirectoryPath()
{
    return
        (File::getSpecialLocation(File::userMusicDirectory)).getFullPathName()
        + DIRECTORY_SEPARATOR
        + "Plugins"
        + DIRECTORY_SEPARATOR
        + "Saempl"
        + DIRECTORY_SEPARATOR
        + "SampleLibraryFiles";
}
