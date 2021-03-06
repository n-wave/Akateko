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

#include "EnvelopeComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
using akateko::env_timing_ratios;
using akateko::initMidiRow;
using akateko::MidiRow;
using akateko::IgnoreRightClick;

//addParameter(envOneLoopEnable = new
//addParameter(envOneLoopAmount = new ));
//addParameter(envOneLoopDirection = new ));
//addParameter(envOneSusEnable = new ));
//addParameter(envOneSusDirection = new ));
//addParameter(envOneTriggerSource = new ));
//[/MiscUserDefs]

//==============================================================================
EnvelopeComponent::EnvelopeComponent (const String &name, AkatekoAudioProcessor &p, int env, Label &label)
    : Component::Component(name),
      processor(p),
      beatsPerMinute(120.0),
      duration(nullptr),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    /* Same setup as the LFO component for now we only have
     * 2 Parameter the length and loop amount of the envelope.
     * More parameters are going to be added,
     *
     * check against the following labels
     *
     *    AudioParameter       paramCom        label
     *  -------------------------------------------------
     *  AudioParameterBool  : ParamToggle   : "enable"
     *  AudioParameterBool  : ParamToggle   : "sync"
     *  AudioParameterFloat : ParamSlider   : "duration" //length
     *  AudioParameterFloat : ParamSlider   : "loop-nr"
     *  AudioParameterInt   : ParamComboBox : "loop-dir"
     *  AudioParameterInt   : ParamComboBox : "sus-dir"
     *  AudioParameterInt   : ParamComboBox : "tr-src"
     *
     *  ParamLabel : Length
     *  ParamLabel : loop
     */

    StringArray paramNames;

    if(env == 1){
        paramIndices = p.getParameterIndices(AkatekoAudioProcessor::Envelope1Id);
        paramNames = p.getParameterIds(AkatekoAudioProcessor::Envelope1Id);

        envNumber = env;
    } else if(env == 2){
        paramIndices = p.getParameterIndices(AkatekoAudioProcessor::Envelope2Id);
        paramNames = p.getParameterIds(AkatekoAudioProcessor::Envelope2Id);

        envNumber = env;
    }

    int nrOfParams = paramIndices.size();

    if(nrOfParams == 8){
        requestMenuIds[0] = paramNames[0].hash();
        requestMenuIds[1] = paramNames[1].hash();
        requestMenuIds[2] = paramNames[2].hash();
        requestMenuIds[3] = paramNames[3].hash();
        requestMenuIds[4] = paramNames[4].hash();
        requestMenuIds[5] = paramNames[5].hash();

        //std::cout << "Envlope : " << envNumber << " Duration : " << paramIndices[2] << std::endl;

        addAndMakeVisible (envEnableToggle = new ParamToggle(paramNames[0], *p.getParameters().getUnchecked(paramIndices [0]), label));
        addAndMakeVisible (envSyncToggle = new ParamToggle(paramNames[1], *p.getParameters().getUnchecked(paramIndices [1]), label));
        addAndMakeVisible (durationSlider =new IgnoreRightClick<Slider>(paramNames[2]));
        duration = p.getParameters().getUnchecked(paramIndices[2]);

        addAndMakeVisible (envLoopSlider = new ParamSlider(paramNames[3], *p.getParameters().getUnchecked(paramIndices[3]), label));
        addAndMakeVisible (loopDirectionComboBox = new ParamComboBox(paramNames[4], *p.getParameters().getUnchecked(paramIndices[4]), label));
        addAndMakeVisible (susDirectionComboBox = new ParamComboBox(paramNames[5], *p.getParameters().getUnchecked(paramIndices[5]), label));
        addAndMakeVisible (triggerComboBox = new ParamComboBox(paramNames[6], *p.getParameters().getUnchecked(paramIndices[6]), label));
    } else {
        addAndMakeVisible (envEnableToggle = new ToggleButton ("envEnableToggle"));
        addAndMakeVisible (envSyncToggle = new ToggleButton ("envSyncToggle"));
        addAndMakeVisible (durationSlider = new Slider("durationSlider"));
        addAndMakeVisible (envLoopSlider = new Slider("envLoopSlider"));
        addAndMakeVisible (loopDirectionComboBox = new ComboBox("loopDirectionComboBox"));
        addAndMakeVisible (susDirectionComboBox = new ComboBox("susDirectionComboBox"));
        addAndMakeVisible (triggerComboBox = new ComboBox("triggerComboBox"));

        std::cerr << "Parameters are not bound" << std::endl;
    }



    //[/Constructor_pre]

    //[UserPreSize]
    addAndMakeVisible (envelopeDrawing = new EnvelopeDrawing ("env"));
    envelopeDrawing->setName ("envelopeDrawing");

    envelopeDrawing->setColour(EnvelopeDrawing::backgroundColourId, Colours::black);
    envelopeDrawing->setColour(EnvelopeDrawing::outlineColourId, Colour(0xAA70C099));
    envelopeDrawing->setColour(EnvelopeDrawing::fillColourId, Colour(0xBB70C099));
    envelopeDrawing->setColour(EnvelopeDrawing::ellipseColourId, Colour(0xFF60B090));
    envelopeDrawing->setColour(EnvelopeDrawing::highLightColourId, Colour(0x9FA0C9B0));
    envelopeDrawing->setColour(EnvelopeDrawing::overlayGradientTwoId, Colours::transparentBlack);

    durationSlider->setRange (50.f, 5000.f, 0);
    durationSlider->setSliderStyle (Slider::LinearBar);
    durationSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    durationSlider->addListener(this);

    envLoopSlider->setTooltip (TRANS("Loop amount"));
    envLoopSlider->setRange (1, 32, 1);
    envLoopSlider->setSliderStyle (Slider::LinearBar);
    envLoopSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    envLoopSlider->postCommandMessage(ParamSlider::update);
    envLoopSlider->addListener(this);

    envEnableToggle->setButtonText (String());

    addAndMakeVisible (nameLabel = new Label ("nameLabel",
                                              TRANS("ENV x")));
    nameLabel->setText(name, dontSendNotification);
    nameLabel->setFont (Font ("Good Times", 15.00f, Font::plain));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour(Label::backgroundColourId, Colours::transparentBlack);
    nameLabel->setColour (Label::textColourId, Colour(0xFF4A997A));

    loopDirectionComboBox->setTooltip (TRANS("Loop direction \n"));
    loopDirectionComboBox->setEditableText (false);
    loopDirectionComboBox->setJustificationType (Justification::centred);
    loopDirectionComboBox->setTextWhenNothingSelected (TRANS("loop direction"));
    loopDirectionComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    loopDirectionComboBox->addItem (TRANS("normal"), 1);
    loopDirectionComboBox->addItem (TRANS("reverse"), 2);
    loopDirectionComboBox->addItem (TRANS("back&forth"), 3);
    loopDirectionComboBox->postCommandMessage(ParamComboBox::update);
    loopDirectionComboBox->addListener (this);

    susDirectionComboBox->setTooltip (TRANS("Sustain direction"));
    susDirectionComboBox->setEditableText (false);
    susDirectionComboBox->setJustificationType (Justification::centred);
    susDirectionComboBox->setTextWhenNothingSelected (TRANS("sus direction"));
    susDirectionComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    susDirectionComboBox->addItem (TRANS("normal"), 1);
    susDirectionComboBox->addItem (TRANS("reverse"), 2);
    susDirectionComboBox->addItem (TRANS("back&forth"), 3);
    susDirectionComboBox->postCommandMessage(ParamComboBox::update);
    susDirectionComboBox->addListener (this);

    envSyncToggle->setTooltip (TRANS("sync"));
    envSyncToggle->setButtonText (String());
    envSyncToggle->addListener (this);

    triggerComboBox->setTooltip (TRANS("Trigger"));
    triggerComboBox->setEditableText (false);
    triggerComboBox->setJustificationType (Justification::centred);
    triggerComboBox->setTextWhenNothingSelected (TRANS("env trigger"));
    triggerComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    triggerComboBox->addItem (TRANS("MIDI"), 1);
    triggerComboBox->addItem (TRANS("Audio"), 2);
    triggerComboBox->addSeparator();
    triggerComboBox->postCommandMessage(ParamComboBox::update);
    triggerComboBox->addListener (this);

    menu.addItem(1, "learn", true, false, nullptr);
    menu.addSeparator();
    menu.addItem(0xFF, "clear");

    initialiseMIDIStrings();
    //[/UserPreSize]

    setSize (515, 250);


    //[Constructor] You can add your own custom stuff here..
    double bpm = processor.getBeatsPerMinute();

    if(bpm < 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;
    initialiseTimeDivisions();
    calculateTimeDivision(beatsPerMinute);
    //[/Constructor]
}

EnvelopeComponent::~EnvelopeComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    envelopeDrawing = nullptr;
    durationSlider = nullptr;
    envLoopSlider = nullptr;
    envEnableToggle = nullptr;
    nameLabel = nullptr;
    loopDirectionComboBox = nullptr;
    susDirectionComboBox = nullptr;
    envSyncToggle = nullptr;
    triggerComboBox = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    duration = nullptr;
    //[/Destructor]
}

//==============================================================================
void EnvelopeComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colour (0xFF4A997A));
    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("duration"),
                423, 30, 73, 10,
                Justification::centred, true);

    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("loop nr"),
                420, 74, 80, 12,
                Justification::centred, true);

    g.drawText (TRANS("loop dir"),
                420, 119, 80, 12,
                Justification::centred, true);


    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("sustain dir"),
                420, 165, 80, 9,
                Justification::centred, true);

    g.setFont (Font ("Good Times", 11.00f, Font::plain));
    g.drawText (TRANS("trigger"),
                424, 209, 75, 14,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EnvelopeComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]
    envelopeDrawing->setBounds (5, 5, 400, 240);

    envEnableToggle->setBounds (492, 1, 24, 24);
    nameLabel->setBounds (429, 2, 66, 24);

    envSyncToggle->setBounds (493, 24, 20, 20);
    durationSlider->setBounds (412, 46, 96, 16);
    envLoopSlider->setBounds (412, 91, 96, 16);

    loopDirectionComboBox->setBounds (412, 136, 96, 16);
    susDirectionComboBox->setBounds (412, 181, 96, 16);
    triggerComboBox->setBounds (412, 226, 96, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EnvelopeComponent::sliderValueChanged(Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == durationSlider)
    {
        //[UserSliderCode_durationSlider] -- add your slider handling code here..
        String tmpName = durationSlider->getName();

        if(envSyncToggle != nullptr && duration != nullptr){
            const bool tmpSync = envSyncToggle->getToggleState();
            if(tmpSync){
                int tmpVal = durationSlider->getValue();

                if(tmpVal < beatDivision.size() &&
                   tmpVal < valueBeatDivision.size())
                {
                    double tmpDur = valueBeatDivision[tmpVal];
                    tmpName += beatDivision[tmpVal];
                    duration->setValue(tmpDur);
                }
            } else {
                double tmpVal = durationSlider->getValue();
                durationSlider->setRange(50.f, 5000.f);
                duration->setValue(tmpVal);

                if(envNumber == 1){

                } else if(envNumber == 2){

                }
                tmpName += String(tmpVal);
            }
            labelRef.setText(tmpName, dontSendNotification);
        }
        //[/UserSliderCode_durationSlider]
    }

    //[UsersliderValueChanged_Post]
    else if(sliderThatWasMoved == envLoopSlider){
        if(envNumber == 1){
            processor.updateEnvelopeOneLoopAmount();
        } else if(envNumber == 2){
            processor.updateEnvelopeTwoLoopAmount();
        }
    }
    //[/UsersliderValueChanged_Post]
}

void EnvelopeComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == envSyncToggle)
    {
        //[UserButtonCode_envSyncToggle] -- add your button handler code here..
        double tmpDur = 0.0;

        if(duration != nullptr &&
           !paramIndices.empty())
        {
            tmpDur = duration->getValue();

            const bool tmpSync = envSyncToggle->getToggleState();
            double tmpDur = duration->getValue();

            if(tmpSync){
                int tmpIndex = findClosestTimeDivision(tmpDur);

                durationSlider->setRange(0, valueBeatDivision.size()-1);
                durationSlider->setValue(tmpIndex, sendNotification);


                duration->setValue(valueBeatDivision[tmpIndex]);

                int last = valueBeatDivision.size()-1;

                if(processor.getRegisteredMidi(paramIndices[2])){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[2]);
                }

                if(envNumber == 1){
                    processor.setEnvelopeOneDurationBounds(valueBeatDivision[0],
                                                           valueBeatDivision[last]);
                } else if(envNumber == 2){
                    processor.setEnvelopeTwoDurationBounds(valueBeatDivision[0],
                                                           valueBeatDivision[last]);
                }
            } else {
                if(tmpDur >= 5000.f){
                    tmpDur = 5000.f;
                }

                if(tmpDur <= 50.f){
                    tmpDur = 50.f;
                }

                if(processor.getRegisteredMidi(paramIndices[2])){
                    processor.changeMidiRowMinMax(50.0, 5000.0, akateko::MIDI_TO_DOUBLE, paramIndices[2]);
                }

                durationSlider->setRange(50.f, 5000.f);
                duration->setValue(tmpDur);
                durationSlider->setValue(tmpDur, sendNotification);

                if(envNumber == 1){
                    processor.setEnvelopeOneDurationBounds(50.f, 5000.f);
                } else if(envNumber == 2){
                    processor.setEnvelopeTwoDurationBounds(50.f, 5000.f);
                }
            }
        } else {
            std::cerr << "Parameters not bound properly, duration not set : " << getName() << std::endl;
        }

        //[/UserButtonCode_envSyncToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void EnvelopeComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == loopDirectionComboBox)
    {
        //[UserComboBoxCode_loopDirectionComboBox] -- add your combo box handling code here..
        if(envNumber == 1){
           processor.updateEnvelopeOneLoopDirection();
        } else if (envNumber == 2){
           processor.updateEnvelopeTwoLoopDirection();
        }
        //getParentComponent()->postCommandMessage(commandLoopDirId);
        //[/UserComboBoxCode_loopDirectionComboBox]
    }
    else if (comboBoxThatHasChanged == susDirectionComboBox)
    {
        //[UserComboBoxCode_susDirectionComboBox] -- add your combo box handling code here..
        //getParentComponent()->postCommandMessage(commandSusDirId);
        if(envNumber == 1){
            processor.updateEnvelopeOneSustainDirection();
        } else if(envNumber == 2){
            processor.updateEnvelopeTwoSustainDirection();
        }
        //[/UserComboBoxCode_susDirectionComboBox]
    }
    else if (comboBoxThatHasChanged == triggerComboBox)
    {
        //[UserComboBoxCode_triggerComboBox] -- add your combo box handling code here..
        // Implement
        int index = triggerComboBox->getSelectedItemIndex();

        if(index == 0){
            StringArray &tmpArray = envOneMIDI;
            int section = 5;

            if(envNumber == 2){
                tmpArray = envTwoMIDI;
                section = 6;
            }

            if(tmpArray.size() == 7 &&
               paramIndices.size() == 8)
            {
                // Add MidiNote will be reset

                MidiRow tmpRow;

                initMidiRow(tmpRow, 6, 0, 127, 0, 1, paramIndices[7], akateko::MIDI_TO_INT_TOGGLE, tmpArray[6], section);
                tmpRow.initialised = true;
                tmpRow.midiMessage = akateko::MIDI_NOTE;
                tmpRow.firstByte = 60;
                tmpRow.channel = 1;


                // Doest not overwrite if the row already
                // existsIn the Midi Table
                processor.setMidiTriggerRow(tmpRow);
            }
        } else if(index == 1 && paramIndices.size() == 8){
            processor.removeMidiRow(paramIndices[7]);
        }

        //[/UserComboBoxCode_triggerComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void EnvelopeComponent::updateEnvelope(int env){
    float startPos = 0.f;
    float endPos = 0.f;
    envelopeDrawing->fillBuffer();
    msmBuffer buffer = envelopeDrawing->getBuffer();

    if(env == 1){
        processor.setEnvelopeOneBuffer(buffer);

        if(envelopeDrawing->getLoopPoints(startPos, endPos)){
            processor.setEnvelopeOneLoopPoints(startPos, endPos);
            processor.setEnvelopeOneLoopEnabled(true);
        } else {
            processor.setEnvelopeOneLoopEnabled(false);
        }

        if(envelopeDrawing->getSustainPoints(startPos, endPos)){
            processor.setEnvelopeOneSustainPoints(startPos, endPos);
            processor.setEnvelopeOneSustainEnabled(true);
        } else {
            processor.setEnvelopeOneSustainEnabled(false);
        }

        envelopeDrawing->getEndPoints(startPos, endPos);
        processor.setEnvelopeOneReleasePoint(startPos, endPos);
        processor.setUIState(envelopeDrawing->toString(), AkatekoAudioProcessor::Envelope1Id);
    } else if(env == 2){
        processor.setEnvelopeTwoBuffer(buffer);

        if(envelopeDrawing->getLoopPoints(startPos, endPos)){
            processor.setEnvelopeTwoLoopPoints(startPos, endPos);
            processor.setEnvelopeTwoLoopEnabled(true);
        } else {
            processor.setEnvelopeTwoLoopEnabled(false);
        }

        if(envelopeDrawing->getSustainPoints(startPos, endPos)){
            processor.setEnvelopeTwoSustainPoints(startPos, endPos);
            processor.setEnvelopeTwoSustainEnabled(true);
        } else {
            processor.setEnvelopeTwoSustainEnabled(false);
        }
        envelopeDrawing->getEndPoints(startPos, endPos);
        processor.setEnvelopeTwoReleasePoint(startPos, endPos);
        processor.setUIState(envelopeDrawing->toString(), AkatekoAudioProcessor::Envelope2Id);
    }
}

void EnvelopeComponent::updateLoop(int env){
    float startPos = 0;
    float endPos = 0;

    if(env == 1){
        if(envelopeDrawing->getLoopPoints(startPos, endPos)){
            processor.setEnvelopeOneLoopPoints(startPos, endPos);
            processor.setEnvelopeOneLoopEnabled(true);
        } else {
            processor.setEnvelopeOneLoopEnabled(false);
        }
        processor.setUIState(envelopeDrawing->toString(), AkatekoAudioProcessor::Envelope1Id);
    } else if(env == 2){
        if(envelopeDrawing->getLoopPoints(startPos, endPos)){
            processor.setEnvelopeTwoLoopPoints(startPos, endPos);
            processor.setEnvelopeTwoLoopEnabled(true);
        } else {
            processor.setEnvelopeTwoLoopEnabled(false);
        }
        processor.setUIState(envelopeDrawing->toString(), AkatekoAudioProcessor::Envelope2Id);
    }
}

void EnvelopeComponent::updateSustain(int env){
    float startPos = 0;
    float endPos = 0;

    if(env == 1){
        if(envelopeDrawing->getSustainPoints(startPos, endPos)){
            processor.setEnvelopeOneSustainPoints(startPos, endPos);
            processor.setEnvelopeOneSustainEnabled(true);
        } else {
            processor.setEnvelopeOneSustainEnabled(false);
        }
        processor.setUIState(envelopeDrawing->toString(), AkatekoAudioProcessor::Envelope1Id);
    } else if(env == 2){
        if(envelopeDrawing->getSustainPoints(startPos, endPos)){
            processor.setEnvelopeTwoSustainPoints(startPos, endPos);
            processor.setEnvelopeTwoSustainEnabled(true);
        } else {
            processor.setEnvelopeTwoSustainEnabled(false);
        }
        processor.setUIState(envelopeDrawing->toString(), AkatekoAudioProcessor::Envelope2Id);
    }
}

bool EnvelopeComponent::getLoopPoints(float &startPos, float &endPos){
    return envelopeDrawing->getLoopPoints(startPos, endPos);
}

bool EnvelopeComponent::getSustainPoints(float &startPos, float &endPos){
   return envelopeDrawing->getSustainPoints(startPos, endPos);
}

void EnvelopeComponent::getEndPoints(float &startPos, float &endPos){
    envelopeDrawing->getEndPoints(startPos, endPos);
}

void EnvelopeComponent::setUIState(String state){
    envelopeDrawing->restoreFromString(state);
    envelopeDrawing->fillBuffer();
    envelopeDrawing->repaint();
}

String EnvelopeComponent::getUIState(){
    return envelopeDrawing->toString();
}

/*
void EnvelopeComponent::setBeatDivisionStrings(StringArray beatDivStr){
    int steps = beatDivStr.size()-1;
    beatDivision.clear();


    while(steps >= 0){
        beatDivision.add(beatDivStr[steps]);
        steps--;
    }
}

void EnvelopeComponent::setBeatDivisionValues(vector<double> beatDivVal){
    int steps = beatDivVal.size()-1;
    valueBeatDivision.clear();

    while(steps >= 0){
        valueBeatDivision.push_back(beatDivVal[steps]);
        steps--;
    }
}
*/
void EnvelopeComponent::initDurationSlider(){
    if(duration != nullptr &&
       envSyncToggle != nullptr &&
       beatDivision.size() != 0 &&
       valueBeatDivision.size() != 0)
    {
        double tmpDur = duration->getValue();
        const bool tmpSync = envSyncToggle->getToggleState();

        if(tmpSync){
            int tmpIndex = findClosestTimeDivision(tmpDur);

            durationSlider->setRange(0, valueBeatDivision.size()-1);
            durationSlider->setValue(tmpIndex);
        } else {
            if(tmpDur >= 5000.f){
                tmpDur = 5000.f;
            }

            if(tmpDur <= 50.f){
                tmpDur = 50.f;
            }

            durationSlider->setRange(50.f, 5000.f);
            durationSlider->setValue(tmpDur);
        }
    } else {
        std::cout << "env generator" << std::endl;

        if(duration == nullptr || envSyncToggle == nullptr){
            std::cerr << "Parameter are not bound properly : " << getName() << std::endl;
        }

        if(beatDivision.size() == 0 || valueBeatDivision.size())
        {
            std::cerr << "Beat division tables not set properly : " << getName() << std::endl;
        }
    }
}

int EnvelopeComponent::findClosestTimeDivision(double period){
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

void EnvelopeComponent::updateGui(){
    envEnableToggle->postCommandMessage(ParamToggle::update);
    envSyncToggle->postCommandMessage(ParamToggle::update);
    envLoopSlider->postCommandMessage(ParamSlider::update);
    loopDirectionComboBox->postCommandMessage(ParamComboBox::update);
    susDirectionComboBox->postCommandMessage(ParamComboBox::update);
    triggerComboBox->postCommandMessage(ParamComboBox::update);

    double bpm = processor.getBeatsPerMinute();

    if(paramIndices.size() == 8){
        if(envSyncToggle->getToggleState()){
            if(processor.getRegisteredMidi(paramIndices[2])){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[2]);
            }
        } else {
            if(processor.getRegisteredMidi(paramIndices[2])){
                processor.changeMidiRowMinMax(50.0, 5000.0, akateko::MIDI_TO_DOUBLE, paramIndices[2]);
            }
        }
    }

    if(bpm < 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;
    initialiseTimeDivisions();
    calculateTimeDivision(beatsPerMinute);

    updateEnvelope(envNumber);
}

void EnvelopeComponent::setLookAndFeel(LookAndFeel *laf){
    envEnableToggle->setLookAndFeel(laf);
    durationSlider->setLookAndFeel(laf);
    envLoopSlider->setLookAndFeel(laf);
    loopDirectionComboBox->setLookAndFeel(laf);
    susDirectionComboBox->setLookAndFeel(laf);
    envSyncToggle->setLookAndFeel(laf);
    triggerComboBox->setLookAndFeel(laf);
    envelopeDrawing->setLookAndFeel(laf);
    menu.setLookAndFeel(laf);
}

void EnvelopeComponent::initialiseMIDIStrings(){
    // Env One
    envOneMIDI.clear();

    envOneMIDI.add(" Env One: Enable");
    envOneMIDI.add(" Env One: Sync");
    envOneMIDI.add(" Env One: Dur");
    envOneMIDI.add(" Env One: Loop Nr");
    envOneMIDI.add(" Env One: Loop Dir");
    envOneMIDI.add(" Env One: Sus Dir");
    envOneMIDI.add(" Env One: Trigger");

    // Env Two
    envTwoMIDI.clear();

    envTwoMIDI.add(" Env Two: Enable");
    envTwoMIDI.add(" Env Two: Sync");
    envTwoMIDI.add(" Env Two: Dur");
    envTwoMIDI.add(" Env Two: Loop Nr");
    envTwoMIDI.add(" Env Two: Loop Dir");
    envTwoMIDI.add(" Env Two: Sus Dir");
    envTwoMIDI.add(" Env Two: Trigger");
}

void EnvelopeComponent::handleCommandMessage(int commandId){
    switch(commandId){
        case EnvelopeDrawing::envelopeUpdated:
            updateEnvelope(envNumber);
            break;
        case EnvelopeDrawing::loopPointsUpdated:
            updateLoop(envNumber);
            break;
        case EnvelopeDrawing::sustainPointsUpdated:
            updateSustain(envNumber);
            break;
    }

    if(paramIndices.size() == 8 &&
       envOneMIDI.size() == 7 &&
       envTwoMIDI.size() == 7)
    {
        int index = -1;
        int param = -1;
        int handling = -1;
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
        } else if(commandId == requestMenuIds[2]){  // Duration           
            index = menu.show();

            param = 2;

            if(envSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                minValue = 50.;
                maxValue = 5000.;
                handling = akateko::MIDI_TO_DOUBLE;
            }
        } else if(commandId == requestMenuIds[3]){  // Loop Number
            index = menu.show();
            param = 3;
            minValue = 1;
            maxValue = 32;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[4]){  // Loop Direction
            index = menu.show();
            param = 4;
            maxValue = 2;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[5]){  // Sustain Direction
            index = menu.show();
            param = 5;
            maxValue = 2;
            handling = akateko::MIDI_TO_INT;
        }

        StringArray &tmpArray = envOneMIDI;
        int section = 5;

        if(envNumber == 2){
            tmpArray = envTwoMIDI;
            section = 6;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling, tmpArray[param], section); // Enable
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);

            String message = tmpArray[param] + String(" cleared");
            labelRef.setText(message, sendNotificationAsync);
        }
    }
}

void EnvelopeComponent::calculateTimeDivision(double bpm){
    if(bpm > 0.0){
        double crotchet = 60000.0/bpm;

        // Calculate 1/128 Note, multiply with 2
        valueBeatDivision.clear();

        for(int i=0; i<beatDivision.size(); i++){
            valueBeatDivision.push_back(crotchet*env_timing_ratios[i]);
        }
    }
    initDurationSlider();
}

void EnvelopeComponent::initialiseTimeDivisions(){
    StringArray noteVals;
    beatDivision.clear();

    noteVals.add("1/16N");
    noteVals.add("1/8N");
    noteVals.add("1/4N");
    noteVals.add("1/2N");
    noteVals.add("1N");
    noteVals.add("2N");
    noteVals.add("3N");
    noteVals.add("4N");

    for(int i=0; i<noteVals.size(); i++){
        String triplet = noteVals[i] + String("T");
        String dot = noteVals[i] + String("D");

        beatDivision.add(triplet);
        beatDivision.add(noteVals[i]);
        beatDivision.add(dot);
    }
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EnvelopeComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, int env, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;commandTrigger(-1)&#10;commandUpdate(-1)&#10;commandLoop(-1)&#10;commandSustain(-1)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="515" initialHeight="250">
  <BACKGROUND backgroundColour="ff323232">
    <TEXT pos="426 30 75 14" fill="solid: ffaaaaaa" hasStroke="0" text="duration"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
    <TEXT pos="440 79 46 12" fill="solid: ffaaaaaa" hasStroke="0" text="loop"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
    <TEXT pos="432 153 67 9" fill="solid: ffaaaaaa" hasStroke="0" text="sustain"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
    <TEXT pos="425 200 75 14" fill="solid: ffaaaaaa" hasStroke="0" text="trigger"
          fontname="Good Times" fontsize="11" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="envelopeDrawing" id="86fad89d553cc7f2" memberName="envelopeDrawing"
                    virtualName="EnvelopeDrawing" explicitFocusOrder="0" pos="5 5 400 240"
                    class="Component" params="&quot;env&quot;"/>
  <SLIDER name="durationSlider" id="db2dceea622285cd" memberName="durationSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 48 104 20"
          min="0" max="1" int="0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="loopAmount" id="ed354e636e331bd5" memberName="envLoopSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="408 96 104 17"
          tooltip="Loop amount" min="0" max="1" int="0.03125" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <TEXTBUTTON name="triggerButton" id="17f7124f53ee557" memberName="triggerButton"
              virtualName="" explicitFocusOrder="0" pos="416 200 12 12" tooltip="Trigger Envelope"
              buttonText="t" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="envEnableToggle" id="629774b54172d615" memberName="envEnableToggle"
                virtualName="" explicitFocusOrder="0" pos="494 0 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <LABEL name="nameLabel" id="a3d123be4d25c5a3" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="428 1 66 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="ENV x" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Good Times"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="releaseButton" id="23afa99b5e6054c6" memberName="releaseButton"
              virtualName="" explicitFocusOrder="0" pos="496 200 12 12" tooltip="Enter Release Stage"
              buttonText="r" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="loopDirection" id="47b3e58ec0ddffd6" memberName="loopDirectionComboBox"
            virtualName="" explicitFocusOrder="0" pos="412 120 96 20" tooltip="Loop direction &#10;"
            editable="0" layout="36" items="normal&#10;reverse&#10;back&amp;forth"
            textWhenNonSelected="loop direction" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="susDirectionComboBox" id="6e18d7f500758902" memberName="susDirectionComboBox"
            virtualName="" explicitFocusOrder="0" pos="412 168 96 20" tooltip="Sustain direction"
            editable="0" layout="36" items="normal&#10;reverse&#10;back&amp;forth"
            textWhenNonSelected="sus direction" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="envSyncToggle" id="6b886aeaa091a029" memberName="envSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="408 24 24 24" tooltip="sync"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="triggerComboBox" id="e52ec3bd37740469" memberName="triggerComboBox"
            virtualName="" explicitFocusOrder="0" pos="412 216 96 20" tooltip="Trigger"
            editable="0" layout="36" items="MIDI&#10;Audio&#10;" textWhenNonSelected="env trigger"
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
