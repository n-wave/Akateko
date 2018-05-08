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

#include "DimensionChorusComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <iostream>
//[/MiscUserDefs]

//==============================================================================
DimensionChorusComponent::DimensionChorusComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..
    vector<int>paramIndices = p.getParameterIndices(AkatekoAudioProcessor::DChorusId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::DChorusId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    if(paramIndices.size() == 9 &&
       paramIds.size() == 9 &&
       params.size() >= paramIndices[8])
    {

        addAndMakeVisible(enableToggle = new ParamToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible (rateSlider = new ParamSlider(paramIds[1], *params.getUnchecked(paramIndices[1]), label, 0.02, 5));

        addAndMakeVisible (spreadSlider = new ParamSlider(paramIds[2], *params.getUnchecked(paramIndices[2]), label));
        addAndMakeVisible (depthSlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label));

        addAndMakeVisible (waveSlider = new Slider (paramIds[4]));
        wave = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible (frequencySlider = new ParamSlider(paramIds[5], *params.getUnchecked(paramIndices[5]), label, 20.f, 20000.f));
        addAndMakeVisible (centerAmpSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), label));
        addAndMakeVisible (centerPanSlider = new ParamSlider(paramIds[7], *params.getUnchecked(paramIndices[7]), label));
        addAndMakeVisible (mixSlider = new ParamSlider(paramIds[8], *params.getUnchecked(paramIndices[8]), label));

    } else {
        addAndMakeVisible (enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (rateSlider = new Slider ("rateSlider"));
        addAndMakeVisible (spreadSlider = new Slider ("spreadSlider"));
        addAndMakeVisible (depthSlider = new Slider ("dephSlider"));
        addAndMakeVisible (waveSlider = new Slider ("waveSlider"));
        addAndMakeVisible (frequencySlider = new Slider ("frequencySlider"));
        addAndMakeVisible (centerAmpSlider = new Slider ("centerMixSlider"));
        addAndMakeVisible (centerPanSlider = new Slider ("centerPanSlider"));
        addAndMakeVisible (mixSlider = new Slider ("rightMixSlider"));

        std::cerr << "DimensionChorus::DimensionChorus" << std::endl;
        std::cerr << "Parameters are not bound properly" << std::endl;

        int index = 0;

        std::cerr << "Param Indices size : " << paramIndices.size() <<  std::endl;
        std::cerr << "Param Ids size : " << paramIds.size() << std::endl;

        while(index < paramIndices.size() &&
              index < paramIds.size())
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

    rateSlider->setRange (0, 1, 0);
    rateSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rateSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    depthSlider->setRange (0, 1, 0);
    depthSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    depthSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    spreadSlider->setRange (0, 1, 0);
    spreadSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    spreadSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    spreadSlider->addListener (this);

    centerAmpSlider->setRange (0, 1, 0);
    centerAmpSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerAmpSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    centerPanSlider->setRange (0, 1, 0);
    centerPanSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerPanSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    frequencySlider->setRange (0, 1, 0);
    frequencySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    frequencySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    waveSlider->setRange (0, 1, 0.5);
    waveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    waveSlider->setMouseDragSensitivity(60);
    waveSlider->addListener (this);

    //[UserPreSize]

    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    if(wave != nullptr){
        const double tmpValue = wave->getValue();
        waveSlider->setValue(tmpValue, dontSendNotification);
    }
    //[/Constructor]
}

DimensionChorusComponent::~DimensionChorusComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    rateSlider = nullptr;
    depthSlider = nullptr;
    spreadSlider = nullptr;
    centerAmpSlider = nullptr;
    mixSlider = nullptr;
    centerPanSlider = nullptr;
    frequencySlider = nullptr;
    waveSlider = nullptr;
    enableToggle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DimensionChorusComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("WAVE"),
                29, 142, 42, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("C-AMP"),
                178, 182, 50, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("RATE"),
                30, 78, 42, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DEPTH"),
                21, 214, 54, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("HPF"),
                112, 78, 38, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("SPREAD"),
                101, 142, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("C-PAN"),
                106, 214, 50, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("MIX"),
                184, 102, 35, 10,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DimensionChorusComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    rateSlider->setBounds (24, 24, 54, 54);
    depthSlider->setBounds (24, 160, 54, 54);
    spreadSlider->setBounds (104, 92, 54, 54);
    centerAmpSlider->setBounds (176, 129, 54, 54);
    mixSlider->setBounds (176, 48, 54, 54);
    centerPanSlider->setBounds (104, 160, 54, 54);
    frequencySlider->setBounds (104, 24, 54, 54);
    waveSlider->setBounds (24, 92, 54, 54);
    enableToggle->setBounds (235, -3, 24, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DimensionChorusComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == spreadSlider)
    {
        //[UserSliderCode_spreadSlider] -- add your slider handling code here..
        //[/UserSliderCode_spreadSlider]
    }
    else if (sliderThatWasMoved == waveSlider)
    {
        //[UserSliderCode_waveSlider] -- add your slider handling code here..
        String tmpName = waveSlider->getName();
        int waveSelection = waveSlider->getValue()*2;
        int tmpValue = 0;

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
                tmpName += String("Cosine");
                tmpValue = QuadratureOscillator::COSINE;
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
void DimensionChorusComponent::handleCommandMessage(int commandId){
    if(commandId == update){
        enableToggle->postCommandMessage(ParamToggle::update);
        rateSlider->postCommandMessage(ParamSlider::update);
        spreadSlider->postCommandMessage(ParamSlider::update);
        depthSlider->postCommandMessage(ParamSlider::update);
        frequencySlider->postCommandMessage(ParamSlider::update);
        centerAmpSlider->postCommandMessage(ParamSlider::update);
        centerPanSlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);

        if(wave != nullptr){
            const double tmpValue = wave->getValue();
            waveSlider->setValue(tmpValue, dontSendNotification);
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

<JUCER_COMPONENT documentType="Component" className="DimensionChorusComponent"
                 componentName="" parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <BACKGROUND backgroundColour="929191">
    <TEXT pos="29 142 42 10" fill="solid: ffaaaaaa" hasStroke="0" text="WAVE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="178 182 50 10" fill="solid: ffaaaaaa" hasStroke="0" text="C-AMP"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="30 78 42 10" fill="solid: ffaaaaaa" hasStroke="0" text="RATE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="21 214 54 10" fill="solid: ffaaaaaa" hasStroke="0" text="DEPTH"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="112 78 38 10" fill="solid: ffaaaaaa" hasStroke="0" text="HPF"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="101 142 60 10" fill="solid: ffaaaaaa" hasStroke="0" text="SPREAD"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="106 214 50 10" fill="solid: ffaaaaaa" hasStroke="0" text="C-PAN"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="184 102 35 10" fill="solid: ffaaaaaa" hasStroke="0" text="MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="rateSlider" id="b9d7855b70230b1a" memberName="rateSlider"
          virtualName="" explicitFocusOrder="0" pos="24 24 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="dephSlider" id="6a0359bd086bb4b8" memberName="depthSlider"
          virtualName="" explicitFocusOrder="0" pos="24 160 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="spreadSlider" id="f3ef318d7ade8956" memberName="spreadSlider"
          virtualName="" explicitFocusOrder="0" pos="104 92 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="centerAmpSlider" id="3595ec68d4d2c2ab" memberName="centerAmpSlider"
          virtualName="" explicitFocusOrder="0" pos="176 129 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="mixSlider" id="f4769a4741fb690a" memberName="mixSlider"
          virtualName="" explicitFocusOrder="0" pos="176 48 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="centerPanSlider" id="baa1ca2054b4d12f" memberName="centerPanSlider"
          virtualName="" explicitFocusOrder="0" pos="104 160 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="frequencySlider" id="ff5a004c0916f695" memberName="frequencySlider"
          virtualName="" explicitFocusOrder="0" pos="104 24 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="waveSlider" id="cf4352871cebc554" memberName="waveSlider"
          virtualName="" explicitFocusOrder="0" pos="24 92 54 54" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
