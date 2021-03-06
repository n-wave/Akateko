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

#include "StepSequencerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <iostream>
#include "Akateko.h"

using std::vector;
using akateko::IgnoreRightClick;
using akateko::MidiRow;
using akateko::sseq_timing_ratios;
/* initialiser list
 *
 *  Component::Component(name),
 *  processor(p),
 *  minDur(0),
 *  maxDur(0),
 *  range(0),
 *  commandExponent(-1),
 *  duration(nullptr),
 *  labelRef(label)
 *
 */

//[/MiscUserDefs]

//==============================================================================
StepSequencerComponent::StepSequencerComponent (const String &name,AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      duration(nullptr),
      beatsPerMinute(120.0),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    /*  check against the following labels
     *
     *    AudioParameter       paramCom        label
     *  -------------------------------------------------
     *  AudioParameterBool  : ParamToggle   : "enable"
     *  AudioParameterBool  : ParamToggle   : "sync"
     *  AudioParameterBool  : ParamToggle   : "chopper"
     *  AudioParameterFloat : ParamSlider   : "duration"
     *  AudioParameterFloat : ParamSlider   : "length"
     *  AudioParameterFloat : ParamSlider   : "offset"
     *  AudioParameterFloat : ParamSlider   : "curve"
     *  AudioParameterInt   : ParamSlider   : "amount"
     *
     */

    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::StepSequencerId);
    StringArray paramName = p.getParameterIds(AkatekoAudioProcessor::StepSequencerId);

    int nrOfParams = paramIndices.size();

    if(nrOfParams == 8){
        requestMenuIds[0] = paramName[0].hash();
        requestMenuIds[1] = paramName[1].hash();
        requestMenuIds[2] = paramName[2].hash();
        requestMenuIds[3] = paramName[3].hash();
        requestMenuIds[4] = paramName[4].hash();
        requestMenuIds[5] = paramName[5].hash();
        requestMenuIds[6] = paramName[6].hash();
        requestMenuIds[7] = paramName[7].hash();

        initialiseMidiStrings();

        addAndMakeVisible(enableToggle = new ParamToggle(paramName[0], *p.getParameters().getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(syncToggle = new ParamToggle(paramName[1], *p.getParameters().getUnchecked(paramIndices[1]), label));
        addAndMakeVisible(chopperToggle = new ParamToggle(paramName[2], *p.getParameters().getUnchecked(paramIndices[2]), label));

        addAndMakeVisible(durationSlider = new IgnoreRightClick<Slider>(paramName[3]));
        duration = p.getParameters().getUnchecked(paramIndices[3]);

        addAndMakeVisible(stepLengthSlider = new ParamSlider(paramName[4], *p.getParameters().getUnchecked(paramIndices[4]), label, 0.05f, 1.00));
        addAndMakeVisible(offsetSlider = new ParamSlider(paramName[5], *p.getParameters().getUnchecked(paramIndices[5]), label, 0.f, 1.f));
        addAndMakeVisible(exponentSlider = new ParamSlider(paramName[6], *p.getParameters().getUnchecked(paramIndices[6]), label));
        addAndMakeVisible(stepComboBox = new ParamComboBox(paramName[7], *p.getParameters().getUnchecked(paramIndices[7]), label));
    } else {
        addAndMakeVisible(enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (syncToggle = new ToggleButton ("syncToggle"));
        addAndMakeVisible (chopperToggle = new ToggleButton("chopperTOggle"));
        addAndMakeVisible (durationSlider = new Slider ("durationSlider"));
        addAndMakeVisible (stepLengthSlider = new Slider ("stepLength"));
        addAndMakeVisible (offsetSlider = new Slider ("offsetSlider"));
        addAndMakeVisible (exponentSlider = new Slider ("exponentSlider"));
        addAndMakeVisible (stepComboBox = new ComboBox ("stepComboBox"));

        std::cout << "StepSequencerComponent::StepSequencerComponent" << std::endl;
        std::cout << "/------------Parameters not bound------------/" << std::endl;
    }

    //[/Constructor_pre

    //[UserPreSize]
    addAndMakeVisible (stepSequencer = new StepSequencer ("sseq"));
    stepSequencer->setName ("stepSequencer");

    stepSequencer->setColour(StepSequencer::backgroundColourId, Colours::black);
    stepSequencer->setColour(StepSequencer::highLightColourId, Colour(0x2FFFFFFF));
    stepSequencer->setColour(StepSequencer::outlineColourId, Colour(0xAA70C099));
    stepSequencer->setColour(StepSequencer::fillGradientOneId, Colour(0xC070D0B0));
    stepSequencer->setColour(StepSequencer::fillGradientTwoId, Colour(0x10007030));
    stepSequencer->setColour(StepSequencer::stepOutlineColourId, Colour(0xBB40DFAA));
    stepSequencer->setColour(StepSequencer::buttonOnColourId, Colour(0xB070D0B0));
    stepSequencer->setColour(StepSequencer::buttonLinkedColourId, Colour(0xAA60C0A0));
    stepSequencer->setColour(StepSequencer::overlayGradientTwoId, Colours::transparentBlack);

    stepLengthSlider->setRange (0, 1, 0);
    stepLengthSlider->setSliderStyle (Slider::LinearBar);
    stepLengthSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

    enableToggle->setButtonText (String());
    enableToggle->addListener (this);

    syncToggle->setTooltip (TRANS("sync"));
    syncToggle->setButtonText (String());
    syncToggle->addListener (this);

    durationSlider->setRange(50.f, 5000.f, 0);
    durationSlider->setSliderStyle (Slider::LinearBar);
    durationSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    durationSlider->addListener(this);

    stepComboBox->setEditableText (false);
    stepComboBox->setJustificationType (Justification::centred);
    stepComboBox->setTextWhenNothingSelected (TRANS("Step Amount"));
    stepComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    stepComboBox->addItem (TRANS("8"), 1);
    stepComboBox->addItem (TRANS("12"), 2);
    stepComboBox->addItem (TRANS("16"), 3);
    stepComboBox->addItem (TRANS("24"), 4);
    stepComboBox->addItem (TRANS("32"), 5);
    stepComboBox->postCommandMessage(ParamComboBox::update);
    stepComboBox->addListener (this);

    addAndMakeVisible (nameLabel = new Label ("nameLabel",
                                              TRANS("SSEQ")));
    nameLabel->setFont (Font ("Good Times", 15.00f, Font::plain));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (Label::textColourId, Colour(0xFF4A997A));
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    offsetSlider->setRange (0, 1, 0);
    offsetSlider->setSliderStyle (Slider::LinearBar);
    offsetSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

    exponentSlider->setRange (0, 1, 0);
    exponentSlider->setSliderStyle (Slider::LinearBar);
    exponentSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    exponentSlider->setDoubleClickReturnValue(true, 0.5);
    exponentSlider->addListener(this);

    chopperToggle->setTooltip (TRANS("Generated curve is affected by the step Length"));
    chopperToggle->setButtonText (String());
    chopperToggle->addListener (this);


    // Build Menu
    menu.addItem(1, "learn", true, false, nullptr);
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
    //[/UserPreSize]

    setSize (515, 250);
    //[Constructor] You can add your own custom stuff here..

    double bpm = processor.getBeatsPerMinute();

    if(bpm < 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;

    initialiseBeatDivision();
    calculateBeatDivision(beatsPerMinute); //standard
    initDurationSlider();
    //[/Constructor]
}

StepSequencerComponent::~StepSequencerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    stepSequencer = nullptr;
    stepLengthSlider = nullptr;
    enableToggle = nullptr;
    syncToggle = nullptr;
    durationSlider = nullptr;
    stepComboBox = nullptr;
    nameLabel = nullptr;
    offsetSlider = nullptr;
    exponentSlider = nullptr;
    chopperToggle = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    duration = nullptr;
    //[/Destructor]
}

//==============================================================================
void StepSequencerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colour (0xFF4A997A));
    g.setFont (Font ("Good Times", 11.00f, Font::plain));

    g.drawText (TRANS("duration"),
                426, 30, 73, 10,
                Justification::centredLeft, true);


    g.drawText (TRANS("Length"),
                436, 74, 52, 12,
                Justification::centredLeft, true);


    g.drawText (TRANS("offvalue"),
                429, 119, 65, 12,
                Justification::centred, true);

    g.drawText (TRANS("curve"),
                436, 164, 49, 12,
                Justification::centred, true);

    g.drawText (TRANS("steps"),
                436, 210, 49, 12,
                Justification::centred, true);


    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StepSequencerComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]


    stepSequencer->setBounds (5, 5, 400, 240);

    enableToggle->setBounds (492, 1, 24, 24);
    nameLabel->setBounds (431, 2, 60, 24);

    durationSlider->setBounds (412, 46, 96, 16);
    stepLengthSlider->setBounds (412, 91, 96, 16);
    offsetSlider->setBounds (412, 136, 96, 16);
    exponentSlider->setBounds (412, 181, 96, 16);
    stepComboBox->setBounds (412, 226, 96, 16);


    syncToggle->setBounds (493, 24, 20, 20);
    chopperToggle->setBounds (493, 159, 20, 20);


    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void StepSequencerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == enableToggle)
    {
        //[UserButtonCode_enableToggle] -- add your button handler code here..
        //[/UserButtonCode_enableToggle]
    }
    else if (buttonThatWasClicked == syncToggle)
    {
        //[UserButtonCode_syncToggle] -- add your button handler code here..
        double tmpDur = 0.0;

        if(duration != nullptr && !paramIndices.empty()){
            tmpDur = duration->getValue();

            const bool tmpSync = syncToggle->getToggleState();
            if(tmpSync){
                const int tmpIndex = findClosestTimeDivision(tmpDur);
                const double startDur =  valueBeatDivision[0];
                const double endDur = valueBeatDivision[valueBeatDivision.size()-1];

                if(processor.getRegisteredMidi(paramIndices[3])){
                    processor.changeMidiRowMinMax(0, 17, akateko::MIDI_TO_INT, paramIndices[3]);
                }

                durationSlider->setRange(0, valueBeatDivision.size()-1);
                durationSlider->setValue(tmpIndex, sendNotification);
                duration->setValue(valueBeatDivision[tmpIndex]);

                processor.setStepDurationBounds(startDur, endDur);
            } else {
                if(tmpDur >= 5000.f){
                    tmpDur = 5000.f;
                }

                if(tmpDur <= 50.f){
                    tmpDur = 50.f;
                }

                if(processor.getRegisteredMidi(paramIndices[3])){
                    processor.changeMidiRowMinMax(50.0, 5000.0, akateko::MIDI_TO_DOUBLE, paramIndices[3]);
                }

                durationSlider->setRange(50.f, 5000.f);
                duration->setValue(tmpDur);
                durationSlider->setValue(tmpDur, sendNotification);

                processor.setStepDurationBounds(50.0, 5000.0);
            }
        } else {
            std::cerr << "Parameters not bound properly, duration not set : " << getName() << std::endl;
        }
        //[/UserButtonCode_syncToggle]
    }
    else if (buttonThatWasClicked == chopperToggle)
    {
        //[UserButtonCode_chopperToggle] -- add your button handler code here..
        processor.updateChopCurve();
        //[/UserButtonCode_chopperToggle]
    }
    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void StepSequencerComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == durationSlider)
    {
        //[UserSliderCode_durationSlider] -- add your slider handling code here..

        String tmpName = durationSlider->getName();

        if(syncToggle != nullptr && duration != nullptr){
            const bool tmpSync = syncToggle->getToggleState();
            if(tmpSync){
                int tmpVal = durationSlider->getValue();

                if(tmpVal < beatDivision.size() &&
                   tmpVal < valueBeatDivision.size())
                {
                    double tmpDur = valueBeatDivision[tmpVal];
                    tmpName += beatDivision[tmpVal];
                    duration->setValue(tmpDur);
                    processor.updateDuration();
                }
            } else {
                double tmpDur = durationSlider->getValue();

                durationSlider->setRange(50.f, 5000.f);
                duration->setValue(tmpDur);
                processor.updateDuration();
                tmpName += String(tmpDur, 2);
            }

           labelRef.setText(tmpName, dontSendNotification);
        }

        //[/UserSliderCode_durationSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void StepSequencerComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == stepComboBox)
    {
        //[UserComboBoxCode_stepComboBox] -- add your combo box handling code here..
        int tmpIndex = comboBoxThatHasChanged->getSelectedItemIndex();
        stepAmount = 8;

        switch(tmpIndex){
            case 0:
                stepAmount = 8;
                break;
            case 1:
                stepAmount = 12;
                break;
            case 2:
                stepAmount = 16;
                break;
            case 3:
                stepAmount = 24;
                break;
            case 4:
                stepAmount = 32;
                break;
        }

        stepSequencer->setStepSize(stepAmount);
        processor.setStepSeq(stepSequencer->getStepValues(), stepSequencer->getButtonStates());
        processor.setUIState(stepSequencer->toString(), AkatekoAudioProcessor::StepSequencerId);
        //[/UserComboBoxCode_stepComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}
//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void StepSequencerComponent::sliderDragEnded(Slider *sliderThatWasDragged){
    if (sliderThatWasDragged == exponentSlider)
    {
        //[UserSliderCode_exponentSlider] -- add your slider handling code here..
        processor.updateStepMidPoint();
        //[/UserSliderCode_exponentSlider]
    }
}

String StepSequencerComponent::getUIState(){
    return stepSequencer->toString();
}

void StepSequencerComponent::setUIState(String state){
   stepSequencer->restoreFromString(state);
}


void StepSequencerComponent::initialiseBeatDivision(){
    StringArray noteVals;

    noteVals.add("1/4N");
    noteVals.add("1/2N");
    noteVals.add("1N");
    noteVals.add("2N");
    noteVals.add("3N");
    noteVals.add("4N");

    beatDivision.clear();

    for(int i=0; i<noteVals.size(); i++){
        String dot = noteVals[i] + String("D");
        String triplet = noteVals[i] + String("T");

        beatDivision.add(triplet);
        beatDivision.add(noteVals[i]);
        beatDivision.add(dot);
    }
}

void StepSequencerComponent::calculateBeatDivision(double bpm){
    if(bpm > 0.0){       
        if(beatsPerMinute != bpm){
            double mult = beatsPerMinute/bpm;
            double tmpValue = duration->getValue()*mult;

            beatsPerMinute = bpm;
            duration->setValue(tmpValue);
        }

        double T = 60000/bpm;

        valueBeatDivision.clear();

        for(int i=0; i<beatDivision.size(); i++){
            valueBeatDivision.push_back(T*sseq_timing_ratios[i]);
        }
    }
    initDurationSlider();
}

void StepSequencerComponent::initDurationSlider(){
    if(syncToggle != nullptr &&
       duration != nullptr &&
       beatDivision.size() != 0 &&
       valueBeatDivision.size() != 0)
    {
        double tmpDur = duration->getValue();
        const bool tmpSync = syncToggle->getToggleState();

        if(tmpSync){
            int tmpIndex = findClosestTimeDivision(tmpDur);

            durationSlider->setRange(0, valueBeatDivision.size()-1);
            durationSlider->setValue(tmpIndex);

            processor.setStepDurationBounds(valueBeatDivision[0],
                                            valueBeatDivision[valueBeatDivision.size()-1]);
        } else {
            if(tmpDur >= 5000.0){
                tmpDur = 5000.0;
            }

            if(tmpDur <= 50.0){
                tmpDur = 50.0;
            }

            durationSlider->setRange(50, 5000.0);
            durationSlider->setValue(tmpDur);

            processor.setStepDurationBounds(50, 5000.0);
        }
    } else {
        if(syncToggle == nullptr || duration == nullptr){
            std::cerr << "Parameters not bound properly" << std::endl;
        }

        if(beatDivision.size() == 0 || valueBeatDivision.size() == 0){
            std::cerr << "Beat Division tables not set" << std::endl;
        }
    }
}

int StepSequencerComponent::getTimeDivisionIndex(vector<double> values, double period){
    int index = 0;

    while(index < values.size()){
        double tmpValue = values[index];
        double error = tmpValue*0.05;

        if(period >= tmpValue-error &&
           period <= tmpValue+error)
        {
            return index;
        }
        index++;
    }
    return 0;
}

int StepSequencerComponent::findClosestTimeDivision(double period){
    double tmpPeriod = period;
    int result = 0;
    int nrOfSteps = valueBeatDivision.size();

    if(period != 0 && nrOfSteps != 0){
        bool run = true;
        int index = 1;

        while(run && index < nrOfSteps-1){
            double prev = valueBeatDivision[index-1];
            double val = valueBeatDivision[index];
            double next = valueBeatDivision[index+1];

            if(val == tmpPeriod){
                result = index;
                run = false;
            }

            if(tmpPeriod >= prev && tmpPeriod < val){
                result = index-1;
                run = false;
            }

            if(tmpPeriod <= next && tmpPeriod > val ){
                result = index+1;
                run = false;
            }
            index +=3;
        }
    }
    return result;
}

void StepSequencerComponent::updateGui(){
    enableToggle->postCommandMessage(ParamToggle::update);
    syncToggle->postCommandMessage(ParamToggle::update);
    chopperToggle->postCommandMessage(ParamToggle::update);
    stepLengthSlider->postCommandMessage(ParamSlider::update);
    offsetSlider->postCommandMessage(ParamSlider::update);
    exponentSlider->postCommandMessage(ParamSlider::update);
    stepComboBox->postCommandMessage(ParamSlider::update);


    if(paramIndices.size() == 8){
        if(syncToggle->getToggleState()){
            if(processor.getRegisteredMidi(paramIndices[3])){
                processor.changeMidiRowMinMax(0, 17, akateko::MIDI_TO_INT, paramIndices[3]);
            }
        } else {
            if(processor.getRegisteredMidi(paramIndices[3])){
                processor.changeMidiRowMinMax(50.0, 5000.0, akateko::MIDI_TO_DOUBLE, paramIndices[3]);
            }
        }
    }

    initDurationSlider();
    processor.setStepValues(stepSequencer->getStepValues());
    processor.setButtonStates(stepSequencer->getButtonStates());
}

void StepSequencerComponent::setLookAndFeel(LookAndFeel *laf){
    stepLengthSlider->setLookAndFeel(laf);
    enableToggle->setLookAndFeel(laf);
    syncToggle->setLookAndFeel(laf);
    stepComboBox->setLookAndFeel(laf);
    durationSlider->setLookAndFeel(laf);
    offsetSlider->setLookAndFeel(laf);
    exponentSlider->setLookAndFeel(laf);
    chopperToggle->setLookAndFeel(laf);
    menu.setLookAndFeel(laf);
}

void StepSequencerComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" Step Seq: Enable");
    midiStrings.add(" Step Seq: Sync");
    midiStrings.add(" Step Seq: Chop");
    midiStrings.add(" Step Seq: Dur");
    midiStrings.add(" Step Seq: Length");
    midiStrings.add(" Step Seq: Off Value");
    midiStrings.add(" Step Seq: Curve");
    midiStrings.add(" Step Seq: Steps");
}

void StepSequencerComponent::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
    switch(commandId){
        case StepSequencer::valuesUpdated:
            processor.setStepValues(stepSequencer->getStepValues());
            processor.setUIState(stepSequencer->toString(), AkatekoAudioProcessor::StepSequencerId);
            break;
        case StepSequencer::buttonsUpdated:
            processor.setButtonStates(stepSequencer->getButtonStates());
            processor.setUIState(stepSequencer->toString(), AkatekoAudioProcessor::StepSequencerId);
            break;
    }

    if(paramIndices.size() == 8 &&
       midiStrings.size())
    {
        int index = -1;
        int param = -1;
        int handling = -1 ;
        double minValue = 0.0;
        double maxValue = 1.0;

        if(commandId == requestMenuIds[0]){         // Enable
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[1]){  // Sync
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[2]){  // Chop Curve
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[3]){  // Duration
            index = menu.show();
            param = 3;
            handling = akateko::MIDI_TO_DOUBLE;

            if(syncToggle->getToggleState()){
                maxValue = 17;
                handling = akateko::MIDI_TO_INT;
            } else {
                minValue = 50.0;
                maxValue = 5000.0;
                handling = akateko::MIDI_TO_DOUBLE;
            }
        } else if(commandId == requestMenuIds[4]){  // Length
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[5]){  // Off Value
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[6]){  // Curve
            index = menu.show();
            param = 6;
            minValue = 0;
            maxValue = 1.0;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[7]){  // Steps
            index = menu.show();
            param = 7;
            maxValue = 4;
            handling = akateko::MIDI_TO_INT;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling, midiStrings[param], 7);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);

            String message = midiStrings[param] + String(" cleared");
            labelRef.setText(message, sendNotificationAsync);
        }
    }
    //[/UserCode_handleCommandMessage]
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StepSequencerComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name,AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;commandValues(-1)&#10;commandStates(-1)&#10;commandSize(-1)&#10;commandOffset(-1)&#10;commandSync(-1)&#10;commandDuration(-1)&#10;commandExponent(-1)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="515" initialHeight="250">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323232">
    <TEXT pos="436 82 52 12" fill="solid: ffaaaaaa" hasStroke="0" text="Length"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="33"/>
    <TEXT pos="431 40 73 10" fill="solid: ffaaaaaa" hasStroke="0" text="duration"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="33"/>
    <TEXT pos="431 126 65 12" fill="solid: ffaaaaaa" hasStroke="0" text="offvalue"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
    <TEXT pos="436 166 49 12" fill="solid: ffaaaaaa" hasStroke="0" text="curve"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="stepSequencer" id="86fad89d553cc7f2" memberName="stepSequencer"
                    virtualName="StepSequencer" explicitFocusOrder="0" pos="5 5 400 240"
                    class="Component" params="&quot;sseq&quot;"/>
  <SLIDER name="stepLength" id="3c6a04987e56e55e" memberName="stepLengthSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 96 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="enableToggle" id="1ba5aa71427704e0" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="494 0 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="syncToggle" id="fc1623111ab0edcb" memberName="syncToggle"
                virtualName="" explicitFocusOrder="0" pos="408 32 24 24" tooltip="sync"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="durationSlider" id="5a0add9013433671" memberName="durationSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 56 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <COMBOBOX name="stepComboBox" id="76c1081265fe0ec3" memberName="stepComboBox"
            virtualName="" explicitFocusOrder="0" pos="412 224 96 20" editable="0"
            layout="33" items="8&#10;12&#10;16&#10;24&#10;32" textWhenNonSelected="Step Amount"
            textWhenNoItems="(no choices)"/>
  <LABEL name="nameLabel" id="a3d123be4d25c5a3" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="431 1 60 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="SSEQ" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Good Times"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="offsetSlider" id="162a5c977a1e137c" memberName="offsetSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 136 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="exponentSlider" id="befbb53b3929a4c8" memberName="exponentSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 184 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="ChopperToggle" id="b40454ae286af469" memberName="chopperToggle"
                virtualName="" explicitFocusOrder="0" pos="408 160 24 24" tooltip="Generated curve is affected by the step Length"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="new button" id="5277f09e753d96aa" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="408 8 23 24" buttonText="t"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
