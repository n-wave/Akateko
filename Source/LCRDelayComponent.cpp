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

#include "LCRDelayComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
LCRDelayComponent::LCRDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..
    vector<int>paramIndices = p.getParameterIndices(AkatekoAudioProcessor::LCRDelayId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::LCRDelayId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    bool proceed = false;

    if(paramIndices.size() == 18 &&
       paramIds.size() == 18 &&
       params.size() >= paramIndices[17])
    {
        addAndMakeVisible(enableToggle = new ParamToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(leftSyncToggle = new ParamToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), label));

        addAndMakeVisible(leftDelaySlider = new Slider(paramIds[2]));
        leftDelay = p.getParameters().getUnchecked(paramIndices[2]);

        addAndMakeVisible(leftFBSlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label, -1.f, 1.f));
        addAndMakeVisible(leftMixSlider = new ParamSlider(paramIds[4], *params.getUnchecked(paramIndices[4]), label));
        addAndMakeVisible(centerSyncToggle = new ParamToggle(paramIds[5], *params.getUnchecked(paramIndices[5]), label));

        addAndMakeVisible(centerDelaySlider = new Slider(paramIds[6]));
        centerDelay = p.getParameters().getUnchecked(paramIndices[6]);

        addAndMakeVisible(centerFBSlider = new ParamSlider(paramIds[7], *params.getUnchecked(paramIndices[7]), label, -1.f, 1.f));
        addAndMakeVisible(hpfEnableToggle = new ParamToggle(paramIds[8], *params.getUnchecked(paramIndices[8]), label));
        addAndMakeVisible(hpfSlider = new ParamSlider(paramIds[9], *params.getUnchecked(paramIndices[9]), label, 20.f, 20000.f));
        addAndMakeVisible(lpfEnableToggle = new ParamToggle(paramIds[10], *params.getUnchecked(paramIndices[10]), label));
        addAndMakeVisible(lpfSlider = new ParamSlider(paramIds[11], *params.getUnchecked(paramIndices[11]), label, 20.f, 20000.f));
        addAndMakeVisible(centerAmpSlider = new ParamSlider(paramIds[12], *params.getUnchecked(paramIndices[12]), label));

        //TODO
        //get Value set Value on slider
        addAndMakeVisible (panSlider = new Slider (paramIds[13]));
        pan = params.getUnchecked(paramIndices[13]);

        addAndMakeVisible(rightSyncToggle = new ParamToggle(paramIds[14], *p.getParameters().getUnchecked(paramIndices[14]), label));

        addAndMakeVisible(rightDelaySlider = new Slider(paramIds[15]));
        rightDelay = params.getUnchecked(paramIndices[15]);

        addAndMakeVisible(rightFBSlider = new ParamSlider(paramIds[16], *p.getParameters().getUnchecked(paramIndices[16]), label, -1.f, 1.f));
        addAndMakeVisible(rightMixSlider = new ParamSlider(paramIds[17], *p.getParameters().getUnchecked(paramIndices[17]), label));

        proceed = true;

    } else {
        addAndMakeVisible (enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (leftSyncToggle = new ToggleButton ("leftSyncToggle"));
        addAndMakeVisible (leftDelaySlider = new Slider ("leftDelaySlider"));
        addAndMakeVisible (leftFBSlider = new Slider ("leftFBSlider"));
        addAndMakeVisible (leftMixSlider = new Slider ("leftMixSlider"));

        addAndMakeVisible (centerSyncToggle = new ToggleButton ("centerSyncToggle"));
        addAndMakeVisible (centerDelaySlider = new Slider ("centerDelaySlider"));
        addAndMakeVisible (centerFBSlider = new Slider ("centerFBSlider"));

        addAndMakeVisible (hpfEnableToggle = new ToggleButton ("hpfEnableToggle"));
        addAndMakeVisible (hpfSlider = new Slider ("hpfSlider"));
        addAndMakeVisible (lpfEnableToggle = new ToggleButton ("lpfEnableToggle"));
        addAndMakeVisible (lpfSlider = new Slider ("lpfSlider"));
        addAndMakeVisible (centerAmpSlider = new Slider ("centerAmpSlider"));
        addAndMakeVisible (panSlider = new Slider ("panSlider"));

        addAndMakeVisible (rightSyncToggle = new ToggleButton ("rightSyncToggle"));
        addAndMakeVisible (rightDelaySlider = new Slider ("rightDelaySlider"));
        addAndMakeVisible (rightFBSlider = new Slider ("rightFBSlider"));
        addAndMakeVisible (rightMixSlider = new Slider ("rightMixSlider"));

        std::cerr << "LCRDelayComponent::LCRDelayComponent" << std::endl;
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
    leftDelaySlider->setRange (0, 1, 0);
    leftDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftDelaySlider->setDoubleClickReturnValue(true, 0.5);
    leftDelaySlider->addListener (this);

    rightDelaySlider->setRange (0, 1, 0);
    rightDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightDelaySlider->setDoubleClickReturnValue(true, 0.5);
    rightDelaySlider->addListener (this);

    rightFBSlider->setRange (0, 1, 0);
    rightFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightFBSlider->setDoubleClickReturnValue(true, 0.5f);

    leftFBSlider->setRange (0, 1, 0);
    leftFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftFBSlider->setDoubleClickReturnValue(true, 0.5f);

    leftMixSlider->setRange (0, 1, 0);
    leftMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftMixSlider->setDoubleClickReturnValue(true, 0.5f);

    enableToggle->setButtonText (String());

    leftSyncToggle->setButtonText (String());
    leftSyncToggle->addListener (this);

    rightSyncToggle->setButtonText (String());
    rightSyncToggle->addListener (this);

    centerDelaySlider->setRange (0, 1, 0);
    centerDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    centerDelaySlider->addListener (this);

    centerSyncToggle->setButtonText (String());
    centerSyncToggle->addListener (this);

    centerFBSlider->setRange (0, 1, 0);
    centerFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    centerFBSlider->setDoubleClickReturnValue(true, 0.5);


    rightMixSlider->setRange (0, 1, 0);
    rightMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightMixSlider->setDoubleClickReturnValue(true, 0.5f);

    panSlider->setRange (0, 1, 0);
    panSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    panSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    panSlider->setDoubleClickReturnValue(true, 0.5f);
    panSlider->addListener (this);

    hpfSlider->setRange (0, 1, 0);
    hpfSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    hpfSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    lpfSlider->setRange (0, 1, 0);
    lpfSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lpfSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    hpfEnableToggle->setButtonText (String());

    lpfEnableToggle->setButtonText (String());

    centerAmpSlider->setRange (0, 1, 0);
    centerAmpSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerAmpSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    centerAmpSlider->setDoubleClickReturnValue(true, 0.f);
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    initialiseTimeDivision();
    calculateTimeDivision(120.0);


    if(pan != nullptr){
        const double  value = pan->getValue();
        panSlider->setValue(value, dontSendNotification);
    }
    //[/Constructor]
}

LCRDelayComponent::~LCRDelayComponent()
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
    centerDelaySlider = nullptr;
    centerSyncToggle = nullptr;
    centerFBSlider = nullptr;
    rightMixSlider = nullptr;
    panSlider = nullptr;
    hpfSlider = nullptr;
    lpfSlider = nullptr;
    hpfEnableToggle = nullptr;
    lpfEnableToggle = nullptr;
    centerAmpSlider = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    leftDelay = nullptr;
    rightDelay = nullptr;
    centerDelay = nullptr;
    pan = nullptr;
    //[/Destructor]
}

//==============================================================================
void LCRDelayComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L DELAY"),
                2, 68, 62, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R DELAY"),
                4, 212, 58, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L FB"),
                94, 68, 38, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R FB"),
                94, 212, 38, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L MIX"),
                184, 68, 40, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R MIX"),
                180, 212, 46, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PAN"),
                214, 134, 31, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("c delay"),
                2, 138, 62, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("HPF"),
                74, 155, 30, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("LPF"),
                122, 155, 30, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("C FB"),
                95, 110, 35, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("C AMP"),
                158, 134, 47, 10,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LCRDelayComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    leftDelaySlider->setBounds (9, 24, 48, 48);
    rightDelaySlider->setBounds (9, 168, 48, 48);
    rightFBSlider->setBounds (89, 168, 48, 48);
    leftFBSlider->setBounds (89, 24, 48, 48);
    leftMixSlider->setBounds (179, 25, 48, 48);
    enableToggle->setBounds (235, -3, 24, 24);
    leftSyncToggle->setBounds (54, 36, 24, 24);
    rightSyncToggle->setBounds (55, 180, 24, 24);
    centerDelaySlider->setBounds (9, 92, 48, 48);
    centerSyncToggle->setBounds (54, 104, 24, 24);
    centerFBSlider->setBounds (97, 80, 32, 32);
    rightMixSlider->setBounds (179, 169, 48, 48);
    panSlider->setBounds (208, 97, 40, 40);
    hpfSlider->setBounds (73, 124, 32, 32);
    lpfSlider->setBounds (121, 124, 32, 32);
    hpfEnableToggle->setBounds (79, 104, 24, 24);
    lpfEnableToggle->setBounds (127, 104, 24, 24);
    centerAmpSlider->setBounds (160, 97, 40, 40);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LCRDelayComponent::sliderValueChanged (Slider* sliderThatWasMoved)
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
    else if (sliderThatWasMoved == centerDelaySlider)
    {
        //[UserSliderCode_centerDelaySlider] -- add your slider handling code here..
        updateSliderValue(centerSyncToggle, centerDelaySlider, centerDelay);
        //[/UserSliderCode_centerDelaySlider]
    }
    else if (sliderThatWasMoved == panSlider)
    {
        //[UserSliderCode_panSlider] -- add your slider handling code here..
        String result = panSlider->getName();
        const double value = panSlider->getValue();
        double left = 1-value;
        double right = value;

        if(pan != nullptr){
            pan->setValue(value);
        }

        result += "[" + String(left, 2) + "," +
                  String(right, 2) + "]";

        labelRef.setText(result, dontSendNotification);

        //[/UserSliderCode_panSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void LCRDelayComponent::buttonClicked (Button* buttonThatWasClicked)
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
        //[/UserButtonCode_leftSyncToggle]
    }
    else if (buttonThatWasClicked == rightSyncToggle)
    {
        //[UserButtonCode_rightSyncToggle] -- add your button handler code here..
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
        //[/UserButtonCode_rightSyncToggle]
    }
    else if (buttonThatWasClicked == centerSyncToggle)
    {
        //[UserButtonCode_centerSyncToggle] -- add your button handler code here..
        updateSliderRange(centerSyncToggle, centerDelaySlider, centerDelay);
        //[/UserButtonCode_centerSyncToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void LCRDelayComponent::initialiseTimeDivision(){
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


void LCRDelayComponent::calculateTimeDivision(double bpm){
    if(bpm > 0.0){
        double T = 60000.0/bpm;

        // Calculate 1/128 Note, multiply with 2
        double tripNote = 0.03125*T*(2.0/3.0);
        double wholeNote = 0.03125*T;
        double dotNote = 0.03125*T*1.5;

        timeDivision.clear();

        timeDivision.push_back(tripNote);
        timeDivision.push_back(wholeNote);
        timeDivision.push_back(dotNote);

        for(int i=3; i<division.size(); i+=3){
            tripNote *= 2.0;
            wholeNote *= 2.0;
            dotNote *= 2.0;

            timeDivision.push_back(tripNote);
            timeDivision.push_back(wholeNote);
            timeDivision.push_back(dotNote);
        }
    }
    updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
    updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
    updateSliderRange(centerSyncToggle, centerDelaySlider, centerDelay);
}

int LCRDelayComponent::findClosestTimeDivision(double time){
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

void LCRDelayComponent::updateSliderRange(Button *toggle,
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

void LCRDelayComponent::updateSliderValue(Button *toggle,
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

void LCRDelayComponent::handleCommandMessage(int commandId){
    if(commandId == update){
        enableToggle->postCommandMessage(ParamToggle::update);
        leftSyncToggle->postCommandMessage(ParamToggle::update);
        leftFBSlider->postCommandMessage(ParamSlider::update);
        leftMixSlider->postCommandMessage(ParamSlider::update);
        centerSyncToggle->postCommandMessage(ParamToggle::update);
        centerFBSlider->postCommandMessage(ParamSlider::update);
        hpfEnableToggle->postCommandMessage(ParamToggle::update);
        hpfSlider->postCommandMessage(ParamSlider::update);
        lpfEnableToggle->postCommandMessage(ParamToggle::update);
        lpfSlider->postCommandMessage(ParamSlider::update);
        centerAmpSlider->postCommandMessage(ParamSlider::update);
        rightSyncToggle->postCommandMessage(ParamToggle::update);
        rightFBSlider->postCommandMessage(ParamSlider::update);
        rightMixSlider->postCommandMessage(ParamSlider::update);

        if(pan != nullptr){
            const double  value = pan->getValue();
            panSlider->setValue(value, dontSendNotification);
        }

        updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
        updateSliderRange(centerSyncToggle, centerDelaySlider, centerDelay);
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LCRDelayComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="2 68 62 10" fill="solid: ffaaaaaa" hasStroke="0" text="L DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="4 212 58 10" fill="solid: ffaaaaaa" hasStroke="0" text="R DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="94 68 38 10" fill="solid: ffaaaaaa" hasStroke="0" text="L FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="94 212 38 10" fill="solid: ffaaaaaa" hasStroke="0" text="R FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="184 68 40 10" fill="solid: ffaaaaaa" hasStroke="0" text="L MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="180 212 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="R MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="214 134 31 10" fill="solid: ffaaaaaa" hasStroke="0" text="PAN"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="2 138 62 10" fill="solid: ffaaaaaa" hasStroke="0" text="c delay"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="74 155 30 10" fill="solid: ffaaaaaa" hasStroke="0" text="HPF"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="122 155 30 10" fill="solid: ffaaaaaa" hasStroke="0" text="LPF"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="95 110 35 10" fill="solid: ffaaaaaa" hasStroke="0" text="C FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="158 134 47 10" fill="solid: ffaaaaaa" hasStroke="0" text="C AMP"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="leftDelaySlider" id="b9d7855b70230b1a" memberName="leftDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="9 24 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightDelaySlider" id="9d4bd59bbad98e0c" memberName="rightDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="9 168 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightFBSlider" id="b8d6f8c56f23720b" memberName="rightFBSlider"
          virtualName="" explicitFocusOrder="0" pos="89 168 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="leftFBSlider" id="f5c510b0381ad3a3" memberName="leftFBSlider"
          virtualName="" explicitFocusOrder="0" pos="89 24 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="leftMixSlider" id="cf347ac5b7359227" memberName="leftMixSlider"
          virtualName="" explicitFocusOrder="0" pos="179 25 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="leftSyncToggle" id="74bc5579149da4c2" memberName="leftSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="54 36 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="rightSyncToggle" id="73efd299c17c4595" memberName="rightSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="55 180 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="centerDelaySlider" id="ea81821a599bf966" memberName="centerDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="9 92 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="centerSyncToggle" id="bde6573e5cb3d0d6" memberName="centerSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="54 104 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="centerFBSlider" id="b6c5b8606828dd51" memberName="centerFBSlider"
          virtualName="" explicitFocusOrder="0" pos="97 80 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="rightMixSlider" id="bb4e6f99eb62c6c0" memberName="rightMixSlider"
          virtualName="" explicitFocusOrder="0" pos="179 169 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="panSlider" id="6d4008ca720020ae" memberName="panSlider"
          virtualName="" explicitFocusOrder="0" pos="208 97 40 40" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="hpfSlider" id="6b2d628874595a2b" memberName="hpfSlider"
          virtualName="" explicitFocusOrder="0" pos="73 124 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="lpfSlider" id="afa2da54fe6847f4" memberName="lpfSlider"
          virtualName="" explicitFocusOrder="0" pos="121 124 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="hpfEnableToggle" id="8900b8258c961b32" memberName="hpfEnableToggle"
                virtualName="" explicitFocusOrder="0" pos="79 104 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="lpfEnableToggle" id="3b3db52817521d52" memberName="lpfEnableToggle"
                virtualName="" explicitFocusOrder="0" pos="127 104 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="centerAmpSlider" id="4fd9e3f8d71dc44e" memberName="centerAmpSlider"
          virtualName="" explicitFocusOrder="0" pos="160 97 40 40" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
