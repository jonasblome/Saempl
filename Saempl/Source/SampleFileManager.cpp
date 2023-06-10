/*
  ==============================================================================

    SampleFileManager.cpp
    Created: 10 Jun 2023 6:16:05pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleFileManager.h"


SampleFileManager::SampleFileManager()
{
    
}

SampleFileManager::~SampleFileManager()
{
    
}

void SampleFileManager::createSampleFile(SampleItem& inSampleItem)
{
    // Create sample file as xml file
    XmlElement sampleFileXml("Blome_SampleFile");
    
    // Adding file path to sample file xml
    sampleFileXml.setAttribute("FilePath", inSampleItem.getFilePath());
    
    // Adding sample tags to sample file xml
    XmlElement* sampleTags = new XmlElement("SampleTags");
    
    for(SampleTag* tag : *inSampleItem.getSampleTags())
    {
        XmlElement* sampleTag = new XmlElement(tag->getName());
        sampleTag->setAttribute("TagValue", tag->getValue());
        sampleTags->addChildElement(sampleTag);
    }
    
    sampleFileXml.addChildElement(sampleTags);
    
    // Write sample file xml to external file
    File audioFile = File(inSampleItem.getFilePath());
    File sampleFile = File(audioFile.getParentDirectory().getFullPathName() + audioFile.getFileNameWithoutExtension() + SAMPLE_FILE_EXTENSION);
    
    if(!sampleFile.exists()) {
        sampleFile.create();
    }
    else {
        sampleFile.deleteFile();
    }
    
    MemoryBlock destinationData;
    AudioPluginInstance::copyXmlToBinary(sampleFileXml, destinationData);
    sampleFile.appendData(destinationData.getData(),
                          destinationData.getSize());
}

SampleItem* SampleFileManager::loadSampleFile(String inFilePath)
{
    File sampleFile = File(inFilePath);
    MemoryBlock sampleFileMemoryBlock;
    sampleFile.loadFileAsData(sampleFileMemoryBlock);
    XmlElement sampleFileXml = *AudioPluginInstance::getXmlFromBinary(sampleFileMemoryBlock.getData(), (int) sampleFileMemoryBlock.getSize());
    XmlElement* sampleFileXmlPointer = &sampleFileXml;
    
    SampleItem* newSampleItem = new SampleItem();
    
    if(sampleFileXmlPointer)
    {
        const String sampleItemFilePath = sampleFileXmlPointer->getAttributeValue(0);
        newSampleItem->setFilePath(sampleItemFilePath);
        
        XmlElement* sampleTagsXml = sampleFileXmlPointer->getChildByName("SampleTags");
        
        for (auto* sampleTagXml: sampleTagsXml->getChildIterator())
        {
            String tagName = sampleTagXml->getTagName();
            float tagValue = sampleTagXml->getDoubleAttribute("TagValue");
            
            newSampleItem->addSampleTag(new SampleTag(tagName, tagValue));
        }
        
        return newSampleItem;
    }
    else
    {
        jassertfalse;
        return nullptr;
    }
}

void SampleFileManager::getXmlForSampleItem(SampleItem& inSampleItem, XmlElement* inElement)
{
    
}

