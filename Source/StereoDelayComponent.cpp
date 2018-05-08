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

    vector<int>paramIndices = p.getParameterIndices(AkatekoAudioProcessor::StereoDelayId);
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
        addAndMakeVisible(enableToggle = new ParamToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(crossEnabledToggle = new ParamToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), label));
        addAndMakeVisible(crossSlider = new ParamSlider(paramIds[2], *params.getUnchecked(paramIndices[2]), label, 0, 1.f));
        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label));

        addAndMakeVisible(leftSyncToggle = new ParamToggle(paramIds[4], *params.getUnchecked(paramIndices[4]), label));
        addAndMakeVisible (leftDelaySlider = new Slider (paramIds[5]));
        leftDelay = p.getParameters().getUnchecked(paramIndices[5]);
        addAndMakeVisible(leftFBSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), label, -1.f, 1.f));

        addAndMakeVisible(rightSyncToggle = new ParamToggle(paramIds[7], *params.getUnchecked(paramIndices[7]), label));
        addAndMakeVisible (rightDelaySlider = new Slider (paramIds[8]));
        rightDelay = params.getUnchecked(paramIndices[8]);
        addAndMakeVisible(rightFBSlider = new ParamSlider(paramIds[9], *params.getUnchecked(paramIndices[9]), label, -1.f, 1.f));
    } else {
        addAndMakeVisible (enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (crossEnabledToggle = new ToggleButton ("crossEnabledToggle"));
        addAndMakeVisible (crossSlider = new Slider ("crossSlider"));
        addAndMakeVisible (mixSlider = new Slider ("mixSlider"));

        addAndMakeVisible (leftSyncToggle = new ToggleButton ("leftSyncToggle"));
        addAndMakeVisible (leftDelaySlider = new Slider ("leftDelaySlider"));
        addAndMakeVisible (leftFBSlider = new Slider ("leftFBSlider"));

        addAndMakeVisible (rightSyncToggle = new ToggleButton ("rightSyncToggle"));
        addAndMakeVisible (rightDelaySlider = new Slider ("rightDelaySlider"));
        addAndMakeVisible (rightFBSlider = new Slider ("rightFBSlider"));

        std::cerr << "StereoDelayComponent::StereoDelayComponent" << std::endl;
        std::cerr << "Parameters not bound properly" << std::endl;
    }

    //[/Constructor_pre]

    //[UserPreSize]

    leftDelaySlider->setRange (0, 1, 0);
    leftDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftDelaySlider->addListener(this);

    rightDelaySlider->setRange (0, 1, 0);
    rightDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightDelaySlider->addListener(this);

    rightFBSlider->setRange (0, 1, 0);
    rightFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightFBSlider->setDoubleClickReturnValue(true, 0.5);

    leftFBSlider->setRange (0, 1, 0);
    leftFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftFBSlider->setDoubleClickReturnValue(true, 0.5);

    crossSlider->setRange (0, 1, 0);
    crossSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    crossSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    leftSyncToggle->setButtonText (String());
    leftSyncToggle->addListener (this);

    rightSyncToggle->setButtonText (String());
    rightSyncToggle->addListener (this);

    crossEnabledToggle->setButtonText (String());
    crossEnabledToggle->addListener (this);
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    initialiseTimeDivisions();
    calculateTimeDivision(120.0);

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
    enableToggle->setBounds (235, -3, 24, 24);
    leftSyncToggle->setBounds (75, 40, 24, 24);
    rightSyncToggle->setBounds (75, 168, 24, 24);
    crossEnabledToggle->setBounds (134, 108, 24, 24);
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

        //[/UserButtonCode_leftSyncToggle]
    }
    else if (buttonThatWasClicked == rightSyncToggle)
    {
        //[UserButtonCode_rightSyncToggle] -- add your button handler code here..
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
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
        double T = 60000/bpm;

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
       labelRef.setText(tmpName, dontSendNotification);
    } else {
        std::cerr << "StereoDelayComponent::updateSliderValue" << std::endl;
    }
}

void StereoDelayComponent::handleCommandMessage(int commandId){
    if(commandId == update){
        enableToggle->postCommandMessage(ParamToggle::update);
        crossEnabledToggle->postCommandMessage(ParamToggle::update);
        crossSlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);

        leftSyncToggle->postCommandMessage(ParamToggle::update);
        leftFBSlider->postCommandMessage(ParamToggle::update);

        rightSyncToggle->postCommandMessage(ParamToggle::update);
        rightFBSlider->postCommandMessage(ParamToggle::update);

       updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
       updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
    }
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
  <TOGGLEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="leftSyncToggle" id="74bc5579149da4c2" memberName="leftSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="75 40 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="rightSyncToggle" id="73efd299c17c4595" memberName="rightSyncToggle"
                virtualName="" explicitFocusOrder="0" pos="75 168 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="crossEnabledToggle" id="4cc58e3caea10610" memberName="crossEnabledToggle"
                virtualName="" explicitFocusOrder="0" pos="134 108 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
