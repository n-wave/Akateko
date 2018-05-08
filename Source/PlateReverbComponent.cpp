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

#include "PlateReverbComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <iostream>
//[/MiscUserDefs]

//==============================================================================
PlateReverbComponent::PlateReverbComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    vector<int>paramIndices = p.getParameterIndices(AkatekoAudioProcessor::pReverbId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::pReverbId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    std::cout << "Param Indices Size : " << paramIndices.size() << std::endl;
    std::cout << "ParamIds Size : " << paramIds.size() << std::endl;

    if(paramIndices.size() == 5 &&
       paramIds.size() == 5 &&
       params.size() >= paramIndices[4])
    {
        addAndMakeVisible(enableToggle = new ParamToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), labelRef));
        addAndMakeVisible(dampingSlider = new ParamSlider(paramIds[1], *params.getUnchecked(paramIndices[1]), labelRef));
        addAndMakeVisible(bandWidthSlider = new ParamSlider(paramIds[2], *params.getUnchecked(paramIndices[2]), labelRef));
        addAndMakeVisible(decaySlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), labelRef));
        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[4], *params.getUnchecked(paramIndices[4]), labelRef));

    } else {
        addAndMakeVisible (enableToggle = new ToggleButton ("enableToggle"));
        addAndMakeVisible (dampingSlider = new Slider ("dampingSlider"));
        addAndMakeVisible (bandWidthSlider = new Slider ("bandWidthSlider"));
        addAndMakeVisible (decaySlider = new Slider("decaySlider"));
        addAndMakeVisible (mixSlider = new Slider ("mixSlider"));

        std::cerr << "PlateReverbComponent::PlateReverbComponent" << std::endl;
        std::cerr << "Parameters are not bound" << std::endl;

        int index = 0;

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

    //[UserPreSize]
    enableToggle->setButtonText (String());

    decaySlider->setRange (0, 1, 0);
    decaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    decaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    dampingSlider->setRange (0, 1, 0);
    dampingSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    dampingSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    bandWidthSlider->setRange (0, 1, 0);
    bandWidthSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    bandWidthSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PlateReverbComponent::~PlateReverbComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    enableToggle = nullptr;
    decaySlider = nullptr;
    dampingSlider = nullptr;
    bandWidthSlider = nullptr;
    mixSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PlateReverbComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DAMPING"),
                38, 208, 74, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("BANDWIDTH"),
                30, 104, 90, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DECAY"),
                160, 208, 50, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("MIX"),
                170, 104, 35, 10,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PlateReverbComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    enableToggle->setBounds (235, -3, 24, 24);
    decaySlider->setBounds (152, 136, 72, 72);
    dampingSlider->setBounds (40, 136, 72, 72);
    bandWidthSlider->setBounds (40, 32, 72, 72);
    mixSlider->setBounds (152, 32, 72, 72);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PlateReverbComponent::handleCommandMessage(int commandId){
    if(commandId == update){
        enableToggle->postCommandMessage(ParamToggle::update);
        dampingSlider->postCommandMessage(ParamSlider::update);
        bandWidthSlider->postCommandMessage(ParamSlider::update);
        decaySlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PlateReverbComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="38 208 74 10" fill="solid: ffaaaaaa" hasStroke="0" text="DAMPING"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="30 104 90 10" fill="solid: ffaaaaaa" hasStroke="0" text="BANDWIDTH"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="160 208 50 10" fill="solid: ffaaaaaa" hasStroke="0" text="DECAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="170 104 35 10" fill="solid: ffaaaaaa" hasStroke="0" text="MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <TOGGLEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="decaySlider" id="17d0dd7cee76576f" memberName="decaySlider"
          virtualName="" explicitFocusOrder="0" pos="152 136 72 72" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="dampingSlider" id="f426d0f51b97a5e0" memberName="dampingSlider"
          virtualName="" explicitFocusOrder="0" pos="40 136 72 72" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="bandWidthSlider" id="5468dfd28d98a886" memberName="bandWidthSlider"
          virtualName="" explicitFocusOrder="0" pos="40 32 72 72" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="mixSlider" id="809fd85b0065f537" memberName="mixSlider"
          virtualName="" explicitFocusOrder="0" pos="152 32 72 72" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
