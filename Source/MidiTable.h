/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_9A1E0D7B1C4F6F14__
#define __JUCE_HEADER_9A1E0D7B1C4F6F14__

//[Headers]     -- You can add your own extra header files here --

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Akateko.h"

using namespace akateko;


//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiTable  : public TableListBox,
                   public TableListBoxModel,
                   public TextEditorListener
{
public:
    //==============================================================================
    MidiTable (AkatekoAudioProcessor &p);
    ~MidiTable();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int getNumRows() override;
    void paintRowBackground (Graphics &g,
                             int rowNumber,
                             int width, int height,
                             bool rowIsSelected) override;

    void paintCell(Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width, int height,
                   bool rowIsSelected) override;


    void initialiseHeader(float width, float height);
    void setLookAndFeel(LookAndFeel *laf);

    void cellClicked(int rowNumber,
                     int columnId,
                     const MouseEvent &event) override;

    void cellDoubleClicked(int rowNumber,
                           int columnId,
                           const MouseEvent &) override;

    void textEditorEscapeKeyPressed(TextEditor &tEdit) override;
    void textEditorReturnKeyPressed(TextEditor &tEdit) override;
    void textEditorFocusLost(TextEditor &tEdit) override;

    int getActiveRow();
    int getActiveColumn();
    Rectangle<int> getClickedCellPosition();

    // Keep order intact using it
    // it for array Indexing
    enum MidiSections{
        all = -2,
        none = -1,
        globalSection = 0,
        filterSection = 1,
        waveshapersection = 2,
        lfoOneSection = 3,
        lfoTwoSection = 4,
        envOneSection = 5,
        envTwoSection = 6,
        sseqSection = 7,
        xyPadSection = 8,
        fxSection = 9,
        sDelaySection = 10,
        ppDelaySection = 11,
        lcrDelaySection = 12,
        sFlangerSection = 13,
        dChorusSection = 14,
        hDelaySection = 15,
        decimatorSection = 16,
        pReverbSection = 17,
        nrOfSections
    };

    enum CommandIds {
        requestEditor
    };

    void calculateTimeDivision(double bpm);

    void restoreMidiRows();
    void displayActiveMidi();
    void displaySection(int section);
    void clearMidiRows();
    void clearAllMidiRows();

    //[/UserMethods]

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void setViewable(int section);
    void setMidiMessage(int section, int rowNumber, int message);
    void setMidiChannel(int section, int rowNumber, int channel);
    void setMidiFirstByte(int section, int rowNumber, int value);
    void setMidiMinValue(int section, int rowNumber, int value);
    void setMidiMaxValue(int section, int rowNumber, int value);

    bool getVariableMinMaxValues(double &minValue, double &maxValue, int &handling, int pIndex);

    int getSizeMainSection(int section);
    int getSizeSection(int section);

    void initialiseMidiRows();
    void initialiseMidiGlobal();
    void initialiseMidiFilter();
    void initialiseMidiWaveShaper();
    void initialiseMidiLFOOne();
    void initialiseMidiLFOTwo();
    void initialiseMidiEnvOne();
    void initialiseMidiEnvTwo();
    void initialiseMidiSSEQ();
    void initialiseMidiXYPad();
    void initialiseMidiFX();
    void initialiseMidiSDelay();
    void initialiseMidiPPDelay();
    void initialiseMidiLCRDelay();
    void initialiseMidiSFlanger();
    void initialiseMidiDChorus();
    void initialiseMidiHDelay();
    void initialiseMidiDecimator();
    void initialiseMidiPRverb();

    MidiRow global[MIDI_GL_SIZE];
    MidiRow filters[MIDI_F_SIZE];
    MidiRow waveShaper[MIDI_WS_SIZE]; // Bind The eight buttons to the same Parameter
    MidiRow lfoOne[MIDI_LFO1_SIZE];     // Bind the eight shape buttons and sync to the same Parameter
    MidiRow lfoTwo[MIDI_LFO2_SIZE];
    MidiRow envOne[MIDI_ENV1_SIZE];
    MidiRow envTwo[MIDI_ENV2_SIZE];
    MidiRow stepSeq[MIDI_SSEQ_SIZE];
    MidiRow xyPad[MIDI_XYPAD_SIZE];
    MidiRow fx[MIDI_FX_SIZE];
    //Fx Rows
    MidiRow sDelay[MIDI_SDEL_SIZE];
    MidiRow ppDelay[MIDI_PPDEL_SIZE];
    MidiRow lcrDelay[MIDI_LCRDEL_SIZE];
    MidiRow sFlanger[MIDI_SFLANG_SIZE];
    MidiRow dChorus[MIDI_DCHOR_SIZE];
    MidiRow hDelay[MIDI_HDEL_SIZE];
    MidiRow decimator[MIDI_DECIM_SIZE];
    MidiRow pReverb[MIDI_PREV_SIZE];

    MidiRow *midiParameters[nrOfSections] = {nullptr};
    int sizes[nrOfSections] = {0};

    std::vector<MidiRow> viewable; //push_back? or pointers to the array
    std::vector<MidiRow> bound;   //Place the MidiRows that are added here

    int activeSection = none;

    StringArray midiMessages;
    StringArray headerOptions;

    int currentMidiMessage = -1;
    int nrOfRows = 0;
    int activeRow = 0;
    int activeCol = 0;
    float cellHeight = 0.f;
    float cellWidth = 0.f;

    double beatsPerMinute = 120.0;
    double timeDivisions[6][2];

    PopupMenu menu;

    Rectangle<int> cellPosition;
    Colour outlineColour;
    Colour highLighColour;

    ScopedPointer<TextEditor> textEditor;
    AkatekoAudioProcessor &processor;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiTable)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9A1E0D7B1C4F6F14__
