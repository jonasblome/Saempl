/*
 ==============================================================================
 
 SampleLibraryManager.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleLibraryManager.h"

SampleLibraryManager::SampleLibraryManager(OwnedArray<SampleItem>& inAllSampleItems, OwnedArray<SampleItem>& inPaletteSampleItems, OwnedArray<SampleItem>& inDeletedSampleItems, OwnedArray<SampleItem>& inAddedSampleItems, OwnedArray<SampleItem>& inAlteredSampleItems)
:
ThreadWithProgressWindow("Synching sample library", true, true, 100000, "Stop loading", nullptr),
ThreadPool(SystemStats::getNumCpus()),
allSampleItems(inAllSampleItems),
paletteSampleItems(inPaletteSampleItems),
deletedSampleItems(inDeletedSampleItems),
addedSampleItems(inAddedSampleItems),
alteredSampleItems(inAlteredSampleItems)
{
    
}

SampleLibraryManager::~SampleLibraryManager()
{
    
}

void SampleLibraryManager::writeSampleItemToXml(SampleItem *sampleItem, XmlElement *sampleItemXml)
{
    sampleItemXml->setAttribute("FilePath", sampleItem->getFilePath());
    
    // Adding sample properties xml to sample item xml
    XmlElement* samplePropertiesXml = new XmlElement("SampleProperties");
    
    // Adding title property
    XmlElement* samplePropertyXml = new XmlElement(PROPERTY_NAMES[0]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getTitle());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding length property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[1]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getLength());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding decibel loudness property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[2]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getLoudnessDecibel());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding LUFS loudness property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[3]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getLoudnessLUFS());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding tempo property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[4]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getTempo());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding key property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[5]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getKey());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding feature vector
    std::vector<float> featureVector = sampleItem->getFeatureVector();
    
    for (int d = 0; d < featureVector.size(); d++)
    {
        String childName = "FV" + std::to_string(d);
        samplePropertyXml = new XmlElement(childName);
        samplePropertyXml->setAttribute("PropertyValue", featureVector[d]);
        samplePropertiesXml->prependChildElement(samplePropertyXml);
    }
    
    sampleItemXml->prependChildElement(samplePropertiesXml);
}

void SampleLibraryManager::updateSampleLibraryFile()
{
    int numItemsToUpdate = deletedSampleItems.size() + addedSampleItems.size() + alteredSampleItems.size();
    int numUpdatedItems = 0;
    
    if (numItemsToUpdate == 0)
    {
        return;
    }
    
    Array<File> allDirectories = File(libraryDirectory).findChildFiles(File::findDirectories, true);
    allDirectories.add(File(libraryDirectory));
    
    for (File libraryFile : allDirectories)
    {
        if (numUpdatedItems == numItemsToUpdate)
        {
            break;
        }
        
        // Check if library file exists
        File sampleLibraryFile = File(mLibraryFilesDirectoryPath
                                      + DIRECTORY_SEPARATOR
                                      + encodeForXml(libraryFile.getFullPathName())
                                      + SAMPLE_LIBRARY_FILE_EXTENSION);
        XmlElement sampleLibraryXml("EMPTY");
        XmlElement* sampleItemsXml;
        String libraryPath;
        
        if (sampleLibraryFile.exists())
        {
            libraryPath = libraryFile.getFullPathName();
            sampleLibraryXml = loadFileAsXml(sampleLibraryFile);
            sampleItemsXml = sampleLibraryXml.getChildByName("SampleItems");
        }
        else
        {
            libraryPath = libraryFile.getFullPathName().convertToPrecomposedUnicode();
            
            // Create sample library file as xml and store path to library
            sampleLibraryXml = XmlElement(encodeForXml(libraryFile.getFileNameWithoutExtension()));
            sampleLibraryXml.setAttribute("LibraryPath", libraryPath);
            sampleLibraryXml.setAttribute("CurrentVersion", currentVersion);
            
            // Adding sample items xml to sample library xml
            sampleItemsXml = new XmlElement("SampleItems");
            sampleLibraryXml.prependChildElement(sampleItemsXml);
        }
        
        // Delete all sample items that are in this directory
        for (SampleItem* sampleItem : deletedSampleItems)
        {
            if (File(sampleItem->getFilePath()).getParentDirectory() == libraryPath)
            {
                sampleItemsXml->removeChildElement(sampleItemsXml->getChildByName(encodeForXml(sampleItem->getFilePath())), true);
                numUpdatedItems++;
            }
        }
        
        // Add all new sample items that are in this directory
        for (SampleItem* sampleItem : addedSampleItems)
        {
            if (File(sampleItem->getFilePath()).getParentDirectory() == libraryPath)
            {
                XmlElement* sampleItemXml = new XmlElement(encodeForXml(sampleItem->getFilePath()));
                writeSampleItemToXml(sampleItem, sampleItemXml);
                sampleItemsXml->prependChildElement(sampleItemXml);
                numUpdatedItems++;
            }
        }
        
        // Alter all sample items that are in this directory
        for (SampleItem* sampleItem : alteredSampleItems)
        {
            if (File(sampleItem->getFilePath()).getParentDirectory() == libraryPath)
            {
                XmlElement* sampleItemXml = sampleItemsXml->getChildByName(encodeForXml(sampleItem->getFilePath()));
                writeSampleItemToXml(sampleItem, sampleItemXml);
                numUpdatedItems++;
            }
        }
        
        // Write library xml to file
        writeXmlToFile(sampleLibraryXml, sampleLibraryFile);
    }
    
    deletedSampleItems.clear();
    addedSampleItems.clear(false);
    alteredSampleItems.clear(false);
}

void SampleLibraryManager::synchWithLibraryDirectory()
{
    launchThread();
}

void SampleLibraryManager::setProgressAndStatus(int numItemsToProcess, int64 startTime)
{
    setProgress(numProcessedItems / (double) numItemsToProcess);
    String statusMessage = String("\n") + "Est. time remaining: ";
    int64 msSinceStart = Time::currentTimeMillis() - startTime;
    int64 estimatedSecondsRemaining = ((msSinceStart / numProcessedItems) * (numItemsToProcess - numProcessedItems)) / 1000;
    
    if (estimatedSecondsRemaining < 60)
    {
        statusMessage = statusMessage + String::toDecimalStringWithSignificantFigures(estimatedSecondsRemaining, 2) + " second(s)";
    }
    else if (estimatedSecondsRemaining < 3600)
    {
        statusMessage = statusMessage + String::toDecimalStringWithSignificantFigures(estimatedSecondsRemaining * 1.0 / 60, 2) + " minute(s)";
    }
    else
    {
        statusMessage = statusMessage + String::toDecimalStringWithSignificantFigures(estimatedSecondsRemaining * 1.0 / 3600, 2) + " hour(s)";
    }
    
    setStatusMessage(statusMessage);
}

void SampleLibraryManager::run()
{
    // Go through all files in directory, check if a corresponding sample item
    // already exists in the sample item list, if not add it
    Array<File> allSampleFiles = libraryDirectory.findChildFiles(File::findFiles,
                                                                 true,
                                                                 SUPPORTED_AUDIO_FORMATS_WILDCARD);
    setProgress(0.0);
    int64 startTime = Time::currentTimeMillis();
    
    // Go through all current sample items,
    // check if corresponding audio file still exists
    // and if not, delete sample item
    // from all items and palette collection
    for (SampleItem* sampleItem : allSampleItems)
    {
        if (threadShouldExit())
        {
            break;
        }
        
        if (!File(sampleItem->getFilePath()).exists())
        {
            deletedSampleItems.add(sampleItem);
        }
    }
    
    for (SampleItem* sampleItem : deletedSampleItems)
    {
        addedFilePaths.removeString(sampleItem->getFilePath());
        paletteSampleItems.removeObject(sampleItem, false);
        allSampleItems.removeObject(sampleItem, false);
    }
    
    // All files have already been loaded
    if (addedFilePaths.size() == allSampleFiles.size())
    {
        setProgress(1.0);
        return;
    }
    
    // Go through all files in directory and add sample items
    // for all that are not yet loaded into the library
    bool isLoadingNewLibrary = allSampleItems.size() == 0;
    int numItemsToProcess = allSampleFiles.size() - allSampleItems.size();
    numProcessedItems = 0;
    
    for (File const & sampleFile : allSampleFiles)
    {
        if (threadShouldExit())
        {
            break;
        }
        
        if (isLoadingNewLibrary || !fileHasBeenAdded(sampleFile.getFullPathName().convertToPrecomposedUnicode()))
        {
            analyseSampleItem(nullptr, sampleFile, false);
        }
        
        // Set progress and status message
        setProgressAndStatus(numItemsToProcess, startTime);
    }
    
    while (getNumJobs() != 0)
    {
        if (threadShouldExit())
        {
            // Remove all queued jobs
            removeAllJobs(false, 100000);
            
            while (getNumJobs() != 0)
            {
                // Wait for current analysis jobs to finish
            }
            
            break;
        }
        
        // Check if jobs are finished and update progress/status message
        setProgressAndStatus(numItemsToProcess, startTime);
    }
    
    if (deletedSampleItems.size() + addedSampleItems.size() != 0)
    {
        updateSampleLibraryFile();
    }
    
    if (threadShouldExit())
    {
        return;
    }
}

void SampleLibraryManager::threadComplete(bool userPressedCancel)
{
    sendChangeMessage();
}

void SampleLibraryManager::loadSampleLibrary(File const & inLibraryDirectory)
{
    addedFilePaths.clear();
    libraryDirectory = inLibraryDirectory;
    loadSampleLibraryFile(inLibraryDirectory);
    
    if (libraryHasOldVersion)
    {
        AlertWindow::showAsync(MessageBoxOptions()
                               .withIconType(MessageBoxIconType::NoIcon)
                               .withTitle("Sample analysis updated!")
                               .withMessage("We have updated our sample analysis. Unfortunately this means that all samples in this library  that were last used in an older version have to be reanalysed. But the analysis is now better, so don't be sad.")
                               .withButton("OK"),
                               nullptr);
        
        libraryHasOldVersion = false;
    }
    
    synchWithLibraryDirectory();
}

void SampleLibraryManager::loadSampleLibraryFile(File const & inLibraryDirectory)
{
    File libraryFile = File(mLibraryFilesDirectoryPath
                            + DIRECTORY_SEPARATOR
                            + encodeForXml(inLibraryDirectory.getFullPathName())
                            + SAMPLE_LIBRARY_FILE_EXTENSION);
    
    // Check if sample library file exists
    if (libraryFile.exists())
    {
        // Get data from library file
        XmlElement libraryXml = loadFileAsXml(libraryFile);
        XmlElement* libraryXmlPointer = &libraryXml;
        
        if (libraryXmlPointer)
        {
            // Check for version compatibility
            if (libraryXmlPointer->getIntAttribute("CurrentVersion") != currentVersion)
            {
                libraryHasOldVersion = true;
                
                for (File subDirectory : inLibraryDirectory.findChildFiles(File::findDirectories, false))
                {
                    loadSampleLibraryFile(subDirectory);
                }
                
                return;
            }
            
            XmlElement* sampleItemsXml = libraryXmlPointer->getChildByName("SampleItems");
            
            // Go over all sample items
            for (XmlElement const * sampleItemXml : sampleItemsXml->getChildIterator())
            {
                // Create new sample item
                SampleItem* sampleItem = allSampleItems.add(new SampleItem());
                String filePath = sampleItemXml->getStringAttribute("FilePath").convertToPrecomposedUnicode();
                sampleItem->setFilePath(filePath);
                addedFilePaths.add(filePath);
                
                // Adding properties to item
                XmlElement* samplePropertiesXml = sampleItemXml->getChildByName("SampleProperties");
                
                // Adding title property to item
                XmlElement* samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[0]);
                String title = samplePropertyXml->getStringAttribute("PropertyValue");
                sampleItem->setTitle(title);
                
                // Adding length property to item
                samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[1]);
                double length = samplePropertyXml->getDoubleAttribute("PropertyValue");
                sampleItem->setLength(length);
                
                // Adding decibel property to item
                samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[2]);
                double loudnessDecibel = samplePropertyXml->getDoubleAttribute("PropertyValue");
                sampleItem->setLoudnessDecibel(loudnessDecibel);
                
                // Adding LUFS property to item
                samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[3]);
                double loudnessLUFS = samplePropertyXml->getDoubleAttribute("PropertyValue");
                sampleItem->setLoudnessLUFS(loudnessLUFS);
                
                // Adding tempo property to item
                samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[4]);
                int tempo = samplePropertyXml->getIntAttribute("PropertyValue");
                sampleItem->setTempo(tempo);
                
                // Adding key property to item
                samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[5]);
                int key = samplePropertyXml->getIntAttribute("PropertyValue");
                sampleItem->setKey(key);
                
                // Adding feature vector to item
                int numDimensions = NUM_CHROMA + NUM_FEATURES + NUM_SPECTRAL_BANDS;
                std::vector<float> featureVector(numDimensions);
                
                for (int d = 0; d < numDimensions; d++)
                {
                    String childName = "FV" + std::to_string(d);
                    samplePropertyXml = samplePropertiesXml->getChildByName(childName);
                    featureVector[d] = samplePropertyXml->getDoubleAttribute("PropertyValue");
                }
                
                sampleItem->setFeatureVector(featureVector);
            }
        }
    }
    
    for (File subDirectory : inLibraryDirectory.findChildFiles(File::findDirectories, false))
    {
        loadSampleLibraryFile(subDirectory);
    }
}

XmlElement SampleLibraryManager::loadFileAsXml(File& inFile)
{
    const InterProcessLock::ScopedLockType scopedLock(mFileLock);
    
    MemoryBlock fileData;
    inFile.loadFileAsData(fileData);
    XmlElement fileXml = *AudioPluginInstance::getXmlFromBinary(fileData.getData(), (int) fileData.getSize());
    
    return fileXml;
}

void SampleLibraryManager::writeXmlToFile(XmlElement& inXml, File& inFile)
{
    const InterProcessLock::ScopedLockType scopedLock(mFileLock);
    
    MemoryBlock destinationData;
    AudioPluginInstance::copyXmlToBinary(inXml, destinationData);
    inFile.replaceWithData(destinationData.getData(), destinationData.getSize());
}

SampleItem* SampleLibraryManager::createSampleItem(File const & inFile)
{
    SampleItem* newItem = allSampleItems.add(new SampleItem());
    newItem->setFilePath(inFile.getFullPathName().convertToPrecomposedUnicode());
    newItem->setTitle(inFile.getFileNameWithoutExtension().convertToPrecomposedUnicode());
    analyseSampleItem(newItem, inFile, false);
    addedFilePaths.add(newItem->getFilePath());
    
    return newItem;
}

SampleItem* SampleLibraryManager::getSampleItemWithFilePath(String const & inFileName)
{
    for (SampleItem* sampleItem : allSampleItems)
    {
        if (sampleItem->getFilePath().convertToPrecomposedUnicode().compare(inFileName.convertToPrecomposedUnicode()) == 0)
        {
            return sampleItem;
        }
    }
    
    return nullptr;
}

bool SampleLibraryManager::fileHasBeenAdded(String const & inFilePath)
{
    return addedFilePaths.contains(inFilePath);
}

void SampleLibraryManager::analyseSampleItem(SampleItem* inSampleItem, File const & inFile, bool forceAnalysis)
{
    addJob(new SampleManagerThread(allSampleItems, addedSampleItems, addedFilePaths, inFile, inSampleItem, forceAnalysis, numProcessedItems), true);
}

String SampleLibraryManager::encodeForXml(String inString)
{
    inString = String("_" + inString);
    inString = inString.replaceCharacters("äàáâæãåāöôòóõœøōüûùúūßśšçćčéèêëėîïíīìñńÿΛ", "aaaaaaaaoooooooouuuuusssccceeeeeiiiiinny_");
    std::string data = inString.toStdString();
    std::string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos)
    {
        switch(data[pos]) {
            case '&':  buffer.append("_");       break;
            case '\"': buffer.append("_");      break;
            case '\'': buffer.append("_");      break;
            case '<':  buffer.append("_");        break;
            case '>':  buffer.append("_");        break;
            case '/':  buffer.append("_");        break;
            case ' ':  buffer.append("_");        break;
            case ',':  buffer.append("_");        break;
            case ')':  buffer.append("_");        break;
            case '(':  buffer.append("_");        break;
            case '#':  buffer.append("_");        break;
            case '[':  buffer.append("_");        break;
            case ']':  buffer.append("_");        break;
            case '{':  buffer.append("_");        break;
            case '}':  buffer.append("_");        break;
            case '@':  buffer.append("_");        break;
            case '+':  buffer.append("_");        break;
            case '!':  buffer.append("_");        break;
            case '?':  buffer.append("_");        break;
            case '^':  buffer.append("_");        break;
            case '$':  buffer.append("_");        break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }
    data.swap(buffer);
    
    return String(data);
}
