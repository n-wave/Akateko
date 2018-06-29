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
#include "Akateko.h"

using akateko::updateFxGUI;
using akateko::MidiRow;
using akateko::initMidiRow;
using akateko::IgnoreRightClick;
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
    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::decimatorId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::decimatorId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    if(paramIndices.size() == 7 &&
       paramIds.size() == 7 &&
       params.size() >= paramIndices[6])
    {
        requestMenuIds[0] = paramIds[0].hash();
        requestMenuIds[1] = paramIds[1].hash();
        requestMenuIds[2] = paramIds[2].hash();
        requestMenuIds[3] = paramIds[3].hash();
        requestMenuIds[4] = paramIds[4].hash();
        requestMenuIds[5] = paramIds[5].hash();
        requestMenuIds[6] = paramIds[6].hash();

        initialiseMidiStrings();

        addAndMakeVisible(enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]),labelRef));
        addAndMakeVisible(reduceEnable = new ParamImageToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), labelRef));
        addAndMakeVisible(srateEnable = new ParamImageToggle(paramIds[2], *params.getUnchecked(paramIndices[2]), labelRef));
        addAndMakeVisible(filterToggle = new ParamImageToggle(paramIds[3], *params.getUnchecked(paramIndices[3]), labelRef));

        addAndMakeVisible(bitSlider = new IgnoreRightClick<Slider>(paramIds[4]));
        bitReduction = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible(rateSlider = new IgnoreRightClick<Slider>(paramIds[5]));
        srateReduction = params.getUnchecked(paramIndices[5]);

        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), labelRef));
    } else {
        std::cerr << "DecimatorComponent::DecimatorComponent" << std::endl;
        std::cerr << "Parameters not bound properly" << std::endl;

        addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));
        addAndMakeVisible (srateEnable = new ImageButton ("srateEnable"));
        addAndMakeVisible (reduceEnable = new ImageButton ("reduceEnable"));
        addAndMakeVisible (filterToggle = new ImageButton ("filterToggle"));
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
    rateSlider->setRange (0.0, 1.0, 0);
    rateSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rateSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rateSlider->addListener (this);


    bitSlider->setRange (0.0, 1.0, 0);
    bitSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    bitSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    bitSlider->addListener (this);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setDoubleClickReturnValue(true, 0.5);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    enableToggle->setButtonText (String());
    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    filterToggle->setButtonText (String());
    filterToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    reduceEnable->setButtonText(String());
    reduceEnable->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    srateEnable->setButtonText (String());
    srateEnable->setImages(false, true, false,
                           ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                           ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                           ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    //[UserPreSize]
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    claf = new CustomLookAndFeel();
    blaf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize));

    menu.setLookAndFeel(claf);
    rateSlider->setLookAndFeel(blaf);
    bitSlider->setLookAndFeel(blaf);
    mixSlider->setLookAndFeel(blaf);

    if(bitReduction != nullptr){
        const double tmpValue = bitReduction->getValue();
        bitSlider->setValue(tmpValue, dontSendNotification);
    }

    if(srateReduction != nullptr){
        const double tmpValue = srateReduction->getValue();
        rateSlider->setValue(tmpValue, dontSendNotification);
    }

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
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
    claf = nullptr;
    blaf = nullptr;
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
    mixSlider->setBounds (154, 32, 72, 72);
    enableToggle->setBounds (238, 0, 18, 18);
    filterToggle->setBounds (143, 88, 16, 16);
    reduceEnable->setBounds (96, 88, 16, 16);
    srateEnable->setBounds (120, 117, 16, 16);
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
        const int labelValue = tmpValue*100;

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
        const int labelValue = tmpValue*100;

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
    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        reduceEnable->postCommandMessage(ParamImageToggle::update);
        srateEnable->postCommandMessage(ParamImageToggle::update);
        filterToggle->postCommandMessage(ParamImageToggle::update);
        mixSlider->postCommandMessage(ParamSlider::update);

        if(bitReduction != nullptr){
            const double tmpValue = bitReduction->getValue();
            bitSlider->setValue(tmpValue, sendNotificationAsync);
        }

        if(srateReduction != nullptr){
            const double tmpValue = srateReduction->getValue();
            rateSlider->setValue(tmpValue, sendNotificationAsync);
        }
    }

    else if(paramIndices.size() == 7 &&
            midiStrings.size() ==  7)
    {
        int index = -1;;
        int param = -1;
        int handling = -1;

        if(commandId == requestMenuIds[0]){         // Enable
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[1]){  // BIT Reduction Enable
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[2]){  // SR Reduction Enable
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[3]){  // Filter Enabled
            index = menu.show();
            param = 3;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[4]){  // Bit reduction
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[5]){  // Srate Reduction
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[6]){  // Mix
            index = menu.show();
            param = 6;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        if(index == 1){
            MidiRow tmpRow;
            initMidiRow(tmpRow, param, 0, 127, 0.0, 1.0, paramIndices[param], handling, midiStrings[param], 16);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }
}

void DecimatorComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" Decimator: Enable");
    midiStrings.add(" Decimator: Red En");
    midiStrings.add(" Decimator: SR En");
    midiStrings.add(" Decimator: Filter");
    midiStrings.add(" Decimator: Reduce");
    midiStrings.add(" Decimator: SRate");
    midiStrings.add(" Decimator: Mix");
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
  <IMAGEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
               virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="filterToggle" id="44e2075e9f6d8a26" memberName="filterToggle"
               virtualName="" explicitFocusOrder="0" pos="142 88 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="reduceEnable" id="cae87162d6a88cf9" memberName="reduceEnable"
               virtualName="" explicitFocusOrder="0" pos="96 88 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="srateEnable" id="510b26acf7761569" memberName="srateEnable"
               virtualName="" explicitFocusOrder="0" pos="119 112 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
