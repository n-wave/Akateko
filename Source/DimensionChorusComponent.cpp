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
#include "Akateko.h"

using akateko::updateFxGUI;
using akateko::IgnoreRightClick;
using akateko::MidiRow;
using akateko::initMidiRow;
//[/MiscUserDefs]

//==============================================================================
DimensionChorusComponent::DimensionChorusComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..
    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::DChorusId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::DChorusId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    if(paramIndices.size() == 9 &&
       paramIds.size() == 9 &&
       params.size() >= paramIndices[8])
    {
        requestMenuIds[0] = paramIds[0].hash();
        requestMenuIds[1] = paramIds[1].hash();
        requestMenuIds[2] = paramIds[2].hash();
        requestMenuIds[3] = paramIds[3].hash();
        requestMenuIds[4] = paramIds[4].hash();
        requestMenuIds[5] = paramIds[5].hash();
        requestMenuIds[6] = paramIds[6].hash();
        requestMenuIds[7] = paramIds[7].hash();
        requestMenuIds[8] = paramIds[8].hash();

        initialiseMidiStrings();

        addAndMakeVisible(enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible (rateSlider = new ParamSlider(paramIds[1], *params.getUnchecked(paramIndices[1]), label, 0.02, 5));

        addAndMakeVisible (spreadSlider = new ParamSlider(paramIds[2], *params.getUnchecked(paramIndices[2]), label));
        addAndMakeVisible (depthSlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label));

        addAndMakeVisible (waveSlider = new IgnoreRightClick<Slider> (paramIds[4]));
        wave = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible (frequencySlider = new ParamSlider(paramIds[5], *params.getUnchecked(paramIndices[5]), label, 20.f, 20000.f));
        addAndMakeVisible (centerAmpSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), label));
        addAndMakeVisible (centerPanSlider = new ParamSlider(paramIds[7], *params.getUnchecked(paramIndices[7]), label));
        addAndMakeVisible (mixSlider = new ParamSlider(paramIds[8], *params.getUnchecked(paramIndices[8]), label));

    } else {
        addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));
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
    mixSlider->setDoubleClickReturnValue(true, 0.5f);


    centerPanSlider->setRange (0, 1, 0);
    centerPanSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerPanSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    centerPanSlider->setDoubleClickReturnValue(true, 0.5f);

    frequencySlider->setRange (0, 1, 0);
    frequencySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    frequencySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    waveSlider->setRange (0, 2, 1);
    waveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    waveSlider->setMouseDragSensitivity(60);

    waveSlider->addListener (this);

    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    //[UserPreSize]

    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..

    blaf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize));
    pblaf = new SliderLookAndFeel(ImageCache::getFromMemory (BinaryData::PanBig_png, BinaryData::PanBig_pngSize));
    w2laf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::WaveStripV2_png, BinaryData::WaveStripV2_pngSize));
    claf = new CustomLookAndFeel();

    rateSlider->setLookAndFeel(blaf);
    depthSlider->setLookAndFeel(blaf);
    spreadSlider->setLookAndFeel(blaf);
    mixSlider->setLookAndFeel(blaf);
    centerPanSlider->setLookAndFeel(pblaf);
    frequencySlider->setLookAndFeel(blaf);
    waveSlider->setLookAndFeel(w2laf);
    centerAmpSlider->setLookAndFeel(blaf);
    menu.setLookAndFeel(claf);

    if(wave != nullptr){
        const double tmpValue = wave->getValue();
        waveSlider->setValue(tmpValue, dontSendNotification);
    }

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
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
    blaf = nullptr;
    pblaf = nullptr;
    w2laf = nullptr;
    claf = nullptr;
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
    waveSlider->setBounds (28, 97, 44, 44);
    enableToggle->setBounds (238, 0, 18, 18);
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
        int waveSelection = waveSlider->getValue();
        int tmpValue = 0;

        switch(waveSelection){
            case 0:
                tmpName += String("Sine");
                break;
            case 1:
                tmpName += String("Triangle");
                break;
            case 2:
                tmpName += String("Cosine");
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
    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        rateSlider->postCommandMessage(ParamSlider::update);
        spreadSlider->postCommandMessage(ParamSlider::update);
        depthSlider->postCommandMessage(ParamSlider::update);
        frequencySlider->postCommandMessage(ParamSlider::update);
        centerAmpSlider->postCommandMessage(ParamSlider::update);
        centerPanSlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);

        if(wave != nullptr){
            const double tmpValue = wave->getValue();
            waveSlider->setValue(tmpValue, sendNotificationAsync);
        }
    }

    if(paramIndices.size() == 9 &&
       midiStrings.size() == 9)
    {
        int index = -1;
        int param = -1;
        int handling = -1;
        double minValue = 0.0;
        double maxValue = 1.0;

        if(commandId == requestMenuIds[0]){         // Enable
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[1]){  // Rate
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[2]){  // Spread
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[3]){  // Depth
            index = menu.show();
            param = 3;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[4]){  // Wave
            index = menu.show();
            param = 4;
            maxValue = 2;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[5]){  // HPF Frequency
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[6]){  // Center Amp
            index = menu.show();
            param = 6;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[7]){  // Center Pan
            index = menu.show();
            param = 7;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[8]){  // Mix
            index = menu.show();
            param = 8;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        if(index == 1){
            MidiRow tmpRow;
            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling, midiStrings[param], 14);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }
}

void DimensionChorusComponent::initialiseMidiStrings()
{
    midiStrings.clear();

    midiStrings.add(" D-Chorus: Enable");
    midiStrings.add(" D-Chorus: Rate");
    midiStrings.add(" D-Chorus: Spread");
    midiStrings.add(" D-Chorus: Depth");
    midiStrings.add(" D-Chorus: Wave");
    midiStrings.add(" D-Chorus: HPF Freq");
    midiStrings.add(" D-Chorus: C-Amp");
    midiStrings.add(" D-Chorus: C-Pan");
    midiStrings.add(" D-Chorus: Mix");
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
  <IMAGEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
               virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
