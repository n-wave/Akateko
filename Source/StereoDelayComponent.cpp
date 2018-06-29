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

#include "StereoDelayComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "Akateko.h"

using akateko::delay_timing_ratios;
using akateko::initMidiRow;
using akateko::updateFxGUI;
using akateko::updateFxBPM;
using akateko::MidiRow;
using akateko::IgnoreRightClick;
/*
stereoDelEnable
stereoDelCrossEnable
stereoDelCrossAmount
stereoDelMix
stereoDelLeftSync
stereoDelLeft
stereoDelLeftFB
stereoDelRightSync
stereoDelRight
stereoDelRightFB

10
*/

using std::vector;

//[/MiscUserDefs]

//==============================================================================
StereoDelayComponent::StereoDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      leftDelaySlider(nullptr),
      rightDelaySlider(nullptr),
      rightFBSlider(nullptr),
      leftFBSlider(nullptr),
      crossSlider(nullptr),
      mixSlider(nullptr),
      enableToggle(nullptr),
      leftSyncToggle(nullptr),
      rightSyncToggle(nullptr),
      crossEnabledToggle(nullptr),
      leftDelay(nullptr),
      rightDelay(nullptr),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::StereoDelayId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::StereoDelayId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    bool proceed = false;

    if(paramIndices.size() == 10 && paramIds.size() == 10){
        int lastIndex = paramIndices[9];

        if(params.size() >= lastIndex){
            proceed = true;
        }
    }

    if(proceed){
        requestMenuIds[0] = paramIds[0].hash();
        requestMenuIds[1] = paramIds[1].hash();
        requestMenuIds[2] = paramIds[2].hash();
        requestMenuIds[3] = paramIds[3].hash();
        requestMenuIds[4] = paramIds[4].hash();
        requestMenuIds[5] = paramIds[5].hash();
        requestMenuIds[6] = paramIds[6].hash();
        requestMenuIds[7] = paramIds[7].hash();
        requestMenuIds[8] = paramIds[8].hash();
        requestMenuIds[9] = paramIds[9].hash();

        initialiseMidiStrings();

        addAndMakeVisible(enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(crossEnabledToggle = new ParamImageToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), label));
        addAndMakeVisible(crossSlider = new ParamSlider(paramIds[2], *params.getUnchecked(paramIndices[2]), label, 0, 1.f));
        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label));

        addAndMakeVisible(leftSyncToggle = new ParamImageToggle(paramIds[4], *params.getUnchecked(paramIndices[4]), label));
        addAndMakeVisible (leftDelaySlider = new IgnoreRightClick<Slider> (paramIds[5]));
        leftDelay = p.getParameters().getUnchecked(paramIndices[5]);
        addAndMakeVisible(leftFBSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), label, -1.f, 1.f));

        addAndMakeVisible(rightSyncToggle = new ParamImageToggle(paramIds[7], *params.getUnchecked(paramIndices[7]), label));
        addAndMakeVisible (rightDelaySlider = new IgnoreRightClick<Slider> (paramIds[8]));
        rightDelay = params.getUnchecked(paramIndices[8]);
        addAndMakeVisible(rightFBSlider = new ParamSlider(paramIds[9], *params.getUnchecked(paramIndices[9]), label, -1.f, 1.f));

        //std::cout << "L-Delay Slider : " << paramIndices[5] << std::endl;
        //std::cout << "R-Delay Slider : " << paramIndices[8] << std::endl;

    } else {
        addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));
        addAndMakeVisible (crossEnabledToggle = new ImageButton ("crossEnabledToggle"));
        addAndMakeVisible (crossSlider = new Slider ("crossSlider"));
        addAndMakeVisible (mixSlider = new Slider ("mixSlider"));

        addAndMakeVisible (leftSyncToggle = new ImageButton ("leftSyncToggle"));
        addAndMakeVisible (leftDelaySlider = new Slider ("leftDelaySlider"));
        addAndMakeVisible (leftFBSlider = new Slider ("leftFBSlider"));

        addAndMakeVisible (rightSyncToggle = new ImageButton ("rightSyncToggle"));
        addAndMakeVisible (rightDelaySlider = new Slider ("rightDelaySlider"));
        addAndMakeVisible (rightFBSlider = new Slider ("rightFBSlider"));

        std::cerr << "StereoDelayComponent::StereoDelayComponent" << std::endl;
        std::cerr << "Parameters not bound properly" << std::endl;
    }

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");

    //[/Constructor_pre]

    //[UserPreSize]

    const Image bigRotary = ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize);
    const Image fbRotary = ImageCache::getFromMemory(BinaryData::PanBig_png, BinaryData::PanBig_pngSize);

    claf = new CustomLookAndFeel();
    blaf = new SliderLookAndFeel(bigRotary);
    fblaf = new SliderLookAndFeel(fbRotary);

    menu.setLookAndFeel(claf);

    leftDelaySlider->setRange (0, 1, 0);
    leftDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftDelaySlider->setLookAndFeel(blaf);
    leftDelaySlider->addListener(this);

    rightDelaySlider->setRange (0, 1, 0);
    rightDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightDelaySlider->setLookAndFeel(blaf);
    rightDelaySlider->addListener(this);

    rightFBSlider->setRange (0, 1, 0);
    rightFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightFBSlider->setLookAndFeel(fblaf);
    rightFBSlider->setDoubleClickReturnValue(true, 0.5);

    leftFBSlider->setRange (0, 1, 0);
    leftFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftFBSlider->setLookAndFeel(fblaf);
    leftFBSlider->setDoubleClickReturnValue(true, 0.5);

    crossSlider->setRange (0, 1, 0);
    crossSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    crossSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    crossSlider->setLookAndFeel(blaf);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mixSlider->setLookAndFeel(blaf);

    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    leftSyncToggle->setButtonText (String());
    leftSyncToggle->setImages(false, true, false,
                              ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                              ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                              ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    leftSyncToggle->addListener (this);

    rightSyncToggle->setButtonText (String());
    rightSyncToggle->setImages(false, true, false,
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                               ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    rightSyncToggle->addListener (this);

    crossEnabledToggle->setButtonText (String());
    crossEnabledToggle->setImages(false, true, false,
                                  ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                                  ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                  ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    crossEnabledToggle->addListener (this);
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    double bpm = processor.getBeatsPerMinute();

    if(bpm < 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;

    initialiseTimeDivisions();
    calculateTimeDivision(beatsPerMinute);

    // Add paramDestinations for use in Matrix Table

    //[/Constructor]
}

StereoDelayComponent::~StereoDelayComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    leftDelaySlider = nullptr;
    rightDelaySlider = nullptr;
    rightFBSlider = nullptr;
    leftFBSlider = nullptr;
    crossSlider = nullptr;
    mixSlider = nullptr;
    enableToggle = nullptr;
    leftSyncToggle = nullptr;
    rightSyncToggle = nullptr;
    crossEnabledToggle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    leftDelay = nullptr;
    rightDelay = nullptr;
    blaf = nullptr;
    claf = nullptr;
    //[/Destructor]
}

//==============================================================================
void StereoDelayComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("MIX"),
                198, 146, 33, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("cross"),
                84, 146, 53, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L FB"),
                147, 82, 33, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R FB"),
                147, 210, 33, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L DELAY"),
                12, 82, 68, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R DELAY"),
                12, 210, 68, 10,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StereoDelayComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    leftDelaySlider->setBounds (16, 24, 60, 60);
    rightDelaySlider->setBounds (16, 152, 60, 60);
    rightFBSlider->setBounds (136, 155, 56, 56);
    leftFBSlider->setBounds (136, 27, 56, 56);
    crossSlider->setBounds (88, 96, 48, 48);
    mixSlider->setBounds (188, 92, 56, 56);

    enableToggle->setBounds (238, 0, 18, 18);
    leftSyncToggle->setBounds (75, 45, 16, 16);
    rightSyncToggle->setBounds (75, 172, 16, 16);
    crossEnabledToggle->setBounds (135, 111, 16, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void StereoDelayComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == leftDelaySlider)
    {
        //[UserSliderCode_leftDelaySlider] -- add your slider handling code here..
        updateSliderValue(leftSyncToggle, leftDelaySlider, leftDelay);
        //[/UserSliderCode_leftDelaySlider]
    }
    else if (sliderThatWasMoved == rightDelaySlider)
    {
        //[UserSliderCode_rightDelaySlider] -- add your slider handling code here..
        updateSliderValue(rightSyncToggle, rightDelaySlider, rightDelay);
        //[/UserSliderCode_rightDelaySlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void StereoDelayComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == leftSyncToggle)
    {
        //[UserButtonCode_leftSyncToggle] -- add your button handler code here..
        updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);


        if(paramIndices.size() == 10 &&
           timeDivision.size() == 24 &&
           processor.getRegisteredMidi(paramIndices[5]))
        {
            if(leftSyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[5]);
            } else {
                processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[5]);
            }
        }

        //[/UserButtonCode_leftSyncToggle]
    }
    else if (buttonThatWasClicked == rightSyncToggle)
    {
        //[UserButtonCode_rightSyncToggle] -- add your button handler code here..
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);

        if(paramIndices.size() == 10 &&
           timeDivision.size() == 24 &&
           processor.getRegisteredMidi(paramIndices[8]))
        {
            if(rightSyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[8]);
            } else {
                processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[8]);
            }
        }
        //[/UserButtonCode_rightSyncToggle]
    }


    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void StereoDelayComponent::initialiseTimeDivisions(){
    StringArray noteVals;

    noteVals.add("1/128N");
    noteVals.add("1/64N");
    noteVals.add("1/32N");
    noteVals.add("1/16N");
    noteVals.add("1/8N");
    noteVals.add("1/4N");
    noteVals.add("1/2N");
    noteVals.add("1N");

    division.clear();

    for(int i=0; i<noteVals.size(); i++){
        String dot = noteVals[i] + String("D");
        String triplet = noteVals[i] + String("T");

        division.add(triplet);
        division.add(noteVals[i]);
        division.add(dot);
    }
}


void StereoDelayComponent::calculateTimeDivision(double bpm){
    if(bpm > 0.0){
        double crotchet = 60000.0/bpm;

        // Calculate 1/128 Note, multiply with 2
        timeDivision.clear();

        for(int i=0; i<division.size(); i++){
            timeDivision.push_back(crotchet*delay_timing_ratios[i]);
        }
    }
    updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
    updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
}

int StereoDelayComponent::getTimeDivisionIndex(vector<double> values, double time){
    int index = 0;

    while(index < values.size()){
        double tmpValue = values[index];
        double error = tmpValue*0.05;

        if(time >= tmpValue-error &&
           time <= tmpValue+error)
        {
            return index;
        }
        index++;
    }
    return 0;
}

int StereoDelayComponent::findClosestTimeDivision(double time){
    double tmpTime = time;
    int result = 0;
    int nrOfSteps = timeDivision.size();

    if(tmpTime != 0 && nrOfSteps != 0){
        bool run = true;
        int index = 1;

        while(run && index < nrOfSteps-1){
            double prev = timeDivision[index-1];
            double val = timeDivision[index];
            double next = timeDivision[index+1];

            if(val == tmpTime){
                result = index;
                run = false;
            }

            if(tmpTime >= prev && tmpTime < val){
                result = index-1;
                run = false;
            }

            if(tmpTime <= next && tmpTime > val){
                result = index+1;
                run = false;
            }
            index += 3;
        }
    }
    return result;
}

void StereoDelayComponent::updateSliderRange(Button *toggle,
                                             Slider *slider,
                                             AudioProcessorParameter *param)
{

    if(toggle != nullptr &&
       slider != nullptr &&
       param != nullptr &&
       division.size() != 0 &&
       timeDivision.size() != 0)
    {
        double tmpTime = param->getValue();
        const bool tmpSync = toggle->getToggleState();

        if(tmpSync){
            const int tmpIndex = findClosestTimeDivision(tmpTime);

            slider->setRange(0, timeDivision.size()-1);
            slider->setValue(tmpIndex, sendNotificationAsync);
            param->setValue(timeDivision[tmpIndex]);
        } else {
            double maxTime = timeDivision[timeDivision.size()-1];

            if(tmpTime >= maxTime){
                tmpTime = maxTime;
            }

            if(tmpTime < 0){
                tmpTime = 0;
            }

            slider->setRange(0.0, maxTime);
            slider->setValue(tmpTime, sendNotificationAsync);
            param->setValue(tmpTime);
        }
    } else {
        std::cerr << "StereoDelayComponent::updateSliderRange" << std::endl;

        if(toggle == nullptr && toggle == nullptr){
             std::cerr << "Parameters not bound properly" << std::endl;
        }

        if(division.size() == 0 || timeDivision.size() == 0){
            std::cerr << "Time Division tables not set" << std::endl;
        }
    }
}

void StereoDelayComponent::updateSliderValue(Button *toggle,
                                             Slider *slider,
                                             AudioProcessorParameter *param)
{
    if(toggle != nullptr &&
       slider != nullptr &&
       param != nullptr &&
       division.size() != 0 &&
       timeDivision.size() != 0)
    {
       String tmpName = slider->getName();
       const bool tmpSync = toggle->getToggleState();

       if(tmpSync){
         int tmpVal = slider->getValue();

         if(tmpVal < division.size() &&
            tmpVal < timeDivision.size())
         {
            double tmpTime = timeDivision[tmpVal];
            tmpName += division[tmpVal];
            param->setValue(tmpTime);
         }
       } else {
            double tmpTime = slider->getValue();
            param->setValue(tmpTime);
            tmpName += String(tmpTime, 2);
       }
       labelRef.setText(tmpName, sendNotificationAsync);
    } else {
        std::cerr << "StereoDelayComponent::updateSliderValue" << std::endl;
    }
}

void StereoDelayComponent::handleCommandMessage(int commandId){
    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        crossEnabledToggle->postCommandMessage(ParamImageToggle::update);
        crossSlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);

        leftSyncToggle->postCommandMessage(ParamImageToggle::update);
        leftFBSlider->postCommandMessage(ParamSlider::update);

        rightSyncToggle->postCommandMessage(ParamImageToggle::update);
        rightFBSlider->postCommandMessage(ParamSlider::update);

        if(paramIndices.size() == 10){
            if(processor.getRegisteredMidi(paramIndices[5])){
                if(leftSyncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[5]);
                } else {
                    processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[5]);
                }
            }

            if(processor.getRegisteredMidi(paramIndices[8])){
                if(rightSyncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[8]);
                } else {
                    processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[8]);
                }
            }
        }

        updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
    } else if(commandId == updateFxBPM){
        double bpm = processor.getBeatsPerMinute();

        if(bpm < 20.0){
            bpm = 20.0;
        }

        beatsPerMinute = bpm;

        calculateTimeDivision(beatsPerMinute);
    }

    if(paramIndices.size() == 10 &&
       midiStrings.size() == 10 &&
       timeDivision.size() == 24)
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
        } else if(commandId == requestMenuIds[1]){  // Cross
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[2]){  // Cross Amplitude
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[3]){  // Mix
            index = menu.show();
            param = 3;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[4]){  // Left Sync
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[5]){  // Left Delay
            index = menu.show();
            param = 5;

            if(leftSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = timeDivision[23];
                handling = akateko::MIDI_TO_DOUBLE;
            }

        } else if(commandId == requestMenuIds[6]){  // Left Feedback
            index = menu.show();
            param = 6;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[7]){  // Right Sync
            index = menu.show();
            param = 7;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[8]){  // Right Delay
            index = menu.show();
            param = 8;

            if(rightSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = timeDivision[timeDivision.size()-1];
                handling = akateko::MIDI_TO_DOUBLE;
            }

        } else if(commandId == requestMenuIds[9]){  // Right Feedback
            index = menu.show();
            param = 9;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param],handling ,midiStrings[param], 10);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }
}

void StereoDelayComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" S-Delay: Enable");
    midiStrings.add(" S-Delay: Cross");
    midiStrings.add(" S-Delay: Cross Amp");
    midiStrings.add(" S-Delay: Mix");
    midiStrings.add(" S-Delay: L-Sync");
    midiStrings.add(" S-Delay: L-Delay");
    midiStrings.add(" S-Delay: L-FB");
    midiStrings.add(" S-Delay: R-Sync");
    midiStrings.add(" S-Delay: R-Delay");
    midiStrings.add(" S-Delay: R-FB");
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StereoDelayComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="198 146 33 10" fill="solid: ffaaaaaa" hasStroke="0" text="MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="84 146 53 10" fill="solid: ffaaaaaa" hasStroke="0" text="cross"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="147 82 33 10" fill="solid: ffaaaaaa" hasStroke="0" text="L FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="147 210 33 10" fill="solid: ffaaaaaa" hasStroke="0" text="R FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="12 82 68 10" fill="solid: ffaaaaaa" hasStroke="0" text="L DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="12 210 68 10" fill="solid: ffaaaaaa" hasStroke="0" text="R DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="leftDelaySlider" id="b9d7855b70230b1a" memberName="leftDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="16 24 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightDelaySlider" id="9d4bd59bbad98e0c" memberName="rightDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="16 152 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightFBSlider" id="b8d6f8c56f23720b" memberName="rightFBSlider"
          virtualName="" explicitFocusOrder="0" pos="136 155 56 56" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="leftFBSlider" id="f5c510b0381ad3a3" memberName="leftFBSlider"
          virtualName="" explicitFocusOrder="0" pos="136 27 56 56" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="crossSlider" id="cf347ac5b7359227" memberName="crossSlider"
          virtualName="" explicitFocusOrder="0" pos="88 96 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="mixSlider" id="6c37cee5e1efaec4" memberName="mixSlider"
          virtualName="" explicitFocusOrder="0" pos="188 92 56 56" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <IMAGEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="leftSyncToggle" id="74bc5579149da4c2" memberName="leftSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="75 40 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="rightSyncToggle" id="73efd299c17c4595" memberName="rightSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="75 168 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="crossEnabledToggle" id="4cc58e3caea10610" memberName="crossEnabledToggle"
                virtualName="" explicitFocusOrder="0" pos="134 108 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
