/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <iostream>

using std::vector;
using namespace akateko;

//==============================================================================
AkatekoAudioProcessor::AkatekoAudioProcessor() :
    modMatrix(AkatekoMatrix()), //create first
    voiceLeft(AkatekoVoice(modMatrix)),
    voiceRight(AkatekoVoice(modMatrix)),

    lfoOne(LowFrequencyTableOsc(modMatrix.getWriteRegister(MODSRC::LFO1),
                                modMatrix.getReadRegister(DESTSRC::LFO1FREQ),
                                modMatrix.getReadRegister(DESTSRC::LFO1PHS),
                                modMatrix.getReadRegister(DESTSRC::LFO1PWM))),

    lfoTwo(LowFrequencyTableOsc(modMatrix.getWriteRegister(MODSRC::LFO2),
                                modMatrix.getReadRegister(DESTSRC::LFO2FREQ),
                                modMatrix.getReadRegister(DESTSRC::LFO2PHS),
                                modMatrix.getReadRegister(DESTSRC::LFO2PWM))),

    envOne(EnvelopeGenerator(modMatrix.getWriteRegister(MODSRC::ENV1),
                             modMatrix.getReadRegister(DESTSRC::ENV1DUR))),

    envTwo(EnvelopeGenerator(modMatrix.getWriteRegister(MODSRC::ENV2),
                             modMatrix.getReadRegister(DESTSRC::ENV2DUR))),

    stepSeq(StepSeqGenerator(modMatrix.getWriteRegister(MODSRC::SSEQ),
                             modMatrix.getReadRegister(DESTSRC::SSQDUR),
                             modMatrix.getReadRegister(DESTSRC::SSQLEN),
                             modMatrix.getReadRegister(DESTSRC::SSQOFF))),

    shapesAreInitialised(false),

#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    presetManager = new ConfigurationFileManager("presets", "*.fxp");
    initialiseParameters();

    double &tmpXPad = modMatrix.getWriteRegister(MODSRC::PADX);
    double &tmpYPad = modMatrix.getWriteRegister(MODSRC::PADY);

    tmpXPad = 0.5;
    tmpYPad = 0.5;

    effectOne = new StereoProcessor();
    effectTwo = new StereoProcessor();
}

AkatekoAudioProcessor::~AkatekoAudioProcessor()
{
}

//==============================================================================
const String AkatekoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AkatekoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AkatekoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double AkatekoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AkatekoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AkatekoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AkatekoAudioProcessor::setCurrentProgram (int index)
{
}

const String AkatekoAudioProcessor::getProgramName (int index)
{
    return String();
}

void AkatekoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AkatekoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback

    const int inputChannels = getTotalNumInputChannels();
    const double sRate = getSampleRate();

    voiceLeft.setSampleRate(sRate);
    voiceRight.setSampleRate(sRate);

    lfoOne.setSampleRate(sRate);
    lfoTwo.setSampleRate(sRate);
    envOne.setSampleRate(sRate);
    envTwo.setSampleRate(sRate);
    stepSeq.setSampleRate(sRate);


    //akatekoGlobal.setSampleRate(this->getSampleRate());

    // initialisation that you need..
}

void AkatekoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AkatekoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif



void AkatekoAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    FloatVectorOperations::disableDenormalisedNumberSupport();

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numberSamples = buffer.getNumSamples();

    /* Gather all Signal Parameters */

    const double fOneFreq = *filterOneFrequency;
    const double fOneReso = *filterOneResonance;
    const double fOnePBG = *filterOnePassBand;
    const double fOneDrive = *filterOneDrive;
    const double fOneVol = *filterOneVolume;

    const double fTwoFreq = *filterTwoFrequency;
    const double fTwoReso = *filterTwoResonance;
    const double fTwoPBG = *filterTwoPassBand;
    const double fTwoDrive = *filterTwoDrive;
    const double fTwoVol = *filterTwoVolume;

    const double wsDrive = *waveShaperDrive;
    const double wsMix = *waveShaperMix;


    //const double wsAmp = *fxInputAmp;
   // const double wsRouting = *fxRouting;


    //Left
    voiceLeft.setFilterOneDrive(fOneDrive);
    voiceLeft.setFilterTwoDrive(fTwoDrive);

    voiceLeft.setFilterOnePassBand(fOnePBG);
    voiceLeft.setFilterTwoPassBand(fTwoPBG);

    voiceLeft.setFilterOneFrequency(fOneFreq);
    voiceLeft.setFilterOneResonance(fOneReso);

    voiceLeft.setFilterTwoFrequency(fTwoFreq);
    voiceLeft.setFilterTwoResonance(fTwoReso);

    voiceLeft.setFilterOneVolume(fOneVol);
    voiceLeft.setFilterTwoVolume(fTwoVol);

    voiceLeft.setWaveShaperDrive(wsDrive);
    voiceLeft.setWaveShaperMix(wsMix);

    //Right
    voiceRight.setFilterOneDrive(fOneDrive);
    voiceRight.setFilterTwoDrive(fTwoDrive);

    voiceRight.setFilterOnePassBand(fOnePBG);
    voiceRight.setFilterTwoPassBand(fTwoPBG);

    voiceRight.setFilterOneFrequency(fOneFreq);
    voiceRight.setFilterOneResonance(fOneReso);

    voiceRight.setFilterTwoFrequency(fTwoFreq);
    voiceRight.setFilterTwoResonance(fTwoReso);

    voiceRight.setFilterOneVolume(fOneVol);
    voiceRight.setFilterTwoVolume(fTwoVol);

    voiceRight.setWaveShaperDrive(wsDrive);
    voiceRight.setWaveShaperMix(wsMix);

    // Collect Effect Parameters

    effectOne->setBlockSize(numberSamples);
    effectTwo->setBlockSize(numberSamples);

    effectOne->collect();
    effectTwo->collect();

    // Low Frequency Oscillator One
    lfoOne.setFrequency(*lfoOneFrequency);
    lfoOne.setPhase(*lfoOnePhase);
    lfoOne.setPulseWidthModulation(*lfoOnePWM);

    // Low Frequency Oscillator Two
    lfoTwo.setFrequency(*lfoTwoFrequency);
    lfoTwo.setPhase(*lfoTwoPhase);
    lfoTwo.setPulseWidthModulation(*lfoTwoPWM);

    // Envelope One
    envOne.setDuration(*envOneDuration);
    // Envelope Two
    envTwo.setDuration(*envTwoDuration);

    // Step Sequencer
    stepSeq.setStepLength(*stepSeqLength);
    stepSeq.setOffValue(*stepSeqOffset);

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.


    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
        if(totalNumInputChannels == 2){
            const float *inputLeft = buffer.getReadPointer(0, 0);
            const float *inputRight = buffer.getReadPointer(1, 0);

            float *leftChannel = buffer.getWritePointer(0, 0);
            float *rightChannel = buffer.getWritePointer(1, 0);

            for(int i=0; i<numberSamples; i++){
                double tmpLeft = inputLeft[i];
                double tmpRight = inputRight[i];

                lfoOne.update();
                lfoTwo.update();
                envOne.update();
                envTwo.update();
                stepSeq.update();

                modMatrix.update();

                voiceLeft.update();
                voiceRight.update();

                effectOne->update();
                effectTwo->update();

                lfoOne.getSample();
                lfoTwo.getSample();
                envOne.getSample();
                envTwo.getSample();
                stepSeq.getSample();

                tmpLeft = voiceLeft.getSample(tmpLeft);
                tmpRight = voiceRight.getSample(tmpRight);

                effectOne->calculateSamples(tmpLeft, tmpRight);
                tmpLeft = effectOne->getLeftSample();
                tmpRight = effectOne->getRightSample();

                effectTwo->calculateSamples(tmpLeft, tmpRight);
                tmpLeft = effectTwo->getLeftSample();
                tmpRight = effectTwo->getRightSample();

                //Multiply with main volume at a later stage
                leftChannel[i] = static_cast<float>(tmpLeft);
                rightChannel[i] = static_cast<float>(tmpRight);
            }
        }

}

//==============================================================================
bool AkatekoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AkatekoAudioProcessor::createEditor()
{
    return new AkatekoAudioProcessorEditor (*this);
}

//==============================================================================

/*
 * Save Preset, This Will open a dialog Window
 * Prompting the user for the appropiate
 * location
 */

bool AkatekoAudioProcessor::save(){
    MemoryBlock state;
    getStateInformation(state);

    return presetManager->saveFile(state);
}

bool AkatekoAudioProcessor::save(PresetRow row){
    MemoryBlock state;

    if(row.file.getFileNameWithoutExtension() !=  row.name){
        String fullPath = row.file.getParentDirectory().getFullPathName();
        fullPath += String("/") + String(row.name) + String(".fxp");

        File file(fullPath);

        row.file.deleteFile();
        row.file = file;
    }


    XmlElement xml("AKATEKOSETTINGS");

    //Add attributes
    xml.setAttribute("Name", row.name);
    xml.setAttribute("Category", row.category);
    xml.setAttribute("Author", row.author);

    for(int i=0; i<getNumParameters(); i++){
        if(AudioProcessorParameterWithID *p = dynamic_cast<AudioProcessorParameterWithID*>(getParameters().getUnchecked(i))){
            xml.setAttribute(p->paramID, p->getValue());
        }
    }

    // Gather Shapes
    for(int i=0; i<lfoOneShapes.size(); i++){
        const String xmlAttribute = String("LFOONE") + String(i);
        xml.setAttribute(xmlAttribute, lfoOneShapes[i]);
    }

    for(int i=0; i<lfoTwoShapes.size(); i++){
        const String xmlAttribute = String("LFOTWO") + String(i);
        xml.setAttribute(xmlAttribute, lfoTwoShape[i]);
    }

    for(int i=0; i<waveShapes.size(); i++){
        const String xmlAttribute = String("WSHAPE") + String(i);
        xml.setAttribute(xmlAttribute, waveShapes[i]);
    }

    // Set UI States
    xml.setAttribute("ENV1", envOneState);
    xml.setAttribute("ENV2", envTwoState);
    xml.setAttribute("SSEQ", stepSeqState);
    xml.setAttribute("XYPAD", xyPadState);
    xml.setAttribute("FXST", fxState);
    xml.setAttribute("MODM", modMatrix.toString());

    copyXmlToBinary(xml, state);

    return presetManager->saveFile(state, row.file);
}

// Changes name based on the file the preset is saved
// If The function returns true, rescan the directory
// Compare the current Files and add the new ones

bool AkatekoAudioProcessor::saveAs(PresetRow row){
    File tmpLocation;

    if(presetManager->chooseFile(tmpLocation)){
        row.file = tmpLocation;
        row.name = tmpLocation.getFileNameWithoutExtension();

        if(save(row)){
            currentPresetName = row.name;

            return true;
        } else {
            return false;
        }
    } else {
        std::cerr << "AkatekoAudioProcessor::saveAs" << std::endl;
        std::cerr << "Canceled save : " << row.name << std::endl;

        return false;
    }
}

// Load the default Preset File and set All the parmeters accordingly
// Gather the filename, name, gategory and supplied author from the
// preset row. overwrite the supplied File in the preset Row and
// overwrite all settings.

bool AkatekoAudioProcessor::clear(PresetRow row){
   // File file = File("/work/programming-projects/vst/vst-plugins/Akateko/Builds/LinuxMakefile/build/config/init.fxp");

   File fileDirectory = File(File::getCurrentWorkingDirectory());
   Array<File> files;

   fileDirectory.findChildFiles(files, 2, true, "init.fxp");

   if(!files.isEmpty()){
       if(files[0].exists()){
         MemoryBlock state;

         files[0].loadFileAsData(state);
         setStateInformation(state.getData(), state.getSize());
         save(row);
       }
   }
}

// Select preset folder return true if succesfull


bool AkatekoAudioProcessor::folder(){
    return presetManager->setDirectory();
}

/* Load preset, Will open a dialog
 * Prompting the user to select a
 * location.
 */

void AkatekoAudioProcessor::load(){
    MemoryBlock state;

    if(presetManager->openFile(state)){
        currentPresetFile = presetManager->getFileName(); //Get loaded preset Name
        setStateInformation(state.getData(), state.getSize());
    }
}

void AkatekoAudioProcessor::load(File file){
    MemoryBlock state;

    if(file.loadFileAsData(state)){
        currentPresetFile = file.getFileName();
        setStateInformation(state.getData(), state.getSize());

        if(currentPresetName.isEmpty()){
            currentPresetName = file.getFileNameWithoutExtension();
        }
    } else {
        std::cerr << "AkatekoAudioProcessor::load(File file)" << std::endl;
        std::cerr << "Could not open supplied file" << std::endl;
    }
}

String AkatekoAudioProcessor::getCurrentPresetFileName(){
    return currentPresetFile;
}

String AkatekoAudioProcessor::getCurrentPresetName(){
    return currentPresetName;
}

vector<PresetRow> AkatekoAudioProcessor::getCurrentPresets(){
    Array<File> tmpPresets;
    vector<PresetRow> presets;

    if(presetManager->scanDirectory(tmpPresets)){
        for(int i=0; i<tmpPresets.size(); i++){
            presets.push_back(PresetRow());
            presets[i].file = tmpPresets[i];

            MemoryBlock state;

            if(presetManager->openFile(state, tmpPresets[i])){
                ScopedPointer<XmlElement> xmlState (getXmlFromBinary(state.getData(), state.getSize()));

                if(xmlState != nullptr){
                    String tmpName = xmlState->getStringAttribute("Name");

                    if(tmpName.isEmpty()){
                        presets[i].name = presets[i].file.getFileNameWithoutExtension();
                    } else {
                        presets[i].name = tmpName;
                    }

                    presets[i].category = xmlState->getStringAttribute("Category");
                    presets[i].author = xmlState->getStringAttribute("Author");
                } else {
                    std::cerr << "AkatekoAudioProcessor::getCurrentPresets" << std::endl;
                    std::cerr << tmpPresets[i].getFileName() << " has been corrupted" << std::endl;
                }

            } else {
                std::cerr << "AkatekoAudioProcessor::getCurrentPresets" << std::endl;
                std::cerr << "Couldn't open the file" << std::endl;
            }
        }
    }  
    return presets;
}

// Save the current state of the plugins as a binary blob
void AkatekoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml("AKATEKOSETTINGS");

    //Add attributes
    xml.setAttribute("Name", "");
    xml.setAttribute("Category", "default");
    xml.setAttribute("Author", "default");

    for(int i=0; i<getNumParameters(); i++){
        if(AudioProcessorParameterWithID *p = dynamic_cast<AudioProcessorParameterWithID*>(getParameters().getUnchecked(i))){
            xml.setAttribute(p->paramID, p->getValue());
        }
    }

    // Gather Shapes
    for(int i=0; i<lfoOneShapes.size(); i++){
        const String xmlAttribute = String("LFOONE") + String(i);
        xml.setAttribute(xmlAttribute, lfoOneShapes[i]);
    }

    for(int i=0; i<lfoTwoShapes.size(); i++){
        const String xmlAttribute = String("LFOTWO") + String(i);
        xml.setAttribute(xmlAttribute, lfoTwoShape[i]);
    }

    for(int i=0; i<waveShapes.size(); i++){
        const String xmlAttribute = String("WSHAPE") + String(i);
        xml.setAttribute(xmlAttribute, waveShapes[i]);
    }

    // Set UI States
    xml.setAttribute("ENV1", envOneState);
    xml.setAttribute("ENV2", envTwoState);
    xml.setAttribute("SSEQ", stepSeqState);
    xml.setAttribute("XYPAD", xyPadState);
    xml.setAttribute("FXST", fxState);
    xml.setAttribute("MODM", modMatrix.toString());

    copyXmlToBinary(xml, destData);
}

void AkatekoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));

    if(xmlState != nullptr){
        if(xmlState->hasTagName("AKATEKOSETTINGS")){
            for (int i = 0; i < this->getNumParameters(); ++i){
                if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i))){
                    p->setValueNotifyingHost ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
                }
            }
        }

        currentPresetName = xmlState->getStringAttribute("Name");

        // Get Shapes from blob and restore
        for(int i=0; i<lfoOneShapes.size(); i++){
            const String xmlAttribute = String("LFOONE") + String(i);
            lfoOneShapes.set(i, xmlState->getStringAttribute(xmlAttribute));
        }

        for(int i=0; i<lfoTwoShapes.size(); i++){
            const String xmlAttribute = String("LFOTWO") + String(i);
            lfoTwoShapes.set(i, xmlState->getStringAttribute(xmlAttribute));
        }

        for(int i=0; i<waveShapes.size(); i++){
            const String xmlAttribute = String("WSHAPE") + String(i);
            waveShapes.set(i, xmlState->getStringAttribute(xmlAttribute));
        }

        // Get Envelope and restore shapes and recreate the buffer
        envOneState = String(xmlState->getStringAttribute("ENV1"));
        envTwoState = String(xmlState->getStringAttribute("ENV2"));
        stepSeqState = String(xmlState->getStringAttribute("SSEQ"));
        xyPadState = String(xmlState->getStringAttribute("XYPAD"));
        fxState = String(xmlState->getStringAttribute("FXST"));
        modMatrix.restoreFromString(xmlState->getStringAttribute("MODM"));
    } else {
        std::cerr << "AkatekoAudioProcessor::setStateInformation" << std::endl;
        std::cerr << "NullPointer ocurred, does the file exist?" << std::endl;
    }
    // whose contents will have been created by the getStateInformation() call.
}

bool AkatekoAudioProcessor::changeAuthor(const File file, const String author){
    Array<File> tmpPresets;
    bool result = false;

    if(presetManager->scanDirectory(tmpPresets)){
        bool foundFile = false;
        int index = 0;

        while(!foundFile && index <tmpPresets.size()){
            if(file == tmpPresets[index]){
                foundFile = true;
            }
            index++;
        }

        if(foundFile){
            MemoryBlock state;

            if(presetManager->openFile(state, file)){
                ScopedPointer<XmlElement> xmlState (getXmlFromBinary(state.getData(), state.getSize()));

                if(xmlState != nullptr){
                  xmlState->setAttribute("Author", author);
                  copyXmlToBinary(*xmlState, state);

                  presetManager->saveFile(state, file);

                  result = true;
                } else {
                    std::cerr << "AkatekoAudioProcessor::changeAuthor" << std::endl;
                    std::cerr << file.getFileName() << " Couldn't open File" << std::endl;
                    std::cerr << "No changes have been applied" << std::endl;
                }

            } else {
                std::cerr << "AkatekoAudioProcessor::changeAuthor" << std::endl;
                std::cerr << "Couldn't retrieve supplied file" << std::endl;
                std::cerr << "No changes have been applied" << std::endl;
            }
        }
    }
    return result;
}

bool AkatekoAudioProcessor::changeCategory(const File file, const String category){
    Array<File> tmpPresets;
    bool result = false;

    if(presetManager->scanDirectory(tmpPresets)){
        bool foundFile = false;
        int index = 0;

        while(!foundFile && index <tmpPresets.size()){
            if(file == tmpPresets[index]){
                foundFile = true;
            }
            index++;
        }

        if(foundFile){
            MemoryBlock state;

            if(presetManager->openFile(state, file)){
                ScopedPointer<XmlElement> xmlState (getXmlFromBinary(state.getData(), state.getSize()));

                if(xmlState != nullptr){
                  xmlState->setAttribute("Category", category);
                  copyXmlToBinary(*xmlState, state);

                  presetManager->saveFile(state, file);
                  result = true;
                } else {
                    std::cerr << "AkatekoAudioProcessor::changeCategory" << std::endl;
                    std::cerr << file.getFileName() << " Couldn't open File" << std::endl;
                    std::cerr << "No changes have been applied" << std::endl;
                }

            } else {
                std::cerr << "AkatekoAudioProcessor::changeCategory" << std::endl;
                std::cerr << "Couldn't retrieve supplied file" << std::endl;
                std::cerr << "No changes have been applied" << std::endl;
            }
        }
    }
    return result;
}

bool AkatekoAudioProcessor::changeName(const File file, const String name){
    Array<File> tmpPresets;
    bool result = false;

    if(presetManager->scanDirectory(tmpPresets)){
        bool foundFile = false;
        int index = 0;

        while(!foundFile && index <tmpPresets.size()){
            if(file == tmpPresets[index]){
                foundFile = true;
            }
            index++;
        }

        if(foundFile){
            MemoryBlock state;

            if(presetManager->openFile(state, file)){
                ScopedPointer<XmlElement> xmlState (getXmlFromBinary(state.getData(), state.getSize()));

                if(xmlState != nullptr){
                  xmlState->setAttribute("Name", name);
                  copyXmlToBinary(*xmlState, state);

                  presetManager->saveFile(state, file);
                  result = true;
                } else {
                    std::cerr << "AkatekoAudioProcessor::changeName" << std::endl;
                    std::cerr << file.getFileName() << " Couldn't open File" << std::endl;
                    std::cerr << "No changes have been applied" << std::endl;
                }

            } else {
                std::cerr << "AkatekoAudioProcessor::changeCategory" << std::endl;
                std::cerr << "Couldn't retrieve supplied file" << std::endl;
                std::cerr << "No changes have been applied" << std::endl;
            }
        }
    }
    return result;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AkatekoAudioProcessor();
}
//==============================================================================
// Filter Configuration
//==============================================================================
void AkatekoAudioProcessor::updateFiltersEnabled(){
    const bool enable = *filterEnable;

    voiceLeft.enableFilters(enable);
    voiceRight.enableFilters(enable);
}

void AkatekoAudioProcessor::updateFilterConfiguration(){
    const int config = *filterConfig;


    voiceLeft.filterConfiguration(config);
    voiceRight.filterConfiguration(config);

}

void AkatekoAudioProcessor::updateFilterType(int filter){
    if(filter >= 1 && filter <= 2){
        int type;
        int rollOff;

        if(filter == 1){
            type = *filterOneType;
            rollOff = *filterOneRollOff;
        } else if(filter == 2){
            type = *filterTwoType;
            rollOff = *filterTwoRollOff;
        }

        if(rollOff == 0){
            switch(type){
                case 0:
                    type = MoogLadderFilter::LPF2;
                    break;
                case 1:
                    type = MoogLadderFilter::HPF2;
                    break;
                case 2:
                    type = MoogLadderFilter::BPF2;
                    break;
            }
        } else if(rollOff == 1){
            switch(type){
                case 0:
                    type = MoogLadderFilter::LPF4;
                    break;
                case 1:
                    type = MoogLadderFilter::HPF4;
                    break;
                case 2:
                    type = MoogLadderFilter::BPF4;
                    break;
            }
        }
        if(filter == 1){
                voiceLeft.setFilterOneType(type);
                voiceRight.setFilterOneType(type);
        } else if(filter == 2){
                voiceLeft.setFilterTwoType(type);
                voiceRight.setFilterTwoType(type);

        }
   }
}

void AkatekoAudioProcessor::updateFilterOneEnabled(){
    const bool enable = *filterOneEnable;

    voiceLeft.enableFilterOne(enable);
    voiceRight.enableFilterOne(enable);
}

void AkatekoAudioProcessor::updateFilterTwoEnabled(){
    const bool enable = *filterTwoEnable;

    voiceLeft.enableFilterTwo(enable);
    voiceRight.enableFilterTwo(enable);
}
//==============================================================================
//WaveShaper
//==============================================================================
void AkatekoAudioProcessor::enableWaveShaper(){
    const bool enable = *waveShaperEnable;

    voiceLeft.enableWaveShaper(enable);
    voiceRight.enableWaveShaper(enable);
}

void AkatekoAudioProcessor::setWaveShaperBuffer(msmBuffer &buffer){

    voiceLeft.setWaveShaperBuffer(buffer);
    voiceRight.setWaveShaperBuffer(buffer);
}
//=============================================================================
// Envelope One
//=============================================================================
void AkatekoAudioProcessor::triggerEnvelopeOne(){
    envOne.reset();
    envOne.triggerEnvelope();
}

void AkatekoAudioProcessor::releaseEnvelopeOne(){
    envOne.releaseEnvelope();
}

void AkatekoAudioProcessor::setEnvelopeOneBuffer(msmBuffer &buffer){
    envOne.setBuffer(buffer);
    envOne.swapBuffer();
}

void AkatekoAudioProcessor::setEnvelopeOneLoopEnabled(bool enabled){
    envOne.setLoopEnabled(enabled);
}

void AkatekoAudioProcessor::setEnvelopeOneLoopPoints(double startPos, double endPos){
    envOne.setLoopPoints(startPos, endPos);
}

void AkatekoAudioProcessor::updateEnvelopeOneLoopDirection(){
    const int direction = *envOneLoopDirection;
    envOne.setLoopDirection(direction);
}

void AkatekoAudioProcessor::updateEnvelopeOneLoopAmount(){
    const int amount = *envOneLoopAmount;
    envOne.setLoopAmount(amount);
}

void AkatekoAudioProcessor::setEnvelopeOneSustainEnabled(bool enabled){
    envOne.setSustainEnabled(enabled);
}

void AkatekoAudioProcessor::setEnvelopeOneSustainPoints(double startPos, double endPos){
    envOne.setSustainPoints(startPos, endPos);
}

void AkatekoAudioProcessor::updateEnvelopeOneSustainDirection(){
    const int direction = *envOneSusDirection;
    envOne.setSustainDirection(direction);
}

void AkatekoAudioProcessor::setEnvelopeOneReleasePoint(double startPos, double endPos){
    envOne.setReleasePoints(startPos, endPos);
}

void AkatekoAudioProcessor::updateEnvelopeOneSyncSource(){

}
//=============================================================================
// Envelope Two
//=============================================================================
void AkatekoAudioProcessor::triggerEnvelopeTwo(){
    envTwo.reset();
    envTwo.triggerEnvelope();
}

void AkatekoAudioProcessor::releaseEnvelopeTwo(){
    envTwo.releaseEnvelope();
}

void AkatekoAudioProcessor::setEnvelopeTwoBuffer(msmBuffer &buffer){
    envTwo.setBuffer(buffer);
    envTwo.swapBuffer();
}

void AkatekoAudioProcessor::setEnvelopeOneDurationBounds(double min, double max){
    envTwo.setDurationBounds(min, max);
}

void AkatekoAudioProcessor::setEnvelopeTwoDurationBounds(double min, double max){
    envOne.setDurationBounds(min, max);
}

void AkatekoAudioProcessor::setEnvelopeTwoLoopEnabled(bool enabled){
    envTwo.setLoopEnabled(enabled);
}

void AkatekoAudioProcessor::setEnvelopeTwoLoopPoints(double startPos, double endPos){
    envTwo.setLoopPoints(startPos, endPos);
}

void AkatekoAudioProcessor::updateEnvelopeTwoLoopDirection(){
    const int direction = *envTwoLoopDirection;
    envTwo.setLoopDirection(direction);
}

void AkatekoAudioProcessor::updateEnvelopeTwoLoopAmount(){
    const int amount = *envTwoLoopAmount;
    envTwo.setLoopAmount(amount);
}

void AkatekoAudioProcessor::setEnvelopeTwoSustainEnabled(bool enabled){
    envTwo.setSustainEnabled(enabled);
}

void AkatekoAudioProcessor::setEnvelopeTwoSustainPoints(double startPos, double endPos){
    envTwo.setSustainPoints(startPos, endPos);
}

void AkatekoAudioProcessor::updateEnvelopeTwoSustainDirection(){
    const int direction = *envTwoSusDirection;
    envTwo.setSustainDirection(direction);
}

void AkatekoAudioProcessor::setEnvelopeTwoReleasePoint(double startPos, double endPos){
    envTwo.setReleasePoints(startPos, endPos);
}

void AkatekoAudioProcessor::updateEnvelopeTwoSyncSource(){

    /* Calculate envelope duration based on the duration */
}
//=============================================================================
// Low Frequency Oscillator One
//=============================================================================
void AkatekoAudioProcessor::setLowFreqOscOneShot(bool enabled){

}
void AkatekoAudioProcessor::setLowFreqOscOneSync(bool enabled){

}

void AkatekoAudioProcessor::setLowFreqOscOneBounds(double min, double max){
    lfoOne.setFrequencyBounds(min, max);
}

void AkatekoAudioProcessor::setLowFreqOscOneBuffer(msmBuffer &buffer){
    lfoOne.setBuffer(buffer);
    lfoOne.swapBuffer();
}

void AkatekoAudioProcessor::resetLowFreqOscOne(){
    lfoOne.reset();
}
//=============================================================================
// LFO Two
//=============================================================================
void AkatekoAudioProcessor::setLowFreqOscTwoShot(bool enabled){

}

void AkatekoAudioProcessor::setLowFreqOscTwoSync(bool enabled){

}

void AkatekoAudioProcessor::setLowFreqOscTwoBounds(double min, double max){
    lfoTwo.setFrequencyBounds(min, max);
}

void AkatekoAudioProcessor::setLowFreqOscTwoBuffer(msmBuffer &buffer){
    lfoTwo.setBuffer(buffer);
    lfoTwo.swapBuffer();
}

void AkatekoAudioProcessor::resetLowFreqOscTwo(){
    lfoTwo.reset();
}
//=============================================================================
// Step Sequencer
//=============================================================================
void AkatekoAudioProcessor::updateStepMidPoint(){
    const double mp = *stepSeqMidPoint;

    //std::cout << "midPoint : " << mp << std::endl;

    if(mp >= 0 && mp <= 1.0){
        stepSeq.setMidPoint(mp);
    } else {
        stepSeq.setMidPoint(0.5);
    }
    stepSeq.calculateCurves();
}

void AkatekoAudioProcessor::updateChopCurve(){
    const bool enableChopper = *stepSeqChopper;
    stepSeq.setCurveChopper(enableChopper);    
}

void AkatekoAudioProcessor::updateDuration(){
    const double duration = *stepSeqDuration;

    stepSeq.setDuration(duration);
}

void AkatekoAudioProcessor::setStepDurationBounds(double min, double max){
    stepSeq.setDurationBounds(min, max);
}

void AkatekoAudioProcessor::setStepValues(vector<double> values){
    stepSeq.setValues(values);
    stepSeq.recalculateCurves();
}

void AkatekoAudioProcessor::setButtonStates(vector<int> states){
    stepSeq.setStates(states);
    stepSeq.calculateCurves();
}

void AkatekoAudioProcessor::setStepSeq(vector<double> values, vector<int> states){
    int tmpIndex = *stepSeqAmount;
    int amount = 16;

    if(tmpIndex >= 0 && tmpIndex <= 4){
        switch(tmpIndex){
            case 0:
                amount = 8;
                break;
            case 1:
                amount = 12;
                break;
            case 2:
                amount = 16;
                break;
            case 3:
                amount = 24;
                break;
            case 4:
                amount = 32;
                break;
        }
    }
    stepSeq.setSteps(values, states, amount);
}
//=============================================================================
// Fx Container
//=============================================================================

void AkatekoAudioProcessor::setEffectOne(int effectProcessor){

    std::cout << effectProcessor << std::endl;

    switch(effectProcessor){
        if(effectProcessor >= 1 && effectProcessor <=4){
            effectTwo->exit();
        }

        case 1:
            effectOne = new StereoProcessor(); //None
            break;
        case 2:
            effectOne = new StereoDelay(getParameters(),
                                        stereoDelIndices,
                                        modMatrix.getReadRegister(DESTSRC::SDCROSS),
                                        modMatrix.getReadRegister(DESTSRC::SDMIX),
                                        modMatrix.getReadRegister(DESTSRC::SDLDEL),
                                        modMatrix.getReadRegister(DESTSRC::SDLFB),
                                        modMatrix.getReadRegister(DESTSRC::SDRDEL),
                                        modMatrix.getReadRegister(DESTSRC::SDRFB));
            break;
        case 3:
            effectOne = new PingPongDelay(getParameters(),
                                          pingPongIndices,
                                          modMatrix.getReadRegister(DESTSRC::PPLDEL),
                                          modMatrix.getReadRegister(DESTSRC::PPLFB),
                                          modMatrix.getReadRegister(DESTSRC::PPLMIX),
                                          modMatrix.getReadRegister(DESTSRC::PPRDEL),
                                          modMatrix.getReadRegister(DESTSRC::PPRFB),
                                          modMatrix.getReadRegister(DESTSRC::PPRMIX));
            break;
        case 4:
            effectOne = new LeftCenterRightDelay(getParameters(),
                                                 lcrDelayIndices,
                                                 modMatrix.getReadRegister(DESTSRC::LCRLDEL),
                                                 modMatrix.getReadRegister(DESTSRC::LCRLFB),
                                                 modMatrix.getReadRegister(DESTSRC::LCRLMIX),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCDEL),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCFB),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCHPF),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCLPF),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCAMP),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCPAN),
                                                 modMatrix.getReadRegister(DESTSRC::LCRRDEL),
                                                 modMatrix.getReadRegister(DESTSRC::LCRRFB),
                                                 modMatrix.getReadRegister(DESTSRC::LCRRMIX));
            break;
        case 5:
            effectOne = new StereoFlanger(getParameters(),
                                          sFlangerIndices,
                                          modMatrix.getReadRegister(DESTSRC::SFLRT),
                                          modMatrix.getReadRegister(DESTSRC::SFLPH),
                                          modMatrix.getReadRegister(DESTSRC::SFLDH),
                                          modMatrix.getReadRegister(DESTSRC::SFLFB));
            break;
        case 6:
            effectOne = new DimensionChorus(getParameters(),
                                            dChorusIndices,
                                            modMatrix.getReadRegister(DESTSRC::DCHORT),
                                            modMatrix.getReadRegister(DESTSRC::DCHOSP),
                                            modMatrix.getReadRegister(DESTSRC::DCHODP),
                                            modMatrix.getReadRegister(DESTSRC::DCHOHPF),
                                            modMatrix.getReadRegister(DESTSRC::DCHOCA),
                                            modMatrix.getReadRegister(DESTSRC::DCHOCP),
                                            modMatrix.getReadRegister(DESTSRC::DCHOMIX));
            break;
        case 7:
            effectOne = new HoldDelay(getParameters(),
                                      hDelayIndices,
                                      modMatrix.getReadRegister(DESTSRC::HDELSPD),
                                      modMatrix.getReadRegister(DESTSRC::HDELLEN),
                                      modMatrix.getReadRegister(DESTSRC::HDELFDE),
                                      modMatrix.getReadRegister(DESTSRC::HDELPAN),
                                      modMatrix.getReadRegister(DESTSRC::HDELMIX));
            break;
        case 8:
            effectOne = new StereoDecimator(getParameters(),
                                            decimatorIndices,
                                            modMatrix.getReadRegister(DESTSRC::DECBIT),
                                            modMatrix.getReadRegister(DESTSRC::DECRTE),
                                            modMatrix.getReadRegister(DESTSRC::DECMIX));
            break;
        case 9:
            effectOne = new PlateReverb(getParameters(),
                                        pReverbIndices,
                                        modMatrix.getReadRegister(DESTSRC::PREVDAM),
                                        modMatrix.getReadRegister(DESTSRC::PREVBW),
                                        modMatrix.getReadRegister(DESTSRC::PREVDEC),
                                        modMatrix.getReadRegister(DESTSRC::PREVMIX));
            break;
        default:
           effectOne = new StereoProcessor();
    }
    effectOne->setSampleRate(getSampleRate());
}

void AkatekoAudioProcessor::setEffectTwo(int effectProcessor){
    switch(effectProcessor){
        if(effectProcessor >= 1 && effectProcessor <=4){
            effectTwo->exit();
        }

        case 1:
            effectTwo = new StereoProcessor();
            break;
        case 2:
            effectTwo = new StereoDelay(getParameters(),
                                       stereoDelIndices,
                                       modMatrix.getReadRegister(DESTSRC::SDCROSS),
                                       modMatrix.getReadRegister(DESTSRC::SDMIX),
                                       modMatrix.getReadRegister(DESTSRC::SDLDEL),
                                       modMatrix.getReadRegister(DESTSRC::SDLFB),
                                       modMatrix.getReadRegister(DESTSRC::SDRDEL),
                                       modMatrix.getReadRegister(DESTSRC::SDRFB));
            break;
        case 3:
            effectTwo = new PingPongDelay(getParameters(),
                                          pingPongIndices,
                                          modMatrix.getReadRegister(DESTSRC::PPLDEL),
                                          modMatrix.getReadRegister(DESTSRC::PPLFB),
                                          modMatrix.getReadRegister(DESTSRC::PPLMIX),
                                          modMatrix.getReadRegister(DESTSRC::PPRDEL),
                                          modMatrix.getReadRegister(DESTSRC::PPRFB),
                                          modMatrix.getReadRegister(DESTSRC::PPRMIX));
            break;
        case 4:
            effectTwo = new LeftCenterRightDelay(getParameters(),
                                                 lcrDelayIndices,
                                                 modMatrix.getReadRegister(DESTSRC::LCRLDEL),
                                                 modMatrix.getReadRegister(DESTSRC::LCRLFB),
                                                 modMatrix.getReadRegister(DESTSRC::LCRLMIX),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCDEL),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCFB),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCHPF),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCLPF),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCAMP),
                                                 modMatrix.getReadRegister(DESTSRC::LCRCPAN),
                                                 modMatrix.getReadRegister(DESTSRC::LCRRDEL),
                                                 modMatrix.getReadRegister(DESTSRC::LCRRFB),
                                                 modMatrix.getReadRegister(DESTSRC::LCRRMIX));
            break;
        case 5:
            effectTwo = new StereoFlanger(getParameters(),
                                          sFlangerIndices,
                                          modMatrix.getReadRegister(DESTSRC::SFLRT),
                                          modMatrix.getReadRegister(DESTSRC::SFLPH),
                                          modMatrix.getReadRegister(DESTSRC::SFLDH),
                                          modMatrix.getReadRegister(DESTSRC::SFLFB));

            break;
        case 6:
            effectTwo = new DimensionChorus(getParameters(),
                                            dChorusIndices,
                                            modMatrix.getReadRegister(DESTSRC::DCHORT),
                                            modMatrix.getReadRegister(DESTSRC::DCHOSP),
                                            modMatrix.getReadRegister(DESTSRC::DCHODP),
                                            modMatrix.getReadRegister(DESTSRC::DCHOHPF),
                                            modMatrix.getReadRegister(DESTSRC::DCHOCA),
                                            modMatrix.getReadRegister(DESTSRC::DCHOCP),
                                            modMatrix.getReadRegister(DESTSRC::DCHOMIX));
            break;
        case 7:
            effectTwo = new HoldDelay(getParameters(),
                                      hDelayIndices,
                                      modMatrix.getReadRegister(DESTSRC::HDELSPD),
                                      modMatrix.getReadRegister(DESTSRC::HDELLEN),
                                      modMatrix.getReadRegister(DESTSRC::HDELFDE),
                                      modMatrix.getReadRegister(DESTSRC::HDELPAN),
                                      modMatrix.getReadRegister(DESTSRC::HDELMIX));
            break;
        case 8:
            effectTwo = new StereoDecimator(getParameters(),
                                            decimatorIndices,
                                            modMatrix.getReadRegister(DESTSRC::DECBIT),
                                            modMatrix.getReadRegister(DESTSRC::DECRTE),
                                            modMatrix.getReadRegister(DESTSRC::DECMIX));
            break;
        case 9:
            effectTwo = new PlateReverb(getParameters(),
                                        pReverbIndices,
                                        modMatrix.getReadRegister(DESTSRC::PREVDAM),
                                        modMatrix.getReadRegister(DESTSRC::PREVBW),
                                        modMatrix.getReadRegister(DESTSRC::PREVDEC),
                                        modMatrix.getReadRegister(DESTSRC::PREVMIX));
            break;

        default:
           effectTwo = new StereoProcessor();
    }
    effectTwo->setSampleRate(getSampleRate());
}


AkatekoMatrix &AkatekoAudioProcessor::getModMatrix(){
    return modMatrix;
}

bool AkatekoAudioProcessor::shapesInitialised(){
    return shapesAreInitialised;
}

void AkatekoAudioProcessor::setShapesInitialised(bool value){
    shapesAreInitialised = value;
}

/* Save copy of the shapes on startup, after startup
 * Update by the appriopiate updateShape update.
 */
void AkatekoAudioProcessor::setShapes(StringArray shapes,
                                      UserInterfaceId id)
{
    switch(id){
        case UserInterfaceId::LFO1Id:
            lfoOneShapes = StringArray(shapes);
            break;
        case UserInterfaceId::LFO2Id:
            lfoTwoShapes = StringArray(shapes);
            break;
        case UserInterfaceId::WaveShapeId:
            waveShapes = StringArray(shapes);
            break;
    }
}

StringArray AkatekoAudioProcessor::getShapes(UserInterfaceId id){
    StringArray tmpArray;

    switch(id){
        case UserInterfaceId::LFO1Id:
            tmpArray = lfoOneShapes;
            break;
        case UserInterfaceId::LFO2Id:
            tmpArray = lfoTwoShapes;
            break;
        case UserInterfaceId::WaveShapeId:
            tmpArray = waveShapes;
            break;
    }

    return tmpArray;
}

void AkatekoAudioProcessor::updateShape(String shape,
                                        UserInterfaceId id)
{
    int shapeIndex = 0xFF;

    switch(id){
        case UserInterfaceId::LFO1Id:
            shapeIndex = *lfoOneShape;

            if(shapeIndex < lfoOneShapes.size()){
                lfoOneShapes.set(shapeIndex, shape);
            } else {
                std::cerr << "Couldn't update LFO 1 Shape" << std::endl;
            }

            break;
        case UserInterfaceId::LFO2Id:
            shapeIndex = *lfoTwoShape;

            if(shapeIndex < lfoTwoShapes.size()){
                lfoTwoShapes.set(shapeIndex, shape);
            } else {
                std::cerr << "Couldn't update LFO 2 Shape" << std::endl;
            }

            break;
        case UserInterfaceId::WaveShapeId:
            shapeIndex = *waveShaperShape;

            if(shapeIndex < waveShapes.size()){
                waveShapes.set(shapeIndex, shape);
            } else {
                std::cerr << "Couldn't update WaveShapes" << std::endl;
            }
            break;
    }
}

vector<int> AkatekoAudioProcessor::getParameterIndices(UserInterfaceId id){
    vector<int>tmpIndices;

    switch(id){
        case GlobalId:
            tmpIndices = globalIndices;
            break;
        case FilterId:
            tmpIndices = filterIndices;
            break;
        case WaveShapeId:
            tmpIndices = wsIndices;
            break;
        case LFO1Id:
            tmpIndices = lfoOneIndices;
            break;
        case LFO2Id:
            tmpIndices = lfoTwoIndices;
            break;
        case Envelope1Id:
            tmpIndices = envOneIndices;
            break;
        case Envelope2Id:
            tmpIndices = envTwoIndices;
            break;
        case StepSequencerId:
            tmpIndices = sseqIndices;
            break;
        case FxContainerId:
            tmpIndices = fxIndices;
            break;
        case StereoDelayId:
            tmpIndices = stereoDelIndices;
            break;
        case PingPongDelayId:
            tmpIndices = pingPongIndices;
            break;
        case LCRDelayId:
            tmpIndices = lcrDelayIndices;
            break;
        case StereoflangerId:
            tmpIndices = sFlangerIndices;
            break;
        case DChorusId:
            tmpIndices = dChorusIndices;
            break;
        case hDelayId:
            tmpIndices = hDelayIndices;
            break;
        case decimatorId:
            tmpIndices = decimatorIndices;
            break;
        case pReverbId:
            tmpIndices = pReverbIndices;
            break;
    }

    return tmpIndices;
}

StringArray AkatekoAudioProcessor::getParameterIds(UserInterfaceId id){
    StringArray tmpParamId;

    switch(id){
        case GlobalId:
            tmpParamId = globalParamIds;
            break;
        case FilterId:
            tmpParamId = filterParamIds;
            break;
        case WaveShapeId:
            tmpParamId = waveShapeParamIds;
            break;
        case LFO1Id:
            tmpParamId = lfoOneParamIds;
            break;
        case LFO2Id:
            tmpParamId = lfoTwoParamIds;
            break;
        case Envelope1Id:
            tmpParamId = envOneParamIds;
            break;
        case Envelope2Id:
            tmpParamId = envTwoParamIds;
            break;
        case StepSequencerId:
            tmpParamId = stepSeqParamIds;
            break;
        case FxContainerId:
            tmpParamId = fxParamIds;
            break;
        case StereoDelayId:
            tmpParamId = stereoDelParamIds;
            break;
        case PingPongDelayId:
            tmpParamId = pingPongParamIds;
            break;
        case LCRDelayId:
            tmpParamId = lcrDelayParamIds;
            break;
        case StereoflangerId:
            tmpParamId = sFlangerParamIds;
            break;
        case DChorusId:
            tmpParamId = dChorusParamIds;
            break;
        case hDelayId:
            tmpParamId = hDelayParamIds;
            break;
        case decimatorId:
            tmpParamId = decimatorParamIds;
            break;
        case pReverbId:
            tmpParamId = pReverbParamIds;
            break;
    }

    return tmpParamId;
}

void AkatekoAudioProcessor::setUIState(const String state, UserInterfaceId id){
    switch(id){
        case FilterId:
            filterState = String(state);
            break;
        case WaveShapeId:        
            wsString = String(state);
            break;
        case LFO1Id:
            lfoOneState = String(state);
            break;
        case LFO2Id:
            lfoTwoState = String(state);
            break;
        case Envelope1Id:
            envOneState = String(state);
            break;
        case Envelope2Id:
            envTwoState = String(state);
            break;
        case StepSequencerId:
            stepSeqState = String(state);
            break;
        case xyPadId:
            xyPadState = String(state);
            break;
        case FxContainerId:
            fxState = String(state);
            break;
    }
}

String AkatekoAudioProcessor::getUIState(UserInterfaceId id){
    String result;

    switch(id){
        case FilterId:
            result = filterState;
            break;
        case WaveShapeId:
            result = wsString;
            break;
        case LFO1Id:
            result = lfoOneState;
            break;
        case LFO2Id:
            result = lfoTwoState;
            break;
        case Envelope1Id:
            result = envOneState;
            break;
        case Envelope2Id:
            result = envTwoState;
            break;
        case StepSequencerId:
            result = stepSeqState;
            break;
        case xyPadId:
            result = xyPadState;
            break;
        case FxContainerId:
            result = fxState;
            break;
    }

    return result;
}

void AkatekoAudioProcessor::initialiseParameters(){
    initialiseGlobalParameters();
    initialiseFilterParameters();
    initialiseWaveShapeParameters();
    initialiseLFO1Parameters();
    initialiseLFO2Parameters();
    initialiseENV1Parameters();
    initialiseENV2Parameters();
    initialiseStepSeqParameters();
    initialiseEffectParameters();
    initialiseStereoDelParameters();
    initialisePingPongDelay();
    initialiseLCRDelay();
    initialiseStereoFlanger();
    initialiseDimensionChorus();
    initialiseHoldDelay();
    initialiseDecimator();
    initialisePlateReverb();
}

void AkatekoAudioProcessor::initialiseGlobalParameters(){
    addParameter(inputVolume = new AudioParameterFloat("GLIVL","Global In : ", 0.f, 1.f, 0.5f));
    addParameter(globalVolume = new AudioParameterFloat("GLMVL", "Global Out : ", 0.f, 1.f, 0.5f));

    globalParamIds.add(inputVolume->name); //Parameter Index 0
    globalParamIds.add(globalVolume->name); //Parameter Index 1

    globalIndices.push_back(0);
    globalIndices.push_back(1);
}

void AkatekoAudioProcessor::initialiseFilterParameters(){
    addParameter(filterEnable = new AudioParameterBool("FLEN","Filters : ", true));
    addParameter(filterConfig = new AudioParameterInt("FLCF", "Filter config : ",0, 1, 0));

    // Filter Ones
    addParameter(filterOneEnable = new AudioParameterBool("F1EN", "Filter 1 : ", true));
    addParameter(filterOneType = new AudioParameterFloat("F1TP", "Filter 1 Type : ",  0, 1, 0.));
    addParameter(filterOneRollOff = new AudioParameterInt("F1RF", "Filter 1 RollOff : ",0, 1, 0));

    addParameter(filterOneFrequency = new AudioParameterFloat("F1FQ","Filter 1 frequency : ", NormalisableRange<float>(20.f, 20000.f), 20000.f));
    addParameter(filterOneResonance = new AudioParameterFloat("F1RE", "Filter 1 Resonance : ", 0.f,  1.f, 0.f));
    addParameter(filterOnePassBand = new AudioParameterFloat("F1PBG", "Filter 1 PassBand : ", 0.f, 1.f, 0.5f));
    addParameter(filterOneDrive = new AudioParameterFloat("F1DRI", "Filter 1 Drive : ", 0.f, 1.f, 0.f));
    addParameter(filterOneVolume = new AudioParameterFloat("F1VOL", "Filter 1 Volume : ", 0.f, 1.f, 1.f));
    //Filter Two
    addParameter(filterTwoEnable = new AudioParameterBool("F2EN", "Filter 2 Enable : ", true));
    addParameter(filterTwoType = new AudioParameterFloat("F2TP", "Filter 2 Type : ", 0.f,  1.f, 1.f));
    addParameter(filterTwoRollOff = new AudioParameterInt("F2RF","Filter 2 RollOff : ", 0, 1, 0));

    addParameter(filterTwoFrequency = new AudioParameterFloat("F2FQ", "Filter 2 Frequency : ", NormalisableRange<float>(20.f, 20000.f), 20.f));
    addParameter(filterTwoResonance = new AudioParameterFloat("F2RE", "Filter 2 Resonance : ", 0.f,1.f, 0.f));
    addParameter(filterTwoPassBand = new AudioParameterFloat("F2PBG", "Filter 2 PassBand : ", 0.f, 1.f, 0.f));
    addParameter(filterTwoDrive = new AudioParameterFloat("F2DRI", "Filter 2 Drive : ", 0.f, 1.f, 0.f));
    addParameter(filterTwoVolume = new AudioParameterFloat("F2VOL", "Filter 2 Volume : ", 0.f, 1.f, 0.f));


    filterParamIds.add(filterEnable->name);         //Parameter Index  2
    filterParamIds.add(filterConfig->name);         //Parameter Index  3
    // Filter One
    filterParamIds.add(filterOneEnable->name);      //Parameter Index  4
    filterParamIds.add(filterOneType->name);        //Parameter Index  5
    filterParamIds.add(filterOneRollOff->name);     //Parameter Index  6

    filterParamIds.add(filterOneFrequency->name);   //Parameter Index  7
    filterParamIds.add(filterOneResonance->name);   //Parameter Index  8
    filterParamIds.add(filterOnePassBand->name);    //Parameter Index  9
    filterParamIds.add(filterOneDrive->name);       //Parameter Index 10
    filterParamIds.add(filterOneVolume->name);      //Parameter Index 11
    //Filter Two
    filterParamIds.add(filterTwoEnable->name);      //Parameter Index 12
    filterParamIds.add(filterTwoType->name);        //Parameter Index 13
    filterParamIds.add(filterTwoRollOff->name);     //Parameter Index 14

    filterParamIds.add(filterTwoFrequency->name);   //Parameter Index 15
    filterParamIds.add(filterTwoResonance->name);   //Parameter Index 16
    filterParamIds.add(filterTwoPassBand->name);    //Parameter Index 17
    filterParamIds.add(filterTwoDrive->name);       //Parameter Index 18
    filterParamIds.add(filterTwoVolume->name);      //Parameter Index 19

    // 18 Params
    for(int i=2; i<20; i++){
        filterIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseWaveShapeParameters(){
    addParameter(waveShaperEnable = new AudioParameterBool("WSEN", "WaveShaper : ", true, "enable"));
    addParameter(waveShaperDrive = new AudioParameterFloat("WSDRI", "WaveShaper Drive : ", NormalisableRange<float>(0.5, 1.f), 0.5, "drive"));
    addParameter(waveShaperMix = new AudioParameterFloat("WSMIX", "WaveShaper Mix : ", NormalisableRange<float>(0.f, 1.f), 0.f, "mix"));
    addParameter(waveShaperShape = new AudioParameterFloat("WSHAPE", "WaveShaper Shape : ", NormalisableRange<float>(0.f, 1.f), 0.f, "shape"));

    waveShapeParamIds.add(waveShaperEnable->name);  //Parameter Index 20
    waveShapeParamIds.add(waveShaperDrive->name);   //Parameter Index 21
    waveShapeParamIds.add(waveShaperMix->name);     //Parameter Index 22
    waveShapeParamIds.add(waveShaperShape->name);   //Parameter Index 23

    // 4 Params
    for(int i=20; i<24; i++){
        wsIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseLFO1Parameters(){
    addParameter(lfoOneEnable = new AudioParameterBool("LF1EN", "LFO 1 : ", true, "enable"));
    addParameter(lfoOneShot = new AudioParameterBool("LF1SH", "LFO 1 One shot : ", true, "one-shot"));
    addParameter(lfoOneSync = new AudioParameterBool("LF1SY", "LFO 1 Sync : ", true, "sync"));
    addParameter(lfoOneFrequency = new AudioParameterFloat("LF1F", "LFO 1 Frequency : ", 0, 1.0, 0.f));
    addParameter(lfoOnePhase = new AudioParameterFloat("LF1P", "LFO 1 Phase : ",NormalisableRange<float>(0.f, 1.f), 0.f, "phase"));
    addParameter(lfoOnePWM = new AudioParameterFloat("LF1PW", "LFO 1 Pulse Width : ",NormalisableRange<float>(-0.95f, 0.95f), 0.f, "pwm"));
    addParameter(lfoOneShape = new AudioParameterFloat("L1SHAPE", "LFO 1 Shape : ", NormalisableRange<float>(0.f, 1.f), 0.f, "shape"));

    lfoOneParamIds.add(lfoOneEnable->name);     //Parameter Index 24
    lfoOneParamIds.add(lfoOneShot->name);       //Parameter Index 25
    lfoOneParamIds.add(lfoOneSync->name);       //Parameter Index 26
    lfoOneParamIds.add(lfoOneFrequency->name);  //Parameter Index 27
    lfoOneParamIds.add(lfoOnePhase->name);      //Parameter Index 28
    lfoOneParamIds.add(lfoOnePWM->name);        //Parameter Index 29
    lfoOneParamIds.add(lfoOneShape->name);      //Parameter Index 30

    // 7 Params
    for(int i=24; i<31; i++){
        lfoOneIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseLFO2Parameters(){
    addParameter(lfoTwoEnable = new AudioParameterBool("LF2EN", "LFO 2 : ", true, "enable"));
    addParameter(lfoTwoShot = new AudioParameterBool("LF2SH", "LFO 2 One Shot L: ", true, "one-shot"));
    addParameter(lfoTwoSync = new AudioParameterBool("LF2SY", "LFO 2 Sync : ", true, "sync"));
    addParameter(lfoTwoFrequency = new AudioParameterFloat("LF2F", "LFO 2 Frequency : ", 0, 1.0, 0.f));
    addParameter(lfoTwoPhase = new AudioParameterFloat("LF2P", "LFO 2 Phase : ", NormalisableRange<float>(0.f, 1.f), 0.f, "phase"));
    addParameter(lfoTwoPWM = new AudioParameterFloat("LF2PW", "LFO 2 Pulse Width : ", NormalisableRange<float>(-0.95f, 0.95f), 0.f, "pwm"));
    addParameter(lfoTwoShape = new AudioParameterFloat("L2SHAPE", "LFO 2 Shape : ", NormalisableRange<float>(0.f, 1.f), 0.f , "shape"));

    lfoTwoParamIds.add(lfoTwoEnable->name);     //Parameter Index 31
    lfoTwoParamIds.add(lfoTwoShot->name);       //Parameter Index 32
    lfoTwoParamIds.add(lfoTwoSync->name);       //Parameter Index 33
    lfoTwoParamIds.add(lfoTwoFrequency->name);  //Parameter Index 34
    lfoTwoParamIds.add(lfoTwoPhase->name);      //Parameter Index 35
    lfoTwoParamIds.add(lfoTwoPWM->name);        //Parameter Index 36
    lfoTwoParamIds.add(lfoTwoShape->name);      //Parameter Index 37

    // 7 Params
    for(int i=31; i<38; i++){
        lfoTwoIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseENV1Parameters(){
    addParameter(envOneEnable = new AudioParameterBool("EV1ENA", "Envelope 1 : ", false, "enable"));
    addParameter(envOneSync = new AudioParameterBool("EV1SYNC", "Envelope 1 Sync : ", false, "sync"));
    addParameter(envOneDuration = new AudioParameterFloat("EV1DUR", "Envelope 1 Dur : ", 0.f, 1.f, 0));
    addParameter(envOneLoopAmount = new AudioParameterFloat("EV1LAM", "Envelope 1 Loop nr : ", NormalisableRange<float>(1.f, 32.f), 0.f, "loop-nr"));
    addParameter(envOneLoopDirection = new AudioParameterFloat("EV1LDR", "Envelope 1 Loop dir : ", NormalisableRange<float>(0, 1), 0, "loop-dir"));
    addParameter(envOneSusDirection = new AudioParameterFloat("EV1SDR", "Envelope 1 Sus dir : ", NormalisableRange<float>(0, 1), 0, "sus-dir"));
    addParameter(envOneTriggerSource = new AudioParameterFloat("EV1TRS", "Envelope 1 Trigger : ", NormalisableRange<float>(0, 1), 0, "tr-src"));

    envOneParamIds.add(envOneEnable->name);         //Parameter Index 38
    envOneParamIds.add(envOneSync->name);           //Parameter Index 39
    envOneParamIds.add(envOneDuration->name);       //Parameter Index 40
    envOneParamIds.add(envOneLoopAmount->name);     //Parameter Index 41
    envOneParamIds.add(envOneLoopDirection->name);  //Parameter Index 42
    envOneParamIds.add(envOneSusDirection->name);   //Parameter Index 43
    envOneParamIds.add(envOneTriggerSource->name);  //Parameter Index 44

    // 7 Params
    for(int i=38; i<45; i++){
        envOneIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseENV2Parameters(){
    addParameter(envTwoEnable = new AudioParameterBool("EV2ENA", "Envelope 2 : ", false, "enable"));
    addParameter(envTwoSync = new AudioParameterBool("EV2SYNC", "Envelope 2 Sync : ", false, "sync"));
    addParameter(envTwoDuration = new AudioParameterFloat("EV2DUR", "Envelope 2 Dur : ", 0.f, 1.f, 0.f));
    addParameter(envTwoLoopAmount = new AudioParameterFloat("EV2LAM", "Envelope 2 Loop nr : ", NormalisableRange<float>(1.f, 32.f), 0.f, "loop-nr"));
    addParameter(envTwoLoopDirection = new AudioParameterFloat("EV2LDR", "Envelope 2 loop dir : ",NormalisableRange<float>(0, 1), 0, "loop-dir"));
    addParameter(envTwoSusDirection = new AudioParameterFloat("EV2SDR", "Envelope 2 Sus dir : ", NormalisableRange<float>(0, 1), 0, "sus-dir"));
    addParameter(envTwoTriggerSource = new AudioParameterFloat("EV2TRS", "Envelope 2 Trigger : ", NormalisableRange<float>(0, 1) , 0, "tr-src"));

    envTwoParamIds.add(envTwoEnable->name);         //Parameter Index 45
    envTwoParamIds.add(envTwoSync->name);           //Parameter Index 46
    envTwoParamIds.add(envTwoDuration->name);       //Parameter Index 47
    envTwoParamIds.add(envTwoLoopAmount->name);     //Parameter Index 48
    envTwoParamIds.add(envTwoLoopDirection->name);  //Parameter Index 49
    envTwoParamIds.add(envTwoSusDirection->name);   //Parameter Index 50
    envTwoParamIds.add(envTwoTriggerSource->name);  //Parameter Index 51

    //7 Params
    for(int i=45; i<52; i++){
        envTwoIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseStepSeqParameters(){
    //Enable
    addParameter(stepSeqEnable = new AudioParameterBool("SSQENA", "Step Sequencer : ", false, "enable"));
    addParameter(stepSeqSync = new AudioParameterBool("SSQSYN", "Step Sequencer Sync : ", false, "sync"));
    addParameter(stepSeqChopper = new AudioParameterBool("SSQCHP", "Step Sequencer Chop Curve : ", false, "chopper"));
    addParameter(stepSeqDuration = new AudioParameterFloat("SSQDUR", "Step Sequencer Duration : ", 0.f, 1.0f, 0.f));
    addParameter(stepSeqLength = new AudioParameterFloat("SSQLEN", "Step Sequencer Length : ", NormalisableRange<float>(0.05, 1.0), 1.f, "length"));
    addParameter(stepSeqOffset = new AudioParameterFloat("SSQOFF", "Step Sequencer Off Value : ", NormalisableRange<float>(0.f, 1.f), 0.f, "offValue"));
    addParameter(stepSeqMidPoint = new AudioParameterFloat("SSQEXP", "Step Sequencer Curve : ", NormalisableRange<float>(0.05, 0.95), 0.5f, "curve"));
    addParameter(stepSeqAmount = new AudioParameterFloat("SSQST", "Step Sequencer Steps : ", NormalisableRange<float>(0.0, 1.0), 0, "amount"));

    stepSeqParamIds.add(stepSeqEnable->name);   //Parameter Index 52
    stepSeqParamIds.add(stepSeqSync->name);     //Parameter Index 53
    stepSeqParamIds.add(stepSeqChopper->name);  //Parameter Index 54
    stepSeqParamIds.add(stepSeqDuration->name); //Parameter Index 55
    stepSeqParamIds.add(stepSeqLength->name);   //Parameter Index 56
    stepSeqParamIds.add(stepSeqOffset->name);   //Parameter Index 57
    stepSeqParamIds.add(stepSeqMidPoint->name); //Parameter Index 58
    stepSeqParamIds.add(stepSeqAmount->name);   //Parameter Index 59

    //8 Params
    for(int i=52; i<60; i++){
        sseqIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseEffectParameters(){
    addParameter(fxEnable = new AudioParameterBool("FXENA", "FX : ", true, "enable"));
    addParameter(fxConfig = new AudioParameterFloat("FXCONF", "FX Config : ", 0.f, 1.0, 0.f));
    addParameter(fxInputAmp = new AudioParameterFloat("FXAMP", "FX Input : ", NormalisableRange<float>(0.f, 2.f), 0.f, "amp"));
    addParameter(fxRouting = new AudioParameterFloat("FXROUT", "FX Routing : ", NormalisableRange<float>(0.f, 1.f), 0.5f, "routing"));

    fxParamIds.add(fxEnable->name);     //Parameter Index 60
    fxParamIds.add(fxConfig->name);     //Parameter Index 61
    fxParamIds.add(fxInputAmp->name);   //Parameter Index 62
    fxParamIds.add(fxRouting->name);    //Parameter Index 63

    //4 Params
    for(int i=60; i<64; i++){
        fxIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseStereoDelParameters(){
    addParameter(stereoDelEnable = new AudioParameterBool("SDELENA", "S-Delay : ", true));
    addParameter(stereoDelCrossEnable = new AudioParameterBool("SDELCRS", "S-Delay Cross : ", false));
    addParameter(stereoDelCrossAmount = new AudioParameterFloat("SDELCAM", "S-Delay Cross Amp : ", 0.0, 1.0, 0.f));
    addParameter(stereoDelMix = new AudioParameterFloat("SDELMIX", "S-Delay Mix :", 0.f, 1.f, 0.f));

    addParameter(stereoDelLeftSync = new AudioParameterBool("SDELLS", "S-Delay L-Sync  : ", false));
    addParameter(stereoDelLeft = new AudioParameterFloat("SDELLTIM", "S-Delay L-Delay : ", 0.f, 1.f, 0.f));
    addParameter(stereoDelLeftFB = new AudioParameterFloat("SDELLFB", "S-Delay L-FB : ", NormalisableRange<float>(-1.f, 1.f), 0.f));

    addParameter(stereoDelRightSync = new AudioParameterBool("SDELRS", "S-Delay R-Sync : ", false));
    addParameter(stereoDelRight = new AudioParameterFloat("SDELRTIM", "S-Delay R-Delay : ", 0.f, 1.f, 0.f));
    addParameter(stereoDelRightFB = new AudioParameterFloat("SDELRFB", "S-Delay R-FB : ", NormalisableRange<float>(-1.f, 1.f), 0.f));

    stereoDelParamIds.add(stereoDelEnable->name);       //Param index 0
    stereoDelParamIds.add(stereoDelCrossEnable->name);  //Param index 1
    stereoDelParamIds.add(stereoDelCrossAmount->name);  //Param index 2
    stereoDelParamIds.add(stereoDelMix->name);          //Param index 3
    stereoDelParamIds.add(stereoDelLeftSync->name);     //Param index 4
    stereoDelParamIds.add(stereoDelLeft->name);         //Param index 5
    stereoDelParamIds.add(stereoDelLeftFB->name);       //Param index 6
    stereoDelParamIds.add(stereoDelRightSync->name);    //Param index 7
    stereoDelParamIds.add(stereoDelRight->name);        //Param index 8
    stereoDelParamIds.add(stereoDelRightFB->name);      //Param index 9

    //10 params
    for(int i=64; i<74; i++){
        stereoDelIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialisePingPongDelay(){
    addParameter(pingPongEnable = new AudioParameterBool("PPENA", "Ping Pong : ", true));
    addParameter(pingPongLeftSync = new AudioParameterBool("PPLSYN", "Ping Pong L-Sync : ", false));
    addParameter(pingPongLeftDelay = new AudioParameterFloat("PPLDEL", "Ping Pong L-Delay : ", 0.f, 1.f, 0.f));
    addParameter(pingPongLeftFB = new AudioParameterFloat("PPLFB", "Ping Pong L-FB : ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(pingPongLeftMix = new AudioParameterFloat("PPLMIX", "Ping Pong L-Mix : ", 0.f, 1.f, 0.f));
    addParameter(pingPongRightSync = new AudioParameterBool("PPRSYN", "Ping Pong R-Sync : ", false));
    addParameter(pingPongRightDelay = new AudioParameterFloat("PPRDEL", "Ping Pong R-Delay : ", 0.f, 1.f, 0.f));
    addParameter(pingPongRightFB = new AudioParameterFloat("PPRFB", "Ping Pong R-FB : ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(pingPongRightMix = new AudioParameterFloat("PPRMIX", "Ping Pong R-Mix : ", 0.f, 1.f, 0.f));

    pingPongParamIds.add(pingPongEnable->name);     //Param Index 0
    pingPongParamIds.add(pingPongLeftSync->name);   //Param Index 1
    pingPongParamIds.add(pingPongLeftDelay->name);  //Param Index 2
    pingPongParamIds.add(pingPongLeftFB->name);     //Param Index 3
    pingPongParamIds.add(pingPongLeftMix->name);    //Param Index 4
    pingPongParamIds.add(pingPongRightSync->name);  //Param Index 6
    pingPongParamIds.add(pingPongRightDelay->name); //Param Index 7
    pingPongParamIds.add(pingPongRightFB->name);    //Param Index 8
    pingPongParamIds.add(pingPongRightMix->name);   //Param Index 9

    //9 params
    for(int i=74; i<83; i++){
        pingPongIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseLCRDelay(){
    addParameter(LCREnable = new AudioParameterBool("LCRENA", "LCR Delay : ", true));
    addParameter(LCRLeftSync = new AudioParameterBool("LCRLSYN", "LCR Delay L-Sync : ", false));
    addParameter(LCRLeftDelay = new AudioParameterFloat("LCRLDEL", "LCR Delay L-Delay : ", 0.f, 1.f, 0.f));
    addParameter(LCRLeftFB = new AudioParameterFloat("LCRLFB", "LCR Delay L-FB : ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(LCRLeftMix = new AudioParameterFloat("LCRLMIX", "LCR Delay L-Mix : ", 0.f, 1.f, 0.f));

    addParameter(LCRCenterSync = new AudioParameterBool("LCRCSYN", "LCR Delay C-Sync : ", false));
    addParameter(LCRCenterDelay = new AudioParameterFloat("LCRCDEL", "LCR Delay C-Delay : ", 0.f, 1.f, 0.f));
    addParameter(LCRCenterFB = new AudioParameterFloat("LCRCFB", "LCR Delay C-FB : ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(LCRhpfEnable = new AudioParameterBool("LCRHPFEN", "LCR Delay HPF : ", false));
    addParameter(LCRhpfFrequency = new AudioParameterFloat("LCRHPFFQ", "LCR Delay HPF F : ", NormalisableRange<float>(20.f, 20000.f), 0.f));
    addParameter(LCRlpfEnable = new AudioParameterBool("LCRLPREN", "LCR Delay LPF : ", false));
    addParameter(LCRlpfFrequency = new AudioParameterFloat("LCRLPFFQ", "LCR Delay LPF F : ", NormalisableRange<float>(20.f, 20000.f), 18000.f));
    addParameter(LCRCenterAmp = new AudioParameterFloat("LCRCAMP", "LCR Delay C-Amp : ", 0.f, 1.f, 0.f));
    addParameter(LCRCenterPan = new AudioParameterFloat("LCRCPAN", "LCR Delay C-Pan : ", 0.f, 1.f, 0.5f));

    addParameter(LCRRightSync = new AudioParameterBool("LCRRSYN", "LCR Delay R-Sync : ", false));
    addParameter(LCRRightDelay = new AudioParameterFloat("LCRRDEL", "LCR Delay R-Delay : ", 0.f, 1.f, 0.f));
    addParameter(LCRRightFB = new AudioParameterFloat("LCRRFB", "LCR Delay R-FB : ", NormalisableRange<float>(-1.f, 1.f),0.f));
    addParameter(LCRRightMix = new AudioParameterFloat("LCRRMIX", "LCR Delay R-Mix : ", 0.f, 1.f, 0.f));

    lcrDelayParamIds.add(LCREnable->name);          //Param Index  0
    lcrDelayParamIds.add(LCRLeftSync->name);        //Param Index  1
    lcrDelayParamIds.add(LCRLeftDelay->name);       //Param Index  2
    lcrDelayParamIds.add(LCRLeftFB->name);          //Param Index  3
    lcrDelayParamIds.add(LCRLeftMix->name);         //Param Index  4

    lcrDelayParamIds.add(LCRCenterSync->name);      //Param Index  5
    lcrDelayParamIds.add(LCRCenterDelay->name);     //Param Index  6
    lcrDelayParamIds.add(LCRCenterFB->name);        //Param Index  7
    lcrDelayParamIds.add(LCRhpfEnable->name);       //Param Index  8
    lcrDelayParamIds.add(LCRhpfFrequency->name);    //Param Index  9
    lcrDelayParamIds.add(LCRlpfEnable->name);       //Param Index 10
    lcrDelayParamIds.add(LCRlpfFrequency->name);    //Param Index 11
    lcrDelayParamIds.add(LCRCenterAmp->name);       //Param Index 12
    lcrDelayParamIds.add(LCRCenterPan->name);       //Param Index 13

    lcrDelayParamIds.add(LCRRightSync->name);       //Param Index 14
    lcrDelayParamIds.add(LCRRightDelay->name);      //Param Index 15
    lcrDelayParamIds.add(LCRRightFB->name);         //Param Index 16
    lcrDelayParamIds.add(LCRRightMix->name);        //Param Index 17

    // 18 params
    for(int i=83; i<101; i++){
        lcrDelayIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseStereoFlanger(){
    addParameter(stereoFlangerEnable = new AudioParameterBool("SFLENA", "S-Flanger : ", true));
    addParameter(stereoFlangerCross = new AudioParameterBool("SFLCRSS", "S-Flanger Cross : ", false));
    addParameter(stereoFlangerSync = new AudioParameterBool("SFLSYNC", "S-Flanger Sync : ", false));

    addParameter(stereoFlangerRate = new AudioParameterFloat("SFLRATE", "S-Flanger Rate : ", 0.f, 1.f, 0.f));
    addParameter(stereoFlangerPhase = new AudioParameterFloat("SFLPHASE", "S-Flanger Phase : ", 0.f, 1.f, 0.5f));
    addParameter(stereoFlangerDepth = new AudioParameterFloat("SFLDEPTH", "S-Flanger Depth : ", 0.f, 1.f, 0.f));
    addParameter(stereoFlangerFeedBack = new AudioParameterFloat("SFLFB", "S-Flanger Feedback : ", NormalisableRange<float>(-1.f, 1.f), 0.5f));

    addParameter(stereoFlangerWave = new AudioParameterFloat("SFLWAVE", "S-Flanger Wave : ", 0.f, 1.f, 0.f));

    sFlangerParamIds.add(stereoFlangerEnable->name);    // Param Index 0
    sFlangerParamIds.add(stereoFlangerCross->name);     // Param Index 1
    sFlangerParamIds.add(stereoFlangerSync->name);      // Param Index 2
    sFlangerParamIds.add(stereoFlangerRate->name);      // Param Index 3
    sFlangerParamIds.add(stereoFlangerPhase->name);     // Param Index 4
    sFlangerParamIds.add(stereoFlangerDepth->name);     // Param Index 5
    sFlangerParamIds.add(stereoFlangerFeedBack->name);  // Param Index 6
    sFlangerParamIds.add(stereoFlangerWave->name);      // Param Index 7

    // 8 Params
    for(int i=101; i<109; i++){
        sFlangerIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseDimensionChorus(){
    addParameter(dChorusEnable = new AudioParameterBool("DCHOENA", "D-Chorus : ", true));
    addParameter(dChorusRate = new AudioParameterFloat("DCHORATE", "D-Chorus Rate :", NormalisableRange<float>(0.02f, 5.f), 0.5f));
    addParameter(dChorusSpread = new AudioParameterFloat("DCHOSPRD", "D-Chorus Spread : ", 0.f, 1.f, 0.f));
    addParameter(dChorusDepth = new AudioParameterFloat("DCHODEPTH", "D-Chorus Depth : ", 0.f, 1.f, 0.f));
    addParameter(dChorusWave = new AudioParameterFloat("DCHOWAVE", "D-Chorus Wave : ", 0.f, 1.0f, 0.f));
    addParameter(dChorusFrequency = new AudioParameterFloat("DCHOFREQ", "D-Chorus Frequency : ", NormalisableRange<float>(20.f, 20000.f), 0.5f));
    addParameter(dChorusCenterAmp = new AudioParameterFloat("DCHOCM", "D-Chorus C-Amp : ", 0.f, 1.f, 0.5f));
    addParameter(dChorusCenterPan = new AudioParameterFloat("DCHOCP", "D-Chorus C-Pan : ", 0.f, 1.f, 0.f));
    addParameter(dChorusMix = new AudioParameterFloat("DCHOMIX", "D-Chorus Mix : ", 0.f, 1.f, 0.5f));

    dChorusParamIds.add(dChorusEnable->name);       // Param Index 0
    dChorusParamIds.add(dChorusRate->name);         // Param Index 1
    dChorusParamIds.add(dChorusSpread->name);       // Param Index 2
    dChorusParamIds.add(dChorusDepth->name);        // Param Index 3
    dChorusParamIds.add(dChorusWave->name);         // Param Index 4
    dChorusParamIds.add(dChorusFrequency->name);    // Param Index 5
    dChorusParamIds.add(dChorusCenterAmp->name);    // Param Index 6
    dChorusParamIds.add(dChorusCenterPan->name);    // Param Index 7
    dChorusParamIds.add(dChorusMix->name);          // Param Index 8

    // 9 Params
    for(int i=109; i<118; i++){
        dChorusIndices.push_back(i);
    }
}

/*
    AudioParameterBool *hDelayEnable;
    AudioParameterBool *hDelayTrigger;
    AudioParameterBool *hDelaySync;
    AudioParameterFloat *hDelayLoop;
    AudioParameterFloat *hDelaySpeed;

    AudioParameterBool *hDelaySmooth;
    AudioParameterFloat *hDelayGlide;
    AudioParameterFloat *hDelayDirection;

    AudioParameterBool *hDelayGapPosition;
    AudioParameterFloat *hDelayGap;

    AudioParameterFloat *hDelayFade;

    AudioParameterFloat *hDelayPan;
    AudioParameterFloat *hDelayMix;

 */
void AkatekoAudioProcessor::initialiseHoldDelay(){
    addParameter(hDelayEnable = new AudioParameterBool("HDELENA", "H-Delay : ", true));
    addParameter(hDelayTrigger = new AudioParameterBool("HDELTRIG", "H-Delay Trigger : ", false));
    addParameter(hDelaySync = new AudioParameterBool("HDELSYN", "H-Delay Beat : ", true));
    addParameter(hDelayLoop = new AudioParameterFloat("HDELLOOP", "H-Delay Loop :  ", 0.f, 1.0f, 500.f));
    addParameter(hDelaySpeed = new AudioParameterFloat("HDELSPD", "H-Delay Speed : ", 0.f, 1.f, 1.0f));

    addParameter(hDelaySmooth = new AudioParameterBool("HDELSMTH", "H-Delay Glide : ", false));
    addParameter(hDelayGlide = new AudioParameterFloat("HDELGLI", "H-Delay Glide : ", 0.f, 1.f, 4.f));
    addParameter(hDelayDirection = new AudioParameterFloat("HDELDIR", "H-Delay Dir : ", 0.f, 1.f, 0.f));

    addParameter(hDelayGap = new AudioParameterBool("HDELGPOS", "H-Delay Gap : ", false));
    addParameter(hDelayLength = new AudioParameterFloat("HDELLEN", "H-Delay Length : ",  0.f, 1.f, 1.f)); //Make it dependent on length selected

    addParameter(hDelayFade = new AudioParameterFloat("HDELFADE", "H-Delay Fade : ", 0.f, 1.f, 0.f));

    addParameter(hDelayPan = new AudioParameterFloat("HDELPAN", "H-Delay Pan : ", 0.f, 1.f, 0.5f));
    addParameter(hDelayMix= new AudioParameterFloat("HDELMIX", "H-Delay Mix : ", 0.f, 1.0f, 1.0f));

    hDelayParamIds.add(hDelayEnable->name);     // Param Index 0
    hDelayParamIds.add(hDelayTrigger->name);    // Param Index 1
    hDelayParamIds.add(hDelaySync->name);       // Param Index 2
    hDelayParamIds.add(hDelayLoop->name);       // Param Index 3
    hDelayParamIds.add(hDelaySpeed->name);      // Param Index 4

    hDelayParamIds.add(hDelaySmooth->name);     // Param Index 5
    hDelayParamIds.add(hDelayGlide->name);      // Param Index 6
    hDelayParamIds.add(hDelayDirection->name);  // Param Index 7
    hDelayParamIds.add(hDelayGap->name);        // Param Index 8

    hDelayParamIds.add(hDelayLength->name);     // Param Index 9
    hDelayParamIds.add(hDelayFade->name);       // Param Index 10

    hDelayParamIds.add(hDelayPan->name);        // Param Index 11
    hDelayParamIds.add(hDelayMix->name);        // Param Index 12

    // 13 Params
    for(int i=118; i<131; i++){
        hDelayIndices.push_back(i);
    }
}

/*
    AudioParameterBool *decimatorEnable;
    AudioParameterBool *decimatorReduceEna;
    AudioParameterBool *decimatorSRateEna;
    AudioParameterBool *decimatorFilter;
    AudioParameterFloat *decimatorBits;
    AudioParameterFloat *decimatorSRate;
    AudioParameterFloat *decimatorMix;
 */

void AkatekoAudioProcessor::initialiseDecimator(){
    addParameter(decimatorEnable = new AudioParameterBool("DECENA", "Decimator : ", true));
    addParameter(decimatorReduceEna = new AudioParameterBool("DECRENA", "Decimator Reducer : ", true));
    addParameter(decimatorSRateEna = new AudioParameterBool("DECSRENA", "Decimator S-Rate : ", true));
    addParameter(decimatorFilter = new AudioParameterBool("DECFIL", "Decimator Filter : ", true));
    addParameter(decimatorBits = new AudioParameterFloat("DECBIT", "Decimator Reduce : ", 0.f, 1.f, 0.f));
    addParameter(decimatorSRate = new AudioParameterFloat("DECSRT", "Decimator S-Rate :", 0.f, 1.f, 0.f));
    addParameter(decimatorMix = new AudioParameterFloat("DECMIX", "Decimator Mix : ", 0.f, 1.f, 0.5f));

    decimatorParamIds.add(decimatorEnable->name);       // Param Index 0
    decimatorParamIds.add(decimatorReduceEna->name);    // Param Index 1
    decimatorParamIds.add(decimatorSRate->name);        // param Index 2
    decimatorParamIds.add(decimatorFilter->name);       // Param Index 3
    decimatorParamIds.add(decimatorBits->name);         // Param Index 4
    decimatorParamIds.add(decimatorSRate->name);        // Param Index 5
    decimatorParamIds.add(decimatorMix->name);          // Param Index 7

    //7 Params
    for(int i=131; i<138; i++){
        decimatorIndices.push_back(i);
    }
}

/*
    AudioParameterBool *pReverbEnable;
    AudioParameterFloat *pReverbDamping;
    AudioParameterFloat *pReverbBandWidth;
    AudioParameterFloat *pReverbDecay;
    AudioParameterFloat *pReverbMix;
 */

void AkatekoAudioProcessor::initialisePlateReverb(){
    addParameter(pReverbEnable = new AudioParameterBool("PREVEN", "Plate Reverb : ", true));
    addParameter(pReverbDamping = new AudioParameterFloat("PREVDAM", "Plate Reverb Damping : ", 0.f, 1.0f, 0.005f));
    addParameter(pReverbBandWidth = new AudioParameterFloat("PREVBW", "Plate Reverb BandWidth : ", 0.f, 1.f, 0.995f));
    addParameter(pReverbDecay = new AudioParameterFloat("PREVDEC", "Plate Reverb Decay :", 0.f, 1.0f, 0.5f));
    addParameter(pReverbMix = new AudioParameterFloat("PREVMIX", "Plate Reverb Mix : ", 0.f, 1.f, 0.5f));

    pReverbParamIds.add(pReverbEnable->name);       // Param Index 0
    pReverbParamIds.add(pReverbDamping->name);      // Param Index 1
    pReverbParamIds.add(pReverbBandWidth->name);    // Param Index 2
    pReverbParamIds.add(pReverbDecay->name);        // Param Index 3
    pReverbParamIds.add(pReverbMix->name);          // Param Index 5

    // 5 Params
    for(int i=138; i<143; i++){
        pReverbIndices.push_back(i);
    }
}
