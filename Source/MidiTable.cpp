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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MidiTable.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using akateko::LFOONEMIDI;
using akateko::MIDIHANDLING::MIDI_TO_INT;
using akateko::MIDIHANDLING::MIDI_TO_INT_BUTTON;
using akateko::MIDIHANDLING::MIDI_TO_DOUBLE;
using akateko::MIDIHANDLING::MIDI_TO_CONSTANT;

using akateko::frequency_ratios;
using akateko::env_timing_ratios;
using akateko::sseq_timing_ratios;
using akateko::delay_timing_ratios;
using akateko::h_delay_timing_ratios;
using akateko::h_delay_latch_ratios;

//[/MiscUserDefs]

//==============================================================================
MidiTable::MidiTable(AkatekoAudioProcessor &p) :
    processor(p),
    outlineColour(Colour(0xFF70C099)),
    highLighColour(Colour(0x6F20AA9A))
{
    //[Constructor_pre] You can add your own custom stuff here.
    midiMessages.add(String::empty);
    midiMessages.add("Note");
    midiMessages.add("Controller");
    midiMessages.add("Pitch Bend");
    midiMessages.add("None");

    // Build pupup Menu
    menu.addItem(1, midiMessages[1], true, false);
    menu.addItem(2, midiMessages[2], true, false);
    menu.addItem(3, midiMessages[3], true, false);
    menu.addSeparator();
    menu.addItem(-1, midiMessages[4], true, false);

    // Header Options
    headerOptions.clear();

    headerOptions.add("Note Number");
    headerOptions.add("Velocity Off Value");
    headerOptions.add("Velocity On Value");
    headerOptions.add("Controller Number");
    headerOptions.add("Controller Min Value");
    headerOptions.add("Controller Max Value");
    headerOptions.add("Pitch Bend Mapping");
    headerOptions.add("Lower Range");
    headerOptions.add("Upper Range");

    initialiseMidiRows();

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setVisible(false);
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (false);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setInputRestrictions(4, "0123456789-");
    textEditor->addListener(this);
    //[/Constructor_pre]
    //[UserPreSize]

    //[/UserPreSize]

    setSize (600, 400);

    //[Constructor] You can add your own custom stuff here..
    setColour(TableListBox::backgroundColourId, Colours::black);

    //Restore MIDI Rows


    // Bind to midiParameter which holds pointers to
    // MidiRows use it to bind arrays to a specific
    // Offset, this will eliminate allot of
    // conditional branching with switch cases

    double bpm = processor.getBeatsPerMinute();

    if(bpm <= 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;

    calculateTimeDivision(beatsPerMinute);

    restoreMidiRows();
    //[/Constructor]
}

MidiTable::~MidiTable()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    for(int i=0; i<nrOfSections; i++){
        midiParameters[i] = nullptr;
    }

    textEditor = nullptr;
    //[/Destructor_pre]


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

int MidiTable::getNumRows()
{
    return nrOfRows;
}

void MidiTable::displayActiveMidi(){
    viewable.clear();
    activeSection = all;
    viewable =  processor.getMidiRows();
    nrOfRows = viewable.size();

    updateContent();
    repaint();
}

void MidiTable::displaySection(int section){
    viewable.clear();

    if(section >= globalSection &&
       section <= xyPadSection)
    {
         activeSection = section;
         setViewable(section);

         nrOfRows = viewable.size();

    } else if(section == fxSection){

        activeSection = section;
        setViewable(fxSection);

        nrOfRows = viewable.size();

    } else if(section >= sDelaySection &&
              section <= pReverbSection ||
              section == none)
    {
        activeSection = section;
        setViewable(fxSection);         

        MidiRow empty;

        if(section != none){
            viewable.push_back(empty);
            nrOfRows++;
        }

        setViewable(section);

        nrOfRows = viewable.size();
    }

    updateContent();
    repaint();
}

void MidiTable::clearMidiRows(){
    for(int i=0; i<viewable.size(); i++){
        if(viewable[i].midiMessage != -1){
            const int arrayIndex = viewable[i].arrayIndex;
            const int section = viewable[i].section;

            setMidiMessage(section, arrayIndex, -1);

            if(midiParameters[section] != nullptr &&
               arrayIndex < sizes[section])
            {
                viewable[i] = midiParameters[section][arrayIndex];
            }
        }
    }

    if(activeSection == all){
        viewable.clear();
        nrOfRows = 0;
    }

    updateContent();
    repaint();
}

void MidiTable::clearAllMidiRows(){
    vector<MidiRow> tmpRows = processor.getMidiRows();
    processor.clearAllMidiRows();

    for(int i=0; i< tmpRows.size(); i++){
        const int tmpSection = tmpRows[i].section;
        const int arrayIndex = tmpRows[i].arrayIndex;

        setMidiMessage(tmpSection, arrayIndex, -1);
    }

    if(activeSection == all){
        viewable.clear();
        nrOfRows = 0;
    } else {
        displaySection(activeSection);
    }

    updateContent();
    repaint();
}


// First set One Row later device a scheme for displaying multiple
// Rows
void MidiTable::setViewable(int section){
    const int size = sizes[section];

    //Work Around for not displaying the
    //Parameters in the section which aren't
    //bound

    bound = processor.getMidiRows(); //Get Current Midi Messages from the processor

    //Clear all messages
    for(int i=0; i<size; i++){
        midiParameters[section][i].midiMessage = akateko::MIDI_NONE;
    }

    for(int i=0; i<bound.size(); i++){
        if(bound[i].section == section &&
           bound[i].midiMessage != akateko::MIDI_NONE)
        {
           const int aIndex = bound[i].arrayIndex;
           midiParameters[section][aIndex].midiMessage = bound[i].midiMessage ;
        }
    }

    if(midiParameters[section] != nullptr){
        for(int i=0; i<size; i++){          
           viewable.push_back(midiParameters[section][i]);
        }
    }
}

void MidiTable::restoreMidiRows(){
    bound = processor.getMidiRows();

    if(!bound.empty()){
        for(int i=0; i<bound.size(); i++){
            int tmpSection = bound[i].section;
            int tmpIndex = bound[i].arrayIndex;
            int tmpSize = sizes[tmpSection];

            if(midiParameters[tmpSection] != nullptr &&
               tmpIndex < tmpSize)
            {
                midiParameters[tmpSection][tmpIndex] = bound[i];
            }
        }
    }
}


void MidiTable::setMidiMessage(int section, int rowNumber, int message){
    int size = sizes[section];

    if(rowNumber >= 0 &&
       rowNumber < size &&
       midiParameters[section] != nullptr)
    {
        midiParameters[section][rowNumber].midiMessage = message;

        // First Time Binding
        if(!midiParameters[section][rowNumber].initialised){
            midiParameters[section][rowNumber].channel = 1;
            midiParameters[section][rowNumber].minRange = 0;
            midiParameters[section][rowNumber].maxRange = 127;

            double minValue = 0;
            double maxValue = 0;
            int handling = 0;
            const int pIndex = midiParameters[section][rowNumber].paramIndex;

            if(getVariableMinMaxValues(minValue, maxValue, handling,  pIndex)){
                midiParameters[section][rowNumber].minValue = minValue;
                midiParameters[section][rowNumber].maxValue = maxValue;
                midiParameters[section][rowNumber].handling = handling;
            }

            if(message == akateko::MIDI_NOTE){
                midiParameters[section][rowNumber].firstByte = 48;
            } else if(message == akateko::MIDI_CC){
                midiParameters[section][rowNumber].firstByte = 12;
            }

            midiParameters[section][rowNumber].initialised = true;
            //Update table in the processor
        } else {
            double minValue = 0;
            double maxValue = 0;
            int handling = 0;
            const int pIndex = midiParameters[section][rowNumber].paramIndex;

            if(getVariableMinMaxValues(minValue, maxValue, handling, pIndex)){
                midiParameters[section][rowNumber].minValue = minValue;
                midiParameters[section][rowNumber].maxValue = maxValue;
                midiParameters[section][rowNumber].handling = handling;
            }
        }

        if(message == -1){
            // Remove Message

            processor.removeMidiRow (midiParameters[section][rowNumber].arrayIndex,
                                     midiParameters[section][rowNumber].paramIndex);
        } else if(message > 0){

            processor.setMidiRow(midiParameters[section][rowNumber]);
        }
    }
}

void MidiTable::setMidiChannel(int section, int rowNumber, int channel){
    const int size = sizes[section];

    if(rowNumber < size && midiParameters[section] != nullptr){

        if(channel < 1){
            channel = 1;
        } else if(channel > 16){
            channel = 16;
        }

        midiParameters[section][rowNumber].channel = channel;
        //Update table in the processor
        processor.setMidiRow(midiParameters[section][rowNumber]);
    }
}

void MidiTable::setMidiFirstByte(int section, int rowNumber, int value){
    const int size = sizes[section];

    if(rowNumber < size && midiParameters[section] != nullptr){
        if(value > 127){
            value = 127;
        }
        midiParameters[section][rowNumber].firstByte = value;
        //Update table in the processor
        processor.setMidiRow(midiParameters[section][rowNumber]);
    }
}

void MidiTable::setMidiMinValue(int section, int rowNumber, int value){
    const int size = sizes[section];

    if(rowNumber < size && midiParameters[section] != nullptr){
        const int tmpMax = midiParameters[section][rowNumber].maxRange;

        if(value >= tmpMax){
            value = tmpMax;
        }

        midiParameters[section][rowNumber].minRange = value;
        //Update table in the processor
        processor.setMidiRow(midiParameters[section][rowNumber]);
    }
}

void MidiTable::setMidiMaxValue(int section, int rowNumber, int value){
    const int size = sizes[section];

    if(rowNumber < size && midiParameters[section] != nullptr){
        const int tmpMin = midiParameters[section][rowNumber].minRange;

        if(value <= tmpMin){
            value = tmpMin;
        }
        midiParameters[section][rowNumber].maxRange = value;
        //Update table in the processor
        midiParameters[section][rowNumber].maxRange = value;
    }
}

int MidiTable::getSizeSection(int section){
    int result = 0;

    switch(section){
        case fxSection: result = MIDI_FX_SIZE;
        case sDelaySection: result = MIDI_SDEL_SIZE;
        case ppDelaySection: result = MIDI_PPDEL_SIZE;
        case lcrDelaySection: result = MIDI_ENV1_SIZE;
        case sFlangerSection: result = MIDI_SFLANG_SIZE;
        case dChorusSection: result = MIDI_DCHOR_SIZE;
        case hDelaySection: result = MIDI_HDEL_SIZE;
        case decimatorSection: result = MIDI_DECIM_SIZE;
        case pReverbSection: result = MIDI_PREV_SIZE;
    }

    return result;
}

void MidiTable::paintRowBackground(Graphics &g,
                                   int rowNumber,
                                   int width,
                                   int height,
                                   bool rowIsSelected)
{
    Colour colour = Colours::black;

    if(rowIsSelected){
        colour = highLighColour;

        if(activeSection >= sDelaySection &&
           activeSection <= pReverbSection &&
           rowNumber == 3)
        {
            colour = Colours::black;
        } else {
            colour = highLighColour;
        }
    }

    g.setColour(colour);
    g.fillAll();
}

void MidiTable::paintCell(Graphics &g,
                          int rowNumber,
                          int columnId,
                          int width,
                          int height,
                          bool rowIsSelected)
{
    g.setColour(outlineColour);
    g.fillRect(0, height-1, width, 1);

    String tmpCell;

    if(rowNumber < viewable.size()){
        if(viewable[rowNumber].midiMessage == -1){

            if(!(activeSection >= sDelaySection &&
               activeSection <= pReverbSection &&
               rowNumber == 3))
            {
                if(columnId == 1){
                    g.fillRect(0, 0, 1, height);
                } else if(columnId == 6){
                    tmpCell = viewable[rowNumber].destination;
                    g.drawText(tmpCell, 0, 0, width, height, Justification::centredLeft);
                }


                g.fillRect(width-1, 0, 1, height);
            }
        } else if(viewable[rowNumber].destination.isNotEmpty()){
                switch(columnId){
                    case 1:
                        g.fillRect(0, 0, 1, height);
                        tmpCell = midiMessages[viewable[rowNumber].midiMessage];
                        g.drawText(tmpCell, 0, 0, width, height, Justification::centred);
                        break;
                    case 2:
                        tmpCell = String(viewable[rowNumber].channel);
                        g.drawText(tmpCell, 0, 0, width, height, Justification::centred);
                        break;
                    case 3:
                        tmpCell = String(viewable[rowNumber].firstByte);
                        g.drawText(tmpCell, 0, 0, width, height, Justification::centred);
                        break;
                    case 4:
                        tmpCell = String(viewable[rowNumber].minRange);
                        g.drawText(tmpCell, 0, 0, width, height, Justification::centred);
                        break;
                    case 5:
                        tmpCell = String(viewable[rowNumber].maxRange);
                        g.drawText(tmpCell, 0, 0, width, height, Justification::centred);
                        break;
                    case 6:
                        tmpCell = viewable[rowNumber].destination;
                        g.drawText(tmpCell, 0, 0, width, height, Justification::centredLeft);
                        break;
                }
            g.fillRect(width-1, 0, 1, height);
         }
     }
}

void MidiTable::cellClicked(int rowNumber,
                            int columnId,
                            const MouseEvent &event)
{
    ModifierKeys modifier = event.mods;
    activeRow = rowNumber;


    if(modifier.isRightButtonDown() &&
       columnId == 1 &&
       midiParameters[activeSection] != nullptr)
    {
        int midiMessage;

        if(activeSection >= sDelaySection &&
           activeSection <= pReverbSection &&
           rowNumber == 3)
        {
            midiMessage = -1;
        } else {
            midiMessage = menu.show();
        }

        if(midiMessage == 0){
            return;
        }

        int size = sizes[activeSection];

        if(activeSection == all &&
           rowNumber < viewable.size())
        {
           int tmpSection = viewable[rowNumber].section;
           int tmpIndex = viewable[rowNumber].arrayIndex;

           //std::cout << "Active Section" << tmpSection << std::endl;
          // std::cout << "Array Index" << tmpIndex << std::endl;

           setMidiMessage(tmpSection, tmpIndex, midiMessage);

           viewable.clear();
           viewable = processor.getMidiRows();
           nrOfRows = viewable.size();

        } else if(activeSection >= globalSection &&
           activeSection <= xyPadSection &&
           rowNumber < size)
        {
           setMidiMessage(activeSection, rowNumber, midiMessage);
           viewable[rowNumber] =  midiParameters[activeSection][rowNumber];
        }

        else if(activeSection == fxSection &&
                  rowNumber < MIDI_FX_SIZE)
        {
            setMidiMessage(activeSection, rowNumber, midiMessage);
            viewable[rowNumber] =  midiParameters[activeSection][rowNumber];
        }


        else if(activeSection >= sDelaySection &&
                activeSection <= pReverbSection)
        {
            if(rowNumber < MIDI_FX_SIZE && midiParameters[fxSection] != nullptr){
                setMidiMessage(fxSection, rowNumber, midiMessage);
                viewable[rowNumber] =  midiParameters[fxSection][rowNumber];
            }

            else if(rowNumber > MIDI_FX_SIZE &&
                    rowNumber < viewable.size())
            {
                int tmpRowNumber = rowNumber-4;

                setMidiMessage(activeSection, tmpRowNumber, midiMessage);
                viewable[rowNumber] =  midiParameters[activeSection][tmpRowNumber];
            }
        }

        updateContent();
        repaint();
    }

        //Set Header Names
    int tmpMidiMessage = 0;

    if(!viewable.empty()){
        tmpMidiMessage = viewable[rowNumber].midiMessage;
    }

    if(currentMidiMessage != tmpMidiMessage &&
       tmpMidiMessage != -1)
    {

        currentMidiMessage = tmpMidiMessage;

        std::cout << currentMidiMessage << std::endl;

        switch(currentMidiMessage){
            case MIDI_NOTE:
                getHeader().setColumnName(3, headerOptions[0]);
                getHeader().setColumnName(4, headerOptions[1]);
                getHeader().setColumnName(5, headerOptions[2]);
                break;
            case MIDI_CC:
                getHeader().setColumnName(3, headerOptions[3]);
                getHeader().setColumnName(4, headerOptions[4]);
                getHeader().setColumnName(5, headerOptions[5]);
                break;
            case MIDI_PB:
                getHeader().setColumnName(3, headerOptions[6]);
                getHeader().setColumnName(4, headerOptions[7]);
                getHeader().setColumnName(5, headerOptions[8]);
                break;
        }
        updateContent();
        repaint();
    }
}

void MidiTable::cellDoubleClicked(int rowNumber,
                                  int columnId,
                                  const MouseEvent &)
{
    if(rowNumber < viewable.size() &&
       columnId >= 2 &&
       columnId <= 5)
    {        
        cellPosition = getCellPosition(columnId, rowNumber, true);
        cellPosition.removeFromBottom(1);
        cellPosition.removeFromRight(1);

        activeRow = rowNumber;
        activeCol = columnId;

        textEditor->setBounds(cellPosition);
        textEditor->setVisible(true);
        textEditor->grabKeyboardFocus();
    }
}

void MidiTable::initialiseHeader(float width, float height)
{
    cellWidth = width*0.167;
    cellHeight = 20;

    getHeader().addColumn("MIDI Message", 1, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn("MIDI Channel", 2, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn(headerOptions[0], 3, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn(headerOptions[1], 4, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn(headerOptions[2], 5, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);
    getHeader().addColumn("Destination", 6, cellWidth, cellWidth, -1, TableHeaderComponent::visible, -1);

    getHeader().setStretchToFitActive(true);
    getHeader().setPopupMenuActive(false);
    getViewport()->setScrollBarsShown(false,false,true,false); //enable mouse scrolling for now

    setHeaderHeight(cellHeight);
    setRowHeight(cellHeight);
    setModel(this);
}

void MidiTable::setLookAndFeel(LookAndFeel *laf){
    TableListBox::setLookAndFeel(laf);
    getHeader().setLookAndFeel(laf);
    menu.setLookAndFeel(laf);
}

void MidiTable::textEditorEscapeKeyPressed(TextEditor &tEdit){
    grabKeyboardFocus();
    tEdit.clear();
    tEdit.setVisible(false);
}

void MidiTable::textEditorReturnKeyPressed(TextEditor &tEdit){
    String tmpText = tEdit.getText();

    textEditor->clear();
    textEditor->setVisible(false);

    if(activeSection == all){
        const int tmpValue = abs(tmpText.getIntValue());
        const int tmpSection = viewable[activeRow].section;
        const int tmpIndex = viewable[activeRow].arrayIndex;

        switch(activeCol){
            case 2:
                setMidiChannel(tmpSection, tmpIndex, tmpValue);
                break;
            case 3:
                setMidiFirstByte(tmpSection, tmpIndex, tmpValue);
                break;
            case 4:
                setMidiMinValue(tmpSection, tmpIndex, tmpValue);
                break;
            case 5:
                setMidiMaxValue(tmpSection, tmpIndex, tmpValue);
                break;
        }

        if(midiParameters[tmpSection] != nullptr &&
           tmpIndex < sizes[tmpSection])
        {
            viewable[activeRow] = midiParameters[tmpSection][tmpIndex];
        }

        updateContent();
        repaint();

    } else if(activeSection >= globalSection &&
        activeSection <= fxSection)
    {
        const int tmpValue = abs(tmpText.getIntValue());
        const int tmpSize = sizes[activeSection];

        switch(activeCol){
            case 2:
                setMidiChannel(activeSection, activeRow, tmpValue);
                break;
            case 3:
                setMidiFirstByte(activeSection, activeRow, tmpValue);
                break;
            case 4:
                setMidiMinValue(activeSection, activeRow, tmpValue);
                break;
            case 5:
                setMidiMaxValue(activeSection, activeRow, tmpValue);
                break;
        }

        if(activeRow < tmpSize && midiParameters[activeSection] != nullptr){
            viewable[activeRow] = midiParameters[activeSection][activeRow];
        }

        updateContent();
        repaint();

    } else if(activeSection >= sDelaySection &&
        activeSection <= pReverbSection)
    {
        const int tmpValue = abs(tmpText.getIntValue());
        const int tmpSize = sizes[activeSection];
        const int fxRow = activeRow-4;

        switch(activeCol){
            case 2:
                setMidiChannel(activeSection, fxRow, tmpValue);
                break;
            case 3:
                setMidiFirstByte(activeSection, fxRow, tmpValue);
                break;
            case 4:
                setMidiMinValue(activeSection, fxRow, tmpValue);
                break;
            case 5:
                setMidiMaxValue(activeSection, fxRow, tmpValue);
                break;
        }

        if(fxRow < tmpSize &&
           activeRow < viewable.size() &&
           midiParameters[activeSection] != nullptr)

        {
            viewable[activeRow] = midiParameters[activeSection][fxRow];
        }

        updateContent();
        repaint();
    }

}

void MidiTable::textEditorFocusLost(TextEditor &tEdit){
    tEdit.setText("");
    tEdit.setVisible(false);
}

void MidiTable::initialiseMidiRows(){
    initialiseMidiGlobal();
    initialiseMidiFilter();
    initialiseMidiWaveShaper();
    initialiseMidiLFOOne();
    initialiseMidiLFOTwo();
    initialiseMidiEnvOne();
    initialiseMidiEnvTwo();
    initialiseMidiSSEQ();
    initialiseMidiXYPad();
    initialiseMidiFX();
    initialiseMidiSDelay();
    initialiseMidiPPDelay();
    initialiseMidiLCRDelay();
    initialiseMidiSFlanger();
    initialiseMidiDChorus();
    initialiseMidiHDelay();
    initialiseMidiDecimator();
    initialiseMidiPRverb();
}

void MidiTable::calculateTimeDivision(double bpm){
    if(bpm > 0){
        const double minOvrBpm = 60000/bpm;

        //timeDivisions[5][2];

        // Frequency Ratio use for LFO's and FLanger
        timeDivisions[0][0] = bpm*frequency_ratios[0];
        timeDivisions[0][1] = bpm*frequency_ratios[23];

        // Envelope Ratios;
        timeDivisions[1][0] = minOvrBpm*env_timing_ratios[0];
        timeDivisions[1][1] = minOvrBpm*env_timing_ratios[23];

        // Step Sequencer
        timeDivisions[2][0] = minOvrBpm*sseq_timing_ratios[0];
        timeDivisions[2][1] = minOvrBpm*sseq_timing_ratios[17];

        // S-Delay, PP-Delay and LCR-Delays
        timeDivisions[3][0] = minOvrBpm*delay_timing_ratios[0];
        timeDivisions[3][1] = minOvrBpm*delay_timing_ratios[23];

        // H-Delay Loop
        timeDivisions[4][0] = minOvrBpm*h_delay_timing_ratios[0];
        timeDivisions[4][1] = minOvrBpm*h_delay_timing_ratios[17];

        // H-Delay Latch
        timeDivisions[5][0] = minOvrBpm*h_delay_latch_ratios[0];
        timeDivisions[5][1] = minOvrBpm*h_delay_latch_ratios[14];
    }
}

// Need to distinguish by the individual parameters and
// call the appropiate sync AudioProcessor Parameters
// On a later point add additional audioprocessor parameters
// and delegate the correct handling to the dsp classes
bool  MidiTable::getVariableMinMaxValues(double &minValue,
                                        double &maxValue,
                                        int &handling,
                                        int pIndex)
{
    // LFO 1, LFO 2 and Flanger
    if(pIndex == 27 || pIndex == 35 || pIndex == 111){
        int syncIndex = pIndex-1;
        bool sync = processor.getParameter(syncIndex);

        if(sync){
            minValue = 0;
            maxValue = 23;
            handling = akateko::MIDI_TO_INT;
        } else if(pIndex == 27 || pIndex == 35){
            minValue = 0.0001;
            maxValue = 20.0;
            handling = akateko::MIDI_TO_DOUBLE;
        } else {
            minValue = timeDivisions[0][0];
            maxValue = timeDivisions[0][1];
            handling = akateko::MIDI_TO_DOUBLE;
        }

        return true;
    }
    // Envelope 1, Envelope 2
    else if(pIndex == 42 || pIndex == 50){
        const int syncIndex = pIndex-1;
        const bool sync = processor.getParameter(syncIndex);

        if(sync){
            minValue = 0;
            maxValue = 23;
            handling = akateko::MIDI_TO_INT;
        } else {
            minValue = timeDivisions[1][0];
            maxValue = timeDivisions[1][1];
            handling = akateko::MIDI_TO_DOUBLE;
        }

        return true;
    }
    // Step Sequencer
    else if(pIndex == 59){
        const bool sync = processor.getParameter(57);

        if(sync){
            minValue = 0;
            maxValue = 17;
            handling = akateko::MIDI_TO_INT;
        } else {
            minValue = 50.0;
            maxValue = 5000.0;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        return true;
    }
    // S-Delay, PP-Delay and LCR-Delay
    else if(pIndex == 76 || pIndex == 79 ||
              pIndex == 83 || pIndex == 87 ||
              pIndex == 92 || pIndex == 96 ||
              pIndex == 105)
    {
        const int syncIndex = pIndex-1;
        const bool sync = processor.getParameter(syncIndex);

        if(sync){
            minValue = 0;
            maxValue = 23;
            handling = akateko::MIDI_TO_INT;
        } else {
            minValue = timeDivisions[3][0];
            maxValue = timeDivisions[3][1];
            handling = akateko::MIDI_TO_DOUBLE;
        }

        return true;
    }
    // H-Delay Loop Timing
    else if(pIndex == 128){
        const int syncIndex = pIndex-1;
        const bool sync = processor.getParameter(syncIndex);

        if(sync){
            minValue = 0;
            maxValue = 17;
            handling = akateko::MIDI_TO_INT;
        } else {
            minValue = timeDivisions[4][0];
            maxValue = timeDivisions[4][1];
            handling = akateko::MIDI_TO_DOUBLE;
        }

        return true;
    }
    // H-Delay Latch
    else if(pIndex == 139){
        minValue = 0;
        maxValue = 14;
        handling = akateko::MIDI_TO_INT;

        return true;
    }

    return false;
}


void MidiTable::initialiseMidiGlobal(){
    initMidiRow(global[MIDI_GL_INPUT], MIDI_GL_INPUT, 0.0, 1.0, 0, MIDI_TO_DOUBLE, " Global In Volume", globalSection);
    initMidiRow(global[MIDI_GL_OUTPUT], MIDI_GL_OUTPUT, 0.0, 1.0, 1, MIDI_TO_DOUBLE, " Global Out Volume", globalSection);

    midiParameters[globalSection] = global;
    sizes[globalSection] = MIDI_GL_SIZE;
}

void MidiTable::initialiseMidiFilter(){
    //Global 'Bypass'
    initMidiRow(filters[MIDI_F_ENABLE], MIDI_F_ENABLE, 0, 1, 2, MIDI_TO_INT_TOGGLE, " Filters Enable", filterSection);
    //Filter Ones
    initMidiRow(filters[MIDI_F1_ENABLE], MIDI_F1_ENABLE, 0, 1, 4, MIDI_TO_INT_TOGGLE," Filter 1 Enable", filterSection);
    initMidiRow(filters[MIDI_F1_TYPE], MIDI_F1_TYPE, 0, 2, 5, MIDI_TO_INT, " Filter 1 Type", filterSection);
    initMidiRow(filters[MIDI_F1_ROFF], MIDI_F1_ROFF, 0, 1, 6, MIDI_TO_INT, " Filter 1 Roll Off", filterSection);
    initMidiRow(filters[MIDI_F1_FREQ], MIDI_F1_FREQ, 0.0, 1.0, 7, MIDI_TO_DOUBLE, " Filter 1 Freq", filterSection);
    initMidiRow(filters[MIDI_F1_RESO], MIDI_F1_RESO,0.0, 1.0, 8, MIDI_TO_DOUBLE, " Filter 1 Reso", filterSection);
    initMidiRow(filters[MIDI_F1_PBG], MIDI_F1_PBG, 0.0, 1.0, 9, MIDI_TO_DOUBLE, " Filter 1 PBG", filterSection);
    initMidiRow(filters[MIDI_F1_DRIVE], MIDI_F1_DRIVE, 0.0, 1.0, 10, MIDI_TO_DOUBLE, " Filter 1 Drive", filterSection);
    initMidiRow(filters[MIDI_F1_VOLUME], MIDI_F1_VOLUME, 0.0, 1.0, 11, MIDI_TO_DOUBLE, " Filter 1 Vol", filterSection);
    //Filter Two
    initMidiRow(filters[MIDI_F2_ENABLE], MIDI_F2_ENABLE, 0, 1, 12, MIDI_TO_INT_BUTTON, " Filter 2 Enable", filterSection);
    initMidiRow(filters[MIDI_F2_TYPE], MIDI_F2_TYPE, 0, 2, 13, MIDI_TO_INT, " Filter 2 Type", filterSection);
    initMidiRow(filters[MIDI_F2_ROFF], MIDI_F2_ROFF, 0, 1, 14, MIDI_TO_INT_BUTTON, " Filter 2 Roll Off", filterSection);
    initMidiRow(filters[MIDI_F2_FREQ], MIDI_F2_FREQ, 0.0, 1.0, 15, MIDI_TO_DOUBLE, " Filter 2 Freq", filterSection);
    initMidiRow(filters[MIDI_F2_RESO], MIDI_F2_RESO, 0.0, 1.0, 16, MIDI_TO_DOUBLE, " Filter 2 Reso", filterSection);
    initMidiRow(filters[MIDI_F2_PBG], MIDI_F2_PBG, 0.0, 1.0, 17, MIDI_TO_DOUBLE, " Filter 2 PGB", filterSection);
    initMidiRow(filters[MIDI_F2_DRIVE], MIDI_F2_DRIVE, 0.0, 1.0, 18, MIDI_TO_DOUBLE, " Filter 2 Drive", filterSection);
    initMidiRow(filters[MIDI_F2_VOLUME], MIDI_F2_VOLUME, 0.0, 1.0, 19, MIDI_TO_DOUBLE, " Filter 2 Vol", filterSection);

    midiParameters[filterSection] = filters;
    sizes[filterSection] = MIDI_F_SIZE;
}

void MidiTable::initialiseMidiWaveShaper(){
    initMidiRow(waveShaper[MIDI_WS_ENABLE], MIDI_WS_ENABLE, 0, 1, 20, MIDI_TO_INT_TOGGLE, " WShaper Enable", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_DRIVE], MIDI_WS_DRIVE, 0.0, 1.0, 21, MIDI_TO_DOUBLE, " WShaper Drive", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_MIX], MIDI_WS_MIX, 0.0, 1.0, 22, MIDI_TO_DOUBLE, " WShaper Mix", waveshapersection);
    // Select a specific shape by sending a value from 0 to 7 to the shape param[23]
    // Should be Handled differently in the MidiProcessor
    initMidiRow(waveShaper[MIDI_WS_SHAPE_1], MIDI_WS_SHAPE_1, 0, 0, 23, MIDI_TO_CONSTANT, " WShaper Shape 1", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_SHAPE_2], MIDI_WS_SHAPE_2, 1, 1, 23, MIDI_TO_CONSTANT, " WShaper Shape 2", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_SHAPE_3], MIDI_WS_SHAPE_3, 2, 2, 23, MIDI_TO_CONSTANT, " WShaper Shape 3", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_SHAPE_4], MIDI_WS_SHAPE_4, 3, 3, 23, MIDI_TO_CONSTANT, " WShaper Shape 4", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_SHAPE_5], MIDI_WS_SHAPE_5, 4, 4, 23, MIDI_TO_CONSTANT, " WShaper Shape 5", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_SHAPE_6], MIDI_WS_SHAPE_6,5, 5, 23, MIDI_TO_CONSTANT,  " WShaper Shape 6", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_SHAPE_7], MIDI_WS_SHAPE_7, 6, 6, 23, MIDI_TO_CONSTANT, " WShaper Shape 7", waveshapersection);
    initMidiRow(waveShaper[MIDI_WS_SHAPE_8], MIDI_WS_SHAPE_8, 7, 7, 23, MIDI_TO_CONSTANT, " WShaper Shape 8", waveshapersection);

    midiParameters[waveshapersection] = waveShaper;
    sizes[waveshapersection] = MIDI_WS_SIZE;
}

void MidiTable::initialiseMidiLFOOne(){
    initMidiRow(lfoOne[MIDI_LFO1_ENABLE], MIDI_LFO1_ENABLE, 0, 1, 24, MIDI_TO_INT_TOGGLE, " LFO One Enable", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_ONESHOT], MIDI_LFO1_ONESHOT, 0, 1, 25, MIDI_TO_INT_TOGGLE, " LFO One OneShot", lfoOneSection);
    //Free and Sync bound to one parameter, Handled differently in the MidiProcessor
    initMidiRow(lfoOne[MIDI_LFO1_FREE], MIDI_LFO1_FREE, 0, 0, 26, MIDI_TO_CONSTANT, " LFO One Free", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SYNC], MIDI_LFO1_SYNC, 1, 1, 26, MIDI_TO_CONSTANT, " LFO One Sync", lfoOneSection);
    // Flexible use the current Free or sync setting
    initMidiRow(lfoOne[MIDI_LFO1_FREQ], MIDI_LFO1_FREQ, 0.0, 20.0, 27, MIDI_TO_DOUBLE, " LFO One Freq", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_PHASE], MIDI_LFO1_PHASE, 0, 1, 28, MIDI_TO_DOUBLE, " LFO One Phase", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_PWM], MIDI_LFO1_PWM, 0, 1.0, 29, MIDI_TO_DOUBLE, " LFO One PWM", lfoOneSection);
    // Select a specific shape by sending a value from 0 to 7 to the shape param[23]
    // Should be Handled differently in the MidiProcessor
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_1], MIDI_LFO1_SHAPE_1, 0, 0, 30, MIDI_TO_CONSTANT, " LFO One Shape 1", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_2], MIDI_LFO1_SHAPE_2, 1, 1, 30, MIDI_TO_CONSTANT, " LFO One Shape 2", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_3], MIDI_LFO1_SHAPE_3, 2, 2, 30, MIDI_TO_CONSTANT, " LFO One Shape 3", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_4], MIDI_LFO1_SHAPE_4, 3, 3, 30, MIDI_TO_CONSTANT, " LFO One Shape 4", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_5], MIDI_LFO1_SHAPE_5, 4, 4, 30, MIDI_TO_CONSTANT, " LFO One Shape 5", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_6], MIDI_LFO1_SHAPE_6, 5, 5, 30, MIDI_TO_CONSTANT, " LFO One Shape 6", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_7], MIDI_LFO1_SHAPE_7, 6, 6, 30, MIDI_TO_CONSTANT, " LFO One Shape 7", lfoOneSection);
    initMidiRow(lfoOne[MIDI_LFO1_SHAPE_8], MIDI_LFO1_SHAPE_8, 7, 7, 30, MIDI_TO_CONSTANT, " LFO One Shape 8", lfoOneSection);

    initMidiRow(lfoOne[MIDI_LFO1_SMOOTH], MIDI_LFO1_SMOOTH, 0, 1.0, 31, MIDI_TO_DOUBLE, " LFO One Smooth", lfoOneSection);

    midiParameters[lfoOneSection] = lfoOne;
    sizes[lfoOneSection] = MIDI_LFO1_SIZE;
}

void MidiTable::initialiseMidiLFOTwo(){
    initMidiRow(lfoTwo[MIDI_LFO2_ENABLE], MIDI_LFO2_ENABLE, 0, 1, 32, MIDI_TO_INT_TOGGLE, " LFO Two Enable", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_ONESHOT], MIDI_LFO2_ONESHOT, 0, 1, 33, MIDI_TO_INT_BUTTON, " LFO Two OneShot", lfoTwoSection);
    //Free and Sync bound to one parameter, Handled differently in the MidiProcessor
    initMidiRow(lfoTwo[MIDI_LFO2_FREE], MIDI_LFO2_FREE, 0, 0, 34, MIDI_TO_CONSTANT, " LFO Two Free", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SYNC], MIDI_LFO2_SYNC, 1, 1, 34, MIDI_TO_CONSTANT, " LFO Two Sync", lfoTwoSection);
    // Flexible use the current Free or sync setting
    initMidiRow(lfoTwo[MIDI_LFO2_FREQ], MIDI_LFO2_FREQ, 0.0, 20.0, 35, MIDI_TO_DOUBLE, " LFO Two Freq", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_PHASE], MIDI_LFO2_PHASE, 0, 1, 36, MIDI_TO_DOUBLE, " LFO Two Phase", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_PWM], MIDI_LFO2_PWM, 0.0, 1.0, 37, MIDI_TO_DOUBLE, " LFO Two PWM", lfoTwoSection);
    // Select a specific shape by sending a value from 0 to 7 to the shape param[23]
    // Should be Handled differently in the MidiProcessor
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_1], MIDI_LFO2_SHAPE_1, 0, 0, 38, MIDI_TO_CONSTANT, " LFO Two Shape 1", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_2], MIDI_LFO2_SHAPE_2, 1, 1, 38, MIDI_TO_CONSTANT, " LFO Two Shape 2", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_3], MIDI_LFO2_SHAPE_3, 2, 2, 38, MIDI_TO_CONSTANT, " LFO Two Shape 3", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_4], MIDI_LFO2_SHAPE_4, 3, 3, 38, MIDI_TO_CONSTANT, " LFO Two Shape 4", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_5], MIDI_LFO2_SHAPE_5, 4, 4, 38, MIDI_TO_CONSTANT, " LFO Two Shape 5", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_6], MIDI_LFO2_SHAPE_6, 5, 5, 38, MIDI_TO_CONSTANT, " LFO Two Shape 6", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_7], MIDI_LFO2_SHAPE_7, 6, 6, 38, MIDI_TO_CONSTANT, " LFO Two Shape 7", lfoTwoSection);
    initMidiRow(lfoTwo[MIDI_LFO2_SHAPE_8], MIDI_LFO2_SHAPE_8, 7, 7, 38, MIDI_TO_CONSTANT, " LFO Two Shape 8", lfoTwoSection);

    initMidiRow(lfoTwo[MIDI_LFO2_SMOOTH], MIDI_LFO1_SMOOTH, 0.0, 1.0, 39, MIDI_TO_DOUBLE, " LFO Two Smooth", lfoTwoSection);

    midiParameters[lfoTwoSection] = lfoTwo;
    sizes[lfoTwoSection] = MIDI_LFO2_SIZE;
}

void MidiTable::initialiseMidiEnvOne(){
    initMidiRow(envOne[MIDI_ENV1_ENABLE], MIDI_ENV1_ENABLE, 0, 1, 40, MIDI_TO_INT_TOGGLE, " Env One Enable", envOneSection);
    initMidiRow(envOne[MIDI_ENV1_SYNC], MIDI_ENV1_SYNC, 0, 1, 41, MIDI_TO_INT_TOGGLE, " Env One Sync", envOneSection);
    initMidiRow(envOne[MIDI_ENV1_DURATION], MIDI_ENV1_DURATION, 0.f, 5000.f, 42, MIDI_TO_DOUBLE, " Env One Dur", envOneSection);
    initMidiRow(envOne[MIDI_ENV1_LOOP_NR], MIDI_ENV1_LOOP_NR, 1, 32, 43, MIDI_TO_INT," Env One Loop Nr", envOneSection);
    initMidiRow(envOne[MIDI_ENV1_LOOP_DIR], MIDI_ENV1_LOOP_DIR, 0, 2, 44, MIDI_TO_INT," Env One Loop Dir", envOneSection);
    initMidiRow(envOne[MIDI_ENV1_SUS_DIR], MIDI_ENV1_SUS_DIR, 0, 2, 45, MIDI_TO_INT, " Env One Sus Dir", envOneSection);
    initMidiRow(envOne[MIDI_ENV1_TRIGGER], MIDI_ENV1_TRIGGER, 0, 1, 47, MIDI_TO_INT_BUTTON, " Env One Trigger", envOneSection);

    midiParameters[envOneSection] = envOne;
    sizes[envOneSection] = MIDI_ENV1_SIZE;
}

void MidiTable::initialiseMidiEnvTwo(){
    initMidiRow(envTwo[MIDI_ENV2_ENABLE], MIDI_ENV2_ENABLE, 0, 1, 48, MIDI_TO_INT_TOGGLE, " Env Two Enable", envTwoSection);
    initMidiRow(envTwo[MIDI_ENV2_SYNC], MIDI_ENV2_SYNC, 0, 1, 49, MIDI_TO_INT_TOGGLE, " Env Two Sync", envTwoSection);
    initMidiRow(envTwo[MIDI_ENV2_DURATION], MIDI_ENV2_DURATION, 0, 5000., 50, MIDI_TO_DOUBLE, " Env Two Dur", envTwoSection);
    initMidiRow(envTwo[MIDI_ENV2_LOOP_NR], MIDI_ENV2_LOOP_NR, 1, 32, 51, MIDI_TO_INT, " Env Two Loop Nr", envTwoSection);
    initMidiRow(envTwo[MIDI_ENV2_LOOP_DIR], MIDI_ENV2_LOOP_DIR, 0, 2, 52, MIDI_TO_INT, " Env Two Loop Dir", envTwoSection);
    initMidiRow(envTwo[MIDI_ENV2_SUS_DIR], MIDI_ENV2_SUS_DIR, 0, 2, 53, MIDI_TO_INT, " Env Two Sus Dir", envTwoSection);
    initMidiRow(envTwo[MIDI_ENV2_TRIGGER], MIDI_ENV2_TRIGGER, 0, 1, 55, MIDI_TO_INT_BUTTON, " Env Two Trigger", envTwoSection);

    midiParameters[envTwoSection] = envTwo;
    sizes[envTwoSection] = MIDI_ENV2_SIZE;
}

void MidiTable::initialiseMidiSSEQ(){
    initMidiRow(stepSeq[MIDI_SSEQ_ENABLE], MIDI_SSEQ_ENABLE, 0, 1, 56, MIDI_TO_INT_TOGGLE, " Step Seq Enable", sseqSection);
    initMidiRow(stepSeq[MIDI_SSEQ_SYNC], MIDI_SSEQ_SYNC, 0, 1, 57, MIDI_TO_INT_TOGGLE, " Step Seq Sync", sseqSection);
    initMidiRow(stepSeq[MIDI_SSEQ_CHOP], MIDI_SSEQ_CHOP, 0, 1, 58, MIDI_TO_INT_TOGGLE, " Step Seq Chop", sseqSection);
    //Make Flexible
    initMidiRow(stepSeq[MIDI_SSEQ_DURATION], MIDI_SSEQ_DURATION, 0.0, 5000.0, 59, MIDI_TO_DOUBLE, " Step Seq Dur", sseqSection);
    initMidiRow(stepSeq[MIDI_SSEQ_LENGTH], MIDI_SSEQ_LENGTH, 0.0, 1.0, 60, MIDI_TO_DOUBLE, " Step Seq Length", sseqSection);
    initMidiRow(stepSeq[MIDI_SSEQ_OFF_VALUE], MIDI_SSEQ_OFF_VALUE, 0.0, 1.0, 61, MIDI_TO_INT_BUTTON, " Step Seq Off Value", sseqSection);
    initMidiRow(stepSeq[MIDI_SSEQ_CURVE], MIDI_SSEQ_CURVE, 0.0, 1.0, 62, MIDI_TO_DOUBLE, " Step Seq Curve", sseqSection);
    initMidiRow(stepSeq[MIDI_SSEQ_STEPS], MIDI_SSEQ_STEPS, 0, 4, 63, MIDI_TO_INT, " Step Seq Steps", sseqSection);

    midiParameters[sseqSection] = stepSeq;
    sizes[sseqSection] = MIDI_SSEQ_SIZE;
}

void MidiTable::initialiseMidiXYPad(){
    initMidiRow(xyPad[MIDI_XYPAD_X_AXIS], MIDI_XYPAD_X_AXIS, 0.0, 1.0, 64, MIDI_TO_DOUBLE, " XY-Pad X-Axis", xyPadSection);
    initMidiRow(xyPad[MIDI_XYPAD_Y_AXIS], MIDI_XYPAD_Y_AXIS, 0.0, 1.0, 65, MIDI_TO_DOUBLE, " XY-Pad Y-Axis", xyPadSection);
    initMidiRow(xyPad[MIDI_XYPAD_TRIGGER], MIDI_XYPAD_TRIGGER, 0, 1, 66, MIDI_TO_INT_BUTTON, " XY-Pad Trigger", xyPadSection);


    midiParameters[xyPadSection] = xyPad;
    sizes[xyPadSection] = MIDI_XYPAD_SIZE;
}

void MidiTable::initialiseMidiFX(){
    initMidiRow(fx[MIDI_FX_ENABLE], MIDI_FX_ENABLE, 0, 1, 67, MIDI_TO_INT_TOGGLE, " FX Enable", fxSection);
    initMidiRow(fx[MIDI_FX_INPUT], MIDI_FX_INPUT, 0.0, 1.0, 69, MIDI_TO_DOUBLE, " FX Input Vol", fxSection);
    initMidiRow(fx[MIDI_FX_ROUTING], MIDI_FX_ROUTING, 0.0, 2.0, 70, MIDI_TO_DOUBLE, " FX Routing", fxSection);

    midiParameters[fxSection] = fx;
    sizes[fxSection] = MIDI_FX_SIZE;
}

void MidiTable::initialiseMidiSDelay(){
    initMidiRow(sDelay[MIDI_SDEL_ENABLE], MIDI_SDEL_ENABLE, 0, 1, 71, MIDI_TO_INT_TOGGLE, " S-Delay Enable", sDelaySection);

    initMidiRow(sDelay[MIDI_SDEL_CROSS_EN], MIDI_SDEL_CROSS_EN, 0.0, 1.0, 72, MIDI_TO_INT_TOGGLE, " S-Delay Cross", sDelaySection);
    initMidiRow(sDelay[MIDI_SDEL_CROSS_AMP], MIDI_SDEL_CROSS_AMP, 0.0, 1.0, 73, MIDI_TO_DOUBLE, " S-Delay Cross Amp", sDelaySection);
    initMidiRow(sDelay[MIDI_SDEL_MIX], MIDI_SDEL_MIX, 0.0, 1.0, 74, MIDI_TO_DOUBLE, " S-Delay Mix", sDelaySection);
    initMidiRow(sDelay[MIDI_SDEL_L_SYNC], MIDI_SDEL_L_SYNC, 0, 1, 75, MIDI_TO_INT_TOGGLE, " S-Delay L-Sync", sDelaySection);
    initMidiRow(sDelay[MIDI_SDEL_L_DELAY], MIDI_SDEL_L_DELAY, 0.0, 2000.0, 76, MIDI_TO_DOUBLE, " S-Delay L-Delay", sDelaySection); // Flexible
    initMidiRow(sDelay[MIDI_SDEL_L_FB], MIDI_SDEL_L_FB, 0.0, 1.0, 77, MIDI_TO_DOUBLE, " S-Delay L-FB", sDelaySection);
    initMidiRow(sDelay[MIDI_SDEL_R_SYNC], MIDI_SDEL_R_SYNC, 0, 1, 78, MIDI_TO_INT_TOGGLE, " S-Delay R-Sync", sDelaySection);
    initMidiRow(sDelay[MIDI_SDEL_R_DELAY], MIDI_SDEL_R_DELAY, 0.0, 2000.0, 79, MIDI_TO_DOUBLE, " S-Delay R-Delay", sDelaySection); //flexible
    initMidiRow(sDelay[MIDI_SDEL_R_FB], MIDI_SDEL_R_FB, 0.0, 1.0, 80, MIDI_TO_DOUBLE, " S-Delay R-FB", sDelaySection);

    midiParameters[sDelaySection] = sDelay;
    sizes[sDelaySection] = MIDI_SDEL_SIZE;
}

void MidiTable::initialiseMidiPPDelay(){
    initMidiRow(ppDelay[MIDI_PPDEL_ENABLE], MIDI_PPDEL_ENABLE, 0, 1, 81, MIDI_TO_INT_TOGGLE, " PP-Delay Enable", ppDelaySection);
    initMidiRow(ppDelay[MIDI_PPDEL_L_SYNC], MIDI_PPDEL_L_SYNC, 0, 1, 82, MIDI_TO_INT_TOGGLE, " PP-Delay L-Sync", ppDelaySection);

    initMidiRow(ppDelay[MIDI_PPDEL_L_DELAY], MIDI_PPDEL_L_DELAY, 0.0, 2000.0, 83, MIDI_TO_DOUBLE, " PP-Delay L-Delay", ppDelaySection);
    initMidiRow(ppDelay[MIDI_PPDEL_L_FB], MIDI_PPDEL_L_FB, 0.0, 1.0, 84, MIDI_TO_DOUBLE, " PP-Delay L-FB", ppDelaySection); //Probably limit
    initMidiRow(ppDelay[MIDI_PPDEL_L_MIX], MIDI_PPDEL_L_MIX, 0.0, 1.0, 85, MIDI_TO_DOUBLE, " PP-Delay L-Mix", ppDelaySection);
    initMidiRow(ppDelay[MIDI_PPDEL_R_SYNC], MIDI_PPDEL_R_SYNC, 0, 1, 86, MIDI_TO_INT_TOGGLE, " PP-Delay R-Sync", ppDelaySection);
    initMidiRow(ppDelay[MIDI_PPDEL_R_DELAY], MIDI_PPDEL_R_DELAY, 0, 2000.0, 87, MIDI_TO_DOUBLE, " PP-Delay R-Delay", ppDelaySection);
    initMidiRow(ppDelay[MIDI_PPDEL_R_FB], MIDI_PPDEL_R_FB, 0.0, 1.0, 88, MIDI_TO_DOUBLE, " PP-Delay R-FB", ppDelaySection);
    initMidiRow(ppDelay[MIDI_PPDEL_R_MIX], MIDI_PPDEL_R_MIX, 0, 1, 89, MIDI_TO_DOUBLE, " PP-Delay R-Mix", ppDelaySection);

    midiParameters[ppDelaySection] = ppDelay;
    sizes[ppDelaySection] = MIDI_PPDEL_SIZE;
}

void MidiTable::initialiseMidiLCRDelay(){
    initMidiRow(lcrDelay[MIDI_LCRDEL_ENABLE], MIDI_LCRDEL_ENABLE, 0, 1, 90, MIDI_TO_INT_TOGGLE, " LCR-Delay Enable", lcrDelaySection);

    initMidiRow(lcrDelay[MIDI_LCRDEL_L_SYNC], MIDI_LCRDEL_L_SYNC, 0, 1, 91, MIDI_TO_INT_TOGGLE, " LCR-Delay L-Sync", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_L_DELAY], MIDI_LCRDEL_L_DELAY, 0, 1, 92, MIDI_TO_DOUBLE, " LCR-Delay L-Delay", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_L_FB], MIDI_LCRDEL_L_FB, 0.0, 1.0, 93, MIDI_TO_DOUBLE, " LCR-Delay L-FB", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_L_MIX], MIDI_LCRDEL_L_MIX, 0, 1, 94, MIDI_TO_DOUBLE, " LCR-Delay L-Mix", lcrDelaySection);

    initMidiRow(lcrDelay[MIDI_LCRDEL_C_SYNC], MIDI_LCRDEL_C_SYNC, 0, 1, 95, MIDI_TO_INT_TOGGLE, " LCR-Delay C-Sync", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_C_DELAY], MIDI_LCRDEL_C_DELAY, 0, 1, 96, MIDI_TO_DOUBLE, " LCR-Delay C-Delay", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_C_FB], MIDI_LCRDEL_C_FB, 0.0, 1.0, 97, MIDI_TO_DOUBLE, " LCR-Delay C-FB", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_HPF_ENA], MIDI_LCRDEL_HPF_ENA, 0, 1, 98, MIDI_TO_INT_TOGGLE, " LCR-Delay HPF En", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_HPF_FREQ], MIDI_LCRDEL_HPF_FREQ, 0.0, 1.0, 99, MIDI_TO_DOUBLE, " LCR-Delay HPF Freq", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_LPF_ENA], MIDI_LCRDEL_LPF_ENA, 0, 1, 100, MIDI_TO_INT_TOGGLE, " LCR-Delay LFP En", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_LPF_FREQ], MIDI_LCRDEL_LPF_FREQ, 0.0, 1.0, 101, MIDI_TO_DOUBLE, " LCR-Delay LPF Freq", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_C_AMP], MIDI_LCRDEL_C_AMP, 0, 1, 102, MIDI_TO_DOUBLE, " LCR-Delay C-Amp", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_C_PAN], MIDI_LCRDEL_C_PAN, 0, 1, 103, MIDI_TO_DOUBLE, " LCR-Delay C-Pan", lcrDelaySection);

    initMidiRow(lcrDelay[MIDI_LCRDEL_R_SYNC], MIDI_LCRDEL_R_SYNC, 0, 1, 104, MIDI_TO_INT_TOGGLE, " LCR-Delay R-Sync", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_R_DELAY], MIDI_LCRDEL_R_DELAY, 0, 1, 105, MIDI_TO_DOUBLE, " LCR-Delay R-Delay", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_R_FB], MIDI_LCRDEL_R_FB, 0.0, 1.0, 106, MIDI_TO_DOUBLE, " LCR-Delay R-FB", lcrDelaySection);
    initMidiRow(lcrDelay[MIDI_LCRDEL_R_MIX], MIDI_LCRDEL_R_MIX, 0, 1, 107, MIDI_TO_DOUBLE, " LCR-Delay R-Mix", lcrDelaySection);

    midiParameters[lcrDelaySection] = lcrDelay;
    sizes[lcrDelaySection] = MIDI_LCRDEL_SIZE;
}

void MidiTable::initialiseMidiSFlanger(){
    initMidiRow(sFlanger[MIDI_SFLANG_ENABLE], MIDI_SFLANG_ENABLE, 0, 1, 108, MIDI_TO_INT_TOGGLE, " S-Flanger Enable", sFlangerSection);

    initMidiRow(sFlanger[MIDI_SFLANG_CROSS], MIDI_SFLANG_CROSS, 0, 1, 109, MIDI_TO_INT_TOGGLE, " S-Flanger Cross", sFlangerSection);
    initMidiRow(sFlanger[MIDI_SFLANG_SYNC], MIDI_SFLANG_SYNC, 0, 1, 110, MIDI_TO_INT_TOGGLE, " S-Flanger Sync", sFlangerSection);
    initMidiRow(sFlanger[MIDI_SFLANG_RATE], MIDI_SFLANG_RATE, 0, 20.0, 111, MIDI_TO_DOUBLE, " S-Flanger Rate", sFlangerSection); //flexible
    initMidiRow(sFlanger[MIDI_SFLANG_PHASE], MIDI_SFLANG_PHASE, 0, 1, 112, MIDI_TO_DOUBLE, " S-Flanger Phase", sFlangerSection);
    initMidiRow(sFlanger[MIDI_SFLANG_DEPTH], MIDI_SFLANG_DEPTH, 0, 1, 113, MIDI_TO_DOUBLE, " S-Flanger Depth", sFlangerSection);
    initMidiRow(sFlanger[MIDI_SFLANG_FB], MIDI_SFLANG_FB, 0.0, 1.0, 114, MIDI_TO_DOUBLE, " S-Flanger FB", sFlangerSection);
    initMidiRow(sFlanger[MIDI_SFLANG_WAVE], MIDI_SFLANG_WAVE, 0, 4, 115, MIDI_TO_INT, " S-Flanger Wave", sFlangerSection);

    midiParameters[sFlangerSection] = sFlanger;
    sizes[sFlangerSection] = MIDI_SFLANG_SIZE;
}

void MidiTable::initialiseMidiDChorus(){
    initMidiRow(dChorus[MIDI_DCHOR_ENABLE], MIDI_DCHOR_ENABLE, 0, 1, 116, MIDI_TO_INT_TOGGLE, " D-Chorus Enable", dChorusSection);

    initMidiRow(dChorus[MIDI_DCHOR_RATE], MIDI_DCHOR_RATE, 0, 1.0, 116, MIDI_TO_DOUBLE, " D-Chorus Rate", dChorusSection);
    initMidiRow(dChorus[MIDI_DCHOR_SPREAD], MIDI_DCHOR_SPREAD, 0.0, 1.0, 116, MIDI_TO_DOUBLE, " D-Chorus Spread", dChorusSection);
    initMidiRow(dChorus[MIDI_DCHOR_DEPTH], MIDI_DCHOR_DEPTH, 0.0, 1.0, 116, MIDI_TO_DOUBLE, " D-Chorus Depth", dChorusSection);
    initMidiRow(dChorus[MIDI_DCHOR_WAVE], MIDI_DCHOR_WAVE, 0, 2, 116, MIDI_TO_INT, " D-Chorus Wave", dChorusSection);
    initMidiRow(dChorus[MIDI_DCHOR_FREQ], MIDI_DCHOR_FREQ, 0.0, 1.0, 116, MIDI_TO_DOUBLE, " D-Chorus HPF Freq", dChorusSection);
    initMidiRow(dChorus[MIDI_DCHOR_C_AMP], MIDI_DCHOR_C_AMP, 0, 1, 116, MIDI_TO_DOUBLE, " D-Chorus C-Amp", dChorusSection);
    initMidiRow(dChorus[MIDI_DCHOR_C_PAN], MIDI_DCHOR_C_PAN, 0, 1, 116, MIDI_TO_DOUBLE, " D-Chorus C-Pan", dChorusSection);
    initMidiRow(dChorus[MIDI_DCHOR_MIX], MIDI_DCHOR_MIX, 0, 1, 116, MIDI_TO_DOUBLE, " D-Chorus Mix", dChorusSection);

    midiParameters[dChorusSection] = dChorus;
    sizes[dChorusSection] = MIDI_DCHOR_SIZE;
}

void MidiTable::initialiseMidiHDelay(){
    initMidiRow(hDelay[MIDI_HDEL_ENABLE], MIDI_HDEL_ENABLE, 0, 1, 125, MIDI_TO_INT_TOGGLE, " H-Delay Enable", hDelaySection);

    initMidiRow(hDelay[MIDI_HDEL_TRIGGER], MIDI_HDEL_TRIGGER, 0, 1, 126, MIDI_TO_INT_BUTTON, " H-Delay Trigger", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_BEAT], MIDI_HDEL_BEAT, 0, 1, 127, MIDI_TO_INT_BUTTON, " H-Delay Quantise", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_LOOP], MIDI_HDEL_LOOP, 0, 1, 128, MIDI_TO_DOUBLE, " H-Delay Loop", hDelaySection); //Flexible
    initMidiRow(hDelay[MIDI_HDEL_SPEED], MIDI_HDEL_SPEED, 0.25, 4.0, 129, MIDI_TO_DOUBLE, " H-Delay Speed", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_GLIDE_OPT], MIDI_HDEL_GLIDE_OPT, 0, 1, 130, MIDI_TO_INT_BUTTON, " H-Delay Glide opt", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_GLIDE], MIDI_HDEL_GLIDE, 1, 32, 131, MIDI_TO_INT, " H-Delay Glide", hDelaySection); //Flexible
    initMidiRow(hDelay[MIDI_HDEL_DIR], MIDI_HDEL_DIR, 0, 2, 132, MIDI_TO_INT, " H-Delay Dir", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_GAP_DIR], MIDI_HDEL_GAP_DIR, 0, 1, 133, MIDI_TO_INT_TOGGLE, " H-Delay Gap Dir", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_LENGTH], MIDI_HDEL_LENGTH, 0.10, 1.0, 134, MIDI_TO_DOUBLE, " H-Delay Length", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_FADE], MIDI_HDEL_FADE, 0, 1, 135, MIDI_TO_DOUBLE, " H-Delay Fade", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_PAN], MIDI_HDEL_PAN, 0, 1, 136, MIDI_TO_DOUBLE, " H-Delay Pan", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_MIX], MIDI_HDEL_MIX, 0, 1, 137, MIDI_TO_DOUBLE, " H-Delay Mix", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_LATCH_ENA], MIDI_HDEL_LATCH_ENA, 0, 1, 138, MIDI_TO_INT_TOGGLE, " H-Delay Latch En", hDelaySection);
    initMidiRow(hDelay[MIDI_HDEL_LATCH], MIDI_HDEL_LATCH, 0, 14, 139, MIDI_TO_INT, " H-Delay Latch", hDelaySection);

    midiParameters[hDelaySection] = hDelay;
    sizes[hDelaySection] = MIDI_HDEL_SIZE;
}

void MidiTable::initialiseMidiDecimator(){
    initMidiRow(decimator[MIDI_DECIM_ENABLE], MIDI_DECIM_ENABLE, 0, 1, 140, MIDI_TO_INT_TOGGLE, " Decimator Enable", decimatorSection);

    initMidiRow(decimator[MIDI_DECIM_REDUCE_ENA], MIDI_DECIM_REDUCE_ENA, 0, 1, 141, MIDI_TO_INT_TOGGLE, " Decimator Reduce En", decimatorSection);
    initMidiRow(decimator[MIDI_DECIM_SRATE_ENA], MIDI_DECIM_SRATE_ENA, 0, 1, 142, MIDI_TO_INT_TOGGLE, " Decimator SRate En", decimatorSection);
    initMidiRow(decimator[MIDI_DECIM_FILTER_ENA], MIDI_DECIM_FILTER_ENA, 0, 1, 143, MIDI_TO_INT_TOGGLE, " Decimator Filter En", decimatorSection);
    initMidiRow(decimator[MIDI_DECIM_REDUCE], MIDI_DECIM_REDUCE, 0, 1, 144, MIDI_TO_DOUBLE, " Decimator Reduce", decimatorSection); //Needs to be limited implement later
    initMidiRow(decimator[MIDI_DECIM_SRATE], MIDI_DECIM_SRATE, 0, 1, 145, MIDI_TO_DOUBLE, " Decimator SRate", decimatorSection);
    initMidiRow(decimator[MIDI_DECIM_MIX], MIDI_DECIM_MIX, 0, 1, 146, MIDI_TO_DOUBLE, " Decimator Mix", decimatorSection);

    midiParameters[decimatorSection] = decimator;
    sizes[decimatorSection] = MIDI_DECIM_SIZE;;
}

void MidiTable::initialiseMidiPRverb(){
    initMidiRow(pReverb[MIDI_PREV_ENABLE], MIDI_PREV_ENABLE, 0.0, 1.0, 147, MIDI_TO_INT_TOGGLE, " P-Reverb Enable", pReverbSection);

    initMidiRow(pReverb[MIDI_PREV_DARMPING], MIDI_PREV_DARMPING, 0.0, 1.0, 148, MIDI_TO_DOUBLE, " P-Reverb Damping", pReverbSection);
    initMidiRow(pReverb[MIDI_PREV_BANDWIDTH], MIDI_PREV_BANDWIDTH, 0.0, 1.0, 149, MIDI_TO_DOUBLE, " P-Reverb Bandwidth", pReverbSection);
    initMidiRow(pReverb[MIDI_PREV_DECAY], MIDI_PREV_DECAY, 0.0, 1.0, 150, MIDI_TO_DOUBLE, " P-Reverb Decay", pReverbSection);
    initMidiRow(pReverb[MIDI_PREV_MIX], MIDI_PREV_MIX, 0.0, 1.0, 151, MIDI_TO_DOUBLE, " P-Reverb Mix", pReverbSection);

    midiParameters[pReverbSection] = pReverb;
    sizes[pReverbSection] = MIDI_PREV_SIZE;;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiTable" componentName=""
                 parentClasses="public TableListBox, public TableListBoxModel"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
