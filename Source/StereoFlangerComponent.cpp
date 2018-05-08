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

#include "StereoFlangerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using std::vector;
//[/MiscUserDefs]

//==============================================================================
StereoFlangerComponent::StereoFlangerComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      wave(nullptr),
      sync(nullptr),
      rate(nullptr),
      phase(nullptr),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    vector<int>paramIndices = p.getParameterIndices(AkatekoAudioProcessor::StereoflangerId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::StereoflangerId);
    const OwnedArray<AudioProcessorParameter> &params  = p.getParameters();

    bool proceed = false;

    if(paramIndices.size() == 8 && paramIds.size() == 8){
        int lastIndex = paramIndices[7];

        if(params.size() >= lastIndex){
            proceed = true;
        }
    }

    if(proceed)
    {
        addAndMakeVisible(enableToggle = new ParamToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), labelRef));
        addAndMakeVisible(crossFB = new ParamToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), labelRef));

        addAndMakeVisible(SyncToggle = new ToggleButton(paramIds[2]));
        sync = params.getUnchecked(paramIndices[2]);

        addAndMakeVisible(rateSlider = new Slider(paramIds[3]));
        rate = params.getUnchecked(paramIndices[3]);

        addAndMakeVisible(phaseSlider = new Slider(paramIds[4]));
        phase = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible(depthSlider = new ParamSlider(paramIds[5], *params.getUnchecked(paramIndices[5]), labelRef));
        addAndMakeVisible(feedBackSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), labelRef, -1.0, 1.0));

        addAndMakeVisible(waveSlider = new Slider(paramIds[7]));
        wave = params.getUnchecked(paramIndices[7]);

    } else {
        std::cerr << "StereoFlangerComponent::StereoFlangerComponents" << std::endl;
        std::cerr << "Parameters not bound properly" << std::endl;


        addAndMakeVisible (enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (crossFB = new ToggleButton ("crossFB"));
        addAndMakeVisible (SyncToggle = new ToggleButton ("SyncToggle"));

        addAndMakeVisible (rateSlider = new Slider ("rateSlider"));
        addAndMakeVisible (phaseSlider = new Slider ("phaseSlider"));
        addAndMakeVisible (depthSlider = new Slider ("depthSlider"));
        addAndMakeVisible (feedBackSlider = new Slider ("feedBackSlider"));
        addAndMakeVisible (waveSlider = new Slider ("waveSlider"));

        int index = 0;

        while(index < paramIndices.size() &&
              index < paramIds.size())
        {
            std::cerr << paramIds[index] << "["
                      << paramIds[index]  << "] : "
                      << std::endl;

            index++;
        }
    }
    //[/Constructor_pre]

    //[UserPreSize]
    feedBackSlider->setRange (0, 1, 0);
    feedBackSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    feedBackSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    feedBackSlider->setDoubleClickReturnValue(true, 0.5);

    SyncToggle->setButtonText (String());
    SyncToggle->addListener (this);

    phaseSlider->setRange (0, 1, 0);
    phaseSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    phaseSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    phaseSlider->setDoubleClickReturnValue(true, 0.5f);
    phaseSlider->addListener (this);

    depthSlider->setRange (0, 1, 0);
    depthSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    depthSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    rateSlider->setRange (0, 1, 0);
    rateSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rateSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rateSlider->addListener (this);

    crossFB->setButtonText (String());

    enableToggle->setButtonText (String());

    waveSlider->setRange (0, 1, 0.25);
    waveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    waveSlider->addListener (this);
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    beatDivisor.calculateBeatDivisions(120.0);

    setBeatDivisionStrings(beatDivisor.getStringArray());
    setBeatDivisionvalues(beatDivisor.getValues());

    initRateSlider();

    if(sync != nullptr){
        const bool tmpSync = sync->getValue();\
        SyncToggle->setToggleState(tmpSync, dontSendNotification);
    }

    if(wave != nullptr){
        const double tmpWave = wave->getValue()*0.25;
        waveSlider->setValue(tmpWave, dontSendNotification);
    }

    if(phase != nullptr){
        phaseSlider->setValue(phase->getValue());
    }

    //[/Constructor]
}

StereoFlangerComponent::~StereoFlangerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    feedBackSlider = nullptr;
    SyncToggle = nullptr;
    phaseSlider = nullptr;
    depthSlider = nullptr;
    rateSlider = nullptr;
    crossFB = nullptr;
    enableToggle = nullptr;
    waveSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    wave = nullptr;
    sync = nullptr;
    rate = nullptr;
    phase = nullptr;
    //[/Destructor]
}

//==============================================================================
void StereoFlangerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("FB"),
                189, 86, 27, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("RATE"),
                37, 86, 35, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DEPTH"),
                175, 206, 51, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PHASE"),
                29, 206, 51, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("WAVE"),
                101, 150, 51, 12,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StereoFlangerComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    feedBackSlider->setBounds (169, 24, 64, 64);
    SyncToggle->setBounds (80, 64, 24, 24);
    phaseSlider->setBounds (24, 144, 64, 64);
    depthSlider->setBounds (169, 144, 64, 64);
    rateSlider->setBounds (24, 24, 64, 64);
    crossFB->setBounds (157, 64, 24, 24);
    enableToggle->setBounds (235, -3, 24, 24);
    waveSlider->setBounds (96, 88, 64, 64);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void StereoFlangerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == SyncToggle)
    {
        //[UserButtonCode_SyncToggle] -- add your button handler code here..
        if(sync != nullptr && rate != nullptr){
            const int tmpState = SyncToggle->getToggleState();
            double tmpRate = rate->getValue();
            sync->setValue(tmpState);

            if(tmpState){
                int tmpIndex = findClosestTimeDivision(tmpRate);
                rateSlider->setRange(0, valueBeatDivision.size()-1, 1);
                rateSlider->setValue(tmpIndex);

                // Convert Period to Frequency
                tmpRate = 1.0/(valueBeatDivision[tmpIndex]*0.001);

                rate->setValue(tmpRate);
            } else {
                const double maxFreq = 1.0/(valueBeatDivision[valueBeatDivision.size()-1]*0.001);
                const double minFreq = 1.0/(valueBeatDivision[0]*0.001);

                rateSlider->setRange(minFreq, maxFreq, 0);
                rateSlider->setValue(tmpRate);
            }
        } else {
            std::cerr << "Parameters are not bound properly" << std::endl;
        }
        //[/UserButtonCode_SyncToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void StereoFlangerComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == phaseSlider)
    {
        //[UserSliderCode_phaseSlider] -- add your slider handling code here..
        const double tmpValue = phaseSlider->getValue();
        const int phaseDegree = 180.0*phaseSlider->getValue()-90;
        const String tmpName = phaseSlider->getName() + String(phaseDegree) + "°";

        labelRef.setText(tmpName, dontSendNotification);

        if(phase != nullptr){
            phase->setValue(tmpValue);
        }
        //[/UserSliderCode_phaseSlider]
    }
    else if (sliderThatWasMoved == rateSlider)
    {
        //[UserSliderCode_rateSlider] -- add your slider handling code here..

        String tmpName = rateSlider->getName();
        const bool tmpSync = sync->getValue();

        if(tmpSync){
            int tmpVal = rateSlider->getValue();

            if(tmpVal < beatDivision.size() &&
               tmpVal < valueBeatDivision.size())
            {
                float tmpRate = 1.0/(valueBeatDivision[tmpVal]*0.001);
                tmpName += beatDivision[tmpVal];

                rate->setValue(tmpRate);
            }
        } else {
            double tmpVal = rateSlider->getValue();
            rate->setValue(tmpVal);
            tmpName += String(tmpVal, 2);
        }

        labelRef.setText(tmpName, dontSendNotification);

        //[/UserSliderCode_rateSlider]
    }
    else if (sliderThatWasMoved == waveSlider)
    {
        //[UserSliderCode_waveSlider] -- add your slider handling code here..
        int waveSelection = 4*waveSlider->getValue();
        int tmpValue = 0;
        String tmpName = waveSlider->getName();

        switch(waveSelection){
            case 0:
                tmpName += String("Sine");
                tmpValue = QuadratureOscillator::SINE;
                break;
            case 1:
                tmpName += String("Triangle");
                tmpValue = QuadratureOscillator::TRIANGLE;
                break;
            case 2:
                tmpName += String("Pulse");
                tmpValue = QuadratureOscillator::PULSE;
                break;
            case 3:
                tmpName += String("Saw Up");
                tmpValue = QuadratureOscillator::SAWUP;
                break;
            case 4:
                tmpName += String("Saw Down");
                tmpValue = QuadratureOscillator::SAWDOWN;
                break;
        }

        labelRef.setText(tmpName, dontSendNotification);

        if(wave != nullptr){
            wave->setValue(tmpValue);
        }
        //[/UserSliderCode_waveSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void StereoFlangerComponent::setBeatDivisionStrings(StringArray beatDivStr){
    beatDivision.swapWith(beatDivStr);
}

void StereoFlangerComponent::setBeatDivisionvalues(std::vector<double>beatDivVal){
    valueBeatDivision.swap(beatDivVal);
}

int StereoFlangerComponent::findClosestTimeDivision(double freq){
    double tmpPeriod = 0.0;
    int result = 0;

    if(freq != 0){
        bool run = true;
        int nrOfSteps = valueBeatDivision.size();
        tmpPeriod= (1.0/freq)*1000;

        if(nrOfSteps != 0){
            bool run = true;
            int halvation = nrOfSteps*0.5;
            int index = 1;

            if(tmpPeriod <= valueBeatDivision[nrOfSteps-1]){
                run = false;
                result = nrOfSteps-1;
            }

            while(run && index < nrOfSteps-1){
                double prev = valueBeatDivision[index-1];
                double val = valueBeatDivision[index];
                double next = valueBeatDivision[index+1];

                if(val == tmpPeriod){
                    result = index;
                    run = false;
                }

                if(prev >= tmpPeriod && val < tmpPeriod){
                    result = index-1;
                    run = false;
                }

                if(val > tmpPeriod && next <= tmpPeriod){
                    result = index+1;
                    run = false;
                }

                index +=3;
            }
        }
    }
    return result;
}

void StereoFlangerComponent::initRateSlider(){
    if(sync != nullptr &&
       rate != nullptr &&
       beatDivision.size() != 0 &&
       valueBeatDivision.size() != 0)
    {
        const bool tmpSync = sync->getValue();
        double tmpRate = rate->getValue();

        if(tmpSync){
            int tmpIndex = findClosestTimeDivision(tmpRate);

            rateSlider->setRange(0, valueBeatDivision.size()-1, 1);
            rateSlider->setValue(tmpIndex, dontSendNotification);
        } else {        
            const double maxFreq = 1.0/(valueBeatDivision[valueBeatDivision.size()-1]*0.001);
            const double minFreq = 1.0/(valueBeatDivision[0]*0.001);

            rateSlider->setRange(minFreq, maxFreq, 0);
            rateSlider->setValue(tmpRate, sendNotificationAsync);
        }
    } else {
        if(sync == nullptr || rate == nullptr){
            std::cerr << "Parameters bound not properly : " << getName() << std::endl;
        }

        if(beatDivision.size() == 0 || valueBeatDivision.size() == 0){
            std::cerr << "Beat Divisor tables not set properly" << std::endl;
        }
    }
}

void StereoFlangerComponent::handleCommandMessage(int commandId){
    if(commandId == update){
        enableToggle->postCommandMessage(ParamToggle::update);
        crossFB->postCommandMessage(ParamToggle::update);
        depthSlider->postCommandMessage(ParamSlider::update);
        feedBackSlider->postCommandMessage(ParamSlider::update);

        initRateSlider();

        if(sync != nullptr){
            const bool tmpSync = sync->getValue();\
            SyncToggle->setToggleState(tmpSync, dontSendNotification);
        }

        if(wave != nullptr){
            const double tmpWave = wave->getValue()*0.25;
            waveSlider->setValue(tmpWave, dontSendNotification);
        }

        if(phase != nullptr){
            phaseSlider->setValue(phase->getValue());
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

<JUCER_COMPONENT documentType="Component" className="StereoFlangerComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;wave(nullptr)&#10;sync(nullptr)&#10;rate(nullptr)&#10;phase(nullptr)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <BACKGROUND backgroundColour="ffe4e4">
    <TEXT pos="189 86 27 12" fill="solid: ffaaaaaa" hasStroke="0" text="FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="37 86 35 12" fill="solid: ffaaaaaa" hasStroke="0" text="RATE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="175 206 51 12" fill="solid: ffaaaaaa" hasStroke="0" text="DEPTH"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="29 206 51 12" fill="solid: ffaaaaaa" hasStroke="0" text="PHASE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="101 150 51 12" fill="solid: ffaaaaaa" hasStroke="0" text="WAVE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="feedBackSlider" id="e00bf10d85d4f442" memberName="feedBackSlider"
          virtualName="" explicitFocusOrder="0" pos="169 24 64 64" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="SyncToggle" id="b22b43d4900d0f35" memberName="SyncToggle"
                virtualName="" explicitFocusOrder="0" pos="80 64 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="phaseSlider" id="4a82caecce18144e" memberName="phaseSlider"
          virtualName="" explicitFocusOrder="0" pos="24 144 64 64" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="depthSlider" id="11922ba5b0de9d70" memberName="depthSlider"
          virtualName="" explicitFocusOrder="0" pos="169 144 64 64" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="rateSlider" id="6095d353315c62cd" memberName="rateSlider"
          virtualName="" explicitFocusOrder="0" pos="24 24 64 64" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="crossFB" id="ba0dd7e095193965" memberName="crossFB" virtualName=""
                explicitFocusOrder="0" pos="157 64 24 24" buttonText="" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="enableToggle" id="dfef00aeb55135fd" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="waveSlider" id="285937208f9d68f" memberName="waveSlider"
          virtualName="" explicitFocusOrder="0" pos="96 88 64 64" min="0"
          max="1" int="0.2000000000000000111" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
