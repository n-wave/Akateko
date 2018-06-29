/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <iostream>
#include <algorithm>

using std::vector;
using std::sort;
using namespace akateko;

//==============================================================================
AkatekoAudioProcessor::AkatekoAudioProcessor():
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
    suspendProcessing(true);
    loadConfigurationOptions(); //Load up Configuration Options
    updateGuiSections.clear();

    updateGuiSections.clear();

    for(int i=0; i<nrOfSections; i++){
        updateGuiSections.push_back(false);
    }

    midiLearn = false;;
    midiMsgBounded = false;

    presetManager = new ConfigurationFileManager(defaultPresetDirectory, "*.fxp");

    initialiseShapes();
    initialiseParameters();


    //First get resonance scalar from
    //Configuration file

    double tmpScalar;

    switch(filterResonance){
        case 0: tmpScalar = 2.0; break;
        case 1: tmpScalar = 2.5; break;
        case 2: tmpScalar = 3.0; break;
        case 3: tmpScalar = 3.5; break;
        case 4: tmpScalar = 4.0; break;

        default: tmpScalar = 3.0; break;
    }

    voiceLeft.setResonanceScalar(tmpScalar);
    voiceRight.setResonanceScalar(tmpScalar);

    //Set to pointer after backup
    effectOne = new StereoProcessor();
    effectTwo = new StereoProcessor();

    suspendProcessing(false);
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
    suspendProcessing(true);

    const int inputChannels = getTotalNumInputChannels();
    const double sRate = getSampleRate();

    voiceLeft.setSampleRate(sRate);
    voiceRight.setSampleRate(sRate);



    lfoOne.setSampleRate(sRate);
    lfoTwo.setSampleRate(sRate);
    envOne.setSampleRate(sRate);
    envTwo.setSampleRate(sRate);
    stepSeq.setSampleRate(sRate);
    audioTrigger.setSamplerate(sRate);

    lfoOne.reset();
    lfoTwo.reset();
    envOne.reset();
    envTwo.reset();
    stepSeq.reset();
    audioTrigger.reset();

    if(effectOne != nullptr){
        effectOne->setSampleRate(sRate);
        effectOne->reset();
    }

    if(effectTwo != nullptr){
        effectTwo->setSampleRate(sRate);
        effectTwo->reset();
    }



    //akatekoGlobal.setSampleRate(this->getSampleRate());
    suspendProcessing(false);
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

    getPlayHead()->getCurrentPosition(position);
    //Midi Messages
    int time;
    MidiMessage message;

    for(MidiBuffer::Iterator i(midiMessages); i.getNextEvent(message, time);){
        /************************ MIDI Learn ************************/
        if(midiLearn){
            if(mrIndex >= 0 && mrIndex < midiTable.size()){
                if(message.isNoteOn()){
                    midiTable[mrIndex].midiMessage = MIDI_NOTE;
                    midiTable[mrIndex].channel = message.getChannel();
                    midiTable[mrIndex].firstByte = message.getNoteNumber();
                    midiLearn = false;
                    midiMsgBounded = true;
                } else if(message.isController()){
                    midiTable[mrIndex].midiMessage = MIDI_CC;
                    midiTable[mrIndex].channel = message.getChannel();
                    midiTable[mrIndex].firstByte = message.getControllerNumber();
                    midiLearn = false;
                    midiMsgBounded = true;
                } else if(message.isPitchWheel()){
                    midiTable[mrIndex].midiMessage = MIDI_PB;
                    midiTable[mrIndex].channel = message.getChannel();
                    midiLearn = false;
                    midiMsgBounded = true;
                }
            }
        }

        /************************ Check MIDI Messages ************************/

        int receivedMessage = -1;
        int firstByte = -1;
        int receivedChannel = message.getChannel();
        int receivedValue = 0;
        bool noteOn = false;
        bool noteOff = false;


        if(message.isNoteOn()){
            receivedMessage = akateko::MIDI_NOTE;
            firstByte = message.getNoteNumber();
            receivedValue = message.getVelocity();
            noteOn = true;
        } else if(message.isNoteOff()){
            receivedMessage = akateko::MIDI_NOTE;
            firstByte = message.getNoteNumber();
            receivedValue = message.getVelocity();
            noteOff = true;
        } else if(message.isController()){
            receivedMessage = akateko::MIDI_CC;
            firstByte = message.getControllerNumber();
            receivedValue = message.getControllerValue();
        } else if(message.isPitchWheel()){
            //Midi To Double?
            receivedMessage = akateko::MIDI_PB;
            receivedValue = message.getPitchWheelValue()*preScalar14Bit;
        }

        if(receivedMessage != -1 && !midiTable.empty()){
            bool run = true;
            int index = 0;


            while(run && index < midiTable.size()){
                if(receivedChannel == midiTable[index].channel &&
                   firstByte == midiTable[index].firstByte &&
                   receivedMessage == midiTable[index].midiMessage)
                {
                    const int handling = midiTable[index].handling;
                    //std::cout << "Received Param Index : " << midiTable[index].paramIndex << std::endl;

                    if(handling == akateko::MIDI_TO_INT_BUTTON){

                        if(noteOn){
                            setParameter(midiTable[index].paramIndex, 1.0);
                        } else if(noteOff){
                            setParameter(midiTable[index].paramIndex, 0.0);
                        }

                        updateGuiSections[midiTable[index].section] = true;
                        updateGui = true;
                    } else if(handling == akateko::MIDI_TO_INT_TOGGLE && !noteOff){
                        const bool tmpValue = !getParameter(midiTable[index].paramIndex);
                        setParameter(midiTable[index].paramIndex, tmpValue);

                        updateGuiSections[midiTable[index].section] = true;
                        updateGui = true;
                    } else if(handling == akateko::MIDI_TO_INT && !noteOff){
                        const int delta =  midiTable[index].maxValue - midiTable[index].minValue;
                        const int pIndex = midiTable[index].paramIndex;
                        int tmpValue = midiTable[index].minValue+(receivedValue*scalar7Bit*delta);

                        beatsPerMinute = 120.0;

                        if(pIndex == 27 || pIndex == 35 || pIndex == 111){ // Lfo's, flanger
                            const double tmpFreq = frequency_ratios[tmpValue]*beatsPerMinute;
                            setParameter(pIndex, tmpFreq);
                        } else if(pIndex == 42 || pIndex == 50){ // Envelopes
                            const double tmpDuration = env_timing_ratios[tmpValue]*(60000.0/beatsPerMinute);
                            setParameter(pIndex, tmpDuration);
                        } else if(pIndex == 59){
                            const double tmpDuration = sseq_timing_ratios[tmpValue]*(60000.0/beatsPerMinute);
                            setParameter(pIndex, tmpDuration); // S-Delay, PP-Delay and LCR-Delay
                        } else if(pIndex == 76 || pIndex == 79 || pIndex == 83 || pIndex == 87 ||
                                  pIndex == 92 || pIndex == 96 || pIndex == 105)
                        {
                            const double tmpTime = delay_timing_ratios[tmpValue]*(60000.0/beatsPerMinute);
                            setParameter(pIndex, tmpTime);
                        } else if(pIndex == 128){
                            const double tmpTime = h_delay_timing_ratios[tmpValue]*(60000.0/beatsPerMinute);
                            setParameter(pIndex, tmpTime);
                        } else if(pIndex == 139){
                            const double tmpLatch = h_delay_latch_ratios[tmpValue];
                            setParameter(pIndex, tmpLatch);
                        } else {
                            setParameter(midiTable[index].paramIndex, tmpValue);
                        }

                        updateGuiSections[midiTable[index].section] = true;
                        updateGui = true;
                    } else if(handling == akateko::MIDI_TO_DOUBLE && !noteOff){
                        const double delta = midiTable[index].maxValue - midiTable[index].minValue;
                        const double tmpValue = midiTable[index].minValue+(receivedValue*scalar7Bit*delta);

                        setParameter(midiTable[index].paramIndex, tmpValue);
                        updateGuiSections[midiTable[index].section] = true;
                        updateGui = true;
                    } else if(handling == akateko::MIDI_TO_CONSTANT && !noteOff){
                        // Button Handling
                        const double minValue = midiTable[index].minValue;
                        const double maxValue = midiTable[index].maxValue;
                        int tmpValue;

                        if(minValue == maxValue){
                            tmpValue = midiTable[index].maxValue;
                        }

                        setParameter(midiTable[index].paramIndex, tmpValue);
                        updateGuiSections[midiTable[index].section] = true;
                        updateGui = true;
                    }

                    run = false;
                }

                index++;
            }
        }
    }

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int nrOfSamples = buffer.getNumSamples();
    /* Gather all Signal Parameters */

    const double inputScalar = *inputVolume;
    const double outputScalar = *globalVolume;
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

    const double aThresh = *aTriggerThreshold;
    const double aPreDelay = *aTriggerPreDelay;
    const double aHoldDelay = *aTriggerHoldDelay;
    const double aTriggerInput = *aTriggerInputSelection;

    const bool fxEnabled = *fxEnable;
    const double fxConfiguration = *fxConfig;
    const double fxInputAmplitude = *fxInputAmp;
    const double fxChannelRouting = *fxRouting;

    bool updateEditor = false;

    modMatrix.getWriteRegister(MODSRC::PADX) = *xyPadX;
    modMatrix.getWriteRegister(MODSRC::PADY) = *xyPadY;
    modMatrix.getWriteRegister(MODSRC::PADT) = *xyPadT;

    if(beatsPerMinute != position.bpm ||
       timeSigNumerator != position.timeSigNumerator ||
       timeSigDenominator != position.timeSigDenominator)
    {

        if(beatsPerMinute >= 20.0){
            beatsPerMinute = position.bpm;
        }
        timeSigNumerator = position.timeSigNumerator;
        timeSigDenominator = position.timeSigDenominator;

        stepSeq.setBeatsPerMinute(beatsPerMinute);
        lfoOne.setBeatsPerMinute(beatsPerMinute);

        effectOne->setBPM(beatsPerMinute);
        effectTwo->setBPM(beatsPerMinute);
        //Todo update parameters if bpm changed

        updateEditor = true;
    }

    // Convert to crotchet use normal bpm calculation
    // for generating ppq wrapper in the affected classes
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

    effectOne->setBlockSize(nrOfSamples);
    effectTwo->setBlockSize(nrOfSamples);

    effectOne->collect();
    effectTwo->collect();

    // Low Frequency Oscillator One
    lfoOne.setFrequency(*lfoOneFrequency);
    lfoOne.setPhase(*lfoOnePhase);
    lfoOne.setPulseWidthModulation(*lfoOnePWM);
    lfoOne.setSmooth(*lfoOneSmooth);

    // Low Frequency Oscillator Two
    lfoTwo.setFrequency(*lfoTwoFrequency);
    lfoTwo.setPhase(*lfoTwoPhase);
    lfoTwo.setPulseWidthModulation(*lfoTwoPWM);
    lfoTwo.setSmooth(*lfoTwoSmooth);

    // Envelope One
    envOne.setDuration(*envOneDuration);
    // Envelope Two
    envTwo.setDuration(*envTwoDuration);

    // Step Sequencer
    stepSeq.setStepLength(*stepSeqLength);
    stepSeq.setOffValue(*stepSeqOffset);

    audioTrigger.setPreDelay(aPreDelay);
    audioTrigger.setHoldDelay(aHoldDelay);
    audioTrigger.setThreshold(aThresh);

    double convertedPPQ = 0.0;

    if(timeSigDenominator >= 2 && timeSigDenominator != 4){
        convertedPPQ = (4.0/timeSigDenominator)*position.ppqPosition;
    } else {
        convertedPPQ = position.ppqPosition;
    }

    const bool lfo1Sync = *lfoOneSync;
    const bool lfo2Sync = *lfoTwoSync;
    const bool stepSync = *stepSeqSync;

    if(lfo1Sync){lfoOne.setPosition(convertedPPQ);}
    if(lfo2Sync){lfoTwo.setPosition(convertedPPQ);}
    if(stepSync){stepSeq.setPosition(convertedPPQ);}

    effectOne->setPosition(convertedPPQ);
    effectTwo->setPosition(convertedPPQ);
    ppqPosition = position.ppqPosition;

    // Clear Buffer
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // AudioProcessing
        if(totalNumInputChannels == 2){
            const float *inputLeft = buffer.getReadPointer(0, 0);
            const float *inputRight = buffer.getReadPointer(1, 0);

            float *leftChannel = buffer.getWritePointer(0, 0);
            float *rightChannel = buffer.getWritePointer(1, 0);

            for(int i=0; i<nrOfSamples; i++){
                double tmpLeft = inputLeft[i]*inputScalar;
                double tmpRight = inputRight[i]*inputScalar;
                double aTrigSample;

                // Audio Trigger
                if(aTriggerInput == 0){
                    aTrigSample = tmpLeft;
                } else if(aTriggerInput == 1){
                    aTrigSample = tmpRight;
                } else if(aTriggerInput == 2){
                    aTrigSample = 0.5*(tmpLeft+tmpRight);
                }

                audioTrigger.update();
                const bool tmpTrigger = audioTrigger.getTrigger(aTrigSample);

                if(aTrigger != tmpTrigger){
                    aTrigger = tmpTrigger;

                    if(aTrigger){
                        envOne.triggerEnvelope();
                    } else {
                        envOne.releaseEnvelope();
                    }
                }

                // Modulation Components
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

                // Entering the FX Section

                /*
                 *
                 *  const bool fxEnabled = *fxEnable;
                 *  const double fxConfiguration = *fxConfig;
                 *  const double fxInputAmplitude = *fxInputAmp;
                 *  const double fxChannelRouting = *fxRouting;
                 *
                 */

                if(fxEnabled){
                    tmpLeft *= fxInputAmplitude;
                    tmpRight *= fxInputAmplitude;

                    double fxOneRouting = 1.0-fxChannelRouting;
                    double fxTwoRouting = fxChannelRouting;

                    double fxOneLeft = tmpLeft*fxOneRouting;
                    double fxOneRight = tmpRight*fxOneRouting;
                    double fxTwoLeft = tmpLeft*fxTwoRouting;
                    double fxTwoRight = tmpRight*fxTwoRouting;

                    if(fxConfiguration == 0){ //Stereo
                        effectOne->calculateSamples(fxOneLeft, fxOneRight);

                        //Add and use as input if fxTwoRouting is 0
                        // it is full stereo else it functions as hybrid
                        tmpLeft = effectOne->getLeftSample() + fxTwoLeft;
                        tmpRight = effectOne->getRightSample() + fxTwoRight;

                        effectTwo->calculateSamples(tmpLeft, tmpRight);
                        tmpLeft = effectTwo->getLeftSample();
                        tmpRight = effectTwo->getRightSample();
                    } else { //Paralell
                        effectOne->calculateSamples(fxOneLeft, fxOneRight);
                        effectTwo->calculateSamples(fxTwoLeft, fxTwoRight);

                        tmpLeft = effectOne->getLeftSample() + effectTwo->getLeftSample();
                        tmpRight = effectOne->getRightSample() + effectTwo->getRightSample();
                    }
                }

                //Multiply with main volume at a later stage
                leftChannel[i] = static_cast<float>(tmpLeft*outputScalar);
                rightChannel[i] = static_cast<float>(tmpRight*outputScalar);
            }
        }
        //Update editor if neccessary


   if(updateEditor){
        AudioProcessorEditor *tmpEditor = getActiveEditor();

        if(tmpEditor != nullptr){
            tmpEditor->postCommandMessage(AkatekoAudioProcessorEditor::updateTimeId);
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

int AkatekoAudioProcessor::getActiveEffect(int fx){
    if(fx == 1){
        return fxOne;
    } else if(fx == 2) {
        return fxTwo;
    } else {
        return 0;
    }
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
        std::cerr << "Canceled save: " << row.name << std::endl;

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

    if(lfoOneShapes.size() == 8 &&
       lfoTwoShapes.size() == 8 &&
       waveShapes.size() == 8)
    {
        for(int i=0; i<8; i++){
            const String lfoOneAttr = String("LFOONE") + String(i);
            const String lfoTwoAttr = String("LFOTWO") + String(i);
            const String wsAttr = String("WSHAPE") + String(i);

            xml.setAttribute(lfoOneAttr, lfoOneShapes[i]);
            xml.setAttribute(lfoTwoAttr, lfoTwoShapes[i]);
            xml.setAttribute(wsAttr, waveShapes[i]);
        }
    }

    // Set UI States
    xml.setAttribute("ENV1", envOneState);
    xml.setAttribute("ENV2", envTwoState);
    xml.setAttribute("SSEQ", stepSeqState);
    xml.setAttribute("XYPAD", xyPadState);
    xml.setAttribute("FXST", fxState);
    xml.setAttribute("MODM", modMatrix.toString());

    // Add an Attribute with the total number of MidiRows
    // Use the size to gather the total of mrows in the
    // preset
    xml.setAttribute("NRMROW", String(midiTable.size()));

    for(int i=0; i<midiTable.size(); i++){
        const String xmlAttribute = String("MROW") + String(i);
        const String sRow = akateko::midiRowToString(midiTable[i]);

        xml.setAttribute(xmlAttribute, sRow);
    }

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

        for(int i=0; i<8; i++){
            const String lfoOneAttr = String("LFOONE") + String(i);
            const String lfoTwoAttr = String("LFOTWO") + String(i);
            const String wsAttr = String("WSHAPE") + String(i);

            lfoOneShapes.set(i, xmlState->getStringAttribute(lfoOneAttr));
            lfoTwoShapes.set(i, xmlState->getStringAttribute(lfoTwoAttr));
            waveShapes.set(i, xmlState->getStringAttribute(wsAttr));
        }

        // Get Envelope and restore shapes and recreate the buffer
        envOneState = String(xmlState->getStringAttribute("ENV1"));
        envTwoState = String(xmlState->getStringAttribute("ENV2"));
        stepSeqState = String(xmlState->getStringAttribute("SSEQ"));
        xyPadState = String(xmlState->getStringAttribute("XYPAD"));
        fxState = String(xmlState->getStringAttribute("FXST"));
        modMatrix.restoreFromString(xmlState->getStringAttribute("MODM"));

        // Get Number of MidiRows
        const int nrOfMidiRows = xmlState->getIntAttribute("NRMROW");

        midiTable.clear();

        // Restore From the String stored in the xml
        for(int i=0; i<nrOfMidiRows; i++){
            String xmlAttr = String("MROW") + String(i);
            String sRow = xmlState->getStringAttribute(xmlAttr);

            std::cout << sRow << std::endl;

            MidiRow newRow;



            if(akateko::restoreMidiRow(newRow, sRow)){
                midiTable.push_back(newRow);
            }
        }

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
// Configuration Handling
//==============================================================================

void AkatekoAudioProcessor::storeConfigurationOption(const String value,
                                                     const GlobalOptions option)
{
    File xmlFile = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile("config/config.xml");

    if(xmlFile.existsAsFile()){
        //std::cout << "Succesfully founded the file proceed in load xml" << std::endl;

        XmlDocument xmlDocument(xmlFile);
        ScopedPointer<XmlElement> xmlState = xmlDocument.getDocumentElement();

        if(xmlState != nullptr){
            if(xmlState->hasTagName("AKATEKOGLOBAL-V1.00b")){
                //std::cout << "Config File found setting Attribute" << std::endl;
                bool proceed = true;

                switch(option){
                    case DefaultPresetDirectoryId:
                        defaultPresetDirectory = value;
                        xmlState->setAttribute("PRSTDIR", value);
                        break;
                    case OverSamplingId:
                        overSampling = value.getIntValue();
                        xmlState->setAttribute("OVERSAM", value);
                        break;
                    case FilterResonanceId:
                        filterResonance = value.getIntValue();
                        xmlState->setAttribute("FILRESO", value);
                        break;
                    default:
                        proceed = false;
                        std::cerr << "AkatekoAudioProcessor::storeConfigurationOption" << std::endl;
                        std::cerr << "Received Invalid Option, breaking operation" << std::endl;
                        break;
                }

                if(proceed){
                    xmlState->writeToFile(xmlFile, String::empty);
                } else {
                    std::cerr << "AkatekoAudioProcessor::storeConfigurationOption" << std::endl;
                    std::cerr << "Could not write to file" << std::endl;
                }
            }
        } else {
            std::cerr << "AkatekoAudioProcessor::storeConfigurationOption" << std::endl;
            std::cerr << "Received Nullptr, File has been corrupted" << std::endl;
        }

    } else {
        std::cerr << "AkatekoAudioProcessor::storeConfigurationOption" << std::endl;
        std::cerr << "File doesn't exist" << std::endl;
    }

}

void AkatekoAudioProcessor::loadConfigurationOptions(){
    File xmlFile = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile("config/config.xml");

    if(xmlFile.existsAsFile()){
        XmlDocument xmlDocument(xmlFile);
        ScopedPointer<XmlElement> xmlState = xmlDocument.getDocumentElement();

        if(xmlState != nullptr){
            if(xmlState->hasTagName("AKATEKOGLOBAL-V1.00b")){
                defaultPresetDirectory = xmlState->getStringAttribute("PRSTDIR");
                overSampling = xmlState->getStringAttribute("OVERSAM").getIntValue();
                filterResonance = xmlState->getStringAttribute("FILRESO").getIntValue();
            } else {
                std::cerr << "AkatekoAudioProcessor::storeConfigurationOption" << std::endl;
                std::cerr << "Incorrect Tag, Wrong File" << std::endl;
                std::cerr << "Falling back to default values, creating new config.xml" << std::endl;

                //TODo implement fail safe method
            }
        } else {
            std::cerr << "AkatekoAudioProcessor::storeConfigurationOption" << std::endl;
            std::cerr << "Received Nullptr, File has been corrupted" << std::endl;
            std::cerr << "Falling back to default values, creating new config.xml" << std::endl;
            // TODO implement fail safe method
        }


    } else {
        std::cerr << "AkatekoAudioProcessor::loadConfigurationOptions" << std::endl;
        std::cerr << "Configuration File doest not exist, falling back to default" << std::endl;
    }

    // Gives the current application File
}
/* Todo Implement
 *
 * Only called when the file is missing or corrupted
 * Under normal circumenstances this will not occur.
 *
 * This function will check if the folder is present,
 * if not an additional folder will created with a new
 * config.xml file with default values.
 *
 * This also means it will check if a preset folder
 * is present if not an empty folder will be created
 * with no presets
 *
 */

void AkatekoAudioProcessor::createDefaultConfigurationFile(){

}

String AkatekoAudioProcessor::getDefaultPresetDirectory(){
    return defaultPresetDirectory;
}

int AkatekoAudioProcessor::getOverSampling(){
    return overSampling;
}

int AkatekoAudioProcessor::getFilterResonance(){
    return filterResonance;
}

//==============================================================================

// Return Time Signature via the Arguments
bool AkatekoAudioProcessor::getTimeSignature(double &bpm, double &tden){
    if(beatsPerMinute != 0 ||
       timeSigNumerator != 0 ||
       timeSigDenominator)
    {
        bpm = beatsPerMinute;
        tden = timeSigDenominator;

        return true;
    }
    return false; //If 0 zero don't update
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

void AkatekoAudioProcessor::updateResonanceScalar(double scalar){
    voiceLeft.setResonanceScalar(scalar);
    voiceRight.setResonanceScalar(scalar);
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

    //std::cout << "midPoint: " << mp << std::endl;

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
    suspendProcessing(true);

    if(effectProcessor >= 1 && effectProcessor <=9){
        effectOne->exit();
        fxOne = effectProcessor;
    }


    switch(effectProcessor){
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
    effectOne->reset();
    effectOne->setBPM(beatsPerMinute);
    suspendProcessing(false);
}

void AkatekoAudioProcessor::setEffectTwo(int effectProcessor){
    suspendProcessing(true);

    if(effectProcessor >= 1 && effectProcessor <=9){
        effectTwo->exit();
        fxTwo = effectProcessor;
    }

    switch(effectProcessor){
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
    effectTwo->reset();
    effectTwo->setBPM(beatsPerMinute);
    suspendProcessing(false);
}

void AkatekoAudioProcessor::initiateMidiLearn(MidiRow row)
{
    if(midiLearn && mrIndex == midiTable.size()-1){
        midiTable.pop_back();
        mrIndex =  0;
    }

    midiLearn = true;
    midiMsgBounded = false;

    bool run = true;
    int index = 0;

    //Suplied Indices
    mrpIndex = row.paramIndex;
    const int saIndex = row.arrayIndex;

    while(run && index < midiTable.size()){
        const int pIndex = midiTable[index].paramIndex;
        const int aIndex = midiTable[index].arrayIndex;

        if(pIndex == mrpIndex && saIndex == aIndex){
            midiTable[index] = row;
            mrIndex = index; //Store for fast retrieval
            run = false;
        }
        index++;
    }

    if(run){
        mrIndex = index; //If minus One bind to lastRow
        midiTable.push_back(row);
    }
}

bool AkatekoAudioProcessor::getMidiLearnStatus(){
    return midiLearn;
}

bool AkatekoAudioProcessor::getMidiMsgBoundStatus(){
    bool result = midiMsgBounded;
    midiMsgBounded = false; //Reset after polling

    return result;
}

String AkatekoAudioProcessor::getMidiMessageBounded(){
    String result = "Error";

    if(mrpIndex < getParameters().size()){
        result = getParameters().getUnchecked(mrpIndex)->getName(32);
    }

    return result;
}
// Use for trigger functionality of Envelope components

void AkatekoAudioProcessor::setMidiTriggerRow(MidiRow row){
    bool run = true;
    int index = 0;

    //Suplied Indices
    const int spIndex = row.paramIndex;

    while(run && index < midiTable.size()){
        const int pIndex = midiTable[index].paramIndex;

        if(pIndex == spIndex){
            mrIndex = index; //Store for fast retrieval
            run = false;
        }
        index++;
    }
    // If not push on the stack
    if(run){
        mrIndex = index; //If minus One bind to lastRow
        midiTable.push_back(row);
    }
}

// Set Midi Row copy the added MidiRow and alter the midirow
// if it has been found, the shape buttons should be handed
// differently, for now leave as it is.
// We could use multiple Arrays and add them to the rows
// based on the Midi Messages. This will make it easier
// when midi Messages are arriving.

void AkatekoAudioProcessor::setMidiRow(MidiRow row){
    const int paramIndex = row.paramIndex;
    int index = 0;
    bool run = true;
    bool addButton = false; // Used for ws, lfo1, lfo2 Buttons
    bool sortTable = false;

    //std::cout << "Adding midi Row" << std::endl;

    while(run && index < midiTable.size()){
        int tmpIndex = midiTable[index].paramIndex;

        if(tmpIndex == paramIndex){

            if(paramIndex == 23 ||
               paramIndex == 30 ||
               paramIndex == 38)
            {
                int arrayIndex = row.arrayIndex;
                int currentIndex = midiTable[index].arrayIndex;

                if(arrayIndex == currentIndex){
                    midiTable[index] = row;
                    addButton = false;
                    run = false;
                } else {
                    addButton = true;
                }

            } else {
                midiTable[index] = row;
                run = false;
            }
        }

        index++;
    }

    //Param Index wasn't found push_back
    if(run || addButton){
        midiTable.push_back(row);
        sort(midiTable.begin(), midiTable.end(), &akateko::sortMidiRow);
    }


    for(int i=0; i<midiTable.size(); i++){
        std::cout << midiTable[i].destination << "[" << midiTable[i].paramIndex << "]" << std::endl;
    }
}

void AkatekoAudioProcessor::removeMidiRow(int arrayIndex, int paramIndex){
    int index = 0;
    bool run = true;

    while(run && index < midiTable.size()){
        int tmpArrayIndex = midiTable[index].arrayIndex;
        int tmpIndex = midiTable[index].paramIndex;


       if(tmpIndex == paramIndex &&
          tmpArrayIndex == arrayIndex)
       {
           midiTable.erase(midiTable.begin() + index);
           sort(midiTable.begin(), midiTable.end(), &akateko::sortMidiRow);
           run = false;
       }
       index++;
    }
    midiLearn = false;
}

void AkatekoAudioProcessor::removeMidiRow(int paramIndex){
    int index = 0;
    bool run = true;

    while(run && index < midiTable.size()){
        int tmpIndex = midiTable[index].paramIndex;


       if(tmpIndex == paramIndex)
       {
           midiTable.erase(midiTable.begin() + index);
           sort(midiTable.begin(), midiTable.end(), &akateko::sortMidiRow);
           run = false;
       }
       index++;
    }
    midiLearn = false;
}


void AkatekoAudioProcessor::clearAllMidiRows(){
    midiTable.clear();
}

vector<MidiRow> AkatekoAudioProcessor::getMidiRows(){
    return midiTable;
}

bool AkatekoAudioProcessor::requestGuiUpdate(){
    bool result = updateGui;
    updateGui = false;

    return result;
}

std::vector<bool> AkatekoAudioProcessor::getUpdatedSections(){
    return updateGuiSections;
}

// Scan midiTable return true if available
bool AkatekoAudioProcessor::getRegisteredMidi(int pIndex){
    bool result = false;
    int index = 0;

    while(!result && index < midiTable.size()){
        if(midiTable[index].paramIndex == pIndex){
            result = true;
        }
        index++;
    }
    return result;
}

void AkatekoAudioProcessor::changeMidiRowMinMax(double minValue, double maxValue, int handling, int pIndex){
    bool run = true;
    int index = 0;

    while(run && index < midiTable.size()){
        if(pIndex == midiTable[index].paramIndex){
            midiTable[index].minValue = minValue;
            midiTable[index].maxValue = maxValue;
            midiTable[index].handling = handling;

            run = false;
        }
        index++;
    }
}

double AkatekoAudioProcessor::getBeatsPerMinute(){
    return beatsPerMinute; // beatsPerMinute;
}


AkatekoMatrix &AkatekoAudioProcessor::getModMatrix(){
    return modMatrix;
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

void AkatekoAudioProcessor::setShape(String shape,
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
        case xyPadId:
            tmpIndices = xyPadIndices;
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
        case aTriggerId:
            tmpIndices = aTriggerIndices;
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
        case xyPadId:
            tmpParamId = xyPadParamIds;
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
        case aTriggerId:
            tmpParamId = aTriggerParamIds;
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

void AkatekoAudioProcessor::initialiseShapes(){

    lfoOneShapes.clear();

    lfoOneShapes.add(akateko::lfo_default_shapes[0]);
    lfoOneShapes.add(akateko::lfo_default_shapes[1]);
    lfoOneShapes.add(akateko::lfo_default_shapes[2]);
    lfoOneShapes.add(akateko::lfo_default_shapes[3]);
    lfoOneShapes.add(akateko::lfo_default_shapes[4]);
    lfoOneShapes.add(akateko::lfo_default_shapes[5]);
    lfoOneShapes.add(akateko::lfo_default_shapes[6]);
    lfoOneShapes.add(akateko::lfo_default_shapes[7]);

    lfoTwoShapes.clear();
    lfoTwoShapes = StringArray(lfoOneShapes);

    waveShapes.clear();
    waveShapes.add(akateko::ws_default_shapes[0]);
    waveShapes.add(akateko::ws_default_shapes[1]);
    waveShapes.add(akateko::ws_default_shapes[2]);
    waveShapes.add(akateko::ws_default_shapes[3]);
    waveShapes.add(akateko::ws_default_shapes[4]);
    waveShapes.add(akateko::ws_default_shapes[5]);
    waveShapes.add(akateko::ws_default_shapes[6]);
    waveShapes.add(akateko::ws_default_shapes[7]);
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
    initialiseXYPadParameters();
    initialiseEffectParameters();
    initialiseStereoDelParameters();
    initialisePingPongDelay();
    initialiseLCRDelay();
    initialiseStereoFlanger();
    initialiseDimensionChorus();
    initialiseHoldDelay();
    initialiseDecimator();
    initialisePlateReverb();
    initialiseAudioTrigger();
}

void AkatekoAudioProcessor::initialiseGlobalParameters(){
    addParameter(inputVolume = new AudioParameterFloat("GLIVL","Global In: ", 0.f, 1.f, 1.f));
    addParameter(globalVolume = new AudioParameterFloat("GLMVL", "Global Out: ", 0.f, 1.f, 1.f));

    globalParamIds.clear();
    globalParamIds.add(inputVolume->name); //Parameter Index 0
    globalParamIds.add(globalVolume->name); //Parameter Index 1

    globalIndices.clear();
    globalIndices.push_back(inputVolume->getParameterIndex());
    globalIndices.push_back(globalVolume->getParameterIndex());
}

void AkatekoAudioProcessor::initialiseFilterParameters(){
    addParameter(filterEnable = new AudioParameterBool("FLEN","Filters: ", true));
    addParameter(filterConfig = new AudioParameterInt("FLCF", "Filter Config: ",0, 1, 0));

    // Filter Ones
    addParameter(filterOneEnable = new AudioParameterBool("F1EN", "Filter 1: ", true));
    addParameter(filterOneType = new AudioParameterFloat("F1TP", "Filter 1 Type: ",  0, 1, 0.));
    addParameter(filterOneRollOff = new AudioParameterInt("F1RF", "Filter 1 RollOff: ",0, 1, 0));
    addParameter(filterOneFrequency = new AudioParameterFloat("F1FQ","Filter 1 frequency: ", NormalisableRange<float>(20.f, 20000.f), 20000.f));
    addParameter(filterOneResonance = new AudioParameterFloat("F1RE", "Filter 1 Resonance: ", 0.f,  1.f, 0.f));
    addParameter(filterOnePassBand = new AudioParameterFloat("F1PBG", "Filter 1 PassBand: ", 0.f, 1.f, 0.5f));
    addParameter(filterOneDrive = new AudioParameterFloat("F1DRI", "Filter 1 Drive: ", 0.f, 1.f, 0.f));
    addParameter(filterOneVolume = new AudioParameterFloat("F1VOL", "Filter 1 Volume: ", 0.f, 1.f, 0.707f));
    //Filter Two
    addParameter(filterTwoEnable = new AudioParameterBool("F2EN", "Filter 2: ", true));
    addParameter(filterTwoType = new AudioParameterFloat("F2TP", "Filter 2 Type: ", 0.f,  1.f, 1.f));
    addParameter(filterTwoRollOff = new AudioParameterInt("F2RF","Filter 2 RollOff: ", 0, 1, 0));
    addParameter(filterTwoFrequency = new AudioParameterFloat("F2FQ", "Filter 2 Frequency: ", NormalisableRange<float>(20.f, 20000.f), 20.f));
    addParameter(filterTwoResonance = new AudioParameterFloat("F2RE", "Filter 2 Resonance: ", 0.f,1.f, 0.f));
    addParameter(filterTwoPassBand = new AudioParameterFloat("F2PBG", "Filter 2 PassBand: ", 0.f, 1.f, 0.5f));
    addParameter(filterTwoDrive = new AudioParameterFloat("F2DRI", "Filter 2 Drive: ", 0.f, 1.f, 0.f));
    addParameter(filterTwoVolume = new AudioParameterFloat("F2VOL", "Filter 2 Volume: ", 0.f, 1.f, 0.707f));

    filterParamIds.clear();
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
    filterIndices.clear();

    filterIndices.push_back(filterEnable->getParameterIndex());         //Parameter Index  2
    filterIndices.push_back(filterConfig->getParameterIndex());         //Parameter Index  3
    filterIndices.push_back(filterOneEnable->getParameterIndex());      //Parameter Index  4
    filterIndices.push_back(filterOneType->getParameterIndex());        //Parameter Index  5
    filterIndices.push_back(filterOneRollOff->getParameterIndex());     //Parameter Index  6
    filterIndices.push_back(filterOneFrequency->getParameterIndex());   //Parameter Index  7
    filterIndices.push_back(filterOneResonance->getParameterIndex());   //Parameter Index  8
    filterIndices.push_back(filterOnePassBand->getParameterIndex());    //Parameter Index  9
    filterIndices.push_back(filterOneDrive->getParameterIndex());       //Parameter Index 10
    filterIndices.push_back(filterOneVolume->getParameterIndex());      //Parameter Index 11
    filterIndices.push_back(filterTwoEnable->getParameterIndex());      //Parameter Index 12
    filterIndices.push_back(filterTwoType->getParameterIndex());        //Parameter Index 13
    filterIndices.push_back(filterTwoRollOff->getParameterIndex());     //Parameter Index 14
    filterIndices.push_back(filterTwoFrequency->getParameterIndex());   //Parameter Index 15
    filterIndices.push_back(filterTwoResonance->getParameterIndex());   //Parameter Index 16
    filterIndices.push_back(filterTwoPassBand->getParameterIndex());    //Parameter Index 17
    filterIndices.push_back(filterTwoDrive->getParameterIndex());       //Parameter Index 18
    filterIndices.push_back(filterTwoVolume->getParameterIndex());      //Parameter Index 19
}

void AkatekoAudioProcessor::initialiseWaveShapeParameters(){
    addParameter(waveShaperEnable = new AudioParameterBool("WSEN", "WaveShaper: ", true, "enable"));
    addParameter(waveShaperDrive = new AudioParameterFloat("WSDRI", "WaveShaper Drive: ", NormalisableRange<float>(0.5, 1.f), 0.5, "drive"));
    addParameter(waveShaperMix = new AudioParameterFloat("WSMIX", "WaveShaper Mix: ", NormalisableRange<float>(0.f, 1.f), 0.f, "mix"));
    addParameter(waveShaperShape = new AudioParameterFloat("WSHAPE", "WaveShaper Shape: ", 0, 1, 0.f));

    waveShapeParamIds.clear();
    waveShapeParamIds.add(waveShaperEnable->name);  //Parameter Index 20
    waveShapeParamIds.add(waveShaperDrive->name);   //Parameter Index 21
    waveShapeParamIds.add(waveShaperMix->name);     //Parameter Index 22
    waveShapeParamIds.add(waveShaperShape->name);   //Parameter Index 23

    // 4 Params
    wsIndices.clear();
    wsIndices.push_back(waveShaperEnable->getParameterIndex());  //Parameter Index 20
    wsIndices.push_back(waveShaperDrive->getParameterIndex());   //Parameter Index 21
    wsIndices.push_back(waveShaperMix->getParameterIndex());     //Parameter Index 22
    wsIndices.push_back(waveShaperShape->getParameterIndex());   //Parameter Index 23
}

void AkatekoAudioProcessor::initialiseLFO1Parameters(){
    addParameter(lfoOneEnable = new AudioParameterBool("LF1EN", "LFO 1: ", true, "enable"));
    addParameter(lfoOneShot = new AudioParameterBool("LF1SH", "LFO 1 One shot: ", true, "one-shot"));
    addParameter(lfoOneSync = new AudioParameterBool("LF1SY", "LFO 1 Sync: ", true, "sync"));
    addParameter(lfoOneFrequency = new AudioParameterFloat("LF1F", "LFO 1 Frequency: ", 0, 1.0, 10.f));
    addParameter(lfoOnePhase = new AudioParameterFloat("LF1P", "LFO 1 Phase: ",NormalisableRange<float>(0.05f, 0.95f), 0.f));
    addParameter(lfoOnePWM = new AudioParameterFloat("LF1PW", "LFO 1 Pulse Width: ", NormalisableRange<float>(-0.95f, 0.95f), 0.f));
    addParameter(lfoOneShape = new AudioParameterFloat("L1SHAPE", "LFO 1 Shape: ", NormalisableRange<float>(0.f, 1.f), 0.f));
    addParameter(lfoOneSmooth = new AudioParameterFloat("L1SMOOTH", "LFO 1 Smooth: ", NormalisableRange<float>(0.0, 0.99), 0.f));

    lfoOneParamIds.clear();
    lfoOneParamIds.add(lfoOneEnable->name);
    lfoOneParamIds.add(lfoOneShot->name);
    lfoOneParamIds.add(lfoOneSync->name);
    lfoOneParamIds.add(lfoOneFrequency->name);
    lfoOneParamIds.add(lfoOnePhase->name);
    lfoOneParamIds.add(lfoOnePWM->name);
    lfoOneParamIds.add(lfoOneShape->name);
    lfoOneParamIds.add(lfoOneSmooth->name);

    //Load Up Indicies
    lfoOneIndices.clear();
    lfoOneIndices.push_back(lfoOneEnable->getParameterIndex());     //Parameter Index 0
    lfoOneIndices.push_back(lfoOneShot->getParameterIndex());       //Parameter Index 1
    lfoOneIndices.push_back(lfoOneSync->getParameterIndex());       //Parameter Index 2
    lfoOneIndices.push_back(lfoOneFrequency->getParameterIndex());  //Parameter Index 3
    lfoOneIndices.push_back(lfoOnePhase->getParameterIndex());      //Parameter Index 4
    lfoOneIndices.push_back(lfoOnePWM->getParameterIndex());        //Parameter Index 5
    lfoOneIndices.push_back(lfoOneShape->getParameterIndex());      //Parameter Index 6
    lfoOneIndices.push_back(lfoOneSmooth->getParameterIndex());     //Parameter Index 7
}

void AkatekoAudioProcessor::initialiseLFO2Parameters(){
    addParameter(lfoTwoEnable = new AudioParameterBool("LF2EN", "LFO 2: ", true, "enable"));
    addParameter(lfoTwoShot = new AudioParameterBool("LF2SH", "LFO 2 One Shot L: ", true, "one-shot"));
    addParameter(lfoTwoSync = new AudioParameterBool("LF2SY", "LFO 2 Sync: ", true, "sync"));
    addParameter(lfoTwoFrequency = new AudioParameterFloat("LF2F", "LFO 2 Frequency: ", 0, 1.0, 10.f));
    addParameter(lfoTwoPhase = new AudioParameterFloat("LF2P", "LFO 2 Phase: ", NormalisableRange<float>(0.05f, 0.95f), 0.f));
    addParameter(lfoTwoPWM = new AudioParameterFloat("LF2PW", "LFO 2 Pulse Width: ", NormalisableRange<float>(-0.95f, 0.95f), 0.f));
    addParameter(lfoTwoShape = new AudioParameterFloat("L2SHAPE", "LFO 2 Shape: ", NormalisableRange<float>(0.f, 1.f), 0.f , "shape"));
    addParameter(lfoTwoSmooth = new AudioParameterFloat("L2SMOOTH", "LFO 2 Smooth: ", NormalisableRange<float>(0.0, 0.99), 0.f));

    lfoTwoParamIds.clear();
    lfoTwoParamIds.add(lfoTwoEnable->name);     //Parameter Index 0
    lfoTwoParamIds.add(lfoTwoShot->name);       //Parameter Index 1
    lfoTwoParamIds.add(lfoTwoSync->name);       //Parameter Index 2
    lfoTwoParamIds.add(lfoTwoFrequency->name);  //Parameter Index 3
    lfoTwoParamIds.add(lfoTwoPhase->name);      //Parameter Index 4
    lfoTwoParamIds.add(lfoTwoPWM->name);        //Parameter Index 5
    lfoTwoParamIds.add(lfoTwoShape->name);      //Parameter Index 6
    lfoTwoParamIds.add(lfoTwoSmooth->name);     //Parameter Index 7

    //Load LFO two Parameters
    lfoTwoIndices.clear();
    lfoTwoIndices.push_back(lfoTwoEnable->getParameterIndex());     //Parameter Index 0
    lfoTwoIndices.push_back(lfoTwoShot->getParameterIndex());       //Parameter Index 1
    lfoTwoIndices.push_back(lfoTwoSync->getParameterIndex());       //Parameter Index 2
    lfoTwoIndices.push_back(lfoTwoFrequency->getParameterIndex());  //Parameter Index 3
    lfoTwoIndices.push_back(lfoTwoPhase->getParameterIndex());      //Parameter Index 4
    lfoTwoIndices.push_back(lfoTwoPWM->getParameterIndex());        //Parameter Index 5
    lfoTwoIndices.push_back(lfoTwoShape->getParameterIndex());      //Parameter Index 6
    lfoTwoIndices.push_back(lfoTwoSmooth->getParameterIndex());     //Parameter Index 7
}

void AkatekoAudioProcessor::initialiseENV1Parameters(){
    addParameter(envOneEnable = new AudioParameterBool("EV1ENA", "Envelope 1: ", true, "enable"));
    addParameter(envOneSync = new AudioParameterBool("EV1SYNC", "Envelope 1 Sync: ", false, "sync"));
    addParameter(envOneDuration = new AudioParameterFloat("EV1DUR", "Envelope 1 Dur: ", 0.f, 1.f, 0));
    addParameter(envOneLoopAmount = new AudioParameterFloat("EV1LAM", "Envelope 1 Loop nr: ", 0, 1, 1));
    addParameter(envOneLoopDirection = new AudioParameterFloat("EV1LDR", "Envelope 1 Loop dir: ", 0, 1, 0));
    addParameter(envOneSusDirection = new AudioParameterFloat("EV1SDR", "Envelope 1 Sus dir: ", 0, 1, 0));
    addParameter(envOneTriggerSource = new AudioParameterFloat("EV1TRS", "Envelope 1 T-Source: ", 0, 1, 0));
    addParameter(envOneTrigger = new AudioParameterBool("EV1TRIG", "Envelope 1 Trigger:", false));

    envOneParamIds.clear();
    envOneParamIds.add(envOneEnable->name);         //Parameter Index 0
    envOneParamIds.add(envOneSync->name);           //Parameter Index 1
    envOneParamIds.add(envOneDuration->name);       //Parameter Index 2
    envOneParamIds.add(envOneLoopAmount->name);     //Parameter Index 3
    envOneParamIds.add(envOneLoopDirection->name);  //Parameter Index 4
    envOneParamIds.add(envOneSusDirection->name);   //Parameter Index 5
    envOneParamIds.add(envOneTriggerSource->name);  //Parameter Index 6
    envOneParamIds.add(envOneTrigger->name);        //Parameter Index 7

    // 8 Params
    envOneIndices.clear();
    envOneIndices.push_back(envOneEnable->getParameterIndex());         //Parameter Index 0
    envOneIndices.push_back(envOneSync->getParameterIndex());           //Parameter Index 1
    envOneIndices.push_back(envOneDuration->getParameterIndex());       //Parameter Index 2
    envOneIndices.push_back(envOneLoopAmount->getParameterIndex());     //Parameter Index 3
    envOneIndices.push_back(envOneLoopDirection->getParameterIndex());  //Parameter Index 4
    envOneIndices.push_back(envOneSusDirection->getParameterIndex());   //Parameter Index 5
    envOneIndices.push_back(envOneTriggerSource->getParameterIndex());  //Parameter Index 6
    envOneIndices.push_back(envOneTrigger->getParameterIndex());        //Parameter Index 7
}

void AkatekoAudioProcessor::initialiseENV2Parameters(){
    addParameter(envTwoEnable = new AudioParameterBool("EV2ENA", "Envelope 2: ", true, "enable"));
    addParameter(envTwoSync = new AudioParameterBool("EV2SYNC", "Envelope 2 Sync: ", false, "sync"));
    addParameter(envTwoDuration = new AudioParameterFloat("EV2DUR", "Envelope 2 Dur: ", 0.f, 1.f, 0.f));
    addParameter(envTwoLoopAmount = new AudioParameterFloat("EV2LAM", "Envelope 2 Loop nr: ", 0.f, 1.f, 1.f));
    addParameter(envTwoLoopDirection = new AudioParameterFloat("EV2LDR", "Envelope 2 loop dir: ", 0, 1, 0));
    addParameter(envTwoSusDirection = new AudioParameterFloat("EV2SDR", "Envelope 2 Sus dir: ", 0, 1, 0));
    addParameter(envTwoTriggerSource = new AudioParameterFloat("EV2TRS", "Envelope 2 T-Source: ", 0, 1, 0));
    addParameter(envTwoTrigger = new AudioParameterBool("EV2TRIG", "Envelope 2 Trigger:", false));

    envTwoParamIds.clear();
    envTwoParamIds.add(envTwoEnable->name);         //Parameter Index 0
    envTwoParamIds.add(envTwoSync->name);           //Parameter Index 1
    envTwoParamIds.add(envTwoDuration->name);       //Parameter Index 2
    envTwoParamIds.add(envTwoLoopAmount->name);     //Parameter Index 3
    envTwoParamIds.add(envTwoLoopDirection->name);  //Parameter Index 4
    envTwoParamIds.add(envTwoSusDirection->name);   //Parameter Index 5
    envTwoParamIds.add(envTwoTriggerSource->name);  //Parameter Index 6
    envTwoParamIds.add(envTwoTrigger->name);        //Parameter Index 7

    //8 Params
    envTwoIndices.clear();
    envTwoIndices.push_back(envTwoEnable->getParameterIndex());         //Parameter Index 0
    envTwoIndices.push_back(envTwoSync->getParameterIndex());           //Parameter Index 1
    envTwoIndices.push_back(envTwoDuration->getParameterIndex());       //Parameter Index 2
    envTwoIndices.push_back(envTwoLoopAmount->getParameterIndex());     //Parameter Index 3
    envTwoIndices.push_back(envTwoLoopDirection->getParameterIndex());  //Parameter Index 4
    envTwoIndices.push_back(envTwoSusDirection->getParameterIndex());   //Parameter Index 5
    envTwoIndices.push_back(envTwoTriggerSource->getParameterIndex());  //Parameter Index 6
    envTwoIndices.push_back(envTwoTrigger->getParameterIndex());        //Parameter Index 7
}

void AkatekoAudioProcessor::initialiseStepSeqParameters(){
    //Enable
    addParameter(stepSeqEnable = new AudioParameterBool("SSQENA", "Step Sequencer: ", false, "enable"));
    addParameter(stepSeqSync = new AudioParameterBool("SSQSYN", "Step Sequencer Sync: ", false, "sync"));
    addParameter(stepSeqChopper = new AudioParameterBool("SSQCHP", "Step Sequencer Chop Curve: ", false, "chopper"));
    addParameter(stepSeqDuration = new AudioParameterFloat("SSQDUR", "Step Sequencer Duration: ", 0.f, 1.0f, 0.f));
    addParameter(stepSeqLength = new AudioParameterFloat("SSQLEN", "Step Sequencer Length: ", NormalisableRange<float>(0.05, 1.0), 1.f, "length"));
    addParameter(stepSeqOffset = new AudioParameterFloat("SSQOFF", "Step Sequencer Off Value: ", NormalisableRange<float>(0.f, 1.f), 0.f, "offValue"));
    addParameter(stepSeqMidPoint = new AudioParameterFloat("SSQEXP", "Step Sequencer Curve: ", NormalisableRange<float>(0.05, 0.95), 0.5f, "curve"));
    addParameter(stepSeqAmount = new AudioParameterFloat("SSQST", "Step Sequencer Steps: ", 0.0, 1.0, 0));

    stepSeqParamIds.clear();
    stepSeqParamIds.add(stepSeqEnable->name);   //Parameter Index 52
    stepSeqParamIds.add(stepSeqSync->name);     //Parameter Index 53
    stepSeqParamIds.add(stepSeqChopper->name);  //Parameter Index 54
    stepSeqParamIds.add(stepSeqDuration->name); //Parameter Index 55
    stepSeqParamIds.add(stepSeqLength->name);   //Parameter Index 56
    stepSeqParamIds.add(stepSeqOffset->name);   //Parameter Index 57
    stepSeqParamIds.add(stepSeqMidPoint->name); //Parameter Index 58
    stepSeqParamIds.add(stepSeqAmount->name);   //Parameter Index 59

    // 8 Params
    sseqIndices.clear();
    sseqIndices.push_back(stepSeqEnable->getParameterIndex());   //Parameter Index 52
    sseqIndices.push_back(stepSeqSync->getParameterIndex());     //Parameter Index 53
    sseqIndices.push_back(stepSeqChopper->getParameterIndex());  //Parameter Index 54
    sseqIndices.push_back(stepSeqDuration->getParameterIndex()); //Parameter Index 55
    sseqIndices.push_back(stepSeqLength->getParameterIndex());   //Parameter Index 56
    sseqIndices.push_back(stepSeqOffset->getParameterIndex());   //Parameter Index 57
    sseqIndices.push_back(stepSeqMidPoint->getParameterIndex()); //Parameter Index 58
    sseqIndices.push_back(stepSeqAmount->getParameterIndex());   //Parameter Index 59
}

void AkatekoAudioProcessor::initialiseXYPadParameters(){
    addParameter(xyPadX = new AudioParameterFloat("XYPADX", "XYPad X-Axis: ", 0.f, 1.f, 0.5f));
    addParameter(xyPadY = new AudioParameterFloat("XYPADY", "XYPad Y-Axis: ", 0.f, 1.f, 0.5f));
    addParameter(xyPadT = new AudioParameterBool("XYPADT", "XYPad Trigger: ", false));

    xyPadParamIds.clear();
    xyPadParamIds.add(xyPadX->name); //Parameter Index
    xyPadParamIds.add(xyPadY->name); //
    xyPadParamIds.add(xyPadT->name);

    // 3 Params

    xyPadIndices.clear();

    xyPadIndices.push_back(xyPadX->getParameterIndex()); //Parameter Index 0
    xyPadIndices.push_back(xyPadY->getParameterIndex()); //Parameter Index 1
    xyPadIndices.push_back(xyPadT->getParameterIndex()); //Parameter Index 2
}

void AkatekoAudioProcessor::initialiseEffectParameters(){
    addParameter(fxEnable = new AudioParameterBool("FXENA", "FX: ", true, "enable"));
    addParameter(fxConfig = new AudioParameterFloat("FXCONF", "FX Config: ", 0.f, 1.0, 0.f));
    addParameter(fxInputAmp = new AudioParameterFloat("FXAMP", "FX Input: ", NormalisableRange<float>(0.f, 2.f), 1.f, "amp"));
    addParameter(fxRouting = new AudioParameterFloat("FXROUT", "FX Routing: ", NormalisableRange<float>(0.f, 1.f), 0.5f, "routing"));

    fxParamIds.clear();
    fxParamIds.add(fxEnable->name);     //Parameter Index 60
    fxParamIds.add(fxConfig->name);     //Parameter Index 61
    fxParamIds.add(fxInputAmp->name);   //Parameter Index 62
    fxParamIds.add(fxRouting->name);    //Parameter Index 63

    //4 Params
    fxIndices.clear();

    fxIndices.push_back(fxEnable->getParameterIndex());     //Parameter Index 60
    fxIndices.push_back(fxConfig->getParameterIndex());     //Parameter Index 61
    fxIndices.push_back(fxInputAmp->getParameterIndex());   //Parameter Index 62
    fxIndices.push_back(fxRouting->getParameterIndex());    //Parameter Index 63
}

void AkatekoAudioProcessor::initialiseStereoDelParameters(){
    addParameter(stereoDelEnable = new AudioParameterBool("SDELENA", "S-Delay: ", true));
    addParameter(stereoDelCrossEnable = new AudioParameterBool("SDELCRS", "S-Delay Cross: ", false));
    addParameter(stereoDelCrossAmount = new AudioParameterFloat("SDELCAM", "S-Delay Cross Amp: ", 0.0, 1.0, 0.f));
    addParameter(stereoDelMix = new AudioParameterFloat("SDELMIX", "S-Delay Mix: ", 0.f, 1.f, 0.f));

    addParameter(stereoDelLeftSync = new AudioParameterBool("SDELLS", "S-Delay L-Sync: ", false));
    addParameter(stereoDelLeft = new AudioParameterFloat("SDELLTIM", "S-Delay L-Delay: ", 0.f, 1.f, 0.f));
    addParameter(stereoDelLeftFB = new AudioParameterFloat("SDELLFB", "S-Delay L-FB: ", NormalisableRange<float>(-1.f, 1.f), 0.f));

    addParameter(stereoDelRightSync = new AudioParameterBool("SDELRS", "S-Delay R-Sync: ", false));
    addParameter(stereoDelRight = new AudioParameterFloat("SDELRTIM", "S-Delay R-Delay: ", 0.f, 1.f, 0.f));
    addParameter(stereoDelRightFB = new AudioParameterFloat("SDELRFB", "S-Delay R-FB: ", NormalisableRange<float>(-1.f, 1.f), 0.f));

    stereoDelParamIds.clear();
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

    stereoDelIndices.clear();
    stereoDelIndices.push_back(stereoDelEnable->getParameterIndex());       //Param index 0
    stereoDelIndices.push_back(stereoDelCrossEnable->getParameterIndex());  //Param index 1
    stereoDelIndices.push_back(stereoDelCrossAmount->getParameterIndex());  //Param index 2
    stereoDelIndices.push_back(stereoDelMix->getParameterIndex());          //Param index 3
    stereoDelIndices.push_back(stereoDelLeftSync->getParameterIndex());     //Param index 4
    stereoDelIndices.push_back(stereoDelLeft->getParameterIndex());         //Param index 5
    stereoDelIndices.push_back(stereoDelLeftFB->getParameterIndex());       //Param index 6
    stereoDelIndices.push_back(stereoDelRightSync->getParameterIndex());    //Param index 7
    stereoDelIndices.push_back(stereoDelRight->getParameterIndex());        //Param index 8
    stereoDelIndices.push_back(stereoDelRightFB->getParameterIndex());      //Param index 9
}

void AkatekoAudioProcessor::initialisePingPongDelay(){
    addParameter(pingPongEnable = new AudioParameterBool("PPENA", "Ping Pong: ", true));
    addParameter(pingPongLeftSync = new AudioParameterBool("PPLSYN", "Ping Pong L-Sync: ", false));
    addParameter(pingPongLeftDelay = new AudioParameterFloat("PPLDEL", "Ping Pong L-Delay: ", 0.f, 1.f, 0.f));
    addParameter(pingPongLeftFB = new AudioParameterFloat("PPLFB", "Ping Pong L-FB: ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(pingPongLeftMix = new AudioParameterFloat("PPLMIX", "Ping Pong L-Mix: ", 0.f, 1.f, 0.f));
    addParameter(pingPongRightSync = new AudioParameterBool("PPRSYN", "Ping Pong R-Sync: ", false));
    addParameter(pingPongRightDelay = new AudioParameterFloat("PPRDEL", "Ping Pong R-Delay: ", 0.f, 1.f, 0.f));
    addParameter(pingPongRightFB = new AudioParameterFloat("PPRFB", "Ping Pong R-FB: ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(pingPongRightMix = new AudioParameterFloat("PPRMIX", "Ping Pong R-Mix: ", 0.f, 1.f, 0.f));

    pingPongParamIds.clear();
    pingPongParamIds.add(pingPongEnable->name);     //Param Index 0
    pingPongParamIds.add(pingPongLeftSync->name);   //Param Index 1
    pingPongParamIds.add(pingPongLeftDelay->name);  //Param Index 2
    pingPongParamIds.add(pingPongLeftFB->name);     //Param Index 3
    pingPongParamIds.add(pingPongLeftMix->name);    //Param Index 4
    pingPongParamIds.add(pingPongRightSync->name);  //Param Index 6
    pingPongParamIds.add(pingPongRightDelay->name); //Param Index 7
    pingPongParamIds.add(pingPongRightFB->name);    //Param Index 8
    pingPongParamIds.add(pingPongRightMix->name);   //Param Index 9

    pingPongIndices.clear();
    pingPongIndices.push_back(pingPongEnable->getParameterIndex());     //Param Index 0
    pingPongIndices.push_back(pingPongLeftSync->getParameterIndex());   //Param Index 1
    pingPongIndices.push_back(pingPongLeftDelay->getParameterIndex());  //Param Index 2
    pingPongIndices.push_back(pingPongLeftFB->getParameterIndex());     //Param Index 3
    pingPongIndices.push_back(pingPongLeftMix->getParameterIndex());    //Param Index 4
    pingPongIndices.push_back(pingPongRightSync->getParameterIndex());  //Param Index 6
    pingPongIndices.push_back(pingPongRightDelay->getParameterIndex()); //Param Index 7
    pingPongIndices.push_back(pingPongRightFB->getParameterIndex());    //Param Index 8
    pingPongIndices.push_back(pingPongRightMix->getParameterIndex());   //Param Index 9
}

void AkatekoAudioProcessor::initialiseLCRDelay(){
    addParameter(LCREnable = new AudioParameterBool("LCRENA", "LCR Delay: ", true));
    addParameter(LCRLeftSync = new AudioParameterBool("LCRLSYN", "LCR Delay L-Sync: ", false));
    addParameter(LCRLeftDelay = new AudioParameterFloat("LCRLDEL", "LCR Delay L-Delay: ", 0.f, 1.f, 0.f));
    addParameter(LCRLeftFB = new AudioParameterFloat("LCRLFB", "LCR Delay L-FB: ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(LCRLeftMix = new AudioParameterFloat("LCRLMIX", "LCR Delay L-Mix: ", 0.f, 1.f, 0.f));

    addParameter(LCRCenterSync = new AudioParameterBool("LCRCSYN", "LCR Delay C-Sync: ", false));
    addParameter(LCRCenterDelay = new AudioParameterFloat("LCRCDEL", "LCR Delay C-Delay: ", 0.f, 1.f, 0.f));
    addParameter(LCRCenterFB = new AudioParameterFloat("LCRCFB", "LCR Delay C-FB: ", NormalisableRange<float>(-1.f, 1.f), 0.f));
    addParameter(LCRhpfEnable = new AudioParameterBool("LCRHPFEN", "LCR Delay HPF: ", false));
    addParameter(LCRhpfFrequency = new AudioParameterFloat("LCRHPFFQ", "LCR Delay HPF F: ", NormalisableRange<float>(20.f, 20000.f), 20.f));
    addParameter(LCRlpfEnable = new AudioParameterBool("LCRLPREN", "LCR Delay LPF: ", false));
    addParameter(LCRlpfFrequency = new AudioParameterFloat("LCRLPFFQ", "LCR Delay LPF F: ", NormalisableRange<float>(20.f, 20000.f), 18000.f));
    addParameter(LCRCenterAmp = new AudioParameterFloat("LCRCAMP", "LCR Delay C-Amp: ", 0.f, 1.f, 0.f));
    addParameter(LCRCenterPan = new AudioParameterFloat("LCRCPAN", "LCR Delay C-Pan: ", 0.f, 1.f, 0.5f));

    addParameter(LCRRightSync = new AudioParameterBool("LCRRSYN", "LCR Delay R-Sync: ", false));
    addParameter(LCRRightDelay = new AudioParameterFloat("LCRRDEL", "LCR Delay R-Delay: ", 0.f, 1.f, 0.f));
    addParameter(LCRRightFB = new AudioParameterFloat("LCRRFB", "LCR Delay R-FB: ", NormalisableRange<float>(-1.f, 1.f),0.f));
    addParameter(LCRRightMix = new AudioParameterFloat("LCRRMIX", "LCR Delay R-Mix: ", 0.f, 1.f, 0.f));

    lcrDelayParamIds.clear();
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
    lcrDelayIndices.clear();
    lcrDelayIndices.push_back(LCREnable->getParameterIndex());          //Param Index  0
    lcrDelayIndices.push_back(LCRLeftSync->getParameterIndex());        //Param Index  1
    lcrDelayIndices.push_back(LCRLeftDelay->getParameterIndex());       //Param Index  2
    lcrDelayIndices.push_back(LCRLeftFB->getParameterIndex());          //Param Index  3
    lcrDelayIndices.push_back(LCRLeftMix->getParameterIndex());         //Param Index  4

    lcrDelayIndices.push_back(LCRCenterSync->getParameterIndex());      //Param Index  5
    lcrDelayIndices.push_back(LCRCenterDelay->getParameterIndex());     //Param Index  6
    lcrDelayIndices.push_back(LCRCenterFB->getParameterIndex());        //Param Index  7
    lcrDelayIndices.push_back(LCRhpfEnable->getParameterIndex());       //Param Index  8
    lcrDelayIndices.push_back(LCRhpfFrequency->getParameterIndex());    //Param Index  9
    lcrDelayIndices.push_back(LCRlpfEnable->getParameterIndex());       //Param Index 10
    lcrDelayIndices.push_back(LCRlpfFrequency->getParameterIndex());    //Param Index 11
    lcrDelayIndices.push_back(LCRCenterAmp->getParameterIndex());       //Param Index 12
    lcrDelayIndices.push_back(LCRCenterPan->getParameterIndex());       //Param Index 13

    lcrDelayIndices.push_back(LCRRightSync->getParameterIndex());       //Param Index 14
    lcrDelayIndices.push_back(LCRRightDelay->getParameterIndex());      //Param Index 15
    lcrDelayIndices.push_back(LCRRightFB->getParameterIndex());         //Param Index 16
    lcrDelayIndices.push_back(LCRRightMix->getParameterIndex());        //Param Index 17
}

void AkatekoAudioProcessor::initialiseStereoFlanger(){
    addParameter(stereoFlangerEnable = new AudioParameterBool("SFLENA", "S-Flanger: ", true));
    addParameter(stereoFlangerCross = new AudioParameterBool("SFLCRSS", "S-Flanger Cross: ", false));
    addParameter(stereoFlangerSync = new AudioParameterBool("SFLSYNC", "S-Flanger Sync: ", false));

    addParameter(stereoFlangerRate = new AudioParameterFloat("SFLRATE", "S-Flanger Rate: ", 0.f, 1.f, 0.f));
    addParameter(stereoFlangerPhase = new AudioParameterFloat("SFLPHASE", "S-Flanger Phase: ", 0.f, 1.f, 0.5f));
    addParameter(stereoFlangerDepth = new AudioParameterFloat("SFLDEPTH", "S-Flanger Depth: ", 0.f, 1.f, 0.f));
    addParameter(stereoFlangerFeedBack = new AudioParameterFloat("SFLFB", "S-Flanger Feedback: ", NormalisableRange<float>(-1.f, 1.f), 0.f));

    addParameter(stereoFlangerWave = new AudioParameterFloat("SFLWAVE", "S-Flanger Wave: ", 0.f, 1.f, 0.f));

    sFlangerParamIds.clear();
    sFlangerParamIds.add(stereoFlangerEnable->name);    // Param Index 0
    sFlangerParamIds.add(stereoFlangerCross->name);     // Param Index 1
    sFlangerParamIds.add(stereoFlangerSync->name);      // Param Index 2
    sFlangerParamIds.add(stereoFlangerRate->name);      // Param Index 3
    sFlangerParamIds.add(stereoFlangerPhase->name);     // Param Index 4
    sFlangerParamIds.add(stereoFlangerDepth->name);     // Param Index 5
    sFlangerParamIds.add(stereoFlangerFeedBack->name);  // Param Index 6
    sFlangerParamIds.add(stereoFlangerWave->name);      // Param Index 7

    sFlangerIndices.clear();
    sFlangerIndices.push_back(stereoFlangerEnable->getParameterIndex());    // Param Index 0
    sFlangerIndices.push_back(stereoFlangerCross->getParameterIndex());     // Param Index 1
    sFlangerIndices.push_back(stereoFlangerSync->getParameterIndex());      // Param Index 2
    sFlangerIndices.push_back(stereoFlangerRate->getParameterIndex());      // Param Index 3
    sFlangerIndices.push_back(stereoFlangerPhase->getParameterIndex());     // Param Index 4
    sFlangerIndices.push_back(stereoFlangerDepth->getParameterIndex());     // Param Index 5
    sFlangerIndices.push_back(stereoFlangerFeedBack->getParameterIndex());  // Param Index 6
    sFlangerIndices.push_back(stereoFlangerWave->getParameterIndex());      // Param Index 7
}

void AkatekoAudioProcessor::initialiseDimensionChorus(){
    addParameter(dChorusEnable = new AudioParameterBool("DCHOENA", "D-Chorus: ", true));
    addParameter(dChorusRate = new AudioParameterFloat("DCHORATE", "D-Chorus Rate:", NormalisableRange<float>(0.02f, 5.f), 0.5f));
    addParameter(dChorusSpread = new AudioParameterFloat("DCHOSPRD", "D-Chorus Spread: ", 0.f, 1.f, 0.f));
    addParameter(dChorusDepth = new AudioParameterFloat("DCHODEPTH", "D-Chorus Depth: ", 0.f, 1.f, 0.f));
    addParameter(dChorusWave = new AudioParameterFloat("DCHOWAVE", "D-Chorus Wave: ", 0.f, 1.0, 0.f));
    addParameter(dChorusFrequency = new AudioParameterFloat("DCHOFREQ", "D-Chorus Frequency: ", NormalisableRange<float>(20.f, 20000.f), 20.f));
    addParameter(dChorusCenterAmp = new AudioParameterFloat("DCHOCM", "D-Chorus C-Amp: ", 0.f, 1.f, 0.5f));
    addParameter(dChorusCenterPan = new AudioParameterFloat("DCHOCP", "D-Chorus C-Pan: ", 0.f, 1.f, 0.f));
    addParameter(dChorusMix = new AudioParameterFloat("DCHOMIX", "D-Chorus Mix: ", 0.f, 1.f, 0.5f));

    dChorusParamIds.clear();
    dChorusParamIds.add(dChorusEnable->name);       // Param Index 0
    dChorusParamIds.add(dChorusRate->name);         // Param Index 1
    dChorusParamIds.add(dChorusSpread->name);       // Param Index 2
    dChorusParamIds.add(dChorusDepth->name);        // Param Index 3
    dChorusParamIds.add(dChorusWave->name);         // Param Index 4
    dChorusParamIds.add(dChorusFrequency->name);    // Param Index 5
    dChorusParamIds.add(dChorusCenterAmp->name);    // Param Index 6
    dChorusParamIds.add(dChorusCenterPan->name);    // Param Index 7
    dChorusParamIds.add(dChorusMix->name);          // Param Index 8

    dChorusIndices.clear();
    dChorusIndices.push_back(dChorusEnable->getParameterIndex());       // Param Index 0
    dChorusIndices.push_back(dChorusRate->getParameterIndex());         // Param Index 1
    dChorusIndices.push_back(dChorusSpread->getParameterIndex());       // Param Index 2
    dChorusIndices.push_back(dChorusDepth->getParameterIndex());        // Param Index 3
    dChorusIndices.push_back(dChorusWave->getParameterIndex());         // Param Index 4
    dChorusIndices.push_back(dChorusFrequency->getParameterIndex());    // Param Index 5
    dChorusIndices.push_back(dChorusCenterAmp->getParameterIndex());    // Param Index 6
    dChorusIndices.push_back(dChorusCenterPan->getParameterIndex());    // Param Index 7
    dChorusIndices.push_back(dChorusMix->getParameterIndex());          // Param Index 8
}

void AkatekoAudioProcessor::initialiseHoldDelay(){
    addParameter(hDelayEnable = new AudioParameterBool("HDELENA", "H-Delay: ", true));
    addParameter(hDelayTrigger = new AudioParameterBool("HDELTRIG", "H-Delay Trigger: ", false));
    addParameter(hDelaySync = new AudioParameterBool("HDELSYN", "H-Delay Beat: ", true));
    addParameter(hDelayLoop = new AudioParameterFloat("HDELLOOP", "H-Delay Loop:  ", 0.f, 1.0f, 500.f));
    addParameter(hDelaySpeed = new AudioParameterFloat("HDELSPD", "H-Delay Speed: ", 0.f, 1.f, 1.0f));

    addParameter(hDelaySmooth = new AudioParameterBool("HDELSMTH", "H-Delay Glide En: ", false));
    addParameter(hDelayGlide = new AudioParameterFloat("HDELGLI", "H-Delay Glide: ", 0.f, 1.f, 4.f));
    addParameter(hDelayDirection = new AudioParameterFloat("HDELDIR", "H-Delay Dir: ", 0.f, 1.f, 0.f));

    addParameter(hDelayGap = new AudioParameterBool("HDELGPOS", "H-Delay Gap: ", false));
    addParameter(hDelayLength = new AudioParameterFloat("HDELLEN", "H-Delay Length: ",  0.f, 1.f, 1.f)); //Make it dependent on length selected

    addParameter(hDelayFade = new AudioParameterFloat("HDELFADE", "H-Delay Fade: ", 0.f, 1.f, 0.f));

    addParameter(hDelayPan = new AudioParameterFloat("HDELPAN", "H-Delay Pan: ", 0.f, 1.f, 0.5f));
    addParameter(hDelayMix= new AudioParameterFloat("HDELMIX", "H-Delay Mix: ", 0.f, 1.0f, 1.0f));
    addParameter(hDelayLatchEnable = new AudioParameterBool("HDELLATEN", "H-Delay Latch En: ", true));
    addParameter(hDelayLatch = new AudioParameterFloat("HDELLATCH", "H-Delay Latch: ", 0.f,  1.0f, 0.f));

    hDelayParamIds.clear();
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
    hDelayParamIds.add(hDelayLatchEnable->name);// Param Index 13
    hDelayParamIds.add(hDelayLatch->name);      // Param Index 14

    // Load up Indices
    hDelayIndices.clear();
    hDelayIndices.push_back(hDelayEnable->getParameterIndex());     // Param Index 0
    hDelayIndices.push_back(hDelayTrigger->getParameterIndex());    // Param Index 1
    hDelayIndices.push_back(hDelaySync->getParameterIndex());       // Param Index 2
    hDelayIndices.push_back(hDelayLoop->getParameterIndex());       // Param Index 3
    hDelayIndices.push_back(hDelaySpeed->getParameterIndex());      // Param Index 4
    hDelayIndices.push_back(hDelaySmooth->getParameterIndex());     // Param Index 5
    hDelayIndices.push_back(hDelayGlide->getParameterIndex());      // Param Index 6
    hDelayIndices.push_back(hDelayDirection->getParameterIndex());  // Param Index 7
    hDelayIndices.push_back(hDelayGap->getParameterIndex());        // Param Index 8
    hDelayIndices.push_back(hDelayLength->getParameterIndex());     // Param Index 9
    hDelayIndices.push_back(hDelayFade->getParameterIndex());       // Param Index 10
    hDelayIndices.push_back(hDelayPan->getParameterIndex());        // Param Index 11
    hDelayIndices.push_back(hDelayMix->getParameterIndex());        // Param Index 12
    hDelayIndices.push_back(hDelayLatchEnable->getParameterIndex());// Param Index 13
    hDelayIndices.push_back(hDelayLatch->getParameterIndex());      // Param Index 14
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
    addParameter(decimatorEnable = new AudioParameterBool("DECENA", "Decimator: ", true));
    addParameter(decimatorReduceEna = new AudioParameterBool("DECRENA", "Decimator Reduce En: ", true));
    addParameter(decimatorSRateEna = new AudioParameterBool("DECSRENA", "Decimator S-Rate En: ", true));
    addParameter(decimatorFilter = new AudioParameterBool("DECFIL", "Decimator Filter: ", true));
    addParameter(decimatorBits = new AudioParameterFloat("DECBIT", "Decimator Reduce: ",  NormalisableRange<float>(0.0f, 0.95f), 0.f));
    addParameter(decimatorSRate = new AudioParameterFloat("DECSRT", "Decimator S-Rate: ", NormalisableRange<float>(0.0f, 0.99f), 0.f));
    addParameter(decimatorMix = new AudioParameterFloat("DECMIX", "Decimator Mix: ", 0.f, 1.f, 0.5f));

    decimatorParamIds.clear();
    decimatorParamIds.add(decimatorEnable->name);       // Param Index 0
    decimatorParamIds.add(decimatorReduceEna->name);    // Param Index 1
    decimatorParamIds.add(decimatorSRateEna->name);     // param Index 2
    decimatorParamIds.add(decimatorFilter->name);       // Param Index 3
    decimatorParamIds.add(decimatorBits->name);         // Param Index 4
    decimatorParamIds.add(decimatorSRate->name);        // Param Index 5
    decimatorParamIds.add(decimatorMix->name);          // Param Index 6
    //7 Params

    // Load Parameter Indices
    decimatorIndices.clear();
    decimatorIndices.push_back(decimatorEnable->getParameterIndex());       // Param Index 0
    decimatorIndices.push_back(decimatorReduceEna->getParameterIndex());    // Param Index 1
    decimatorIndices.push_back(decimatorSRateEna->getParameterIndex());     // param Index 2
    decimatorIndices.push_back(decimatorFilter->getParameterIndex());       // Param Index 3
    decimatorIndices.push_back(decimatorBits->getParameterIndex());         // Param Index 4
    decimatorIndices.push_back(decimatorSRate->getParameterIndex());        // Param Index 5
    decimatorIndices.push_back(decimatorMix->getParameterIndex());          // Param Index 6
}

/*
    AudioParameterBool *pReverbEnable;
    AudioParameterFloat *pReverbDamping;
    AudioParameterFloat *pReverbBandWidth;
    AudioParameterFloat *pReverbDecay;
    AudioParameterFloat *pReverbMix;
 */

void AkatekoAudioProcessor::initialisePlateReverb(){
    addParameter(pReverbEnable = new AudioParameterBool("PREVEN", "Plate Reverb: ", true));
    addParameter(pReverbDamping = new AudioParameterFloat("PREVDAM", "Plate Reverb Damping: ", 0.f, 1.0f, 0.005f));
    addParameter(pReverbBandWidth = new AudioParameterFloat("PREVBW", "Plate Reverb BandWidth: ", 0.f, 1.f, 0.995f));
    addParameter(pReverbDecay = new AudioParameterFloat("PREVDEC", "Plate Reverb Decay:", 0.f, 1.0f, 0.5f));
    addParameter(pReverbMix = new AudioParameterFloat("PREVMIX", "Plate Reverb Mix: ", 0.f, 1.f, 0.5f));

    pReverbParamIds.clear();
    pReverbParamIds.add(pReverbEnable->name);       // Param Index 0
    pReverbParamIds.add(pReverbDamping->name);      // Param Index 1
    pReverbParamIds.add(pReverbBandWidth->name);    // Param Index 2
    pReverbParamIds.add(pReverbDecay->name);        // Param Index 3
    pReverbParamIds.add(pReverbMix->name);          // Param Index 5

    pReverbIndices.clear();
    pReverbIndices.push_back(pReverbEnable->getParameterIndex());       // Param Index 0
    pReverbIndices.push_back(pReverbDamping->getParameterIndex());      // Param Index 1
    pReverbIndices.push_back(pReverbBandWidth->getParameterIndex());    // Param Index 2
    pReverbIndices.push_back(pReverbDecay->getParameterIndex());        // Param Index 3
    pReverbIndices.push_back(pReverbMix->getParameterIndex());          // Param Index 5
}

void AkatekoAudioProcessor::initialiseAudioTrigger(){
    addParameter(aTriggerPreGain = new AudioParameterFloat("ATRIGPG", "Pre gain: ", 0.f, 1.0, 1.0f));
    addParameter(aTriggerThreshold = new AudioParameterFloat("ATRIGTH", "Threshold: ", 0.1f, 0.95f, 0.1f));
    addParameter(aTriggerPreDelay = new AudioParameterFloat("ATRIGPD", "Pre Delay: ", 0.f, 250.f, 0.f));
    addParameter(aTriggerHoldDelay = new AudioParameterFloat("ATRIGHD", "Hold Delay: ", 0.f, 250.f, 0.f));
    addParameter(aTriggerInputSelection = new AudioParameterFloat("ATRIGS", "Input: ", 0, 1, 2));

    aTriggerParamIds.clear();
    aTriggerParamIds.add(aTriggerPreGain->name);
    aTriggerParamIds.add(aTriggerThreshold->name);
    aTriggerParamIds.add(aTriggerPreDelay->name);
    aTriggerParamIds.add(aTriggerHoldDelay->name);
    aTriggerParamIds.add(aTriggerInputSelection->name);

    aTriggerIndices.clear();
    aTriggerIndices.push_back(aTriggerPreGain->getParameterIndex());
    aTriggerIndices.push_back(aTriggerThreshold->getParameterIndex());
    aTriggerIndices.push_back(aTriggerPreDelay->getParameterIndex());
    aTriggerIndices.push_back(aTriggerHoldDelay->getParameterIndex());
    aTriggerIndices.push_back(aTriggerInputSelection->getParameterIndex());
}

void AkatekoAudioProcessor::printPositionInfo(){
    std::cout << "/------CurrentPosition Struct------/" << std::endl;
    std::cout << "/----------------------------------/" << std::endl;

    std::cout << "BPM: " << position.bpm << std::endl;
    std::cout << "Time: " << position.timeSigNumerator << "/" << position.timeSigDenominator << std::endl;
    std::cout << "ppq Position: " << position.ppqPosition << std::endl;
    std::cout << "ppq Start Position: " << position.ppqPositionOfLastBarStart << std::endl;
    std::cout << "TimeInSamples Position: " << position.timeInSamples << std::endl;

    std::cout << "Is Playing: " <<  position.isPlaying << std::endl;
    std::cout << "Is Looping: " << position.isLooping << std::endl; //ignored in Qtractor
    std::cout << "ppqLoop Start: " << position.ppqLoopStart << std::endl; //Available
    std::cout << "ppqLoop End: " << position.ppqLoopEnd << std::endl;     //Available
}

void AkatekoAudioProcessor::printRegisteredParameters(){
    std::cout << "Printing Parameters" << std::endl;

    const OwnedArray<AudioProcessorParameter> &params = getParameters();

    for(int i=0; i<params.size(); i++){
        if(AudioProcessorParameterWithID *p = dynamic_cast<AudioProcessorParameterWithID*>(params.getUnchecked(i))){
            std::cout << "Parameter Index [" << i << "]: " << p->name << std::endl;
        }
    }
}

