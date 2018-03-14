/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "/work/programming-projects/msm/msm-dsp/Filters/VirtualAnalogOnePole.h"
#include "/work/programming-projects/msm/msm-dsp/Filters/ChebyLP.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/LowFrequencyTableOsc.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/EnvelopeGenerator.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/StepSeqGenerator.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/WaveShaper.h"
#include "/work/programming-projects/msm/msm-dsp/Filters/MoogLadderFilter.h"

#include "AkatekoVoice.h"
#include "AkatekoMatrix.h"
#include <vector>

using std::vector;
//==============================================================================
/**
*/
class AkatekoAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AkatekoAudioProcessor();
    ~AkatekoAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    enum UserInterfaceId {
        GlobalId,
        PreAmpId,
        FilterId,
        WaveShapeId,
        LFO1Id,
        LFO2Id,
        Envelope1Id,
        Envelope2Id,
        StepSequencerId,
    };

    bool shapesInitialised();
    void setShapesInitialised(bool value);

    void resetLfo(); //reset phase
    void setLfoBuffer(msmBuffer buffer, int lfo);

    // Filter Configuration
    void updateFiltersEnabled();
    void updateFilterConfiguration();
    void updateFilterType(int filter);

    void updateFilterOneEnabled();
    void updateFilterTwoEnabled();

    //WaveShaper
    void enableWaveShaper(bool enable);
    void setWaveShaperBuffer(msmBuffer &buffer);

    /* New functions */
    void triggerEnvelopeOne();
    void releaseEnvelopeOne();
    void setEnvelopeOneBuffer(msmBuffer &buffer);

    void setEnvelopeOneLoopEnabled(bool enabled);
    void setEnvelopeOneLoopPoints(double startPos, double endPos);
    void updateEnvelopeOneLoopDirection();
    void setEnvelopeOneSustainEnabled(bool enabled);
    void setEnvelopeOneSustainPoints(double startPos, double endPos);
    void updateEnvelopeOneSustainDirection();
    void setEnvelopeOneReleasePoint(double startPos, double endPos);
    void updateEnvelopeOneSyncSource();

    // Envelope Two
    void triggerEnvelopeTwo();
    void releaseEnvelopeTwo();
    void setEnvelopeTwoBuffer(msmBuffer &buffer);

    void setEnvelopeTwoLoopEnabled(bool enabled);
    void setEnvelopeTwoLoopPoints(double startPos, double endPos);
    void updateEnvelopeTwoLoopDirection();
    void setEnvelopeTwoSustainEnabled(bool enabled);
    void setEnvelopeTwoSustainPoints(double startPos, double endPos);
    void updateEnvelopeTwoSustainDirection();
    void setEnvelopeTwoReleasePoint(double startPos, double endPos);
    void updateEnvelopeTwoSyncSource();
    // Low Frequency Osc One

    // LFO One
    void setLowFreqOscOneShot(bool enabled);
    void setLowFreqOscOneSync(bool enabled);
    void setLowFreqOscOneBuffer(msmBuffer &buffer);
    void resetLowFreqOscOne();

    // LFO Two
    void setLowFreqOscTwoShot(bool enabled);
    void setLowFreqOscTwoSync(bool enabled);
    void setLowFreqOscTwoBuffer(msmBuffer &buffer);
    void resetLowFreqOscTwo();

    void triggerEnvelope(int env);
    void releaseEnvelope(int env);
    void setEnvelopeLoopDirection(int env);
    void setEnvelopeSustainDirection(int env);
    void setEnvBuffer(msmBuffer buffer, int env);

    // Step Sequencer
    void enableStepSeq();
    void setStepMidPoint();
    void setChopCurve();
    void setStepValues(vector<double> values);
    void setButtonStates(vector<int> states);
    void setStepSeq(vector<double> values, vector<int>states);

    std::vector<int> getParameterIndices(UserInterfaceId id); //return indices

    StringArray getParameterIds(UserInterfaceId id);
    void setUIState(const String state, UserInterfaceId id);
    String getUIState(UserInterfaceId id);

    void setShapes(StringArray shapes, UserInterfaceId id);
    void updateShape(String shape, UserInterfaceId id);
    StringArray getShapes(UserInterfaceId id);

    AkatekoMatrix &getModMatrix();
private:
    //==============================================================================
    void initialiseParameters();
    void initialiseGlobalParameters();
    void initialisePreAmpParameters();
    void initialiseFilterParameters();
    void initialiseWaveShapeParameters();
    void initialiseLFO1Parameters();
    void initialiseLFO2Parameters();
    void initialiseENV1Parameters();
    void initialiseENV2Parameters();
    void initialiseStepSeqParameters();

    //AkatekoGlobal akatekoGlobal;
    /* Mod Matrix */
    AkatekoMatrix modMatrix;
    OwnedArray<AkatekoVoice> voices;

    //Modulators
    LowFrequencyTableOsc lfoOne;
    LowFrequencyTableOsc lfoTwo;
    EnvelopeGenerator envOne;
    EnvelopeGenerator envTwo;
    StepSeqGenerator stepSeq;

    /* are constructed before editor is created */

    /* Parameter ID's phase out later
     * Set them Directly via the hash map
     */


    // Global Section
    AudioParameterBool *globalBypass;
    AudioParameterFloat *globalVolume;
    // PreAmp Section
    AudioParameterBool *preAmpEnable;
    AudioParameterFloat *preAmpVolume;
    AudioParameterFloat *preAmpDrive;
    // Filter Section
    AudioParameterBool *filterEnable;
    AudioParameterInt *filterConfig;
    // Filter one
    AudioParameterBool *filterOneEnable;
    AudioParameterFloat *filterOneType;
    AudioParameterInt * filterOneRollOff;
    AudioParameterFloat *filterOneFrequency;
    AudioParameterFloat *filterOneResonance;
    // Filter Two
    AudioParameterBool *filterTwoEnable;
    AudioParameterFloat *filterTwoType;
    AudioParameterInt *filterTwoRollOff;
    AudioParameterFloat *filterTwoFrequency;
    AudioParameterFloat *filterTwoResonance;
    // WaveShaper
    AudioParameterBool *waveShaperEnable;
    AudioParameterFloat *waveShaperDrive;
    AudioParameterFloat *waveShaperMix;
    AudioParameterFloat *waveShaperAmp;
    AudioParameterFloat *waveShaperRouting;
    AudioParameterFloat *waveShaperShape;
    // LFO One
    AudioParameterBool *lfoOneEnable;
    AudioParameterBool *lfoOneShot;
    AudioParameterBool *lfoOneSync;

    AudioParameterFloat *lfoOneFrequency;
    AudioParameterFloat *lfoOnePhase;
    AudioParameterFloat *lfoOnePWM;
    AudioParameterFloat *lfoOneShape;
    // LFO Two
    AudioParameterBool *lfoTwoEnable;
    AudioParameterBool *lfoTwoShot;
    AudioParameterBool *lfoTwoSync;

    AudioParameterFloat *lfoTwoFrequency;
    AudioParameterFloat *lfoTwoPhase;
    AudioParameterFloat *lfoTwoPWM;
    AudioParameterFloat * lfoTwoShape;
    // Env One
    AudioParameterBool *envOneEnable;
    AudioParameterBool *envOneSync; //divide duration time so it concides with the beat
    AudioParameterFloat *envOneDuration;
    AudioParameterFloat *envOneLoopAmount;
    AudioParameterFloat *envOneLoopDirection;
    AudioParameterFloat *envOneSusDirection;
    AudioParameterFloat *envOneTriggerSource; //Automatically create entry mod matrix
    // Env Two
    AudioParameterBool *envTwoEnable;
    AudioParameterBool *envTwoSync;
    AudioParameterFloat *envTwoDuration;
    AudioParameterFloat *envTwoLoopAmount;
    AudioParameterFloat *envTwoLoopDirection;
    AudioParameterFloat *envTwoSusDirection;
    AudioParameterFloat *envTwoTriggerSource;
    // Step sequencer
    AudioParameterBool *stepSeqEnable;
    AudioParameterBool *stepSeqSync;
    AudioParameterBool *stepSeqChopper;

    AudioParameterFloat *stepSeqDuration;
    AudioParameterFloat *stepSeqLength;
    AudioParameterFloat *stepSeqOffset;
    AudioParameterFloat *stepSeqMidPoint;
    AudioParameterFloat *stepSeqAmount;

    //Parameter Indices
    std::vector<int> globalIndices;
    std::vector<int> preAmpIndices;
    std::vector<int> filterIndices;
    std::vector<int> wsIndices;
    std::vector<int> lfoOneIndices;
    std::vector<int> lfoTwoIndices;
    std::vector<int> envOneIndices;
    std::vector<int> envTwoIndices;
    std::vector<int> sseqIndices;


    // Parameter Id's
    StringArray globalParamIds;
    StringArray preAmpParamIds;
    StringArray filterParamIds;
    StringArray waveShapeParamIds;
    StringArray lfoOneParamIds;
    StringArray lfoTwoParamIds;
    StringArray envOneParamIds;
    StringArray envTwoParamIds;
    StringArray stepSeqParamIds;

    /* Gui representations */
    bool shapesAreInitialised;

    String filterState;
    String wsString;
    String envOneState;
    String envTwoState;
    String lfoOneState;
    String lfoTwoState;
    String stepSeqState;

    StringArray lfoOneShapes;
    StringArray lfoTwoShapes;
    StringArray waveShapes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AkatekoAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
