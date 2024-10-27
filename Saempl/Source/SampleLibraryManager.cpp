/*
 ==============================================================================
 
 SampleLibraryManager.cpp
 Author:  Jonas Blome
 
 ==============================================================================
 */

#include "SampleLibraryManager.h"

SampleLibraryManager::SampleLibraryManager(OwnedArray<SampleItem>& inAllSampleItems,
                                           OwnedArray<SampleItem>& inFavouriteSampleItems,
                                           OwnedArray<SampleItem>& inDeletedSampleItems,
                                           OwnedArray<SampleItem>& inAddedSampleItems,
                                           OwnedArray<SampleItem>& inAlteredSampleItems)
:
ThreadWithProgressWindow("Synching sample library", true, true, 100000, "Stop loading", nullptr),
ThreadPool(SystemStats::getNumCpus()),
allSampleItems(inAllSampleItems),
favouriteSampleItems(inFavouriteSampleItems),
deletedSampleItems(inDeletedSampleItems),
addedSampleItems(inAddedSampleItems),
alteredSampleItems(inAlteredSampleItems)
{
    checkValidityOfLibraryFiles();
}

SampleLibraryManager::~SampleLibraryManager()
{
    
}

void SampleLibraryManager::updateSampleLibraryFiles()
{
    setProgress(0.0);
    setStatusMessage("Removing duplicate sample references");
    
    // Removing duplicates from library
    for (int i = 0; i < addedFilePaths.size() - 1; ++i)
    {
        String s = addedFilePaths.getReference(i);
        
        for (int nextIndex = i + 1; ; )
        {
            nextIndex = addedFilePaths.indexOf(s, false, nextIndex);
            
            if (nextIndex == -1)
            {
                break;
            }
            
            deletedSampleItems.add(getSampleItemWithFilePath(s));
            addedFilePaths.remove(nextIndex);
        }
        
        setProgress(i / ((double) addedFilePaths.size() - 1));
    }
    
    setProgress(0.0);
    setStatusMessage("Writing data to library files");
    
    int numItemsToUpdate = deletedSampleItems.size() + addedSampleItems.size() + alteredSampleItems.size();
    int numUpdatedItems = 0;
    
    if (numItemsToUpdate == 0)
    {
        return;
    }
    
    Array<File> allDirectories = File(libraryDirectory).findChildFiles(File::findDirectories, true);
    allDirectories.add(File(libraryDirectory));
    
    // Create library files directory if non existent
    File libraryFileDirectory(mLibraryFilesDirectoryPath);
    
    if (!libraryFileDirectory.exists())
    {
        libraryFileDirectory.createDirectory();
    }
    
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
            sampleLibraryXml.removeAttribute("CurrentVersion");
            sampleLibraryXml.setAttribute("CurrentVersion", currentVersion);
            sampleItemsXml = sampleLibraryXml.getChildByName("SampleItems");
        }
        else
        {
            libraryPath = libraryFile.getFullPathName();
#if JUCE_MAC
            libraryPath = libraryPath.convertToPrecomposedUnicode();
#endif
            
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
                sampleItemsXml->removeChildElement(sampleItemsXml->getChildByName(encodeForXml(sampleItem->getFilePath())),
                                                   true);
                numUpdatedItems++;
                setProgress(numUpdatedItems / (double) numItemsToUpdate);
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
                setProgress(numUpdatedItems / (double) numItemsToUpdate);
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
                setProgress(numUpdatedItems / (double) numItemsToUpdate);
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
    String statusMessage = String(std::to_string(numProcessedItems) + "/" + std::to_string(numItemsToProcess) + " Samples analysed" + "\n" + "Est. time remaining: ");
    int64 msSinceStart = Time::currentTimeMillis() - startTime;
    float estimatedSecondsRemaining = ((msSinceStart / numProcessedItems) * (numItemsToProcess - numProcessedItems)) / 1000;
    
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
    int64 startTime = Time::currentTimeMillis();
    setProgress(0.0);
    setStatusMessage("Loading...");
    Array<File> allSampleFiles = libraryDirectory.findChildFiles(File::findFiles,
                                                                 true,
                                                                 SUPPORTED_AUDIO_FORMATS_WILDCARD);
    
    // Go through all current sample items,
    // check if corresponding audio file still exists...
    setStatusMessage("Looking for deleted samples");
    int si = 0;
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
        
        si++;
        setProgress(si / (double) allSampleItems.size());
    }
    
    // ...and if not, delete sample item
    // from all items and favourites collection
    setProgress(0.0);
    setStatusMessage("Removing deleted samples from library file");
    int dsi = 0;
    for (SampleItem* sampleItem : deletedSampleItems)
    {
        addedFilePaths.removeString(sampleItem->getFilePath());
        favouriteSampleItems.removeObject(sampleItem, false);
        allSampleItems.removeObject(sampleItem, false);
        addedSampleItems.removeObject(sampleItem, false);
        alteredSampleItems.removeObject(sampleItem, false);
        
        dsi++;
        setProgress(dsi / (double) deletedSampleItems.size());
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
    int numItemsToProcess = jmax<int>(0, allSampleFiles.size() - allSampleItems.size());
    numProcessedItems = 0;
    setProgress(0.0);
    
    for (File const & sampleFile : allSampleFiles)
    {
        if (threadShouldExit())
        {
            break;
        }
        
        String sampleFileName = sampleFile.getFullPathName();
#if JUCE_MAC
        sampleFileName = sampleFileName.convertToPrecomposedUnicode();
#endif
        
        if (isLoadingNewLibrary || !fileHasBeenAdded(sampleFileName))
        {
            analyseSampleItem(nullptr, sampleFile, false);
        }
        
        // Set progress and status message
        setProgressAndStatus(numItemsToProcess, startTime);
    }
    
    // Wait for all jobs to finish
    while (getNumJobs() != 0)
    {
        if (threadShouldExit())
        {
            // Remove all queued jobs
            removeAllJobs(false, 100000);
            
            while (getNumJobs() != 0)
            {
                // Wait for currently running jobs to finish
            }
            
            break;
        }
        
        // Check if jobs are finished and update progress/status message
        setProgressAndStatus(numItemsToProcess, startTime);
    }
    
    if (deletedSampleItems.size() + addedSampleItems.size() != 0)
    {
        updateSampleLibraryFiles();
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
    
//    // BPM statistics
//    numWithinTwoBPM = 0;
//    numWithinFiveBPM = 0;
//    numWithinTenBPM = 0;
//    numFalseBPMDetected = 0;
//
//    // Key statistics
//    numCorrectKey = 0;
//    numWithinOneKey = 0;
//    numWithinThreeKey = 0;
//    numFalseKeyDetected = 0;
    
    loadSampleLibraryFile(inLibraryDirectory);
    
//    // BPM statistics
//    int totalCorrectBPMDetections = numWithinTwoBPM + numWithinFiveBPM + numWithinTenBPM;
//    int totalBPMDetections = totalCorrectBPMDetections + numFalseBPMDetected;
//
//    if (totalBPMDetections != 0)
//    {
//        float totalCorrectBPMPercentage = totalCorrectBPMDetections * 1.0 / totalBPMDetections;
//        float withinTwoPercentage = numWithinTwoBPM * 1.0 / totalBPMDetections;
//        float withinFivePercentage = numWithinFiveBPM * 1.0 / totalBPMDetections;
//        float withinTenPercentage = numWithinTenBPM * 1.0 / totalBPMDetections;
//        float falseBPMDetectionsPercentage = numFalseBPMDetected * 1.0 / totalBPMDetections;
//    }
//
//    // Key statistics
//    int totalCorrectKeyDetections = numCorrectKey + numWithinOneKey + numWithinThreeKey;
//    int totalKeyDetections = totalCorrectKeyDetections + numFalseKeyDetected;
//
//    if (totalKeyDetections != 0)
//    {
//        float totalCorrectKeyPercentage = totalCorrectKeyDetections * 1.0 / totalKeyDetections;
//        float correctKeyPercentage = numCorrectKey * 1.0 / totalKeyDetections;
//        float withinOneKeyPercentage = numWithinOneKey * 1.0 / totalKeyDetections;
//        float withinThreeKeyPercentage = numWithinThreeKey * 1.0 / totalKeyDetections;
//        float falseKeyDetectionsPercentage = numFalseKeyDetected * 1.0 / totalKeyDetections;
//    }
    
    if (libraryHasOldVersion)
    {
        AlertWindow::showAsync(MessageBoxOptions()
                               .withIconType(MessageBoxIconType::NoIcon)
                               .withTitle("Sample analysis updated!")
                               .withMessage("The sample analysis was updated. Unfortunately this means that all samples in this library  that were last used in an older version have to be reanalysed. But the analysis is now better, so don't be sad.")
                               .withButton("OK"),
                               nullptr);
        
        libraryHasOldVersion = false;
    }
    
    synchWithLibraryDirectory();
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
    
    // Adding LUFS dynamic range property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[6]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getDynamicRange());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding spectral centroid property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[7]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getSpectralCentroid());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding spectral spread property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[8]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getSpectralSpread());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding spectral rolloff property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[9]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getSpectralRolloff());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding spectral flux property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[10]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getSpectralFlux());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding chroma flux property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[11]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getChromaFlux());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding Zero Crossing Rate property
    samplePropertyXml = new XmlElement(PROPERTY_NAMES[12]);
    samplePropertyXml->setAttribute("PropertyValue", sampleItem->getZeroCrossingRate());
    samplePropertiesXml->prependChildElement(samplePropertyXml);
    
    // Adding feature vector
    std::vector<float> spectralDistribution = sampleItem->getSpectralDistribution();
    
    for (int d = 0; d < spectralDistribution.size(); d++)
    {
        String childName = "SB" + std::to_string(d);
        samplePropertiesXml->setAttribute(childName, spectralDistribution[d]);
    }
    
    // Adding feature vector
    std::vector<float> chromaDistribution = sampleItem->getChromaDistribution();
    
    for (int d = 0; d < chromaDistribution.size(); d++)
    {
        String childName = "CH" + std::to_string(d);
        samplePropertiesXml->setAttribute(childName, chromaDistribution[d]);
    }
    
    sampleItemXml->prependChildElement(samplePropertiesXml);
}

void SampleLibraryManager::createSampleItemFromXml(const XmlElement * sampleItemXml)
{
    String filePath = sampleItemXml->getStringAttribute("FilePath");
#if JUCE_MAC
    filePath = filePath.convertToPrecomposedUnicode();
#endif
    SampleItem* sampleItem = allSampleItems.add(new SampleItem());
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
//    if (tempo != 0)
//    {
//        evaluateTempoDetection(tempo, title);
//    }
    
    // Adding key property to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[5]);
    int key = samplePropertyXml->getIntAttribute("PropertyValue");
    sampleItem->setKey(key);
//    if (key != SAMPLE_TOO_LONG_INDEX)
//    {
//        evaluateKeyDetection(key, title);
//    }
    
    // Adding dynamic range to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[6]);
    float dynamicRange = samplePropertyXml->getDoubleAttribute("PropertyValue");
    sampleItem->setDynamicRange(dynamicRange);
    
    // Adding spectral centroid to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[7]);
    float centroid = samplePropertyXml->getDoubleAttribute("PropertyValue");
    sampleItem->setSpectralCentroid(centroid);
    
    // Adding spectral spread to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[8]);
    float spread = samplePropertyXml->getDoubleAttribute("PropertyValue");
    sampleItem->setSpectralSpread(spread);
    
    // Adding spectral rolloff to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[9]);
    float rolloff = samplePropertyXml->getDoubleAttribute("PropertyValue");
    sampleItem->setSpectralRolloff(rolloff);
    
    // Adding spectral flux to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[10]);
    float spectralFlux = samplePropertyXml->getDoubleAttribute("PropertyValue");
    sampleItem->setSpectralFlux(spectralFlux);
    
    // Adding chroma flux to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[11]);
    float chromaFlux = samplePropertyXml->getDoubleAttribute("PropertyValue");
    sampleItem->setChromaFlux(chromaFlux);
    
    // Adding Zero Crossing Rate to item
    samplePropertyXml = samplePropertiesXml->getChildByName(PROPERTY_NAMES[12]);
    float zcr = samplePropertyXml->getDoubleAttribute("PropertyValue");
    sampleItem->setZeroCrossingRate(zcr);
    
    // Adding spectral distribution to item
    std::vector<float> spectralDistribution(NUM_SPECTRAL_BANDS);
    
    for (int sb = 0; sb < NUM_SPECTRAL_BANDS; sb++)
    {
        String attributeName = "SB" + std::to_string(sb);
        spectralDistribution[sb] = samplePropertyXml->getDoubleAttribute(attributeName);
    }
    
    sampleItem->setSpectralDistribution(spectralDistribution);
    
    // Adding spectral distribution to item
    std::vector<float> chromaDistribution(NUM_CHROMA);
    
    for (int sb = 0; sb < NUM_CHROMA; sb++)
    {
        String attributeName = "CH" + std::to_string(sb);
        chromaDistribution[sb] = samplePropertiesXml->getDoubleAttribute(attributeName);
    }
    
    sampleItem->setChromaDistribution(chromaDistribution);
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
                createSampleItemFromXml(sampleItemXml);
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
    InterProcessLock::ScopedLockType const scopedLock(mFileLock);
    
    MemoryBlock fileData;
    inFile.loadFileAsData(fileData);
    XmlElement fileXml = *AudioPluginInstance::getXmlFromBinary(fileData.getData(), (int) fileData.getSize());
    
    return fileXml;
}

void SampleLibraryManager::writeXmlToFile(XmlElement& inXml, File& inFile)
{
    InterProcessLock::ScopedLockType const scopedLock(mFileLock);
    
    MemoryBlock destinationData;
    AudioPluginInstance::copyXmlToBinary(inXml, destinationData);
    inFile.replaceWithData(destinationData.getData(), destinationData.getSize());
}

SampleItem* SampleLibraryManager::createSampleItem(File const & inFile)
{
    SampleItem* newItem = allSampleItems.add(new SampleItem());
    String filePath = inFile.getFullPathName();
    String sampleTitle = inFile.getFileNameWithoutExtension();
#if JUCE_MAC
    filePath = filePath.convertToPrecomposedUnicode();
    sampleTitle = sampleTitle.convertToPrecomposedUnicode();
#endif
    newItem->setFilePath(filePath);
    newItem->setTitle(sampleTitle);
    analyseSampleItem(newItem, inFile, false);
    addedFilePaths.add(newItem->getFilePath());
    
    return newItem;
}

SampleItem* SampleLibraryManager::getSampleItemWithFilePath(String const & inFileName)
{
    for (SampleItem* sampleItem : allSampleItems)
    {
        String sampleItemFilePath = sampleItem->getFilePath();
        String fileName = inFileName;
#if JUCE_MAC
        sampleItemFilePath = sampleItemFilePath.convertToPrecomposedUnicode();
        fileName = fileName.convertToPrecomposedUnicode();
#endif
        
        if (sampleItemFilePath.compare(fileName) == 0)
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
    addJob(new SampleAnalysisJob(allSampleItems,
                                addedSampleItems,
                                addedFilePaths,
                                inFile,
                                inSampleItem,
                                forceAnalysis,
                                numProcessedItems),
           true);
}

String SampleLibraryManager::encodeForXml(String inString)
{
    inString = String("_" + inString);
    inString = inString.replaceCharacters("äàáâæãåāöôòóõœøōüûùúūßśšçćčéèêëėîïíīìñńÿΛ°§´`’…",
                                          "aaaaaaaaoooooooouuuuusssccceeeeeiiiiinny_______");
    std::string data = inString.toStdString();
    std::string buffer;
    buffer.reserve(data.size());
    
    for (size_t pos = 0; pos != data.size(); ++pos)
    {
        switch(data[pos]) {
            case '&':  buffer.append("_"); break;
            case '\"': buffer.append("_"); break;
            case '\'': buffer.append("_"); break;
            case '/':  buffer.append("_"); break;
            case ' ':  buffer.append("_"); break;
            case '.':  buffer.append("_"); break;
            case ',':  buffer.append("_"); break;
            case '#':  buffer.append("_"); break;
            case ')':  buffer.append("_"); break;
            case '(':  buffer.append("_"); break;
            case '[':  buffer.append("_"); break;
            case ']':  buffer.append("_"); break;
            case '{':  buffer.append("_"); break;
            case '}':  buffer.append("_"); break;
            case '<':  buffer.append("_"); break;
            case '>':  buffer.append("_"); break;
            case '@':  buffer.append("_"); break;
            case '+':  buffer.append("_"); break;
            case '*':  buffer.append("_"); break;
            case '~':  buffer.append("_"); break;
            case '%':  buffer.append("_"); break;
            case '!':  buffer.append("_"); break;
            case '?':  buffer.append("_"); break;
            case '^':  buffer.append("_"); break;
            case '$':  buffer.append("_"); break;
            case '=':  buffer.append("_"); break;
            case ':':  buffer.append("_"); break;
            case ';':  buffer.append("_"); break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }
    
    data.swap(buffer);
    
    return String(data);
}

void SampleLibraryManager::evaluateTempoDetection(int detectedTempo, const String& title)
{
    if (title.containsIgnoreCase("bpm"))
    {
        String bpmString = title.upToFirstOccurrenceOf("bpm", false, true);
        bpmString = encodeForXml(bpmString);
        
        if (bpmString.getLastCharacter() == '_')
        {
            bpmString = bpmString.substring(0, bpmString.length() - 1);
        }
        
        bpmString = bpmString.fromLastOccurrenceOf("_", false, true);
        int actualTempo = bpmString.getIntValue();
        
        if ((actualTempo - 2 <= detectedTempo && detectedTempo <= actualTempo + 2) || (actualTempo - 2 <= detectedTempo / 2.0 && detectedTempo / 2.0 <= actualTempo + 2) || (actualTempo - 2 <= detectedTempo * 2.0 && detectedTempo * 2.0 <= actualTempo + 2))
        {
            numWithinTwoBPM++;
        }
        else if ((actualTempo - 5 <= detectedTempo && detectedTempo <= actualTempo + 5) || (actualTempo - 5 <= detectedTempo / 2.0 && detectedTempo / 2.0 <= actualTempo + 5) || (actualTempo - 5 <= detectedTempo * 2.0 && detectedTempo * 2.0 <= actualTempo + 5))
        {
            numWithinFiveBPM++;
        }
        else if ((actualTempo - 10 <= detectedTempo && detectedTempo <= actualTempo + 10) || (actualTempo - 10 <= detectedTempo / 2.0 && detectedTempo / 2.0 <= actualTempo + 10) || (actualTempo - 10 <= detectedTempo * 2.0 && detectedTempo * 2.0 <= actualTempo + 10))
        {
            numWithinTenBPM++;
        }
        else
        {
            numFalseBPMDetected++;
        }
    }
}

void SampleLibraryManager::evaluateKeyDetection(int key, const String& title)
{
    String keyString = title.removeCharacters(" _-");
    
    // Minor keys
    if (keyString.containsIgnoreCase("ebmin") || keyString.containsIgnoreCase("d#min"))
    {
        if (key == 0)
        {
            numCorrectKey++;
        }
        else if (key == 1 || key == 11)
        {
            numWithinOneKey++;
        }
        else if (key == 2 || key == 3 || key == 10 || key == 9)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("bbmin") || keyString.containsIgnoreCase("a#min"))
    {
        if (key == 1)
        {
            numCorrectKey++;
        }
        else if (key == 2 || key == 0)
        {
            numWithinOneKey++;
        }
        else if (key == 3 || key == 4 || key == 11 || key == 10)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("fmin"))
    {
        if (key == 2)
        {
            numCorrectKey++;
        }
        else if (key == 3 || key == 1)
        {
            numWithinOneKey++;
        }
        else if (key == 4 || key == 5 || key == 0 || key == 11)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("cmin"))
    {
        if (key == 3)
        {
            numCorrectKey++;
        }
        else if (key == 4 || key == 2)
        {
            numWithinOneKey++;
        }
        else if (key == 5 || key == 6 || key == 1 || key == 0)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("gmin"))
    {
        if (key == 4)
        {
            numCorrectKey++;
        }
        else if (key == 5 || key == 3)
        {
            numWithinOneKey++;
        }
        else if (key == 6 || key == 7 || key == 2 || key == 1)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("dmin"))
    {
        if (key == 5)
        {
            numCorrectKey++;
        }
        else if (key == 6 || key == 4)
        {
            numWithinOneKey++;
        }
        else if (key == 7 || key == 8 || key == 3 || key == 2)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("amin"))
    {
        if (key == 6)
        {
            numCorrectKey++;
        }
        else if (key == 7 || key == 5)
        {
            numWithinOneKey++;
        }
        else if (key == 8 || key == 9 || key == 4 || key == 3)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("emin"))
    {
        if (key == 7)
        {
            numCorrectKey++;
        }
        else if (key == 8 || key == 6)
        {
            numWithinOneKey++;
        }
        else if (key == 9 || key == 10 || key == 5 || key == 4)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("bmin"))
    {
        if (key == 8)
        {
            numCorrectKey++;
        }
        else if (key == 9 || key == 7)
        {
            numWithinOneKey++;
        }
        else if (key == 10 || key == 11 || key == 6 || key == 5)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("f#min") || keyString.containsIgnoreCase("gbmin"))
    {
        if (key == 9)
        {
            numCorrectKey++;
        }
        else if (key == 10 || key == 8)
        {
            numWithinOneKey++;
        }
        else if (key == 11 || key == 0 || key == 7 || key == 6)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("c#min") || keyString.containsIgnoreCase("dbmin"))
    {
        if (key == 10)
        {
            numCorrectKey++;
        }
        else if (key == 11 || key == 9)
        {
            numWithinOneKey++;
        }
        else if (key == 0 || key == 1 || key == 8 || key == 7)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("g#min") || keyString.containsIgnoreCase("abmin"))
    {
        if (key == 11)
        {
            numCorrectKey++;
        }
        else if (key == 0 || key == 10)
        {
            numWithinOneKey++;
        }
        else if (key == 1 || key == 2 || key == 9 || key == 8)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    
    // Major keys
    else if (keyString.containsIgnoreCase("gbmaj") || keyString.containsIgnoreCase("f#maj"))
    {
        if (key == 0)
        {
            numCorrectKey++;
        }
        else if (key == 1 || key == 11)
        {
            numWithinOneKey++;
        }
        else if (key == 2 || key == 3 || key == 10 || key == 9)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("dbmaj") || keyString.containsIgnoreCase("c#maj"))
    {
        if (key == 1)
        {
            numCorrectKey++;
        }
        else if (key == 2 || key == 0)
        {
            numWithinOneKey++;
        }
        else if (key == 3 || key == 4 || key == 11 || key == 10)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("abmaj") || keyString.containsIgnoreCase("g#maj"))
    {
        if (key == 2)
        {
            numCorrectKey++;
        }
        else if (key == 3 || key == 1)
        {
            numWithinOneKey++;
        }
        else if (key == 4 || key == 5 || key == 0 || key == 11)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("ebmaj") || keyString.containsIgnoreCase("d#maj"))
    {
        if (key == 3)
        {
            numCorrectKey++;
        }
        else if (key == 4 || key == 2)
        {
            numWithinOneKey++;
        }
        else if (key == 5 || key == 6 || key == 1 || key == 0)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("bbmaj") || keyString.containsIgnoreCase("a#maj"))
    {
        if (key == 4)
        {
            numCorrectKey++;
        }
        else if (key == 5 || key == 3)
        {
            numWithinOneKey++;
        }
        else if (key == 6 || key == 7 || key == 2 || key == 1)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("fmaj"))
    {
        if (key == 5)
        {
            numCorrectKey++;
        }
        else if (key == 6 || key == 4)
        {
            numWithinOneKey++;
        }
        else if (key == 7 || key == 8 || key == 3 || key == 2)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("cmaj"))
    {
        if (key == 6)
        {
            numCorrectKey++;
        }
        else if (key == 7 || key == 5)
        {
            numWithinOneKey++;
        }
        else if (key == 8 || key == 9 || key == 4 || key == 3)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("gmaj"))
    {
        if (key == 7)
        {
            numCorrectKey++;
        }
        else if (key == 8 || key == 6)
        {
            numWithinOneKey++;
        }
        else if (key == 9 || key == 10 || key == 5 || key == 4)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("dmaj"))
    {
        if (key == 8)
        {
            numCorrectKey++;
        }
        else if (key == 9 || key == 7)
        {
            numWithinOneKey++;
        }
        else if (key == 10 || key == 11 || key == 6 || key == 5)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("amaj"))
    {
        if (key == 9)
        {
            numCorrectKey++;
        }
        else if (key == 10 || key == 8)
        {
            numWithinOneKey++;
        }
        else if (key == 11 || key == 0 || key == 7 || key == 6)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("emaj"))
    {
        if (key == 10)
        {
            numCorrectKey++;
        }
        else if (key == 11 || key == 9)
        {
            numWithinOneKey++;
        }
        else if (key == 0 || key == 1 || key == 8 || key == 7)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
    else if (keyString.containsIgnoreCase("bmaj"))
    {
        if (key == 11)
        {
            numCorrectKey++;
        }
        else if (key == 0 || key == 10)
        {
            numWithinOneKey++;
        }
        else if (key == 1 || key == 2 || key == 9 || key == 8)
        {
            numWithinThreeKey++;
        }
        else
        {
            numFalseKeyDetected++;
        }
    }
}

void SampleLibraryManager::checkValidityOfLibraryFiles()
{
    Array<File> libraryFiles = File(mLibraryFilesDirectoryPath).findChildFiles(File::findFiles, false);
    Array<File> libraryFilesToDelete;
    
    for (File libraryFile : libraryFiles)
    {
        if (libraryFile.getFileExtension() != ".bslf")
        {
            continue;
        }
        
        XmlElement libraryFileXml = loadFileAsXml(libraryFile);
        
        if (!File(libraryFileXml.getStringAttribute("LibraryPath")).exists())
        {
            libraryFilesToDelete.add(libraryFile);
        }
    }
    
    for (File libraryFileToDelete : libraryFilesToDelete)
    {
        libraryFileToDelete.deleteFile();
    }
}
