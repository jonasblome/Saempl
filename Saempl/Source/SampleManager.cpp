/*
  ==============================================================================

    SampleItemManager.cpp
    Created: 10 Jun 2023 6:16:05pm
    Author:  Jonas Blome

  ==============================================================================
*/

#include "SampleManager.h"


SampleManager::SampleManager()
{
    
}

SampleManager::~SampleManager()
{
    
}

void SampleManager::createSampleFile(SampleItem& inSampleItem)
{
    // Create sample file as xml file
    XmlElement sampleFileXml("Blome_SampleFile");
    
    // Adding sample tags to sample file xml
    XmlElement* sampleItemTags = new XmlElement("SampleTags");
    
    for(SampleTag* tag : *inSampleItem.getSampleTags())
    {
        XmlElement* sampleItemTag = new XmlElement(tag->getTagName());
        sampleItemTag->setAttribute("TagValue", tag->getValue());
        sampleItemTags->addChildElement(sampleItemTag);
    }
    
    sampleFileXml.addChildElement(sampleItemTags);
    
    // Write sample file xml to external file
    File sampleFile = File(inSampleItem.getFilePath() + SAMPLE_FILE_EXTENSION);
    
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

SampleItem* SampleManager::loadSampleFile(String inFilePath)
{
    File sampleFile = File(inFilePath);
    MemoryBlock sampleFileMemoryBlock;
    sampleFile.loadFileAsData(sampleFileMemoryBlock);
    XmlElement sampleFileXml = *AudioPluginInstance::getXmlFromBinary(sampleFileMemoryBlock.getData(), (int) sampleFileMemoryBlock.getSize());
    XmlElement* sampleFileXmlPointer = &sampleFileXml;
    
    SampleItem* newSampleItem = new SampleItem();
    
    if(sampleFileXmlPointer)
    {
        const String sampleItemFilePath = sampleFile.getParentDirectory().getFullPathName() + DIRECTORY_SEPARATOR + sampleFile.getFileNameWithoutExtension();
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

void SampleManager::getXmlForSampleItem(SampleItem& inSampleItem, XmlElement* inElement)
{
    
}

