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


#define FILTER_FC_MODRANGE 46.881879936465680

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
        //Filter
        PRESAT = 0,     //Pre saturation/drive
        PREPBG = 1,     //Pre passband gain
        F1FREQ = 2,     //Filter one frequency
        F1RESO = 3,     //Filter one resonance
        F2FREQ = 4,     //Filter Two frequency
        F2RESO = 5,     //Filter Two resonance
        //WaveShaper
        WSDRI = 6,      //Wave shaper Drive
        WSMIX = 7,      //Wave shaper Mix
        WSAMP = 8,      //Wave shaper Amplitude
        WSRTE = 9,      //Wave shaper Routing
        //Low Frequency Oscillator 1
        LFO1FREQ = 10,  // LFO 1 Frequency
        LFO1PHS = 11,   // LFO 1 Phase
        LFO1PWM = 12,   // LFO 1 PWM
        //Low Frequency Oscillator 2
        LFO2FREQ = 13,  // LFO 2 Frequency
        LFO2PHS = 14,   // LFO 2 Phase
        LFO2PWM = 15,   // LFO 2 PWM
        //Envelopes
        ENV1DUR = 16,   // Envelope 1 Duration
        ENV2DUR = 17,   // Envelope 2 Duration
        //Step Sequencer
        SSQDUR = 18,    // Step Sequencer Duration
        SSQLEN = 19,    // Step Sequencer Length
        SSQOFF = 20,    // Step Sequencer Off Value
        SSQCUR = 21,    // Step Sequencer Curve

        DSTSIZE = 22   //Destination size
    };


} //namespace



#endif  // AKATEKO_H_INCLUDED
