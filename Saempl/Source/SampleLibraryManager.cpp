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
    
}

SampleLibraryManager::~SampleLibraryManager()
{
    
}

void SampleLibraryManager::updateSampleLibraryFile(String inLibraryFilePath, OwnedArray<SampleItem>* inSampleItems)
{
    // Create sample library file as xml
    XmlElement sampleLibraryXml("SampleLibrary");
    
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
    File sampleLibraryFile = File(inLibraryFilePath);
    
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

void SampleLibraryManager::loadSampleLibraryFile(String inFilePath, OwnedArray<SampleItem>* inSampleItems)
{
    File libraryFile = File(inFilePath);
    
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
            
            for (auto* sampleItemXml : sampleItemsXml->getChildIterator())
            {
                SampleItem* sampleItem = new SampleItem();
                sampleItem->setFilePath(sampleItemXml->getStringAttribute("FilePath"));
                XmlElement* sampleTagsXml = sampleItemXml->getChildByName("SampleTags");
                
                for (auto* sampleTagXml : sampleTagsXml->getChildIterator())
                {
                    String tagName = sampleTagXml->getTagName();
                    float tagValue = sampleTagXml->getDoubleAttribute("TagValue");
                    
                    sampleItem->addSampleTag(new SampleTag(tagName, tagValue));
                }
                
                inSampleItems->add(sampleItem);
            }
        }
        else
        {
            jassertfalse;
            return;
        }
    }
    else
    {
        jassertfalse;
        return;
    }
}

