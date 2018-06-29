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

#include "PingPongComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "Akateko.h"

using akateko::updateFxBPM;
using akateko::updateFxGUI;
using akateko::delay_timing_ratios;
using akateko::initMidiRow;
using akateko::IgnoreRightClick;
using akateko::MidiRow;

using std::vector;

//[/MiscUserDefs]

//==============================================================================
PingPongComponent::PingPongComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      beatsPerMinute(120.0),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    /*
     *  pingPongEnable;
     *  pingPongLeftSync;
     *  pingPongLeftDelay;
     *  pingPongLeftFB;
     *  pingPongLeftMix;
     *
     *  pingPongFeedEnable;
     *  pingPongFeed;
     *
     *  pingPongRightSync;
     *  pingPongRightDelay;
     *  pingPongRightFB;
     *  pingPongRightMix;
     *
     * 10
     */

    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::PingPongDelayId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::PingPongDelayId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    bool proceed = false;

    if(paramIndices.size() == 9 && paramIds.size() == 9){
        int lastIndex = paramIndices[8];

        if(params.size() >= lastIndex){
            proceed = true;
        }
    }

    if(proceed){
        requestMenuId[0] = paramIds[0].hash();
        requestMenuId[1] = paramIds[1].hash();
        requestMenuId[2] = paramIds[2].hash();
        requestMenuId[3] = paramIds[3].hash();
        requestMenuId[4] = paramIds[4].hash();
        requestMenuId[5] = paramIds[5].hash();
        requestMenuId[6] = paramIds[6].hash();
        requestMenuId[7] = paramIds[7].hash();
        requestMenuId[8] = paramIds[8].hash();

        initialiseMidiStrings();

        addAndMakeVisible (enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible (leftSyncToggle = new ParamImageToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), label));
        // Left Delay Slider
        addAndMakeVisible (leftDelaySlider = new IgnoreRightClick<Slider>(paramIds[2]));
        leftDelay = params.getUnchecked(paramIndices[2]);

        addAndMakeVisible (leftFBSlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label, -1.f, 1.f));
        addAndMakeVisible (leftMixSlider = new ParamSlider (paramIds[4], *params.getUnchecked(paramIndices[4]), label));

        addAndMakeVisible (rightSyncToggle = new ParamImageToggle (paramIds[5], *params.getUnchecked(paramIndices[5]), label));

        // Left Right Slider
        addAndMakeVisible (rightDelaySlider = new IgnoreRightClick<Slider> (paramIds[6]));
        rightDelay = params.getUnchecked(paramIndices[6]);

        addAndMakeVisible (rightFBSlider = new ParamSlider (paramIds[7], *params.getUnchecked(paramIndices[7]), label, -1.f, 1.f));
        addAndMakeVisible (rightMixSlider = new ParamSlider (paramIds[8], *params.getUnchecked(paramIndices[8]), label));

        //std::cout << "PP L-Delay" << paramIndices[2] << std::endl;
        //std::cout << "PP R-Delay" << paramIndices[6] << std::endl;
    } else {
       addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));

       addAndMakeVisible (leftSyncToggle = new ImageButton ("leftSyncToggle"));
       addAndMakeVisible (leftDelaySlider = new Slider ("leftDelaySlider"));
       addAndMakeVisible (leftFBSlider = new Slider ("leftFBSlider"));
       addAndMakeVisible (leftMixSlider = new Slider ("leftMixSlider"));

       addAndMakeVisible (rightSyncToggle = new ImageButton ("rightSyncToggle"));
       addAndMakeVisible (rightDelaySlider = new Slider ("rightDelaySlider"));
       addAndMakeVisible (rightFBSlider = new Slider ("rightFBSlider"));
       addAndMakeVisible (rightMixSlider = new Slider ("rightMixSlider"));

       std::cerr << "PingPongComponent::PingPongComponent" << std::endl;
       std::cerr << "Parameters not bound properly" << std::endl;

       std::cerr << "/------------------------------------------/" << std::endl;
       std::cerr << "Parsing supplied ParamIndices and paramIds" << std::endl;
       std::cerr << "/------------------------------------------/" << std::endl;

       int index = 0;

       while(paramIds.size() != index &&
             paramIndices.size() != index)
       {
           std::cerr << "ParamIndex ["
                     << paramIndices[index]
                     << "] : "
                     << paramIds[index]
                     << std::endl;

           index++;
       }

    }

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
    leftDelaySlider->addListener (this);

    rightDelaySlider->setRange (0, 1, 0);
    rightDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightDelaySlider->setLookAndFeel(blaf);
    rightDelaySlider->addListener (this);

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

    leftMixSlider->setRange (0, 1, 0);
    leftMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftMixSlider->setLookAndFeel(blaf);

    rightMixSlider->setRange (0, 1, 0);
    rightMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightMixSlider->setDoubleClickReturnValue(true, 0.5);
    rightMixSlider->setLookAndFeel(blaf);
    //[/UserPreSize]

    enableToggle->setButtonText (String());
    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    enableToggle->addListener (this);

    leftSyncToggle->setImages(false, true, false,
                              ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                              ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                              ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    leftSyncToggle->addListener (this);

    rightSyncToggle->addListener (this);
    rightSyncToggle->setImages(false, true, false,
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                               ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    double bpm = processor.getBeatsPerMinute();

    if(bpm < 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;

    initialiseTimeDivisions();
    calculateTimeDivision(beatsPerMinute);

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");

    //[/Constructor]
}

PingPongComponent::~PingPongComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    leftDelaySlider = nullptr;
    rightDelaySlider = nullptr;
    rightFBSlider = nullptr;
    leftFBSlider = nullptr;
    leftMixSlider = nullptr;
    enableToggle = nullptr;
    leftSyncToggle = nullptr;
    rightSyncToggle = nullptr;
    rightMixSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    leftDelay = nullptr;
    rightDelay = nullptr;
    blaf = nullptr;
    claf = nullptr;
    //[/Destructor]
}

//==============================================================================
void PingPongComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L DELAY"),
                13, 101, 62, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R DELAY"),
                13, 197, 62, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L FB"),
                112, 101, 36, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R FB"),
                112, 197, 36, 11,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L MIX"),
                189, 101, 44, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R MIX"),
                189, 197, 44, 10,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PingPongComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    leftDelaySlider->setBounds (15, 40, 60, 60);
    rightDelaySlider->setBounds (15, 136, 60, 60);
    rightFBSlider->setBounds (99, 136, 60, 60);
    leftFBSlider->setBounds (99, 40, 60, 60);
    leftMixSlider->setBounds (183, 40, 60, 60);
    rightMixSlider->setBounds (183, 136, 60, 60);

    enableToggle->setBounds (238, 0, 18, 18);
    leftSyncToggle->setBounds (74, 61, 16, 16);
    rightSyncToggle->setBounds (74, 157, 16, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PingPongComponent::sliderValueChanged (Slider* sliderThatWasMoved)
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

void PingPongComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == enableToggle)
    {
        //[UserButtonCode_enableToggle] -- add your button handler code here..
        //[/UserButtonCode_enableToggle]
    }
    else if (buttonThatWasClicked == leftSyncToggle)
    {
        //[UserButtonCode_leftSyncToggle] -- add your button handler code here..
        updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);

        if(paramIndices.size() == 9 &&
           timeDivision.size() == 24 &&
           processor.getRegisteredMidi(paramIndices[2]))
        {
            if(leftSyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[2]);
            } else {
                processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[2]);
            }
        }

        //[/UserButtonCode_leftSyncToggle]
    }
    else if (buttonThatWasClicked == rightSyncToggle)
    {
        //[UserButtonCode_rightSyncToggle] -- add your button handler code here..
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);

        if(paramIndices.size() == 9 &&
           timeDivision.size() == 24 &&
           processor.getRegisteredMidi(paramIndices[6]))
        {
            if(rightSyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[6]);
            } else {
                processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[6]);
            }
        }
        //[/UserButtonCode_rightSyncToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void PingPongComponent::initialiseTimeDivisions(){
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


void PingPongComponent::calculateTimeDivision(double bpm){
    double crotchet = 60000/bpm;

    // Calculate 1/128 Note, multiply with 2
    timeDivision.clear();

    for(int i=0; i<division.size(); i++){
        timeDivision.push_back(crotchet*delay_timing_ratios[i]);
    }

    updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
    updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
}

int PingPongComponent::getTimeDivisionIndex(std::vector<double> values, double time){
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

int PingPongComponent::findClosestTimeDivision(double time){
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

void PingPongComponent::updateSliderRange(Button *toggle,
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

void PingPongComponent::updateSliderValue(Button *toggle,
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
       labelRef.setText(tmpName, dontSendNotification);
    } else {
        std::cerr << "StereoDelayComponent::updateSliderValue" << std::endl;
    }
}

void PingPongComponent::handleCommandMessage(int commandId){
    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        leftSyncToggle->postCommandMessage(ParamImageToggle::update);
        leftFBSlider->postCommandMessage(ParamSlider::update);
        leftMixSlider->postCommandMessage(ParamSlider::update);
        rightSyncToggle->postCommandMessage(ParamImageToggle::update);
        rightFBSlider->postCommandMessage(ParamSlider::update);
        rightMixSlider->postCommandMessage(ParamSlider::update);

        if(paramIndices.size() == 9){
            if(processor.getRegisteredMidi(paramIndices[2])){
                if(leftSyncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[2]);
                } else {
                    processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[2]);
                }
            }

            if(processor.getRegisteredMidi(paramIndices[6])){
                if(rightSyncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[6]);
                } else {
                    processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[6]);
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

    if(paramIndices.size() == 9 &&
       midiStrings.size() == 9 &&
       timeDivision.size() == 24)
    {
        int index = -1;
        int param = -1;
        int handling = -1;
        double minValue = 0.0;
        double maxValue = 1.0;

        if(commandId == requestMenuId[0]){          // Enable
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[1]){   // Left Sync
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[2]){   // Left Delay
            index = menu.show();
            param = 2;

            if(leftSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = timeDivision[23];
                handling = akateko::MIDI_TO_DOUBLE;
            }

        } else if(commandId == requestMenuId[3]){   // Left Feedback
            index = menu.show();
            param = 3;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[4]){   // Left Mix
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[5]){   // Right Sync
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[6]){   // Right Delay
            index = menu.show();
            param = 6;

            if(rightSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = timeDivision[23];
                handling = akateko::MIDI_TO_DOUBLE;
            }
        } else if(commandId == requestMenuId[7]){   // Right Feedback
            index = menu.show();
            param = 7;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[8]){   // Right Mix
            index = menu.show();
            param = 8;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling , midiStrings[param], 11);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }
}

void PingPongComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" PP-Delay: Enable");
    midiStrings.add(" PP-Delay: L-Sync");
    midiStrings.add(" PP-Delay: L-Delay");
    midiStrings.add(" PP-Delay: L-FB");
    midiStrings.add(" PP-Delay: L-Mix");
    midiStrings.add(" PP-Delay: R-Sync");
    midiStrings.add(" PP-Delay: R-Delay");
    midiStrings.add(" PP-Delay: R-FB");
    midiStrings.add(" PP-Delay: R-Mix");
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PingPongComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="13 101 62 10" fill="solid: ffaaaaaa" hasStroke="0" text="L DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="13 197 62 10" fill="solid: ffaaaaaa" hasStroke="0" text="R DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="112 101 36 10" fill="solid: ffaaaaaa" hasStroke="0" text="L FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="112 197 36 11" fill="solid: ffaaaaaa" hasStroke="0" text="R FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="189 101 44 10" fill="solid: ffaaaaaa" hasStroke="0" text="L MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="189 197 44 10" fill="solid: ffaaaaaa" hasStroke="0" text="R MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="leftDelaySlider" id="b9d7855b70230b1a" memberName="leftDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="15 40 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightDelaySlider" id="9d4bd59bbad98e0c" memberName="rightDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="15 136 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightFBSlider" id="b8d6f8c56f23720b" memberName="rightFBSlider"
          virtualName="" explicitFocusOrder="0" pos="99 136 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="leftFBSlider" id="f5c510b0381ad3a3" memberName="leftFBSlider"
          virtualName="" explicitFocusOrder="0" pos="99 40 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="leftMixSlider" id="6c37cee5e1efaec4" memberName="leftMixSlider"
          virtualName="" explicitFocusOrder="0" pos="183 40 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <IMAGEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
               virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="leftSyncToggle" id="74bc5579149da4c2" memberName="leftSyncToggle"
               virtualName="" explicitFocusOrder="0" pos="72 56 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="rightSyncToggle" id="73efd299c17c4595" memberName="rightSyncToggle"
               virtualName="" explicitFocusOrder="0" pos="72 152 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="rightMixSlider" id="e9d5097e5796b7b7" memberName="rightMixSlider"
          virtualName="" explicitFocusOrder="0" pos="183 136 60 60" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
