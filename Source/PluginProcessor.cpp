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
    lfoOne(LowFrequencyTableOsc(modMatrix.getWriteRegister(MODSRC::LFO1))),
    lfoTwo(LowFrequencyTableOsc(modMatrix.getWriteRegister(MODSRC::LFO2))),
    envOne(EnvelopeGenerator(modMatrix.getWriteRegister(MODSRC::ENV1))),
    envTwo(EnvelopeGenerator(modMatrix.getWriteRegister(MODSRC::ENV2))),
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
    initialiseParameters();
}

AkatekoAudioProcessor::~AkatekoAudioProcessor()
{
    voices.clear(true);
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

    if(inputChannels != voices.size()){
        voices.clear(true);

        for(int i=0; i<inputChannels; i++){
            voices.add(new AkatekoVoice(modMatrix));
            voices[0]->setSampleRate(sRate);
        }
    }

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

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numberSamples = buffer.getNumSamples();

    /* Gather all Signal Parameters */
    const bool fDriveEnabled = *preAmpEnable;
    const double fOneDrive = *preAmpDrive;
    const double fOneAmount = *preAmpVolume;

    const double fOneFreq = *filterOneFrequency;
    const double fOneReso = *filterOneResonance;
    const double fTwoFreq = *filterTwoFrequency;
    const double fTwoReso = *filterTwoResonance;

    const double wsDrive = *waveShaperDrive;
    const double wsAmp = *waveShaperAmp;
    const double wsRouting = *waveShaperRouting;

    /* Update Voices */
    for(int i=0; i<voices.size(); i++){
        voices[i]->setFilterOneFrequency(fOneFreq);
        voices[i]->setFilterOneResonance(fOneReso);

        voices[i]->setFilterTwoFrequency(fTwoFreq);
        voices[i]->setFilterTwoResonance(fTwoReso);

        voices[i]->setWaveShaperDrive(wsDrive);
        voices[i]->setWaveShaperMix(wsAmp);
    }

    /* Gather all Modulation Parameters */
    const double lfoOneFreq = *lfoOneFrequency;
    const double lfoTwoFreq = *lfoTwoFrequency;

    const double envOneDur = *envOneDuration;
    const double envTwoDur = *envTwoDuration;

    lfoOne.setFrequency(lfoOneFreq);
    lfoTwo.setFrequency(lfoTwoFreq);

    envOne.setDuration(envOneDur);
    envTwo.setDuration(envTwoDur);

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    //for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
        if(totalNumInputChannels == 2){
            float *leftChannel = buffer.getWritePointer(0);
            float *rightChannel = buffer.getWritePointer(1);

            voices[0]->update();
            voices[1]->update();

            for(int i=0; i<numberSamples; i++){
                voices[0]->getSample(&leftChannel[i]);
                voices[1]->getSample(&rightChannel[i]);
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
void AkatekoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AkatekoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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

    for(int i=0; i<voices.size(); i++){
        voices[i]->enableFilters(enable);
    }
}

void AkatekoAudioProcessor::updateFilterConfiguration(){
    const int config = *filterConfig;

    for(int i=0; i<voices.size(); i++){
        voices[i]->filterConfiguration(config);
    }
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
            for(int i=0; i<voices.size(); i++){
                voices[i]->setFilterOneType(type);
            }
        } else if(filter == 2){
            for(int i=0; i<voices.size(); i++){
                voices[i]->setFilterTwoType(type);
            }
        }
   }
}

void AkatekoAudioProcessor::updateFilterOneEnabled(){
    const bool enable = *filterOneEnable;

    for(int i=0; i<voices.size(); i++){
        voices[i]->enableFilterOne(enable);
    }
}

void AkatekoAudioProcessor::updateFilterTwoEnabled(){
    const bool enable = *filterTwoEnable;

    for(int i=0; i<voices.size(); i++){
        voices[i]->enableFilterTwo(enable);
    }
}
//==============================================================================
//WaveShaper
//==============================================================================
void AkatekoAudioProcessor::enableWaveShaper(bool enable){
    for(int i=0; i<voices.size(); i++){
        voices[i]->enableWaveShaper(enable);
    }
}

void AkatekoAudioProcessor::setWaveShaperBuffer(msmBuffer &buffer){
    for(int i=0; i<voices.size(); i++){
        voices[i]->setWaveShaperBuffer(buffer);
    }
}
//=============================================================================
// Envelope One
//=============================================================================
void AkatekoAudioProcessor::triggerEnvelopeOne(){
    envOne.triggerEnvelope();
}

void AkatekoAudioProcessor::releaseEnvelopeOne(){
    envOne.releaseEnvelope();
}

void AkatekoAudioProcessor::setEnvelopeOneBuffer(msmBuffer &buffer){
    envOne.setBuffer(buffer);
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
    envTwo.triggerEnvelope();
}

void AkatekoAudioProcessor::releaseEnvelopeTwo(){
    envTwo.releaseEnvelope();
}

void AkatekoAudioProcessor::setEnvelopeTwoBuffer(msmBuffer &buffer){
    envTwo.setBuffer(buffer);
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
    const int sync = *envTwoSync;

    /* Calculate envelope duration based on the duration */
}
//=============================================================================
// Low Frequency Oscillator One
//=============================================================================
void AkatekoAudioProcessor::setLowFreqOscOneShot(bool enabled){

}
void AkatekoAudioProcessor::setLowFreqOscOneSync(bool enabled){

}

void AkatekoAudioProcessor::setLowFreqOscOneBuffer(msmBuffer &buffer){
    lfoOne.setBuffer(buffer);
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

void AkatekoAudioProcessor::setLowFreqOscTwoBuffer(msmBuffer &buffer){
    lfoTwo.setBuffer(buffer);
}

void AkatekoAudioProcessor::resetLowFreqOscTwo(){
    lfoTwo.reset();
}
//=============================================================================
// Step Sequencer Enabled
//=============================================================================
void AkatekoAudioProcessor::setStepMidPoint(){
    const double mp = *stepSeqMidPoint;

    //std::cout << "midPoint : " << mp << std::endl;

    if(mp >= 0 && mp <= 1.0){
        stepSeq.setMidPoint(mp);
    } else {
        stepSeq.setMidPoint(0.5);
    }
    stepSeq.calculateCurves();
}

void AkatekoAudioProcessor::setChopCurve(){
    const bool enableChopper = *stepSeqChopper;
    stepSeq.setCurveChopper(enableChopper);
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

void AkatekoAudioProcessor::resetLfo(){
    //lfoOne.reset();
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
        case PreAmpId:
            tmpIndices = preAmpIndices;
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
    }

    return tmpIndices;
}

StringArray AkatekoAudioProcessor::getParameterIds(UserInterfaceId id){
    StringArray tmpParamId;

    switch(id){
        case GlobalId:
            tmpParamId = globalParamIds;
            break;
        case PreAmpId:
            tmpParamId = preAmpParamIds;
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
    }

    return result;
}

void AkatekoAudioProcessor::initialiseParameters(){
    initialiseGlobalParameters();
    initialisePreAmpParameters();
    initialiseFilterParameters();
    initialiseWaveShapeParameters();
    initialiseLFO1Parameters();
    initialiseLFO2Parameters();
    initialiseENV1Parameters();
    initialiseENV2Parameters();
    initialiseStepSeqParameters();
}

void AkatekoAudioProcessor::initialiseGlobalParameters(){
    addParameter(globalBypass = new AudioParameterBool("GLBP","Global Bypas : ",false));
    addParameter(globalVolume = new AudioParameterFloat("GLVL", "Global Volume : ", 0.f, 1.f, 1.0));

    globalParamIds.add(globalBypass->name); //Parameter Index 0
    globalParamIds.add(globalVolume->name); //Parameter Index 1

    globalIndices.push_back(0);
    globalIndices.push_back(1);
}

void AkatekoAudioProcessor::initialisePreAmpParameters(){
    addParameter(preAmpEnable = new AudioParameterBool("PREN", "PreAmp : ",true));
    addParameter(preAmpDrive = new AudioParameterFloat("PRDR", "Drive : ", 0.f, 1.f, 0.f));
    addParameter(preAmpVolume = new AudioParameterFloat("PRVL", "PassBand : ", NormalisableRange<float>(0,1), 0.5f));

    preAmpParamIds.add(preAmpEnable->name); //Parameter Index 2
    preAmpParamIds.add(preAmpDrive->name);  //Parameter Index 3
    preAmpParamIds.add(preAmpVolume->name); //Parameter Index 4


    for(int i=2; i<5; i++){
        preAmpIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseFilterParameters(){
    addParameter(filterEnable = new AudioParameterBool("FLEN","Filters : ", true));
    addParameter(filterConfig = new AudioParameterInt("FLCF", "Filter config : ",0, 1, 0));
    addParameter(filterOneEnable = new AudioParameterBool("F1EN", "F1 : ", true));
    addParameter(filterOneType = new AudioParameterFloat("F1TP", "F1 Type : ",  0, 1, 0.));
    addParameter(filterOneRollOff = new AudioParameterInt("F1RF", "F1 RollOff : ",0, 1, 0));
    addParameter(filterOneFrequency = new AudioParameterFloat("F1FQ","F1 frequency : ", NormalisableRange<float>(80.f, 18000.f), 320.f));
    addParameter(filterOneResonance = new AudioParameterFloat("F1RE", "F1 Resonance : ", 0.f,  1.f, 0.f));
    addParameter(filterTwoEnable = new AudioParameterBool("F2EN", "F2 Enable : ", true));
    addParameter(filterTwoType = new AudioParameterFloat("F2TP", "F2 Type : ", 0,  1, 1));
    addParameter(filterTwoRollOff = new AudioParameterInt("F2RF","F2 RollOff : ", 0, 1, 0));
    addParameter(filterTwoFrequency = new AudioParameterFloat("F2FQ", "F2 Frequency : ", NormalisableRange<float>(80.f, 18000.f), 13500.f));
    addParameter(filterTwoResonance = new AudioParameterFloat("F2RE", "F2 Resonance : ", 0.f,1.f, 0.f));

    filterParamIds.add(filterEnable->name);         //Parameter Index  5
    filterParamIds.add(filterConfig->name);         //Parameter Index  6
    filterParamIds.add(filterOneEnable->name);      //Parameter Index  7
    filterParamIds.add(filterOneType->name);        //Parameter Index  8
    filterParamIds.add(filterOneRollOff->name);     //Parameter Index  9
    filterParamIds.add(filterOneFrequency->name);   //Parameter Index 10
    filterParamIds.add(filterOneResonance->name);   //Parameter Index 11
    filterParamIds.add(filterTwoEnable->name);      //Parameter Index 12
    filterParamIds.add(filterTwoType->name);        //Parameter Index 13
    filterParamIds.add(filterTwoRollOff->name);     //Parameter Index 14
    filterParamIds.add(filterTwoFrequency->name);   //Parameter Index 15
    filterParamIds.add(filterTwoResonance->name);   //Parameter Index 16

    for(int i=5; i<17; i++){
        filterIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseWaveShapeParameters(){
    addParameter(waveShaperEnable = new AudioParameterBool("WSEN", "WaveShaper : ", true, "enable"));
    addParameter(waveShaperDrive = new AudioParameterFloat("WSDRI", "WS Drive : ", NormalisableRange<float>(0.f, 1.f), 0.5, "drive"));
    addParameter(waveShaperMix = new AudioParameterFloat("WSMIX", "WS Mix : ", NormalisableRange<float>(0.f, 1.f), 0.f, "mix"));
    addParameter(waveShaperAmp = new AudioParameterFloat("WSAMP", "WS Amplitude : ", NormalisableRange<float>(0.f, 2.f), 0.f, "amp"));
    addParameter(waveShaperRouting = new AudioParameterFloat("WROUT", "WS Routing : ", NormalisableRange<float>(-1.f, 1.f), 0.5f, "routing"));
    addParameter(waveShaperShape = new AudioParameterFloat("WSHAPE", "WS Shape : ", NormalisableRange<float>(0.f, 1.f), 0.f, "shape"));

    waveShapeParamIds.add(waveShaperEnable->name);  //Parameter Index 17
    waveShapeParamIds.add(waveShaperDrive->name);   //Parameter Index 18
    waveShapeParamIds.add(waveShaperMix->name);     //Parameter Index 19
    waveShapeParamIds.add(waveShaperAmp->name);     //Parameter Index 20
    waveShapeParamIds.add(waveShaperRouting->name); //Parameter Index 21
    waveShapeParamIds.add(waveShaperShape->name);   //Parameter Index 22

    for(int i=17; i<23; i++){
        wsIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseLFO1Parameters(){
    addParameter(lfoOneEnable = new AudioParameterBool("LF1EN", "LFO 1 : ", true, "enable"));
    addParameter(lfoOneShot = new AudioParameterBool("LF1SH", "LFO 1 One shot : ", true, "one-shot"));
    addParameter(lfoOneSync = new AudioParameterBool("LF1SY", "LFO 1 Sync : ", true, "sync"));
    addParameter(lfoOneFrequency = new AudioParameterFloat("LF1F", "LFO 1 Frequency : ", NormalisableRange<float>(0.001f, 20.f), 0.f, "frequency"));
    addParameter(lfoOnePhase = new AudioParameterFloat("LF1P", "LFO 1 Phase : ",NormalisableRange<float>(0.f, 1.f), 0.f, "phase"));
    addParameter(lfoOnePWM = new AudioParameterFloat("LF1PW", "LFO 1 PWM : ",NormalisableRange<float>(-0.95f, 0.95f), 0.f, "pwm"));
    addParameter(lfoOneShape = new AudioParameterFloat("L1SHAPE", "LFO 1 Shape : ", NormalisableRange<float>(0.f, 1.f), 0.f, "shape"));

    lfoOneParamIds.add(lfoOneEnable->name);     //Parameter Index 23
    lfoOneParamIds.add(lfoOneShot->name);       //Parameter Index 24
    lfoOneParamIds.add(lfoOneSync->name);       //Parameter Index 25
    lfoOneParamIds.add(lfoOneFrequency->name);  //Parameter Index 26
    lfoOneParamIds.add(lfoOnePhase->name);      //Parameter Index 27
    lfoOneParamIds.add(lfoOnePWM->name);        //Parameter Index 28
    lfoOneParamIds.add(lfoOneShape->name);      //Parameter Index 29

    for(int i=23; i<30; i++){
        lfoOneIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseLFO2Parameters(){
    addParameter(lfoTwoEnable = new AudioParameterBool("LF2EN", "LFO 2 : ", true, "enable"));
    addParameter(lfoTwoShot = new AudioParameterBool("LF2SH", "LFO 2 One Shot L: ", true, "one-shot"));
    addParameter(lfoTwoSync = new AudioParameterBool("LF2SY", "LFO 2 Sync : ", true, "sync"));
    addParameter(lfoTwoFrequency = new AudioParameterFloat("LF2F", "LFO 2 Frequency : ", NormalisableRange<float>(0.001f, 20.f), 0.f, "frequency"));
    addParameter(lfoTwoPhase = new AudioParameterFloat("LF2P", "LFO 2 Phase : ", NormalisableRange<float>(0.f, 1.f), 0.f, "phase"));
    addParameter(lfoTwoPWM = new AudioParameterFloat("LF2PW", "LFO 2 PWM : ", NormalisableRange<float>(-0.95f, 0.95f), 0.f, "pwm"));
    addParameter(lfoTwoShape = new AudioParameterFloat("L2SHAPE", "LFO 2 Shape : ", NormalisableRange<float>(0.f, 1.f), 0.f , "shape"));

    lfoTwoParamIds.add(lfoTwoEnable->name);     //Parameter Index 30
    lfoTwoParamIds.add(lfoTwoShot->name);       //Parameter Index 31
    lfoTwoParamIds.add(lfoTwoSync->name);       //Parameter Index 32
    lfoTwoParamIds.add(lfoTwoFrequency->name);  //Parameter Index 33
    lfoTwoParamIds.add(lfoTwoPhase->name);      //Parameter Index 34
    lfoTwoParamIds.add(lfoTwoPWM->name);        //Parameter Index 35
    lfoTwoParamIds.add(lfoTwoShape->name);      //Parameter Index 36

    for(int i=30; i<37; i++){
        lfoTwoIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseENV1Parameters(){
    addParameter(envOneEnable = new AudioParameterBool("EV1ENA", "Env 1 : ", false, "enable"));
    addParameter(envOneSync = new AudioParameterBool("EV1SYNC", "Env 1 Sync : ", false, "sync"));
    addParameter(envOneDuration = new AudioParameterFloat("EV1DUR", "Env 1 Dur : ", NormalisableRange<float>(50.f, 5000.f),0, "duration"));
    addParameter(envOneLoopAmount = new AudioParameterFloat("EV1LAM", "Env 1 Loop nr : ", NormalisableRange<float>(1.f, 32.f), 0.f, "loop-nr"));
    addParameter(envOneLoopDirection = new AudioParameterFloat("EV1LDR", "Env 1 Loop dir : ", NormalisableRange<float>(0, 1), 0, "loop-dir"));
    addParameter(envOneSusDirection = new AudioParameterFloat("EV1SDR", "Env 1 Sus dir : ", NormalisableRange<float>(0, 1), 0, "sus-dir"));
    addParameter(envOneTriggerSource = new AudioParameterFloat("EV1TRS", "Env 1 Trigger : ", NormalisableRange<float>(0, 1), 0, "tr-src"));

    envOneParamIds.add(envOneEnable->name);         //Parameter Index 37
    envOneParamIds.add(envOneSync->name);           //Parameter Index 38
    envOneParamIds.add(envOneDuration->name);       //Parameter Index 39
    envOneParamIds.add(envOneLoopAmount->name);     //Parameter Index 40
    envOneParamIds.add(envOneLoopDirection->name);  //Parameter Index 41
    envOneParamIds.add(envOneSusDirection->name);   //Parameter Index 42
    envOneParamIds.add(envOneTriggerSource->name);  //Parameter Index 43

    for(int i=37; i<44; i++){
        envOneIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseENV2Parameters(){
    addParameter(envTwoEnable = new AudioParameterBool("EV2ENA", "Env 2 : ", false, "enable"));
    addParameter(envTwoSync = new AudioParameterBool("EV2SYNC", "Env 2 Sync : ", false, "sync"));
    addParameter(envTwoDuration = new AudioParameterFloat("EV2DUR", "Env 2 Dur : ", NormalisableRange<float>(50.f, 5000.f), 0.f, "duration"));
    addParameter(envTwoLoopAmount = new AudioParameterFloat("EV2LAM", "Env 2 Loop nr : ", NormalisableRange<float>(1.f, 32.f), 0.f, "loop-nr"));
    addParameter(envTwoLoopDirection = new AudioParameterFloat("EV2LDR", "Env 2 loop dir : ",NormalisableRange<float>(0, 1), 0, "loop-dir"));
    addParameter(envTwoSusDirection = new AudioParameterFloat("EV2SDR", "Env 2 Sus dir : ", NormalisableRange<float>(0, 1), 0, "sus-dir"));
    addParameter(envTwoTriggerSource = new AudioParameterFloat("EV2TRS", "Env 2 Trigger : ", NormalisableRange<float>(0, 1) , 0, "tr-src"));

    envTwoParamIds.add(envTwoEnable->name);         //Parameter Index 44
    envTwoParamIds.add(envTwoSync->name);           //Parameter Index 45
    envTwoParamIds.add(envTwoDuration->name);       //Parameter Index 46
    envTwoParamIds.add(envTwoLoopAmount->name);     //Parameter Index 47
    envTwoParamIds.add(envTwoLoopDirection->name);  //Parameter Index 48
    envTwoParamIds.add(envTwoSusDirection->name);   //Parameter Index 49
    envTwoParamIds.add(envTwoTriggerSource->name);  //Parameter Index 50

    for(int i=44; i<51; i++){
        envTwoIndices.push_back(i);
    }
}

void AkatekoAudioProcessor::initialiseStepSeqParameters(){
    //Enable
    addParameter(stepSeqEnable = new AudioParameterBool("SSQENA", "Step seq : ", false, "enable"));
    addParameter(stepSeqSync = new AudioParameterBool("SSQSYN", "Step seq sync : ", false, "sync"));
    addParameter(stepSeqChopper = new AudioParameterBool("SSQCHP", "Step seq chop curve : ", false, "chopper"));
    addParameter(stepSeqDuration = new AudioParameterFloat("SSQDUR", "Step seq duration : ", NormalisableRange<float>(100.0, 3000.0), 0.f, "duration"));
    addParameter(stepSeqLength = new AudioParameterFloat("SSQLEN", "Step seq length : ", NormalisableRange<float>(0.05, 1.0), 1.f, "length"));
    addParameter(stepSeqOffset = new AudioParameterFloat("SSQOFF", "Step seq off-value : ", NormalisableRange<float>(0.f, 1.f), 0.f, "offValue"));
    addParameter(stepSeqMidPoint = new AudioParameterFloat("SSQEXP", "Step seq curve : ", NormalisableRange<float>(0.05, 0.95), 0.5f, "curve"));
    addParameter(stepSeqAmount = new AudioParameterFloat("SSQST", "Step seq steps : ", NormalisableRange<float>(0.0, 1.0), 0, "amount"));

    stepSeqParamIds.add(stepSeqEnable->name);
    stepSeqParamIds.add(stepSeqSync->name);
    stepSeqParamIds.add(stepSeqChopper->name);
    stepSeqParamIds.add(stepSeqDuration->name);
    stepSeqParamIds.add(stepSeqLength->name);
    stepSeqParamIds.add(stepSeqOffset->name);
    stepSeqParamIds.add(stepSeqMidPoint->name);
    stepSeqParamIds.add(stepSeqAmount->name);

    for(int i=51; i<59; i++){
        sseqIndices.push_back(i);
    }
}
