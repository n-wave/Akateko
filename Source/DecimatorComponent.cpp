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

#include "DecimatorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <iostream>
/*
    AudioParameterBool *decimatorEnable;
    AudioParameterBool *decimatorReduceEna;
    AudioParameterBool *decimatorSRateEna;
    AudioParameterBool *decimatorFilter;
    AudioParameterFloat *decimatorBits;
    AudioParameterFloat *decimatorSRate;
    AudioParameterFloat *decimatorMix;
 */

//[/MiscUserDefs]

//==============================================================================
DecimatorComponent::DecimatorComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      bitReduction(nullptr),
      srateReduction(nullptr),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..
    vector<int>paramIndices = p.getParameterIndices(AkatekoAudioProcessor::decimatorId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::decimatorId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    if(paramIndices.size() == 7 &&
       paramIds.size() == 7 &&
       params.size() >= paramIndices[6])
    {
        addAndMakeVisible(enableToggle = new ParamToggle(paramIds[0], *params.getUnchecked(paramIndices[0]),labelRef));
        addAndMakeVisible(reduceEnable = new ParamToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), labelRef));
        addAndMakeVisible(srateEnable = new ParamToggle(paramIds[2], *params.getUnchecked(paramIndices[2]), labelRef));
        addAndMakeVisible(filterToggle = new ParamToggle(paramIds[3], *params.getUnchecked(paramIndices[3]), labelRef));

        addAndMakeVisible(bitSlider = new Slider(paramIds[4]));
        bitReduction = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible(rateSlider = new Slider(paramIds[5]));
        srateReduction = params.getUnchecked(paramIndices[5]);

        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), labelRef));
    } else {
        std::cerr << "DecimatorComponent::DecimatorComponent" << std::endl;
        std::cerr << "Parameters not bound properly" << std::endl;

        addAndMakeVisible (enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (srateEnable = new ToggleButton ("srateEnable"));
        addAndMakeVisible (reduceEnable = new ToggleButton ("reduceEnable"));
        addAndMakeVisible (filterToggle = new ToggleButton ("filterToggle"));
        addAndMakeVisible (rateSlider = new Slider ("rateSlider"));
        addAndMakeVisible (bitSlider = new Slider ("bitSlider"));
        addAndMakeVisible (mixSlider = new Slider ("mixSlider"));

        int index = 0;

        while(index < paramIndices.size() &&
              index < params.size())
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
    rateSlider->setRange (0.0, 0.99, 0);
    rateSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rateSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rateSlider->addListener (this);


    bitSlider->setRange (0.0, 0.99, 0);
    bitSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    bitSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    bitSlider->addListener (this);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    enableToggle->setButtonText (String());
    filterToggle->setButtonText (String());
    reduceEnable->setButtonText (String());
    srateEnable->setButtonText (String());


    //[UserPreSize]
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..

    if(bitReduction != nullptr){
        const double tmpValue = bitReduction->getValue();
        bitSlider->setValue(tmpValue, dontSendNotification);
    }

    if(srateReduction != nullptr){
        const double tmpValue = srateReduction->getValue();
        rateSlider->setValue(tmpValue, dontSendNotification);
    }

    //[/Constructor]
}

DecimatorComponent::~DecimatorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    rateSlider = nullptr;
    bitSlider = nullptr;
    mixSlider = nullptr;
    enableToggle = nullptr;
    filterToggle = nullptr;
    reduceEnable = nullptr;
    srateEnable = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DecimatorComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("REDUCE"),
                35, 102, 66, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 14.00f, Font::plain));
    g.drawText (TRANS("SRATE"),
                96, 206, 62, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 14.00f, Font::plain));
    g.drawText (TRANS("MIX"),
                171, 102, 34, 8,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DecimatorComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    rateSlider->setBounds (93, 136, 72, 72);
    bitSlider->setBounds (32, 32, 72, 72);
    mixSlider->setBounds (152, 32, 72, 72);
    enableToggle->setBounds (235, -3, 24, 24);
    filterToggle->setBounds (142, 88, 24, 24);
    reduceEnable->setBounds (96, 88, 24, 24);
    srateEnable->setBounds (119, 112, 24, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DecimatorComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == rateSlider)
    {
        //[UserSliderCode_rateSlider] -- add your slider handling code here..
        const double tmpValue = rateSlider->getValue();
        const int labelValue = tmpValue*100+1;

        String tmpName = rateSlider->getName() +
                         String(labelValue) +
                         String("%");


        if(srateReduction != nullptr){
            srateReduction->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_rateSlider]
    }
    else if (sliderThatWasMoved == bitSlider)
    {
        //[UserSliderCode_bitSlider] -- add your slider handling code here..
        const double tmpValue = bitSlider->getValue();
        const int labelValue = tmpValue*100+1;

        String tmpName = bitSlider->getName() +
                         String(labelValue) +
                         String("%");

        if(bitReduction != nullptr){
            bitReduction->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_bitSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DecimatorComponent::handleCommandMessage(int commandId){
    if(commandId == update){
        enableToggle->postCommandMessage(ParamToggle::update);
        reduceEnable->postCommandMessage(ParamToggle::update);
        srateEnable->postCommandMessage(ParamToggle::update);
        filterToggle->postCommandMessage(ParamToggle::update);
        mixSlider->postCommandMessage(ParamToggle::update);

        if(bitReduction != nullptr){
            const double tmpValue = bitReduction->getValue();
            bitSlider->setValue(tmpValue, dontSendNotification);
        }

        if(srateReduction != nullptr){
            const double tmpValue = srateReduction->getValue();
            rateSlider->setValue(tmpValue, dontSendNotification);
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

<JUCER_COMPONENT documentType="Component" className="DecimatorComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="35 102 66 10" fill="solid: ffaaaaaa" hasStroke="0" text="REDUCE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="96 206 62 10" fill="solid: ffaaaaaa" hasStroke="0" text="SRATE"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
    <TEXT pos="171 102 34 8" fill="solid: ffaaaaaa" hasStroke="0" text="MIX"
          fontname="Good Times" fontsize="14" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="rateSlider" id="d2598f216032723b" memberName="rateSlider"
          virtualName="" explicitFocusOrder="0" pos="93 136 72 72" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="bitSlider" id="17d0dd7cee76576f" memberName="bitSlider"
          virtualName="" explicitFocusOrder="0" pos="32 32 72 72" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="mixSlider" id="5c8a738dda3713f4" memberName="mixSlider"
          virtualName="" explicitFocusOrder="0" pos="152 32 72 72" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="filterToggle" id="44e2075e9f6d8a26" memberName="filterToggle"
                virtualName="" explicitFocusOrder="0" pos="142 88 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="reduceEnable" id="cae87162d6a88cf9" memberName="reduceEnable"
                virtualName="" explicitFocusOrder="0" pos="96 88 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="srateEnable" id="510b26acf7761569" memberName="srateEnable"
                virtualName="" explicitFocusOrder="0" pos="119 112 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
