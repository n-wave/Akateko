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
#include "/work/programming-projects/msm/msm-dsp/Common/AudioTrigger.h"

#include "AkatekoVoice.h"
#include "AkatekoMatrix.h"
#include <vector>


using std::vector;
using akateko::MidiRow;
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

    enum UserInterfaceId {
        GlobalId = 0,
        FilterId = 1,
        WaveShapeId = 2,
        LFO1Id = 3,
        LFO2Id = 4,
        Envelope1Id = 5,
        Envelope2Id = 6,
        StepSequencerId = 7,
        xyPadId = 8,
        FxContainerId = 9,
        StereoDelayId = 10,
        PingPongDelayId = 11,
        LCRDelayId = 12,
        StereoflangerId = 13,
        DChorusId = 14,
        hDelayId = 15,
        decimatorId = 16,
        pReverbId = 17,
        aTriggerId = 18,
        dbMeterId = 19,
        nrOfSections
    };

    enum GlobalOptions {
        DefaultPresetDirectoryId = 0,
        OverSamplingId = 1,
        FilterResonanceId = 2
    };

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

    //Get Active Effect
    int getActiveEffect(int fx);

    //==============================================================================
    // Preset Handling
    //==============================================================================

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

    //==============================================================================
    // Configuration Handling
    //==============================================================================

    void storeConfigurationOption(const String value, const GlobalOptions option);
    void loadConfigurationOptions();
    void createDefaultConfigurationFile();

    String getDefaultPresetDirectory();
    int getOverSampling();
    int getFilterResonance();

    //==============================================================================
    void resetLfo(); //reset phase
    void setLfoBuffer(msmBuffer buffer, int lfo);

    //==============================================================================
    // Filter Configuration
    //==============================================================================
    void updateFiltersEnabled();
    void updateFilterConfiguration();
    void updateFilterType(int filter);
    void updateResonanceScalar(double scalar);

    void updateFilterOneEnabled();
    void updateFilterTwoEnabled();

    //==============================================================================
    // Waveshaper
    //==============================================================================
    void enableWaveShaper();
    void setWaveShaperBuffer(msmBuffer &buffer);

    //==============================================================================
    // Envelope One
    //==============================================================================
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

    //==============================================================================
    // Envelope Two
    //==============================================================================
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

    void triggerEnvelope(int env);
    void releaseEnvelope(int env);
    void setEnvelopeLoopDirection(int env);
    void setEnvelopeSustainDirection(int env);
    void setEnvBuffer(msmBuffer buffer, int env);
    //==============================================================================
    // LFO One
    //==============================================================================
    void setLowFreqOscOneShot(bool enabled);
    void setLowFreqOscOneSync(bool enabled);
    void setLowFreqOscOneBounds(double min, double max);
    void setLowFreqOscOneBuffer(msmBuffer &buffer);
    void resetLowFreqOscOne();
    //==============================================================================
    // LFO Two
    //==============================================================================
    void setLowFreqOscTwoShot(bool enabled);
    void setLowFreqOscTwoSync(bool enabled);
    void setLowFreqOscTwoBounds(double min, double max);
    void setLowFreqOscTwoBuffer(msmBuffer &buffer);
    void resetLowFreqOscTwo();
    //==============================================================================
    // Step Sequencer
    //==============================================================================
    void enableStepSeq();
    void updateStepMidPoint();
    void updateChopCurve();
    void updateDuration();
    void setStepDurationBounds(double min, double max);
    void setStepValues(vector<double> values);
    void setButtonStates(vector<int> states);
    void setStepSeq(vector<double> values, vector<int>states);
    //==============================================================================
    // Fx Container
    //==============================================================================
    void setEffectOne(int effectProcessor);
    void setEffectTwo(int effectProcessor);
    //==============================================================================
    // Midi
    //==============================================================================
    void initiateMidiLearn(MidiRow row);
    bool getMidiLearnStatus();
    bool getMidiMsgBoundStatus();
    String getMidiMessageBounded();
    void setMidiRow(MidiRow row);
    void setMidiTriggerRow(MidiRow row);

    void removeMidiRow(int paramIndex);
    void removeMidiRow(int arrayIndex, int paramIndex);
    void clearAllMidiRows();

    vector<MidiRow>getMidiRows();    
    bool getRegisteredMidi(int pIndedx);
    void changeMidiRowMinMax(double minValue, double maxValue, int handling, int pIndex);

    //==============================================================================
    // Gui
    //==============================================================================
    bool requestGuiUpdate();
    std::vector<bool> getUpdatedSections();
    void setUIState(const String state, UserInterfaceId id);
    String getUIState(UserInterfaceId id);

    std::vector<int> getParameterIndices(UserInterfaceId id); //return indices
    StringArray getParameterIds(UserInterfaceId id);

    //==============================================================================
    // Waveform, Envelope and Waveshaper shapes
    //==============================================================================
    void setShapes(StringArray shapes, UserInterfaceId id);
    void setShape(String shape, UserInterfaceId id);
    StringArray getShapes(UserInterfaceId id);

    AkatekoMatrix &getModMatrix();

    //==============================================================================
    // Timing
    //==============================================================================
    double getBeatsPerMinute();
    void printPositionInfo();
    bool getTimeSignature(double &bpm, double &tden);
    void printRegisteredParameters(); //Parse out file after initialisation

private:
    //==============================================================================
    void initialiseShapes();
    void initialiseParameters();
    void initialiseGlobalParameters();
    void initialiseFilterParameters();
    void initialiseWaveShapeParameters();
    void initialiseLFO1Parameters();
    void initialiseLFO2Parameters();
    void initialiseENV1Parameters();
    void initialiseENV2Parameters();
    void initialiseStepSeqParameters();
    void initialiseXYPadParameters();
    void initialiseEffectParameters();
    void initialiseStereoDelParameters();
    void initialisePingPongDelay();
    void initialiseLCRDelay();
    void initialiseStereoFlanger();
    void initialiseDimensionChorus();
    void initialiseHoldDelay();
    void initialiseDecimator();
    void initialisePlateReverb();
    void initialiseAudioTrigger();

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

    AudioTrigger audioTrigger;

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
    AudioParameterFloat *lfoOneSmooth;
    // LFO Two
    AudioParameterBool *lfoTwoEnable;
    AudioParameterBool *lfoTwoShot;
    AudioParameterBool *lfoTwoSync;

    AudioParameterFloat *lfoTwoFrequency;
    AudioParameterFloat *lfoTwoPhase;
    AudioParameterFloat *lfoTwoPWM;
    AudioParameterFloat *lfoTwoShape;
    AudioParameterFloat *lfoTwoSmooth;
    // Env One
    AudioParameterBool *envOneEnable;
    AudioParameterBool *envOneSync; //divide duration time so it concides with the beat
    AudioParameterFloat *envOneDuration;
    AudioParameterFloat *envOneLoopAmount;
    AudioParameterFloat *envOneLoopDirection;
    AudioParameterFloat *envOneSusDirection;
    AudioParameterFloat *envOneTriggerSource; //Automatically create entry mod matrix
    AudioParameterBool *envOneTrigger; //Use for midi triggering
    // Env Two
    AudioParameterBool *envTwoEnable;
    AudioParameterBool *envTwoSync;
    AudioParameterFloat *envTwoDuration;
    AudioParameterFloat *envTwoLoopAmount;
    AudioParameterFloat *envTwoLoopDirection;
    AudioParameterFloat *envTwoSusDirection;
    AudioParameterFloat *envTwoTriggerSource;
    AudioParameterBool *envTwoTrigger;
    // Step sequencer
    AudioParameterBool *stepSeqEnable;
    AudioParameterBool *stepSeqSync;
    AudioParameterBool *stepSeqChopper;

    AudioParameterFloat *stepSeqDuration;
    AudioParameterFloat *stepSeqLength;
    AudioParameterFloat *stepSeqOffset;
    AudioParameterFloat *stepSeqMidPoint;
    AudioParameterFloat *stepSeqAmount;

    // Xy Pad;
    AudioParameterFloat *xyPadX;
    AudioParameterFloat *xyPadY;
    AudioParameterBool  *xyPadT;

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
    AudioParameterBool *hDelayLatchEnable;
    AudioParameterFloat *hDelayLatch;

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

    //Audio Trigger  place at the end
    // of the AudioParameters, doesn't have to
    // be modulated but storing it as audioParameter
    // will make it easier to store in preset

    AudioParameterFloat *aTriggerPreGain;
    AudioParameterFloat *aTriggerThreshold;
    AudioParameterFloat *aTriggerPreDelay;
    AudioParameterFloat *aTriggerHoldDelay;
    AudioParameterFloat *aTriggerInputSelection;

    //Parameter Indices
    std::vector<int> globalIndices;
    std::vector<int> filterIndices;
    std::vector<int> wsIndices;
    std::vector<int> lfoOneIndices;
    std::vector<int> lfoTwoIndices;
    std::vector<int> envOneIndices;
    std::vector<int> envTwoIndices;
    std::vector<int> sseqIndices;
    std::vector<int> xyPadIndices;
    std::vector<int> fxIndices;
    std::vector<int> stereoDelIndices;
    std::vector<int> pingPongIndices;
    std::vector<int> lcrDelayIndices;
    std::vector<int> sFlangerIndices;
    std::vector<int> dChorusIndices;
    std::vector<int> hDelayIndices;
    std::vector<int> decimatorIndices;
    std::vector<int> pReverbIndices;
    std::vector<int> aTriggerIndices;

    // Parameter Id's
    StringArray globalParamIds;
    StringArray filterParamIds;
    StringArray waveShapeParamIds;
    StringArray lfoOneParamIds;
    StringArray lfoTwoParamIds;
    StringArray envOneParamIds;
    StringArray envTwoParamIds;
    StringArray stepSeqParamIds;
    StringArray xyPadParamIds;
    StringArray fxParamIds;
    StringArray stereoDelParamIds;
    StringArray pingPongParamIds;
    StringArray lcrDelayParamIds;
    StringArray sFlangerParamIds;
    StringArray dChorusParamIds;
    StringArray hDelayParamIds;
    StringArray decimatorParamIds;
    StringArray pReverbParamIds;
    StringArray aTriggerParamIds;

    // Gui representations
    bool shapesAreInitialised;

    // Timing
    AudioPlayHead::CurrentPositionInfo position;
    double ppqPosition = 0;
    double beatsPerMinute = 120.0;
    double invTimeSigNum = 0.0;

    bool reset;
    int timeSigNumerator = 4;
    int timeSigDenominator = 4;

    int fxOne = 1;
    int fxTwo = 1;

    //Presets Currently loaded call in editor
    String currentPresetFile; // Preset File Name
    String currentPresetName; // Preset Name in XML

    // Global Configuration
    String defaultPresetDirectory; //Stored in config file
    int overSampling;
    int filterResonance;

    //MidiTable
    std::vector<MidiRow> midiTable;
    std::vector<MidiRow> noteMessages;
    std::vector<MidiRow> ccMessages;
    std::vector<MidiRow> pbMessages;

    int mrIndex; //Midi Row Index
    int mrpIndex;
    bool midiLearn;
    bool midiMsgBounded;
    bool updateGui = false;
    bool aTrigger = false;
    std::vector<bool> updateGuiSections;

    const double scalar7Bit = 0.007874016; // 1/127
    const double preScalar14Bit = 0.007751938; // 1/129 Pre scalar
    String midiLearnMsg;

    // RMS Volume do the sqrt calculation in the qui thread

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
