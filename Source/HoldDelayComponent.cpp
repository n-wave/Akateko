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

#include "HoldDelayComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <iostream>

using std::vector;
/*
AudioParameterBool *hDelayEnable;
AudioParameterBool *hDelayTrigger;
AudioParameterBool *hDelaySync;
AudioParameterFloat *hDelayLoop;
AudioParameterFloat *hDelaySpeed;
AudioParameterFloat *hDelayDirection;
AudioParameterFloat *hDelayPan;
AudioParameterFloat *hDelayMix;

7

*/
//[/MiscUserDefs]

//==============================================================================
HoldDelayComponent::HoldDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label),
      trigger(nullptr),
      loop(nullptr),
      direction(nullptr),
      triggerClicked(false), activeColour(Colour(0x7f007f7f))
{
    //[Constructor_pre] You can add your own custom stuff here..
    vector<int>paramIndices = p.getParameterIndices(AkatekoAudioProcessor::hDelayId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::hDelayId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    if(paramIndices.size() ==  13 &&
       paramIds.size() == 13 &&
       params.size() >= paramIndices[12])
    {
        addAndMakeVisible(enableToggle = new ParamToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));

        triggerId = String(paramIds[1]);
        trigger = params.getUnchecked(paramIndices[1]);

        addAndMakeVisible(syncToggle = new ToggleButton(paramIds[2]));
        sync = params.getUnchecked(paramIndices[2]);

        addAndMakeVisible(loopSlider = new Slider(paramIds[3]));
        loop = params.getUnchecked(paramIndices[3]);

        addAndMakeVisible(speedSlider = new Slider(paramIds[4]));
        speed = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible(glideToggle = new ToggleButton(paramIds[5]));
        smooth = params.getUnchecked(paramIndices[5]);

        addAndMakeVisible(glideSlider = new Slider(paramIds[6]));
        glide = params.getUnchecked(paramIndices[6]);

        addAndMakeVisible(directionSlider = new Slider(paramIds[7]));
        direction = params.getUnchecked(paramIndices[7]);

        addAndMakeVisible(gapPositionToggle = new ToggleButton(paramIds[8]));
        gap = params.getUnchecked(paramIndices[8]);

        addAndMakeVisible(lengthSlider = new Slider(paramIds[9]));
        length = params.getUnchecked(paramIndices[9]);

        addAndMakeVisible(fadeSlider = new Slider(paramIds[10]));
        fade = params.getUnchecked(paramIndices[10]);

        addAndMakeVisible(panSlider = new ParamSlider(paramIds[11], *params.getUnchecked(paramIndices[11]), label));
        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[12], *params.getUnchecked(paramIndices[12]), label));

    } else {
        std::cerr << "HoldDelayComponent::HoldDelayComponent" << std::endl;
        std::cerr << "Parameters are not bound properly" << std::endl;

        addAndMakeVisible (enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (syncToggle = new ToggleButton ("syncToggle"));
        addAndMakeVisible (loopSlider = new Slider ("loopSlider"));
        addAndMakeVisible (speedSlider = new Slider ("speedSlider"));
        addAndMakeVisible (glideToggle = new ToggleButton ("glideToggle"));
        addAndMakeVisible (glideSlider = new Slider ("glideSlider"));
        addAndMakeVisible (directionSlider = new Slider ("directionSlider"));
        addAndMakeVisible (gapPositionToggle = new ToggleButton ("gapPositionToggle"));
        addAndMakeVisible (lengthSlider = new Slider ("lengthSlider"));
        addAndMakeVisible (fadeSlider = new Slider ("fadeSlider"));
        addAndMakeVisible (panSlider = new Slider ("panSlider"));
        addAndMakeVisible (mixSlider = new Slider ("mixSlider"));

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

    loopSlider->setRange (0, 1, 0);
    loopSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    loopSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    loopSlider->addListener (this);


    speedSlider->setRange (0.25, 4.0, 0);
    speedSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    speedSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    speedSlider->setSkewFactorFromMidPoint(1.0);
    speedSlider->setDoubleClickReturnValue(true, 1.0f);
    speedSlider->addListener(this);

    directionSlider->setRange (0, 1, 0.5);
    directionSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    directionSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    directionSlider->setMouseDragSensitivity(50);
    directionSlider->addListener (this);


    enableToggle->setButtonText (String());

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    syncToggle->setButtonText (String());
    syncToggle->addListener (this);


    panSlider->setRange (0, 1, 0);
    panSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    panSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    panSlider->setDoubleClickReturnValue(true, 0.5f);

    lengthSlider->setRange (0.10, 1, 0);
    lengthSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lengthSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    lengthSlider->addListener (this);

    glideSlider->setRange (1, 32, 1);
    glideSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    glideSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    glideSlider->addListener (this);

    fadeSlider->setRange (0, 1, 0);
    fadeSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    fadeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fadeSlider->addListener (this);

    glideToggle->setButtonText (String());
    glideToggle->addListener (this);

    gapPositionToggle->setButtonText (String());
    gapPositionToggle->addListener (this);

    triggerClicked = false;

    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    initialiseTimeDivision();
    calculateTimeDivision(120.0);
    setGlideSliderRange(120.0);

    if(sync != nullptr){
        const bool tmpSync = sync->getValue();
        syncToggle->setToggleState(tmpSync, dontSendNotification);
    }

    if(speed != nullptr){
        const double tmpSpeed = speed->getValue();
        speedSlider->setValue(tmpSpeed, dontSendNotification);
    }

    if(direction != nullptr){
        const double tmpDir = direction->getValue()*0.5;
        directionSlider->setValue(tmpDir, dontSendNotification);
    }

    if(fade != nullptr){
        const double tmpFade = fade->getValue();
        fadeSlider->setValue(tmpFade, dontSendNotification);
    }

    if(gap != nullptr){
        const bool tmpGap = gap->getValue();
        gapPositionToggle->setToggleState(tmpGap, dontSendNotification);
    }

    if(smooth != nullptr){
        const bool tmpSmooth = smooth->getValue();
        glideToggle->setToggleState(tmpSmooth, dontSendNotification);
    }

    if(glide != nullptr){
        const double tmpGlide = glide->getValue();

        glideSlider->setValue(tmpGlide);
    }

    if(fade != nullptr){
        const double tmpFade = fade->getValue();
        fadeSlider->setValue(tmpFade, dontSendNotification);
    }

    if(length != nullptr){
        const double tmpLength = length->getValue();
        lengthSlider->setValue(tmpLength, dontSendNotification);
    }

    //[/Constructor]
}

HoldDelayComponent::~HoldDelayComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    loopSlider = nullptr;
    speedSlider = nullptr;
    directionSlider = nullptr;
    enableToggle = nullptr;
    mixSlider = nullptr;
    syncToggle = nullptr;
    panSlider = nullptr;
    lengthSlider = nullptr;
    glideSlider = nullptr;
    fadeSlider = nullptr;
    glideToggle = nullptr;
    gapPositionToggle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    sync = nullptr;
    trigger = nullptr;
    loop = nullptr;
    speed = nullptr;
    direction = nullptr;

    gap = nullptr;
    smooth = nullptr;
    fade = nullptr;
    glide = nullptr;
    length = nullptr;
    //[/Destructor]
}

//==============================================================================
void HoldDelayComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PAN"),
                200, 181, 32, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("MIX"),
                200, 85, 32, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("Loop"),
                21, 102, 42, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("speed"),
                72, 68, 46, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DIR"),
                24, 164, 27, 12,
                Justification::centred, true);

    g.setGradientFill (ColourGradient (Colour (0xff9a9696),
                                       96.0f, 96.0f,
                                       Colour (0xf01f1f1f),
                                       96.0f, 112.0f,
                                       true));
    g.fillPath (internalPath1);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("Length"),
                68, 200, 56, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("FADE"),
                130, 164, 43, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("GLIDE"),
                128, 100, 46, 10,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..

    if(triggerClicked){
        g.setGradientFill (ColourGradient (Colour (0xff9a9696),
                                           96.0f, 96.0f,
                                           activeColour,
                                           96.0f, 112.0f,
                                           true));
    } else {
        g.setGradientFill (ColourGradient (Colour (0xff9a9696),
                                           96.0f, 96.0f,
                                           Colour (0xf01f1f1f),
                                           96.0f, 112.0f,
                                           true));
    }

    g.fillPath (internalPath1);
    g.setColour (Colour (0xcb403c3c));
    g.strokePath (internalPath1, PathStrokeType (2.000f));
    //[/UserPaint]
}

void HoldDelayComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    loopSlider->setBounds (16, 56, 48, 48);
    speedSlider->setBounds (72, 24, 48, 48);
    directionSlider->setBounds (16, 120, 48, 48);
    enableToggle->setBounds (235, -3, 24, 24);
    mixSlider->setBounds (192, 40, 48, 48);
    syncToggle->setBounds (63, 86, 24, 24);
    panSlider->setBounds (192, 136, 48, 48);
    lengthSlider->setBounds (72, 152, 48, 48);
    glideSlider->setBounds (128, 56, 48, 48);
    fadeSlider->setBounds (128, 120, 48, 48);
    glideToggle->setBounds (111, 86, 24, 24);
    gapPositionToggle->setBounds (87, 126, 24, 24);
    internalPath1.clear();
    internalPath1.startNewSubPath (96.0f, 100.0f);
    internalPath1.cubicTo (104.0f, 100.0f, 110.0f, 106.0f, 110.0f, 114.0f);
    internalPath1.cubicTo (110.0f, 121.0f, 104.0f, 127.0f, 96.0f, 127.0f);
    internalPath1.cubicTo (89.0f, 127.0f, 83.0f, 121.0f, 83.0f, 114.0f);
    internalPath1.cubicTo (83.0f, 106.0f, 89.0f, 100.0f, 96.0f, 100.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void HoldDelayComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == loopSlider)
    {
        //[UserSliderCode_loopSlider] -- add your slider handling code here..
        String tmpName = loopSlider->getName();
        const bool tmpSync = syncToggle->getToggleState();

        if(tmpSync){
            int tmpVal = loopSlider->getValue();

            if(tmpVal < timeDivision.size() &&
               tmpVal < division.size())
            {
                double tmpRate = timeDivision[tmpVal];
                tmpName += division[tmpVal];

                loop->setValue(tmpRate);
            }
        } else {
            double tmpVal = loopSlider->getValue();
            loop->setValue(tmpVal);
            tmpName += String(tmpVal, 2);
        }

        labelRef.setText(tmpName, dontSendNotification);

        //[/UserSliderCode_loopSlider]
    }
    else if (sliderThatWasMoved == directionSlider)
    {
        //[UserSliderCode_directionSlider] -- add your slider handling code here..
        int tmpDirection = directionSlider->getValue()*2.0;
        String tmpName = directionSlider->getName();

        switch(tmpDirection){
            case 0:
                tmpName += String("forward");
                break;
            case 1:
                tmpName += String("reverse");
                break;
            case 2:
                tmpName += String("back&forth");
                break;
        }

        if(direction != nullptr){
            direction->setValue(tmpDirection);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_directionSlider]
    }
    else if (sliderThatWasMoved == lengthSlider)
    {
        //[UserSliderCode_lengthSlider] -- add your slider handling code here..
        const double tmpValue = lengthSlider->getValue();
        const String tmpName = lengthSlider->getName() +
                               String(tmpValue*100.0, 2) +
                               String("%");

        if(length != nullptr){
            length->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_lengthSlider]
    }
    else if (sliderThatWasMoved == glideSlider)
    {
        //[UserSliderCode_glideSlider] -- add your slider handling code here..
        const int tmpValue = glideSlider->getValue();
        const bool tmpState = glideToggle->getToggleState();

        String tmpName = glideSlider->getName() + String(tmpValue);

        if(tmpState){
            tmpName += String(" ms");
        } else{
            tmpName += String(" reps");
        }

        if(glide != nullptr){
            glide->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_glideSlider]
    }
    else if (sliderThatWasMoved == fadeSlider)
    {
        //[UserSliderCode_fadeSlider] -- add your slider handling code here..
        const double tmpValue = fadeSlider->getValue();
        const String tmpName = fadeSlider->getName() +
                               String(tmpValue*50.0, 2) +
                               String("%");

        if(fade != nullptr){
            fade->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);

        //[/UserSliderCode_fadeSlider]
    }

    //[UsersliderValueChanged_Post]
    else if (sliderThatWasMoved == speedSlider){
        double tmpValue = speedSlider->getValue();
        String tmpName = speedSlider->getName() + String(tmpValue, 2);

        if(speed != nullptr){
            speed->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
    }
    //[/UsersliderValueChanged_Post]
}

void HoldDelayComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == syncToggle)
    {
        //[UserButtonCode_syncToggle] -- add your button handler code here..
        if(sync != nullptr && loop != nullptr){
            const int tmpState = syncToggle->getToggleState();
            double tmpLoop = loop->getValue();

            String tmpName = loopSlider->getName();
            sync->setValue(tmpState);

            if(tmpState){
                int tmpIndex = findClosestTimeDivision(tmpLoop);
                loopSlider->setRange(0, timeDivision.size()-1);
                loopSlider->setValue(tmpIndex, sendNotificationAsync);

                // Convert Period to Frequency
                loop->setValue(timeDivision[tmpIndex]);
                tmpName += division[tmpIndex];
            } else {
                const double minPeriod = timeDivision[0];
                const double maxPeriod = timeDivision[timeDivision.size()-1];

                if(tmpLoop > maxPeriod){
                    tmpLoop = maxPeriod;
                } else if(tmpLoop < minPeriod){
                    tmpLoop = minPeriod;
                }

                loopSlider->setRange(minPeriod, maxPeriod, 0);
                loopSlider->setValue(tmpLoop, sendNotificationAsync);
                loop->setValue(tmpLoop);

                tmpName += String(tmpLoop, 2);
            }

            labelRef.setText(tmpName, dontSendNotification);
        } else {
            std::cerr << "Parameters are not bound properly" << std::endl;
        }
        //[/UserButtonCode_syncToggle]
    }
    else if (buttonThatWasClicked == glideToggle)
    {
        //[UserButtonCode_glideToggle] -- add your button handler code here..
        const bool tmpGlide = glideToggle->getToggleState();
        String tmpName = glideToggle->getName();

        if(tmpGlide){
            tmpName += String("Smooth");
        } else {
            tmpName += String("Incremental");
        }

        if(smooth != nullptr){
            smooth->setValue(tmpGlide);
        }

        setGlideSliderRange(beatsPerMinute);

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserButtonCode_glideToggle]
    }
    else if (buttonThatWasClicked == gapPositionToggle)
    {
        //[UserButtonCode_gapPositionToggle] -- add your button handler code here..
        const bool tmpGap = gapPositionToggle->getToggleState();
        String tmpName = gapPositionToggle->getName();

        if(tmpGap){
            tmpName += String("Front");
        } else {
            tmpName += String("Back");
        }

        if(gap != nullptr){
            gap->setValue(tmpGap);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserButtonCode_gapPositionToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void HoldDelayComponent::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    float xPos = e.position.x;
    float yPos = e.position.y;

    if(internalPath1.contains(xPos, yPos)){
        if(!triggerClicked && trigger != nullptr){
            triggerClicked = true;

            trigger->setValue(triggerClicked);
            String tmpName = triggerId + String("On");
            labelRef.setText(tmpName, dontSendNotification);

            repaint();
        }
        //std::cout << "trigger me" << std::endl;
    }
    //[/UserCode_mouseDown]
}

void HoldDelayComponent::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    if(triggerClicked && trigger != nullptr){
        triggerClicked = false;

        trigger->setValue(triggerClicked);
        String tmpName = triggerId + String("Off");
        labelRef.setText(tmpName, dontSendNotification);

        repaint();
    }
    //[/UserCode_mouseUp]
}

bool HoldDelayComponent::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    if(key == KeyPress::spaceKey){

        if(trigger != nullptr){
            triggerClicked = !triggerClicked;

            trigger->setValue(triggerClicked);
            String tmpName = triggerId + String("Off");
            labelRef.setText(tmpName, dontSendNotification);

            repaint();
        }
    }


    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void HoldDelayComponent::setBPM(double bpm){
    beatsPerMinute = bpm;

    calculateTimeDivision(beatsPerMinute);
    setGlideSliderRange(beatsPerMinute);
}


void HoldDelayComponent::initialiseTimeDivision(){
    StringArray noteVals;

    noteVals.add("1/128N");
    noteVals.add("1/64N");
    noteVals.add("1/32N");
    noteVals.add("1/16N");
    noteVals.add("1/8N");
    noteVals.add("1/4N");

    division.clear();

    for(int i=0; i<noteVals.size(); i++){
        String dot = noteVals[i] + String("D");
        String triplet = noteVals[i] + String("T");

        division.add(triplet);
        division.add(noteVals[i]);
        division.add(dot);
    }
}

void HoldDelayComponent::calculateTimeDivision(double bpm){
    if(bpm > 0.0){
        beatsPerMinute = bpm;
        double T  = 60000.0/bpm;

        //Calculate 2dot note
        double tripNote = 0.03125*T*(2.0/3.0);
        double wholeNote = 0.03125*T;
        double dotNote = 0.03125*T*1.5;

        timeDivision.clear();

        timeDivision.push_back(tripNote);
        timeDivision.push_back(wholeNote);
        timeDivision.push_back(dotNote);

        for(int i=3; i<division.size(); i+=3){
            dotNote *= 2.0;
            wholeNote *= 2.0;
            tripNote *= 2.0;

            timeDivision.push_back(tripNote);
            timeDivision.push_back(wholeNote);
            timeDivision.push_back(dotNote);
        }
    }

    /*
    for(int i=0; i<division.size(); i++){
        std::cout << division[i] << timeDivision[i] << std::endl;
    }
    */

    initLoopSlider();
}

int HoldDelayComponent::findClosestTimeDivision(double time){
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

void HoldDelayComponent::initLoopSlider(){
    if(syncToggle != nullptr &&
       loop != nullptr &&
       timeDivision.size() != 0 &&
       division.size() != 0)
    {
        const bool tmpSync = sync->getValue();
        double tmpRate = loop->getValue();

        if(tmpSync){
            int tmpIndex = findClosestTimeDivision(tmpRate);

            loopSlider->setRange(0, timeDivision.size()-1, 1);
            loopSlider->setValue(tmpIndex, dontSendNotification);
        } else {
            const double minPeriod = timeDivision[0];
            const double maxPeriod = timeDivision[timeDivision.size()-1];

            loopSlider->setRange(minPeriod, maxPeriod, 0);
            loopSlider->setValue(tmpRate, sendNotificationAsync);
        }
    } else {
        if(sync == nullptr || loop == nullptr){
            std::cerr << "Parameters bound not properly : " << getName() << std::endl;
        }

        if(division.size() == 0 || timeDivision.size() == 0){
            std::cerr << "Time division tables not set properly" << std::endl;
        }
    }
}

void HoldDelayComponent::setGlideSliderRange(double bpm){
    if(smooth != nullptr &&
       glide != nullptr)
    {
        const bool tmpSmooth = smooth->getValue();
        double tmpGlide = glide->getValue();

        if(tmpSmooth){
            const double maxRange = 60000.0/bpm*4.0;
            glideSlider->setRange(10.0, maxRange);

            if(tmpGlide < 10.0){
                tmpGlide = 10.0;
            } else if(tmpGlide > maxRange){
                tmpGlide = maxRange;
            }

            glide->setValue(tmpGlide);
        } else {
            glideSlider->setRange(1, 32, 1);

            if(tmpGlide < 1){
                tmpGlide = 1;
            } else if(tmpGlide > 32){
                tmpGlide = 32;
            }

            glide->setValue(tmpGlide);
        }
    }
}

/*
bool HoldDelayComponent::keyPressed(const KeyPress &key){
    if(key == KeyPress::spaceKey){
        if(triggerClicked && trigger != nullptr){
            triggerClicked = false;

            trigger->setValue(triggerClicked);
            String tmpName = triggerId + String("Off");
            labelRef.setText(tmpName, dontSendNotification);

            repaint();
        }
    }

    return false;
}
*/

void HoldDelayComponent::mouseEnter(const MouseEvent &event){
    setWantsKeyboardFocus(true);
}

void HoldDelayComponent::mouseExit(const MouseEvent &event){
    setWantsKeyboardFocus(false);
}

void HoldDelayComponent::handleCommandMessage(int commandId){
    if(commandId == update){
        enableToggle->postCommandMessage(ParamToggle::update);
        panSlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);

        initLoopSlider();

        if(sync != nullptr){
            const bool tmpSync = sync->getValue();
            syncToggle->setToggleState(tmpSync, dontSendNotification);
        }

        if(speed != nullptr){
            const double tmpSpeed = speed->getValue();
            speedSlider->setValue(tmpSpeed, dontSendNotification);
        }

        if(direction != nullptr){
            const double tmpDir = direction->getValue()*0.5;
            directionSlider->setValue(tmpDir, dontSendNotification);
        }

        if(fade != nullptr){
            const double tmpFade = fade->getValue();
            fadeSlider->setValue(tmpFade, dontSendNotification);
        }

        if(gap != nullptr){
            const bool tmpGap = gap->getValue();
            gapPositionToggle->setToggleState(tmpGap, dontSendNotification);
        }

        if(smooth != nullptr){
            const bool tmpSmooth = smooth->getValue();
            glideToggle->setToggleState(tmpSmooth, dontSendNotification);
        }

        if(glide != nullptr){
            const double tmpGlide = glide->getValue();

            glideSlider->setValue(tmpGlide);
        }

        if(fade != nullptr){
            const double tmpFade = fade->getValue();
            fadeSlider->setValue(tmpFade, dontSendNotification);
        }

        if(length != nullptr){
            const double tmpLength = length->getValue();
            lengthSlider->setValue(tmpLength, dontSendNotification);
        }
    }
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HoldDelayComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)&#10;trigger(nullptr)&#10;loop(nullptr)&#10;direction(nullptr)&#10;triggerClicked(false), activeColour(Colour(0x7f007f7f))"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <METHODS>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="200 181 32 12" fill="solid: ffaaaaaa" hasStroke="0" text="PAN"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="200 85 32 12" fill="solid: ffaaaaaa" hasStroke="0" text="MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="21 102 42 10" fill="solid: ffaaaaaa" hasStroke="0" text="Loop"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="72 68 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="speed"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="24 164 27 12" fill="solid: ffaaaaaa" hasStroke="0" text="DIR"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <PATH pos="0 0 100 100" fill=" radial: 96 96, 96 112, 0=ff9a9696, 1=f01f1f1f"
          hasStroke="0" nonZeroWinding="1">s 96.5 100 c 103.93 100 110 106.07  110 113.5 c 110 120.93 103.93 127  96.5 127 c 89.07 127 83 120.93  83 113.5 c 83 106.07 89.07 100  96.5 100 x</PATH>
    <TEXT pos="68 200 56 12" fill="solid: ffaaaaaa" hasStroke="0" text="Length"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="130 164 43 12" fill="solid: ffaaaaaa" hasStroke="0" text="FADE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="128 100 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="GLIDE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="loopSlider" id="f4769a4741fb690a" memberName="loopSlider"
          virtualName="" explicitFocusOrder="0" pos="16 56 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="speedSlider" id="d630945e9da5e83d" memberName="speedSlider"
          virtualName="" explicitFocusOrder="0" pos="72 24 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="directionSlider" id="2f7b6f88ae19bb1e" memberName="directionSlider"
          virtualName="" explicitFocusOrder="0" pos="16 120 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="mixSlider" id="d2598f216032723b" memberName="mixSlider"
          virtualName="" explicitFocusOrder="0" pos="192 40 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="syncToggle" id="599c91ee9ff402cf" memberName="syncToggle"
                virtualName="" explicitFocusOrder="0" pos="63 86 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="panSlider" id="c7e7aee6d5c7fa6f" memberName="panSlider"
          virtualName="" explicitFocusOrder="0" pos="192 136 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="lengthSlider" id="71e0ffafea1e4031" memberName="lengthSlider"
          virtualName="" explicitFocusOrder="0" pos="72 152 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="glideSlider" id="bfcc73334e2ab0b1" memberName="glideSlider"
          virtualName="" explicitFocusOrder="0" pos="128 56 48 48" min="4"
          max="64" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="fadeSlider" id="cc9942dd41f988fe" memberName="fadeSlider"
          virtualName="" explicitFocusOrder="0" pos="128 120 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="glideToggle" id="b51864a83bd92dc9" memberName="glideToggle"
                virtualName="" explicitFocusOrder="0" pos="111 86 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="gapPositionToggle" id="8422f566c6bc1cb" memberName="gapPositionToggle"
                virtualName="" explicitFocusOrder="0" pos="87 126 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
