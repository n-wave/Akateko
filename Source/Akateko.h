/*
  ==============================================================================

    Akateko.h
    Created: 24 Feb 2018 10:45:33am
    Author:  mario

  ==============================================================================
*/

#ifndef AKATEKO_H_INCLUDED
#define AKATEKO_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-dsp/Common/msm.h"


/*
 *  void setShapes(StringArray shapes, UserInterfaceId id);
 *  void updateShape(String shape, UserInterfaceId id);
 *  StringArray getShapes(UserInterfaceId id);
 *
 */

namespace akateko {

    //Update BPM command use this
    //when bpm needs to be updated
    //in the fx container components
    enum FXCID{
        updateFxBPM = 0x42504D,
        updateFxGUI = 0x475549
    };

    /* write registeres */
    enum MODSRC
    {
        // Modulators
        LFO1 = 0,
        LFO2 = 1,
        ENV1 = 2,
        ENV2 = 3,
        SSEQ = 4,
        // XY Pad
        PADX = 5,
        PADY = 6,
        PADT = 7,
        // Envelope Follower

        SRCSIZE = 8,
    };

    enum TRANSFORM {
        NONE = -1,
        UNIPOLAR = 0,
        BIPOLAR = 1,
        TSIZE = 2
    };

    /* These will make up the read registers */
    enum DESTSRC
    {
        // Filter
        F1FREQ = 0,     //Filter one frequency
        F1RESO = 1,     //Filter one Resonance
        F1PBG = 2,      //Filter One Pass Band
        F1DRIVE = 3,    //Filter One Drive
        F1VOL = 4,      //Filter One Volume

        F2FREQ = 5,     //Filter Two frequency
        F2RESO = 6,     //Filter Two resonance
        F2PBG = 7,      //Filter Two Pass Band
        F2DRIVE = 8,   //Filter Two Drive
        F2VOL = 9,     //Filter Two Volume

        // WaveShaper
        WSDRI = 10,      //Wave shaper Drive
        WSMIX = 11,      //Wave shaper Mix
        // Low Frequency Oscillator 1
        LFO1FREQ = 12,  // LFO 1 Frequency
        LFO1PHS = 13,   // LFO 1 Phase
        LFO1PWM = 14,   // LFO 1 PWM
        // Low Frequency Oscillator 2
        LFO2FREQ = 15,  // LFO 2 Frequency
        LFO2PHS = 16,   // LFO 2 Phase
        LFO2PWM = 17,   // LFO 2 PWM
        // Envelopes
        ENV1DUR = 18,   // Envelope 1 Duration
        ENV2DUR = 19,   // Envelope 2 Duration
        // Step Sequencer
        SSQDUR = 20,    // Step Sequencer Duration
        SSQLEN = 21,    // Step Sequencer Length
        SSQOFF = 22,    // Step Sequencer Off Value        
        // Stereo Delay
        SDCROSS = 23,   // Stereo Delay Cross Amount
        SDMIX = 24,     // Stereo Delay Mix Dry/Wet
        SDLDEL = 25,    // Stereo Delay Left Delay Time
        SDLFB = 26,     // Stereo Delay Left FeedBack
        SDRDEL = 27,    // Stereo Delay Right Delay Time
        SDRFB = 28,     // Stereo Delay Right FeedBack
        // Ping Pong Delay
        PPLDEL = 29,    // Ping Pong Left Delay
        PPLFB = 30,     // Ping Pong Left FeedBack
        PPLMIX = 31,    // Ping Pong Left Mix
        PPRDEL = 32,    // Ping Pong Right Delay
        PPRFB = 33,     // Ping Pong Right FeedBack
        PPRMIX = 34,    // Ping Pong Right Mix
        // LCR Delay
        LCRLDEL = 35,   // LCR Delay Left Delay
        LCRLFB = 36,    // LCR Delay Left FeedBack
        LCRLMIX = 37,   // LCR Delay Left Mix
        LCRCDEL = 38,   // LCR Delay Center Delay
        LCRCFB = 39,    // LCR Delay Center FeedBack
        LCRCHPF = 40,   // LCR Delay Center HPF
        LCRCLPF = 41,   // LCR Delay Center LPF
        LCRCAMP = 42,   // LCR Delay Center Amp
        LCRCPAN = 43,   // LCR Delay Center Pan
        LCRRDEL = 44,   // LCR Delay Right Delay
        LCRRFB = 45,    // LCD Delay Right FeedBack
        LCRRMIX = 46,   // LCR Delay Right Mix

        // Stereo Flanger
        SFLRT = 47,     // Stereo Flanger Rate
        SFLPH = 48,     // Stereo Flanger Phase
        SFLDH = 49,     // Stereo Flanger Depth
        SFLFB = 50,     // Stereo Flanger Feedback

        //Dimension Chorus
        DCHORT = 51,    // D-Chorus Rate
        DCHOSP = 52,    // D-Chorus Spread
        DCHODP = 53,    // D-Chorus Depth
        DCHOHPF = 54,   // D-Chorus frequency
        DCHOCA = 55,    // D-Chorus Center Amp
        DCHOCP = 56,    // D-Chorus Center Pan
        DCHOMIX = 57,   // D-Chorus Mix

        // Hold Delay
        HDELSPD = 58,   // H-Delay Speed
        HDELLEN = 59,   // H-Delay Length
        HDELFDE = 60,   // H-Delay Fade
        HDELPAN = 61,   // H-Delay Pan
        HDELMIX = 62,   // H-Delay Mix

        // Decimator
        DECBIT = 63,    // Decimator Bits
        DECRTE = 64,    // Decimator S-Rate
        DECMIX = 65,    // Decimator Mix

        // Plate Reverb
        PREVBW = 66,     // P-Reverb BandWidth
        PREVDAM  = 67,   // P-Reverb Damping
        PREVDEC = 68,    // P-Reverb Decay
        PREVMIX = 69,    // P-Reverb Mix

        DSTSIZE = 70    // Destination size
    };

    //Use for retrieving Midi indices
    //In the MidiRow arrays located in
    //the MidiTable Viewer.
    //Make Sure the size coincides with
    //the array represented

    enum GLOBALMIDI{
        MIDI_GL_INPUT,
        MIDI_GL_OUTPUT,
        MIDI_GL_SIZE
    };

    enum FILTERMIDI{
        //Global Bypas Filter
        MIDI_F_ENABLE,
        //Filter One
        MIDI_F1_ENABLE,
        MIDI_F1_TYPE,
        MIDI_F1_ROFF,
        MIDI_F1_FREQ,
        MIDI_F1_RESO,
        MIDI_F1_PBG,
        MIDI_F1_DRIVE,
        MIDI_F1_VOLUME,
        //Filter Two
        MIDI_F2_ENABLE,
        MIDI_F2_TYPE,
        MIDI_F2_ROFF,
        MIDI_F2_FREQ,
        MIDI_F2_RESO,
        MIDI_F2_PBG,
        MIDI_F2_DRIVE,
        MIDI_F2_VOLUME,
        MIDI_F_SIZE
    };

    enum WSHAPEMIDI{
        MIDI_WS_ENABLE,
        MIDI_WS_DRIVE,
        MIDI_WS_MIX,
        MIDI_WS_SHAPE_1,
        MIDI_WS_SHAPE_2,
        MIDI_WS_SHAPE_3,
        MIDI_WS_SHAPE_4,
        MIDI_WS_SHAPE_5,
        MIDI_WS_SHAPE_6,
        MIDI_WS_SHAPE_7,
        MIDI_WS_SHAPE_8,
        MIDI_WS_SIZE
    };

    enum LFOONEMIDI {
        MIDI_LFO1_ENABLE,
        MIDI_LFO1_ONESHOT,
        MIDI_LFO1_FREE,
        MIDI_LFO1_SYNC,
        MIDI_LFO1_FREQ,
        MIDI_LFO1_PHASE,
        MIDI_LFO1_PWM,
        MIDI_LFO1_SHAPE_1,
        MIDI_LFO1_SHAPE_2,
        MIDI_LFO1_SHAPE_3,
        MIDI_LFO1_SHAPE_4,
        MIDI_LFO1_SHAPE_5,
        MIDI_LFO1_SHAPE_6,
        MIDI_LFO1_SHAPE_7,
        MIDI_LFO1_SHAPE_8,
        MIDI_LFO1_SMOOTH,
        MIDI_LFO1_SIZE
    };

    enum LFOTWOMIDI {
        MIDI_LFO2_ENABLE,
        MIDI_LFO2_ONESHOT,
        MIDI_LFO2_FREE,
        MIDI_LFO2_SYNC,
        MIDI_LFO2_FREQ,
        MIDI_LFO2_PHASE,
        MIDI_LFO2_PWM,
        MIDI_LFO2_SHAPE_1,
        MIDI_LFO2_SHAPE_2,
        MIDI_LFO2_SHAPE_3,
        MIDI_LFO2_SHAPE_4,
        MIDI_LFO2_SHAPE_5,
        MIDI_LFO2_SHAPE_6,
        MIDI_LFO2_SHAPE_7,
        MIDI_LFO2_SHAPE_8,
        MIDI_LFO2_SMOOTH,
        MIDI_LFO2_SIZE
    };

    enum ENVONEMIDI {
        MIDI_ENV1_ENABLE,
        MIDI_ENV1_SYNC,
        MIDI_ENV1_DURATION,
        MIDI_ENV1_LOOP_NR,
        MIDI_ENV1_LOOP_DIR,
        MIDI_ENV1_SUS_DIR,
        MIDI_ENV1_TRIGGER,
        MIDI_ENV1_SIZE
    };

    enum ENVTWOMIDI {
        MIDI_ENV2_ENABLE,
        MIDI_ENV2_SYNC,
        MIDI_ENV2_DURATION,
        MIDI_ENV2_LOOP_NR,
        MIDI_ENV2_LOOP_DIR,
        MIDI_ENV2_SUS_DIR,
        MIDI_ENV2_TRIGGER,
        MIDI_ENV2_SIZE
    };

    enum STEPSEQMIDI {
        MIDI_SSEQ_ENABLE,
        MIDI_SSEQ_SYNC,
        MIDI_SSEQ_CHOP,
        MIDI_SSEQ_DURATION,
        MIDI_SSEQ_LENGTH,
        MIDI_SSEQ_OFF_VALUE,
        MIDI_SSEQ_CURVE,
        MIDI_SSEQ_STEPS,
        MIDI_SSEQ_SIZE
    };

    enum XYPADMIDI {
        MIDI_XYPAD_X_AXIS,
        MIDI_XYPAD_Y_AXIS,
        MIDI_XYPAD_TRIGGER,
        MIDI_XYPAD_SIZE
    };

    enum FXMIDI{
        MIDI_FX_ENABLE,
        MIDI_FX_INPUT,
        MIDI_FX_ROUTING,
        MIDI_FX_SIZE
    };

    enum SDELAYMIDI {
        MIDI_SDEL_ENABLE,
        MIDI_SDEL_CROSS_EN,
        MIDI_SDEL_CROSS_AMP,
        MIDI_SDEL_MIX,
        MIDI_SDEL_L_SYNC,
        MIDI_SDEL_L_DELAY,
        MIDI_SDEL_L_FB,
        MIDI_SDEL_R_SYNC,
        MIDI_SDEL_R_DELAY,
        MIDI_SDEL_R_FB,
        MIDI_SDEL_SIZE,
    };

    enum PPDELAYMIDI {
        MIDI_PPDEL_ENABLE,
        MIDI_PPDEL_L_SYNC,
        MIDI_PPDEL_L_DELAY,
        MIDI_PPDEL_L_FB,
        MIDI_PPDEL_L_MIX,
        MIDI_PPDEL_R_SYNC,
        MIDI_PPDEL_R_DELAY,
        MIDI_PPDEL_R_FB,
        MIDI_PPDEL_R_MIX,
        MIDI_PPDEL_SIZE
    };

    enum LCRDELAYMIDI {
        MIDI_LCRDEL_ENABLE,
        MIDI_LCRDEL_L_SYNC,
        MIDI_LCRDEL_L_DELAY,
        MIDI_LCRDEL_L_FB,
        MIDI_LCRDEL_L_MIX,
        MIDI_LCRDEL_C_SYNC,
        MIDI_LCRDEL_C_DELAY,
        MIDI_LCRDEL_C_FB,
        MIDI_LCRDEL_HPF_ENA,
        MIDI_LCRDEL_HPF_FREQ,
        MIDI_LCRDEL_LPF_ENA,
        MIDI_LCRDEL_LPF_FREQ,
        MIDI_LCRDEL_C_AMP,
        MIDI_LCRDEL_C_PAN,
        MIDI_LCRDEL_R_SYNC,
        MIDI_LCRDEL_R_DELAY,
        MIDI_LCRDEL_R_FB,
        MIDI_LCRDEL_R_MIX,
        MIDI_LCRDEL_SIZE
    };

    enum SFLANGERMIDI {
        MIDI_SFLANG_ENABLE,
        MIDI_SFLANG_CROSS,
        MIDI_SFLANG_SYNC,
        MIDI_SFLANG_RATE,
        MIDI_SFLANG_PHASE,
        MIDI_SFLANG_DEPTH,
        MIDI_SFLANG_FB,
        MIDI_SFLANG_WAVE,
        MIDI_SFLANG_SIZE
    };

    enum DCHORUSMIDI {
        MIDI_DCHOR_ENABLE,
        MIDI_DCHOR_RATE,
        MIDI_DCHOR_SPREAD,
        MIDI_DCHOR_DEPTH,
        MIDI_DCHOR_WAVE,
        MIDI_DCHOR_FREQ,
        MIDI_DCHOR_C_AMP,
        MIDI_DCHOR_C_PAN,
        MIDI_DCHOR_MIX,
        MIDI_DCHOR_SIZE
    };

    enum HDELAYMIDI {
        MIDI_HDEL_ENABLE,
        MIDI_HDEL_TRIGGER,
        MIDI_HDEL_BEAT,
        MIDI_HDEL_LOOP,
        MIDI_HDEL_SPEED,
        MIDI_HDEL_GLIDE_OPT,
        MIDI_HDEL_GLIDE,
        MIDI_HDEL_DIR,     //Play back Direction
        MIDI_HDEL_GAP_DIR, //back buffer, front buffer
        MIDI_HDEL_LENGTH,  //Gap Length
        MIDI_HDEL_FADE,
        MIDI_HDEL_PAN,
        MIDI_HDEL_MIX,
        MIDI_HDEL_LATCH_ENA,
        MIDI_HDEL_LATCH,
        MIDI_HDEL_SIZE
    };

    enum DECIMATOR {
        MIDI_DECIM_ENABLE,
        MIDI_DECIM_REDUCE_ENA,
        MIDI_DECIM_SRATE_ENA,
        MIDI_DECIM_FILTER_ENA,
        MIDI_DECIM_REDUCE,
        MIDI_DECIM_SRATE,
        MIDI_DECIM_MIX,
        MIDI_DECIM_SIZE
    };

    enum PREVERB {
        MIDI_PREV_ENABLE,
        MIDI_PREV_DARMPING,
        MIDI_PREV_BANDWIDTH,
        MIDI_PREV_DECAY,
        MIDI_PREV_MIX,
        MIDI_PREV_SIZE
    };

    enum MIDIMESSAGES{
        MIDI_NONE = -1, //i.e clear
        MIDI_NOTE = 1,
        MIDI_CC = 2,
        MIDI_PB = 3,
    };

    enum MIDIHANDLING{
       MIDI_TO_INT = 0,         // Use for Int mapping, ComboBoxes truncated or truncated int Values
       MIDI_TO_INT_BUTTON = 1,  // Use for Buttons, minRange : off value and maxRange : on Value
       MIDI_TO_INT_TOGGLE = 2,  // Use for Toggle Functionality
       MIDI_TO_DOUBLE = 3,      // Controller Values
       MIDI_TO_CONSTANT = 4,    // Use for the shape buttons
    };

    // Use for gather a vector with the current Presets
    struct PresetRow {
        File file;
        String name;
        String category;
        String author;
    };

    // Param Index should coincide with
    // the parameters in the processor

    struct MidiRow {
        bool initialised = false;
        int section = -1;
        int midiMessage = -1;
        int channel = -1;
        int firstByte = -1; //CC value,  Note nr etc

        int minRange = -1;
        int maxRange = -1;
        int transformation = -1;
        int handling = -1;

        //Parameter Info
        //Load on midi learn
        //Update On a change
        double minValue = -1; // Use for scaling functionality
        double maxValue = -1; // In the midi Table update function
        int paramIndex = -1;
        int arrayIndex = -1; // Store for quick retrieval from parent Array
        String destination;
    };

    // Function for handling midiRow
    void initMidiRow(MidiRow &row, int aIndex, double minVal, double maxVal,
                     int pIndex, int handling, const String &dest, int section);

    void initMidiRow(MidiRow &row, int aIndex, int minR, int maxR, double minVal, double maxVal,
                     int pIndex, int handling, const String &dest, int section);

    String midiRowToString(MidiRow &row);
    bool restoreMidiRow(MidiRow &row, String &sRow);

    // Sort algorithms for preset Rows
    bool nameSortAscending(PresetRow const &lhs, PresetRow const &rhs);
    bool nameSortDescending(PresetRow const &lhs, PresetRow const &rhs);

    bool categorySortAscending(PresetRow const &lhs, PresetRow const &rhs);
    bool categorySortDescending(PresetRow const &lhs, PresetRow const &rhs);

    bool authorSortAscending(PresetRow const &lhs, PresetRow const &rhs);
    bool authorSortDescending(PresetRow const &lhs, PresetRow const &rhs);

    bool sortMidiRow(MidiRow const &lhs, MidiRow const &rhs);


    /* Use Only For Juce Button classes */
    template<typename T> class IgnoreRightClick : public T
    {
    public:
        IgnoreRightClick(const String &id) :
            requestMenu(-1)
        {
            T::setName(id);
            requestMenu = id.hash();
        }

        void mouseDown(const MouseEvent &e) override{
            if(e.mods.isPopupMenu()){
                T::getParentComponent()->postCommandMessage(requestMenu);              
                return;
            } else {
                T::mouseDown(e);
            }
        }

        void mouseUp(const MouseEvent &e) override {
            if(e.mods.isPopupMenu()){
                return;
            } else {
                T::mouseUp(e);
            }
        }

    private:
        IgnoreRightClick(){}

        int requestMenu;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IgnoreRightClick)
    };


    // Default shapes
    extern const String lfo_default_shapes[8]; // LFO
    extern const String ws_default_shapes[8];  // WaveShaper

    //Default UI states
    extern const String env_ui_state;
    extern const String step_seq_state;

    // Use for calculating the specified ratio's
    // Need to be used in the gui Component as
    // well as the processing unit when a MIDI
    // message arrives.

    extern const double frequency_ratios[24];    //LFO, Flanger Frequency conversions
    extern const double env_timing_ratios[24];
    extern const double sseq_timing_ratios[18];   //Step Sequencer
    extern const double delay_timing_ratios[24];    //S-Delay, PP-Delay, LCR-delay
    extern const double h_delay_timing_ratios[18];   //Hold Delay Time Ratio
    extern const double h_delay_latch_ratios[15];  //Hold Delay Latch Ratios

} //namespace akateko



#endif  // AKATEKO_H_INCLUDED
