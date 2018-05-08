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
        // MIDI
        SRCSIZE = 7,
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

    // Use for gather a vector with the current Presets
    struct PresetRow {
        File file;
        String name;
        String category;
        String author;
    };

    // Sort algorithms for preset Rows
    bool nameSortAscending(PresetRow const &lhs, PresetRow const &rhs);
    bool nameSortDescending(PresetRow const &lhs, PresetRow const &rhs);

    bool categorySortAscending(PresetRow const &lhs, PresetRow const &rhs);
    bool categorySortDescending(PresetRow const &lhs, PresetRow const &rhs);

    bool authorSortAscending(PresetRow const &lhs, PresetRow const &rhs);
    bool authorSortDescending(PresetRow const &lhs, PresetRow const &rhs);
} //namespace akateko



#endif  // AKATEKO_H_INCLUDED
