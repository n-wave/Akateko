/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-utilities/ConfigurationFileManager.h"

#include "/work/programming-projects/msm/msm-dsp/Common/StereoProcessor.h"
#include "/work/programming-projects/msm/msm-dsp/Filters/VirtualAnalogOnePole.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/LowFrequencyTableOsc.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/EnvelopeGenerator.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/StepSeqGenerator.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/WaveShaper.h"
#include "/work/programming-projects/msm/msm-dsp/Filters/MoogLadderFilter.h"

#include "/work/programming-projects/msm/msm-dsp/Processors/StereoDelay.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/PingPongDelay.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/LeftCenterRightDelay.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/StereoFlanger.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/DimensionChorus.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/HoldDelay.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/StereoDecimator.h"
#include "/work/programming-projects/msm/msm-dsp/Processors/PlateReverb.h"

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


    // Preset Handling
    bool save();
    bool save(akateko::PresetRow row);
    bool saveAs(akateko::PresetRow row);
    bool clear(akateko::PresetRow row);

    bool folder(); //Select folder on completion rescan for presets

    void load();
    void load(File file);



    /* Save/Change preset supplied as coresponding with the file*/
    bool changeName(const File file, const String name);
    bool changeAuthor(const File file, const String author);
    bool changeCategory(const File file, const String category);


    // Get current Presets in selected directory
    String getCurrentPresetFileName();
    String getCurrentPresetName();

    std::vector<akateko::PresetRow> getCurrentPresets();



    enum UserInterfaceId {
        GlobalId,
        FilterId,
        WaveShapeId,
        LFO1Id,
        LFO2Id,
        Envelope1Id,
        Envelope2Id,
        StepSequencerId,
        xyPadId,
        FxContainerId,
        StereoDelayId,
        PingPongDelayId,
        LCRDelayId,
        StereoflangerId,
        DChorusId,
        hDelayId,
        decimatorId,
        pReverbId
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
    void enableWaveShaper();
    void setWaveShaperBuffer(msmBuffer &buffer);

    /* New functions */
    void triggerEnvelopeOne();
    void releaseEnvelopeOne();
    void setEnvelopeOneBuffer(msmBuffer &buffer);
    void setEnvelopeOneDurationBounds(double min, double max);

    void setEnvelopeOneLoopEnabled(bool enabled);
    void setEnvelopeOneLoopPoints(double startPos, double endPos);
    void updateEnvelopeOneLoopAmount();
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
    void setEnvelopeTwoDurationBounds(double min, double max);

    void setEnvelopeTwoLoopEnabled(bool enabled);
    void setEnvelopeTwoLoopPoints(double startPos, double endPos);

    void updateEnvelopeTwoLoopDirection();
    void setEnvelopeTwoSustainEnabled(bool enabled);
    void setEnvelopeTwoSustainPoints(double startPos, double endPos);
    void updateEnvelopeTwoSustainDirection();
    void updateEnvelopeTwoLoopAmount();
    void setEnvelopeTwoReleasePoint(double startPos, double endPos);
    void updateEnvelopeTwoSyncSource();
    // Low Frequency Osc One

    // LFO One
    void setLowFreqOscOneShot(bool enabled);
    void setLowFreqOscOneSync(bool enabled);
    void setLowFreqOscOneBounds(double min, double max);
    void setLowFreqOscOneBuffer(msmBuffer &buffer);
    void resetLowFreqOscOne();

    // LFO Two
    void setLowFreqOscTwoShot(bool enabled);
    void setLowFreqOscTwoSync(bool enabled);
    void setLowFreqOscTwoBounds(double min, double max);
    void setLowFreqOscTwoBuffer(msmBuffer &buffer);
    void resetLowFreqOscTwo();

    void triggerEnvelope(int env);
    void releaseEnvelope(int env);
    void setEnvelopeLoopDirection(int env);
    void setEnvelopeSustainDirection(int env);
    void setEnvBuffer(msmBuffer buffer, int env);

    // Step Sequencer
    void enableStepSeq();
    void updateStepMidPoint();
    void updateChopCurve();
    void updateDuration();

    void setStepDurationBounds(double min, double max);
    void setStepValues(vector<double> values);
    void setButtonStates(vector<int> states);
    void setStepSeq(vector<double> values, vector<int>states);

    // Fx Container
    void setEffectOne(int effectProcessor);
    void setEffectTwo(int effectProcessor);

    double getBPM(){
        return 120.0;
    }

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
    void initialiseFilterParameters();
    void initialiseWaveShapeParameters();
    void initialiseLFO1Parameters();
    void initialiseLFO2Parameters();
    void initialiseENV1Parameters();
    void initialiseENV2Parameters();
    void initialiseStepSeqParameters();
    void initialiseEffectParameters();
    void initialiseStereoDelParameters();
    void initialisePingPongDelay();
    void initialiseLCRDelay();
    void initialiseStereoFlanger();
    void initialiseDimensionChorus();
    void initialiseHoldDelay();
    void initialiseDecimator();
    void initialisePlateReverb();

    // Preset Manager
    ScopedPointer<ConfigurationFileManager> presetManager;


    /* Mod Matrix */
    AkatekoMatrix modMatrix;

    double wsAmplitude;   //amplitude
    double wsRouting;     //routing

    AkatekoVoice voiceLeft;
    AkatekoVoice voiceRight;

    //Modulators
    LowFrequencyTableOsc lfoOne;
    LowFrequencyTableOsc lfoTwo;
    EnvelopeGenerator envOne;
    EnvelopeGenerator envTwo;
    StepSeqGenerator stepSeq;

    //Fx
    ScopedPointer<StereoProcessor> effectOne;
    ScopedPointer<StereoProcessor> effectTwo;

    // Global Section
    AudioParameterFloat *inputVolume;
    AudioParameterFloat *globalVolume;
    // PreAmp Section

    // Filter Section
    AudioParameterBool *filterEnable;
    AudioParameterInt *filterConfig;
    // Filter one
    AudioParameterBool *filterOneEnable;
    AudioParameterFloat *filterOneType;
    AudioParameterInt * filterOneRollOff;

    AudioParameterFloat *filterOneFrequency;
    AudioParameterFloat *filterOneResonance;
    AudioParameterFloat *filterOnePassBand;
    AudioParameterFloat *filterOneDrive;
    AudioParameterFloat *filterOneVolume;

    // Filter Two
    AudioParameterBool *filterTwoEnable;
    AudioParameterFloat *filterTwoType;
    AudioParameterInt *filterTwoRollOff;

    AudioParameterFloat *filterTwoFrequency;
    AudioParameterFloat *filterTwoResonance;
    AudioParameterFloat *filterTwoPassBand;
    AudioParameterFloat *filterTwoDrive;
    AudioParameterFloat *filterTwoVolume;

    // WaveShaper
    AudioParameterBool *waveShaperEnable;
    AudioParameterFloat *waveShaperDrive;
    AudioParameterFloat *waveShaperMix;
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

    //FX
    AudioParameterBool *fxEnable;
    AudioParameterFloat *fxInputAmp;
    AudioParameterFloat *fxRouting;
    AudioParameterFloat *fxConfig;
    //FX Modules

    //Stereo Delay
    AudioParameterBool *stereoDelEnable;
    AudioParameterBool *stereoDelCrossEnable;
    AudioParameterFloat *stereoDelCrossAmount;
    AudioParameterFloat *stereoDelMix;
    AudioParameterBool *stereoDelLeftSync;
    AudioParameterFloat *stereoDelLeft;
    AudioParameterFloat *stereoDelLeftFB;
    AudioParameterBool *stereoDelRightSync;
    AudioParameterFloat *stereoDelRight;
    AudioParameterFloat *stereoDelRightFB;

    //Ping Pong Delay
    AudioParameterBool *pingPongEnable;
    AudioParameterBool *pingPongLeftSync;
    AudioParameterFloat *pingPongLeftDelay;
    AudioParameterFloat *pingPongLeftFB;
    AudioParameterFloat *pingPongLeftMix;
    AudioParameterBool *pingPongRightSync;
    AudioParameterFloat *pingPongRightDelay;
    AudioParameterFloat *pingPongRightFB;
    AudioParameterFloat *pingPongRightMix;

    //Left Center Right Delay
    AudioParameterBool *LCREnable;
    AudioParameterBool *LCRLeftSync;
    AudioParameterFloat *LCRLeftDelay;
    AudioParameterFloat *LCRLeftFB;
    AudioParameterFloat *LCRLeftMix;
    AudioParameterBool *LCRCenterSync;
    AudioParameterFloat *LCRCenterDelay;
    AudioParameterFloat *LCRCenterFB;
    AudioParameterBool *LCRhpfEnable;
    AudioParameterFloat *LCRhpfFrequency;
    AudioParameterBool *LCRlpfEnable;
    AudioParameterFloat *LCRlpfFrequency;
    AudioParameterFloat *LCRCenterAmp;
    AudioParameterFloat *LCRCenterPan;
    AudioParameterBool *LCRRightSync;
    AudioParameterFloat *LCRRightDelay;
    AudioParameterFloat *LCRRightFB;
    AudioParameterFloat *LCRRightMix;

    //Stereo Flanger
    AudioParameterBool *stereoFlangerEnable;
    AudioParameterBool *stereoFlangerCross;
    AudioParameterBool *stereoFlangerSync;
    AudioParameterFloat *stereoFlangerRate;
    AudioParameterFloat *stereoFlangerPhase;
    AudioParameterFloat *stereoFlangerDepth;
    AudioParameterFloat *stereoFlangerFeedBack;
    AudioParameterFloat *stereoFlangerWave;

    //Dimension Chorus
    AudioParameterBool *dChorusEnable;
    AudioParameterFloat *dChorusRate;
    AudioParameterFloat *dChorusSpread;
    AudioParameterFloat *dChorusDepth;
    AudioParameterFloat *dChorusWave;
    AudioParameterFloat *dChorusFrequency;
    AudioParameterFloat *dChorusCenterAmp;
    AudioParameterFloat *dChorusCenterPan;
    AudioParameterFloat *dChorusMix;

    // Hold Delay
    AudioParameterBool *hDelayEnable;
    AudioParameterBool *hDelayTrigger;
    AudioParameterBool *hDelaySync;
    AudioParameterFloat *hDelayLoop;
    AudioParameterFloat *hDelaySpeed;
    AudioParameterBool *hDelaySmooth;
    AudioParameterFloat *hDelayGlide;
    AudioParameterFloat *hDelayDirection;
    AudioParameterBool *hDelayGap;
    AudioParameterFloat *hDelayLength;
    AudioParameterFloat *hDelayFade;
    AudioParameterFloat *hDelayPan;
    AudioParameterFloat *hDelayMix;

    // Decimator
    AudioParameterBool *decimatorEnable;
    AudioParameterBool *decimatorReduceEna;
    AudioParameterBool *decimatorSRateEna;
    AudioParameterBool *decimatorFilter;
    AudioParameterFloat *decimatorBits;
    AudioParameterFloat *decimatorSRate;
    AudioParameterFloat *decimatorMix;

    // Plate Reverb
    AudioParameterBool *pReverbEnable;
    AudioParameterFloat *pReverbDamping;
    AudioParameterFloat *pReverbBandWidth;
    AudioParameterFloat *pReverbDecay;
    AudioParameterFloat *pReverbMix;

    //Parameter Indices
    std::vector<int> globalIndices;
    std::vector<int> filterIndices;
    std::vector<int> wsIndices;
    std::vector<int> lfoOneIndices;
    std::vector<int> lfoTwoIndices;
    std::vector<int> envOneIndices;
    std::vector<int> envTwoIndices;
    std::vector<int> sseqIndices;
    std::vector<int> fxIndices;
    std::vector<int> stereoDelIndices;
    std::vector<int> pingPongIndices;
    std::vector<int> lcrDelayIndices;
    std::vector<int> sFlangerIndices;
    std::vector<int> dChorusIndices;
    std::vector<int> hDelayIndices;
    std::vector<int> decimatorIndices;
    std::vector<int> pReverbIndices;

    // Parameter Id's
    StringArray globalParamIds;
    StringArray filterParamIds;
    StringArray waveShapeParamIds;
    StringArray lfoOneParamIds;
    StringArray lfoTwoParamIds;
    StringArray envOneParamIds;
    StringArray envTwoParamIds;
    StringArray stepSeqParamIds;
    StringArray fxParamIds;
    StringArray stereoDelParamIds;
    StringArray pingPongParamIds;
    StringArray lcrDelayParamIds;
    StringArray sFlangerParamIds;
    StringArray dChorusParamIds;
    StringArray hDelayParamIds;
    StringArray decimatorParamIds;
    StringArray pReverbParamIds;

    /* Gui representations */
    bool shapesAreInitialised;

    //Presets Currently loaded call in editor
    String currentPresetFile; // Preset File Name
    String currentPresetName; // Preset Name in XML

    //Ui States
    String filterState; // * Are we using it?
    String wsString;    // * "              "
    String envOneState;
    String envTwoState;
    String lfoOneState;
    String lfoTwoState;
    String stepSeqState;
    String xyPadState;

    String fxState;

    StringArray lfoOneShapes;
    StringArray lfoTwoShapes;
    StringArray waveShapes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AkatekoAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
