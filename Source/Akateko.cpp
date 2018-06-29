/*
  ==============================================================================

    Akateko.cpp
    Created: 1 May 2018 12:56:31pm
    Author:  mario

  ==============================================================================
*/

#include "Akateko.h"

namespace akateko {
    void initMidiRow(MidiRow &row, int aIndex, double minVal, double maxVal,
                     int pIndex, int handling, const String &dest, int section)
    {
        row.arrayIndex = aIndex;
        row.minValue = minVal;
        row.maxValue = maxVal;
        row.paramIndex = pIndex;
        row.handling = handling;
        row.destination = String(dest);
        row.section = section;
    }

    void initMidiRow(MidiRow &row, int aIndex, int minR, int maxR, double minVal, double maxVal,
                     int pIndex, int handling, const String &dest, int section)
    {
        row.arrayIndex = aIndex;
        row.minRange = minR;
        row.maxRange = maxR;
        row.minValue = minVal;
        row.maxValue = maxVal;
        row.paramIndex = pIndex;
        row.handling = handling;
        row.destination = String(dest);
        row.section = section;
    }

    //Parse the Midi Row into a string to be stored in the Preset

    String midiRowToString(MidiRow &row){
        String midiRow = String("mr ");

        midiRow += String(row.initialised) + " ";
        midiRow += String(row.section) + " ";
        midiRow += String(row.midiMessage) + " ";
        midiRow += String(row.channel) + " ";
        midiRow += String(row.firstByte) + " ";
        midiRow += String(row.minRange) + " ";
        midiRow += String(row.maxRange) + " ";
        midiRow += String(row.transformation) + " ";
        midiRow += String(row.handling) + " ";
        midiRow += String(row.minValue) + " ";
        midiRow += String(row.maxValue) + " ";
        midiRow += String(row.paramIndex) + " ";
        midiRow += String(row.arrayIndex) + " ";
        midiRow += String(row.destination);

        return midiRow;
    }

    bool restoreMidiRow(MidiRow &row, String &sRow){
        StringArray tokens;

        tokens.addTokens(sRow, " ", "\"");

        std::cout << "Token size : " << tokens.size() << std::endl;

        if(tokens.size() > 14 && tokens[0] == "mr"){
            std::cout << "Restore Midi Row : " << tokens.size() << std::endl;

            row.initialised = tokens[1].getIntValue();
            row.section = tokens[2].getIntValue();
            row.midiMessage = tokens[3].getIntValue();
            row.channel = tokens[4].getIntValue();
            row.firstByte = tokens[5].getIntValue();
            row.minRange = tokens[6].getIntValue();
            row.maxRange = tokens[7].getIntValue();
            row.transformation = tokens[8].getIntValue();
            row.handling = tokens[9].getIntValue();
            row.minValue = tokens[10].getDoubleValue();
            row.maxValue = tokens[11].getDoubleValue();
            row.paramIndex = tokens[12].getIntValue();
            row.arrayIndex = tokens[13].getIntValue();

            // Hack & Slash solution on the final version
            // where every thing is set in stone use a
            // Look up table for the strings based on the
            // parameter indicices

            String destination = tokens[14];

            for(int i=15; i<tokens.size(); i++){
                destination += " " + tokens[i];
            }

            row.destination = destination;

            std::cout << destination << std::endl;

            return true;
        } else {
            return false;
        }
    }

    bool nameSortAscending(PresetRow const &lhs, PresetRow const &rhs){
        //Ignore case Comparison
        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? false : true;
    }

    bool nameSortDescending(PresetRow const &lhs, PresetRow const &rhs){
        //Ignore case Comparison
        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? true : false;
    }


    bool categorySortAscending(PresetRow const &lhs, PresetRow const &rhs){
        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? false : true;
        }

        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? false : true;
        }

        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? false : true;
    }

    bool categorySortDescending(const PresetRow &lhs, const PresetRow &rhs){
        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? true : false;
        }

        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? true : false;
        }

        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? true : false;
    }

    bool authorSortAscending(PresetRow const &lhs, PresetRow const &rhs){
        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? false : true;
        }

        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? false : true;
        }

        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? false : true;
    }

    bool authorSortDescending(const PresetRow &lhs, const PresetRow &rhs){
        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? true : false;
        }

        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? true : false;
        }
        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? true : false;
    }

    bool sortMidiRow(const MidiRow &lhs, const MidiRow &rhs){
       return (lhs.paramIndex < rhs.paramIndex) ? true : false;
    }

    // Load up default shapes

    const String lfo_default_shapes[8] = {"m 12 228 q 120 120 228 12",
                                          "m 12 228 q 12 12 120 12 120 228 228 228",
                                          "m 12 120 q 12 228 66 228 120 228 120 120 120 12 174 12 228 12 228 120",
                                          "m 12 120 q 39 174 66 228 93 174 120 120 147 66 174 12 201 66 228 120",
                                          "m 12 120 q 34 228 120 228 120 52 174 29 174 228 228 114",
                                          "m 12 120 q 34 141 34 228 51 66 66 12 85 228 120 120 146 68 174 120 202 168 228 126",
                                          "m 12 120 q 43 158 43 228 54.5 120 66 12 66 140 120 120 166 132 174 228 226 194 228 120",
                                          "m 12 120 q 12 228 66 228 120 228 120 120 130.5 110.5 150 12 151 91.5 174 120 188.5 120 201 228 210 120 228 120"};

    const String ws_default_shapes[8] = {"m 8 152 q 80 80 152 8",
                                         "m 8 152 q 41 152 80 80 115 10 152 8",
                                         "m 8 152 q 80 152 80 80 80 8 152 8",
                                         "m 8 152 q 31 152 44 152 80 152 80 80 80 8 116 8 126 8 152 8",
                                         "m 8 152 q 33 152 44 137 62 108.5 80 80 98 53 116 26 123 8 152 8",
                                         "m 8 152 q 26 144.5 44 137 62 108.5 80 80 98 53 116 26 134 17 152 8",
                                         "m 8 152 q 26 152 44 152 62 116 80 80 98 44 116 8 134 8 152 8",
                                         "m 8 152 q 77 147 80 80 88 8 116 8 134 8 152 8"};

    // Default UI Shapes
    const String env_ui_state = "loop 0 -1 -1 sus 1 0.5 0.75 end 1 0.75 1 ";
    const String step_seq_state = "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1";

    // Multiply with 60000/bpm to retrieve the right period for the
    //Multiply with bpm to retrieve the correct frequency for the oscillator
    //Use for the Low Frequency Oscillators and the Flanger
    const double frequency_ratios[24] = {0.000694444, 0.001041667, 0.0015625,     // 4ND, 4N, 4NT
                                         0.000925926, 0.001388889, 0.002083333,   // 3ND, 3N, 3NT
                                         0.001388889, 0.002083333, 0.003125,      // 2ND, 2N, 2NT
                                         0.002777778, 0.004166667, 0.00625,       // 1ND, 1N, 1NT
                                         0.005555556, 0.008333333, 0.0125,        // 1/2ND, 1/2N, 1/2NT
                                         0.011111112, 0.016666666, 0.025,         // 1/4ND, 1/4N, 1/4NT
                                         0.022222224, 0.033333332, 0.05,          // 1/8ND, 1/8N, 1/8NT
                                         0.044444448, 0.066666664, 0.1};          // 1/16ND 1/16N, 1/16NT

    // Use for the Envelopes
    const double env_timing_ratios[24] = {0.166666667,   0.250, 0.375,   // 1/16NT, 1/16N, 1/16ND
                                          0.333333333,   0.5,   0.75,    // 1/8NT, 1/8N, 1/8ND
                                          0.666666667,   1.0,   1.5,     // 1/4NT, 1/4N, 1/4ND
                                          1.333333333,   2.0,   3.0,     // 1/2NT, 1/2N, 1/2ND
                                          2.666666667,   4.0,   6.0,     // 1NT, 1N, 1ND
                                          5.333333334,   8.0,  12.0,     // 2NT, 2N, 2ND
                                          8.0,          12.0,  18.0,     // 3NT, 3N, 3ND
                                          10.666666668, 16.0,  24.0};     // 4NT, 4N, 4ND

    // Multiply with 60000/bpm to retrieve the right period for the
    // Use for the Step Sequencer
    const double sseq_timing_ratios[18] = {0.666666667,   1.0,   1.5,     // 1/4NT, 1/4N, 1/4ND
                                           1.333333333,   2.0,   3.0,     // 1/2NT, 1/2N, 1/2ND
                                           2.666666667,   4.0,   6.0,     // 1NT, 1N, 1ND
                                           5.333333334,   8.0,  12.0,     // 2NT, 2N, 2ND
                                           8.0,          12.0,  18.0,     // 3NT, 3N, 3ND
                                           10.666666668, 16.0,  24.0};     // 4NT, 4N, 4ND

    // Multiply with 60000/bpm to retrieve the right period for the
    // Stereo Delay, Ping pong Delay and LCR Delay
    const double delay_timing_ratios[24] =  {0.020833333, 0.03125, 0.046875, // 1/128NT, 1/128N, 1/128ND
                                             0.041666666, 0.0625, 0.09375,   // 1/64NT, 1/64N, 1/64ND
                                             0.083333334, 0.125, 0.1875,     // 1/32NT, 1/32N, 1/32ND
                                             0.166666667, 0.250, 0.375,      // 1/16NT, 1/16N, 1/16ND
                                             0.333333333, 0.5, 0.75,         // 1/8NT, 1/8N, 1/8ND
                                             0.666666667, 1.0, 1.5,          // 1/4NT, 1/4N, 1/4ND
                                             1.333333333, 2.0, 3.0,          // 1/2NT, 1/2N, 1/2ND
                                             2.666666667, 4.0, 6.0};         // 1NT, 1N, 1ND

    //Hold Delay Time Ratio
    const double h_delay_timing_ratios[18] = {0.020833333, 0.03125, 0.046875, // 1/128NT, 1/128N, 1/128ND
                                              0.041666666, 0.0625, 0.09375,   // 1/64NT, 1/64N, 1/64ND
                                              0.083333334, 0.125, 0.1875,     // 1/32NT, 1/32N, 1/32ND
                                              0.166666667, 0.250, 0.375,      // 1/16NT, 1/16N, 1/16ND
                                              0.333333333, 0.5, 0.75,         // 1/8NT, 1/8N, 1/8ND
                                              0.666666667, 1.0, 1.5};         // 1/4NT, 1/4N, 1/4ND

     //Hold Delay Latch Ratios, Use direct ratios for the ppq position
    const double h_delay_latch_ratios[15] = {0.166666667, 0.250, 0.375,      // 1/16NT, 1/16N, 1/16ND
                                             0.333333333, 0.5, 0.75,         // 1/8NT, 1/8N, 1/8ND
                                             0.666666667, 1.0, 1.5,          // 1/4NT, 1/4N, 1/4ND
                                             1.333333333, 2.0, 3.0,          // 1/2NT, 1/2N, 1/2ND
                                             2.666666667, 4.0, 6.0};         // 1NT, 1N, 1ND
}
