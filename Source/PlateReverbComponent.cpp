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
#include "Akateko.h"
#include <iostream>

using akateko::updateFxGUI;
using akateko::initMidiRow;
using akateko::MidiRow;
//[/MiscUserDefs]

//==============================================================================
PlateReverbComponent::PlateReverbComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::pReverbId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::pReverbId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    if(paramIndices.size() == 5 &&
       paramIds.size() == 5 &&
       params.size() >= paramIndices[4])
    {
        requestMenuId[0] = paramIds[0].hash();
        requestMenuId[1] = paramIds[1].hash();
        requestMenuId[2] = paramIds[2].hash();
        requestMenuId[3] = paramIds[3].hash();
        requestMenuId[4] = paramIds[4].hash();

        initiateMidiStrings();

        addAndMakeVisible(enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), labelRef));
        addAndMakeVisible(dampingSlider = new ParamSlider(paramIds[1], *params.getUnchecked(paramIndices[1]), labelRef));
        addAndMakeVisible(bandWidthSlider = new ParamSlider(paramIds[2], *params.getUnchecked(paramIndices[2]), labelRef));
        addAndMakeVisible(decaySlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), labelRef));
        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[4], *params.getUnchecked(paramIndices[4]), labelRef));

    } else {
        addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));
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
    const Image bigRotary = ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize);
    blaf = new SliderLookAndFeel(bigRotary);
    claf = new CustomLookAndFeel();

    menu.setLookAndFeel(claf);

    enableToggle->setButtonText (String());
    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));


    decaySlider->setRange (0, 1, 0);
    decaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    decaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    decaySlider->setLookAndFeel(blaf);

    dampingSlider->setRange (0, 1, 0);
    dampingSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    dampingSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    dampingSlider->setLookAndFeel(blaf);

    bandWidthSlider->setRange (0, 1, 0);
    bandWidthSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    bandWidthSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    bandWidthSlider->setLookAndFeel(blaf);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mixSlider->setLookAndFeel(blaf);
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
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
    claf = nullptr;
    blaf = nullptr;
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

    enableToggle->setBounds (238, 0, 18, 18);
    decaySlider->setBounds (152, 136, 72, 72);
    dampingSlider->setBounds (40, 136, 72, 72);
    bandWidthSlider->setBounds (40, 32, 72, 72);
    mixSlider->setBounds (152, 32, 72, 72);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PlateReverbComponent::handleCommandMessage(int commandId){
    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        dampingSlider->postCommandMessage(ParamSlider::update);
        bandWidthSlider->postCommandMessage(ParamSlider::update);
        decaySlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);
    }

    if(paramIndices.size() == 5 &&
       midiStrings.size() == 5)
    {
        int index = -1;
        int param = -1;
        int handling = -1;

        if(commandId == requestMenuId[0]){          // Enable
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[1]){   // Damping
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[2]){   // BandWidth
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[3]){   // Decay
            index = menu.show();
            param = 3;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[4]){   // Mix
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        if(index == 1){
            MidiRow tmpRow;
            initMidiRow(tmpRow, param, 0, 127, 0.0, 1.0, paramIndices[param], handling, midiStrings[param], 17);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }
}

void PlateReverbComponent::initiateMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" P-Reverb: Enable");
    midiStrings.add(" P-Reverb: Damping");
    midiStrings.add(" P-Reverb: Bandwidth");
    midiStrings.add(" P-Reverb: Decay");
    midiStrings.add(" P-Reverb: Mix");
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
  <IMAGEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
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
