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
#include "Akateko.h"

using akateko::MidiRow;
using akateko::IgnoreRightClick;
using akateko::initMidiRow;
using akateko::delay_timing_ratios;

using akateko::updateFxGUI;
using akateko::updateFxBPM;
//[/MiscUserDefs]

//==============================================================================
LCRDelayComponent::LCRDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      beatsPerMinute(120.0),
      processor(p),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..
    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::LCRDelayId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::LCRDelayId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    bool proceed = false;

    if(paramIndices.size() == 18 &&
       paramIds.size() == 18 &&
       params.size() >= paramIndices[17])
    {
        requestMenuId[0] = paramIds[0].hash();
        requestMenuId[1] = paramIds[1].hash();
        requestMenuId[2] = paramIds[2].hash();
        requestMenuId[3] = paramIds[3].hash();
        requestMenuId[4] = paramIds[4].hash();
        requestMenuId[5] = paramIds[5].hash();
        requestMenuId[6] = paramIds[6].hash();
        requestMenuId[7] = paramIds[7].hash();
        requestMenuId[8] = paramIds[8].hash();
        requestMenuId[9] = paramIds[9].hash();
        requestMenuId[10] = paramIds[10].hash();
        requestMenuId[11] = paramIds[11].hash();
        requestMenuId[12] = paramIds[12].hash();
        requestMenuId[13] = paramIds[13].hash();
        requestMenuId[14] = paramIds[14].hash();
        requestMenuId[15] = paramIds[15].hash();
        requestMenuId[16] = paramIds[16].hash();
        requestMenuId[17] = paramIds[17].hash();
        requestMenuId[18] = paramIds[18].hash();

        addAndMakeVisible(enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(leftSyncToggle = new ParamImageToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), label));

        addAndMakeVisible(leftDelaySlider = new IgnoreRightClick<Slider>(paramIds[2]));
        leftDelay = p.getParameters().getUnchecked(paramIndices[2]);

        addAndMakeVisible(leftFBSlider = new ParamSlider(paramIds[3], *params.getUnchecked(paramIndices[3]), label, -1.f, 1.f));
        addAndMakeVisible(leftMixSlider = new ParamSlider(paramIds[4], *params.getUnchecked(paramIndices[4]), label));
        addAndMakeVisible(centerSyncToggle = new ParamImageToggle(paramIds[5], *params.getUnchecked(paramIndices[5]), label));

        addAndMakeVisible(centerDelaySlider = new IgnoreRightClick<Slider>(paramIds[6]));
        centerDelay = p.getParameters().getUnchecked(paramIndices[6]);

        addAndMakeVisible(centerFBSlider = new ParamSlider(paramIds[7], *params.getUnchecked(paramIndices[7]), label, -1.f, 1.f));
        addAndMakeVisible(hpfEnableToggle = new ParamImageToggle(paramIds[8], *params.getUnchecked(paramIndices[8]), label));
        addAndMakeVisible(hpfSlider = new ParamSlider(paramIds[9], *params.getUnchecked(paramIndices[9]), label, 20.f, 20000.f));
        addAndMakeVisible(lpfEnableToggle = new ParamImageToggle(paramIds[10], *params.getUnchecked(paramIndices[10]), label));
        addAndMakeVisible(lpfSlider = new ParamSlider(paramIds[11], *params.getUnchecked(paramIndices[11]), label, 20.f, 20000.f));
        addAndMakeVisible(centerAmpSlider = new ParamSlider(paramIds[12], *params.getUnchecked(paramIndices[12]), label));

        //TODO
        //get Value set Value on slider
        addAndMakeVisible (panSlider = new IgnoreRightClick<Slider> (paramIds[13]));
        pan = params.getUnchecked(paramIndices[13]);

        addAndMakeVisible(rightSyncToggle = new ParamImageToggle(paramIds[14], *p.getParameters().getUnchecked(paramIndices[14]), label));

        addAndMakeVisible(rightDelaySlider = new IgnoreRightClick<Slider> (paramIds[15]));
        rightDelay = params.getUnchecked(paramIndices[15]);

        addAndMakeVisible(rightFBSlider = new ParamSlider(paramIds[16], *p.getParameters().getUnchecked(paramIndices[16]), label, -1.f, 1.f));
        addAndMakeVisible(rightMixSlider = new ParamSlider(paramIds[17], *p.getParameters().getUnchecked(paramIndices[17]), label));

        proceed = true;
    } else {
        addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));
        addAndMakeVisible (leftSyncToggle = new ImageButton ("leftSyncToggle"));
        addAndMakeVisible (leftDelaySlider = new Slider ("leftDelaySlider"));
        addAndMakeVisible (leftFBSlider = new Slider ("leftFBSlider"));
        addAndMakeVisible (leftMixSlider = new Slider ("leftMixSlider"));

        addAndMakeVisible (centerSyncToggle = new ImageButton ("centerSyncToggle"));
        addAndMakeVisible (centerDelaySlider = new Slider ("centerDelaySlider"));
        addAndMakeVisible (centerFBSlider = new Slider ("centerFBSlider"));

        addAndMakeVisible (hpfEnableToggle = new ImageButton ("hpfEnableToggle"));
        addAndMakeVisible (hpfSlider = new Slider ("hpfSlider"));
        addAndMakeVisible (lpfEnableToggle = new ImageButton ("lpfEnableToggle"));
        addAndMakeVisible (lpfSlider = new Slider ("lpfSlider"));
        addAndMakeVisible (centerAmpSlider = new Slider ("centerAmpSlider"));
        addAndMakeVisible (panSlider = new Slider ("panSlider"));

        addAndMakeVisible (rightSyncToggle = new ImageButton ("rightSyncToggle"));
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
    const Image bigRotary = ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize);
    const Image smallRotary = ImageCache::getFromMemory(BinaryData::AkatekoSmallV2_png, BinaryData::AkatekoSmallV2_pngSize);
    const Image panSmallRot = ImageCache::getFromMemory(BinaryData::PanSmall_png, BinaryData::PanSmall_pngSize);
    const Image panBigRot = ImageCache::getFromMemory(BinaryData::PanBig_png, BinaryData::PanBig_pngSize);

    claf = new CustomLookAndFeel();
    blaf = new SliderLookAndFeel(bigRotary);
    slaf = new SliderLookAndFeel(smallRotary);
    plaf = new SliderLookAndFeel(panSmallRot);
    fblaf = new SliderLookAndFeel(panBigRot);

    menu.setLookAndFeel(claf);

    leftDelaySlider->setRange (0, 1, 0);
    leftDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftDelaySlider->setLookAndFeel(blaf);
    leftDelaySlider->setDoubleClickReturnValue(true, 0.5);

    leftDelaySlider->addListener (this);

    rightDelaySlider->setRange (0, 1, 0);
    rightDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightDelaySlider->setDoubleClickReturnValue(true, 0.5);
    rightDelaySlider->setLookAndFeel(blaf);
    rightDelaySlider->addListener (this);

    rightFBSlider->setRange (0, 1, 0);
    rightFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightFBSlider->setLookAndFeel(fblaf);
    rightFBSlider->setDoubleClickReturnValue(true, 0.5f);

    leftFBSlider->setRange (0, 1, 0);
    leftFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftFBSlider->setLookAndFeel(fblaf);
    leftFBSlider->setDoubleClickReturnValue(true, 0.5f);

    leftMixSlider->setRange (0, 1, 0);
    leftMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    leftMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    leftMixSlider->setLookAndFeel(blaf);
    leftMixSlider->setDoubleClickReturnValue(true, 0.5f);

    centerDelaySlider->setRange (0, 1, 0);
    centerDelaySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerDelaySlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    centerDelaySlider->setLookAndFeel(blaf);
    centerDelaySlider->addListener (this);

    centerFBSlider->setRange (0, 1, 0);
    centerFBSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerFBSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    centerFBSlider->setLookAndFeel(plaf);
    centerFBSlider->setDoubleClickReturnValue(true, 0.5);

    rightMixSlider->setRange (0, 1, 0);
    rightMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rightMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rightMixSlider->setLookAndFeel(blaf);
    rightMixSlider->setDoubleClickReturnValue(true, 0.5f);

    panSlider->setRange (0, 1, 0);
    panSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    panSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    panSlider->setDoubleClickReturnValue(true, 0.5f);
    panSlider->setLookAndFeel(plaf);
    panSlider->addListener (this);

    hpfSlider->setRange (0, 1, 0);
    hpfSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    hpfSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    hpfSlider->setLookAndFeel(slaf);

    lpfSlider->setRange (0, 1, 0);
    lpfSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lpfSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    lpfSlider->setLookAndFeel(slaf);

    centerAmpSlider->setRange (0, 1, 0);
    centerAmpSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    centerAmpSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    centerAmpSlider->setLookAndFeel(blaf);
    centerAmpSlider->setDoubleClickReturnValue(true, 0.f);


    enableToggle->setButtonText (String());
    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));


    leftSyncToggle->setButtonText (String());
    leftSyncToggle->setImages(false, true, false,
                              ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                              ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                              ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    leftSyncToggle->addListener (this);

    rightSyncToggle->setButtonText (String());
    rightSyncToggle->setImages(false, true, false,
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                               ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    rightSyncToggle->addListener (this);

    centerSyncToggle->setButtonText (String());
    centerSyncToggle->setImages(false, true, false,
                                ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                                ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    centerSyncToggle->addListener (this);


    hpfEnableToggle->setButtonText (String());
    hpfEnableToggle->setImages(false, true, false,
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                               ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));


    lpfEnableToggle->setButtonText (String());
    lpfEnableToggle->setImages(false, true, false,
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                               ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                               ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..
    double bpm = processor.getBeatsPerMinute();

    if(bpm < 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;

    initialiseTimeDivision();
    calculateTimeDivision(beatsPerMinute);


    if(pan != nullptr){
        const double  value = pan->getValue();
        panSlider->setValue(value, dontSendNotification);
    }

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");

    initialiseMidiStrings();
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

    claf = nullptr;
    blaf = nullptr;
    slaf = nullptr;
    plaf = nullptr;
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
                13, 68, 62, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R DELAY"),
                18, 212, 58, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L FB"),
                112, 68, 38, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R FB"),
                112, 212, 38, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("L MIX"),
                196, 68, 40, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("R MIX"),
                194, 212, 46, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PAN"),
                142, 158, 31, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("c delay"),
                13, 140, 62, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("HPF"),
                92, 110, 30, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("LPF"),
                92, 158, 30, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("C FB"),
                140, 110, 35, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("C AMP"),
                190, 140, 47, 10,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LCRDelayComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    leftDelaySlider->setBounds (22, 24, 48, 48);
    rightDelaySlider->setBounds (22, 168, 48, 48);
    rightFBSlider->setBounds (109, 168, 48, 48);
    leftFBSlider->setBounds (109, 24, 48, 48);
    leftMixSlider->setBounds (191, 24, 48, 48);

    centerDelaySlider->setBounds (22, 96, 48, 48);
    centerFBSlider->setBounds (143, 80, 32, 32);
     centerAmpSlider->setBounds (191, 96, 48, 48);
    rightMixSlider->setBounds (191, 168, 48, 48);
    panSlider->setBounds (143, 128, 32, 32);
    hpfSlider->setBounds (91, 80, 32, 32);
    lpfSlider->setBounds (91, 128, 32, 32);


    enableToggle->setBounds (238, 0, 18, 18);
    hpfEnableToggle->setBounds (124, 87, 16, 16);
    lpfEnableToggle->setBounds (124, 136, 16, 16);
    leftSyncToggle->setBounds (68, 39, 16, 16);
    centerSyncToggle->setBounds (68, 111, 16, 16);
    rightSyncToggle->setBounds (68, 183, 16, 16);
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

        labelRef.setText(result, sendNotificationAsync);

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

        if(paramIndices.size() == 18 &&
           processor.getRegisteredMidi(paramIndices[2]) &&
           timeDivision.size() == 24)
        {
            if(leftSyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[2]);
            } else {
                processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[2]);
            }
        }
        //[/UserButtonCode_leftSyncToggle]
    }
    else if (buttonThatWasClicked == centerSyncToggle)
    {
        //[UserButtonCode_centerSyncToggle] -- add your button handler code here..
        updateSliderRange(centerSyncToggle, centerDelaySlider, centerDelay);

        if(paramIndices.size() == 18 &&
           processor.getRegisteredMidi(paramIndices[6]) &&
           timeDivision.size() == 24)
        {
            if(centerSyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[6]);
            } else {
                processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[6]);
            }
        }
        //[/UserButtonCode_centerSyncToggle]
    }
    else if (buttonThatWasClicked == rightSyncToggle)
    {
        //[UserButtonCode_rightSyncToggle] -- add your button handler code here..
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);

        if(paramIndices.size() == 18 &&
           processor.getRegisteredMidi(paramIndices[15]) &&
           timeDivision.size() == 24)
        {
            if(rightSyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[15]);
            } else {
                processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[15]);
            }
        }
        //[/UserButtonCode_rightSyncToggle]
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
    if(bpm < 20.0){
        bpm = 20.0;
    }

    double crotchet = 60000.0/bpm;

    // Calculate 1/128 Note, multiply with 2

    timeDivision.clear();

    for(int i=0; i<division.size(); i++){
        timeDivision.push_back(delay_timing_ratios[i]*crotchet);
    }

    updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
    updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
    updateSliderRange(centerSyncToggle, centerDelaySlider, centerDelay);
}

int LCRDelayComponent::getTimeDivisionIndex(vector<double> values, double time){
    int index = 0;

    while(index < values.size()){
        double tmpValue = values[index];
        double error = tmpValue*0.05;

        if(time >= tmpValue-error &&
           time <= tmpValue+error)
        {
            return index;
        }
        index++;
    }
    return 0;
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
       labelRef.setText(tmpName, sendNotificationAsync);
    } else {
        std::cerr << "StereoDelayComponent::updateSliderValue" << std::endl;
    }
}

void LCRDelayComponent::handleCommandMessage(int commandId){
    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        leftSyncToggle->postCommandMessage(ParamImageToggle::update);
        leftFBSlider->postCommandMessage(ParamSlider::update);
        leftMixSlider->postCommandMessage(ParamSlider::update);
        centerSyncToggle->postCommandMessage(ParamImageToggle::update);
        centerFBSlider->postCommandMessage(ParamSlider::update);
        hpfEnableToggle->postCommandMessage(ParamImageToggle::update);
        hpfSlider->postCommandMessage(ParamSlider::update);
        lpfEnableToggle->postCommandMessage(ParamImageToggle::update);
        lpfSlider->postCommandMessage(ParamSlider::update);
        centerAmpSlider->postCommandMessage(ParamSlider::update);
        rightSyncToggle->postCommandMessage(ParamImageToggle::update);
        rightFBSlider->postCommandMessage(ParamSlider::update);
        rightMixSlider->postCommandMessage(ParamSlider::update);

        if(paramIndices.size() == 18){
            if(processor.getRegisteredMidi(paramIndices[2])){
                if(leftSyncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[2]);
                } else {
                    processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[2]);
                }
            }

            if(processor.getRegisteredMidi(paramIndices[6])){
                if(centerSyncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[6]);
                } else {
                    processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[6]);
                }
            }

            if(processor.getRegisteredMidi(paramIndices[15])){
                if(rightSyncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[15]);
                } else {
                    processor.changeMidiRowMinMax(0, timeDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[15]);
                }
            }
        }

        if(pan != nullptr){
            const double  value = pan->getValue();
            panSlider->setValue(value, sendNotificationAsync);
        }

        updateSliderRange(leftSyncToggle, leftDelaySlider, leftDelay);
        updateSliderRange(rightSyncToggle, rightDelaySlider, rightDelay);
        updateSliderRange(centerSyncToggle, centerDelaySlider, centerDelay);
    } else if(commandId == updateFxBPM){
        double bpm = processor.getBeatsPerMinute();

        if(bpm <= 20.0){
            bpm = 20.0;
        }

        beatsPerMinute = bpm;

        calculateTimeDivision(beatsPerMinute);
    }

    if(paramIndices.size() == 18 &&
       midiStrings.size() == 18 &&
       timeDivision.size() == 24)
    {
        int index = -1;
        int param = -1;
        int handling = -1;
        double minValue = 0.0;
        double maxValue = 1.0;

        if(commandId == requestMenuId[0]){          // Enable
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[1]){   // Left Sync
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[2]){   // Left Delay
            index = menu.show();
            param = 2;

            if(leftSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = timeDivision[23];
                handling = akateko::MIDI_TO_DOUBLE;
            }

        } else if(commandId == requestMenuId[3]){   // Left Feedback
            index = menu.show();
            param = 3;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[4]){   // Left Mix
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[5]){   // Center Sync
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[6]){   // Center Delay
            index = menu.show();
            param = 6;

            if(centerSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = timeDivision[23];
                handling = akateko::MIDI_TO_DOUBLE;
            }

        } else if(commandId == requestMenuId[7]){   // Center Feedback
            index = menu.show();
            param = 7;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[8]){   // HPF Enable
            index = menu.show();
            param = 8;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[9]){   // HPF Frequency
            index = menu.show();
            param = 9;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[10]){  // LPF Enable
            index = menu.show();
            param = 10;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[11]){  // LPF Frequency
            index = menu.show();
            param = 11;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[12]){  // Center Amplitude
            index = menu.show();
            param = 12;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[13]){  // Center Pan
            index = menu.show();
            param = 13;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[14]){  // Right Sync
            index = menu.show();
            param = 14;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuId[15]){  // Right Delay
            index = menu.show();
            param = 15;

            if(rightSyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = timeDivision[23];
                handling = akateko::MIDI_TO_DOUBLE;
            }
        } else if(commandId == requestMenuId[16]){  // Right Feedback
            index = menu.show();
            param = 16;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuId[17]){  // Right Mix
            index = menu.show();
            param = 17;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling ,midiStrings[param], 12);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }

        repaint();
    }
}

void LCRDelayComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" LCR-Delay: Enable");
    midiStrings.add(" LCR-Delay: L-Sync");
    midiStrings.add(" LCR-Delay: L-Delay");
    midiStrings.add(" LCR-Delay: L-FB");
    midiStrings.add(" LCR-Delay: L-Mix");
    midiStrings.add(" LCR-Delay: C-Sync");
    midiStrings.add(" LCR-Delay: C-Delay");
    midiStrings.add(" LCR-Delay: C-FB");
    midiStrings.add(" LCR-Delay: HPF En");
    midiStrings.add(" LCR-Delay: HPF Freq");
    midiStrings.add(" LCR-Delay: LFP En");
    midiStrings.add(" LCR-Delay: LPF Freq");
    midiStrings.add(" LCR-Delay: C-Amp");
    midiStrings.add(" LCR-Delay: C-Pan");
    midiStrings.add(" LCR-Delay: R-Sync");
    midiStrings.add(" LCR-Delay: R-Delay");
    midiStrings.add(" LCR-Delay: R-FB");
    midiStrings.add(" LCR-Delay: R-Mix");
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
    <TEXT pos="13 68 62 10" fill="solid: ffaaaaaa" hasStroke="0" text="L DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="18 212 58 10" fill="solid: ffaaaaaa" hasStroke="0" text="R DELAY"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="112 68 38 10" fill="solid: ffaaaaaa" hasStroke="0" text="L FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="112 212 38 10" fill="solid: ffaaaaaa" hasStroke="0" text="R FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="196 68 40 10" fill="solid: ffaaaaaa" hasStroke="0" text="L MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="194 212 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="R MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="142 158 31 10" fill="solid: ffaaaaaa" hasStroke="0" text="PAN"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="13 140 62 10" fill="solid: ffaaaaaa" hasStroke="0" text="c delay"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="92 110 30 10" fill="solid: ffaaaaaa" hasStroke="0" text="HPF"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="92 158 30 10" fill="solid: ffaaaaaa" hasStroke="0" text="LPF"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="140 110 35 10" fill="solid: ffaaaaaa" hasStroke="0" text="C FB"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="190 140 47 10" fill="solid: ffaaaaaa" hasStroke="0" text="C AMP"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="leftDelaySlider" id="b9d7855b70230b1a" memberName="leftDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="22 24 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightDelaySlider" id="9d4bd59bbad98e0c" memberName="rightDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="22 168 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="rightFBSlider" id="b8d6f8c56f23720b" memberName="rightFBSlider"
          virtualName="" explicitFocusOrder="0" pos="108 168 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="leftFBSlider" id="f5c510b0381ad3a3" memberName="leftFBSlider"
          virtualName="" explicitFocusOrder="0" pos="108 24 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="leftMixSlider" id="cf347ac5b7359227" memberName="leftMixSlider"
          virtualName="" explicitFocusOrder="0" pos="191 24 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <IMAGEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
               virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="leftSyncToggle" id="74bc5579149da4c2" memberName="leftSyncToggle"
               virtualName="" explicitFocusOrder="0" pos="68 35 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="rightSyncToggle" id="73efd299c17c4595" memberName="rightSyncToggle"
               virtualName="" explicitFocusOrder="0" pos="68 179 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="centerDelaySlider" id="ea81821a599bf966" memberName="centerDelaySlider"
          virtualName="" explicitFocusOrder="0" pos="22 96 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <IMAGEBUTTON name="centerSyncToggle" id="bde6573e5cb3d0d6" memberName="centerSyncToggle"
               virtualName="" explicitFocusOrder="0" pos="68 107 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="centerFBSlider" id="b6c5b8606828dd51" memberName="centerFBSlider"
          virtualName="" explicitFocusOrder="0" pos="142 80 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="rightMixSlider" id="bb4e6f99eb62c6c0" memberName="rightMixSlider"
          virtualName="" explicitFocusOrder="0" pos="191 168 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="panSlider" id="6d4008ca720020ae" memberName="panSlider"
          virtualName="" explicitFocusOrder="0" pos="142 128 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="hpfSlider" id="6b2d628874595a2b" memberName="hpfSlider"
          virtualName="" explicitFocusOrder="0" pos="91 80 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="lpfSlider" id="afa2da54fe6847f4" memberName="lpfSlider"
          virtualName="" explicitFocusOrder="0" pos="91 128 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <IMAGEBUTTON name="hpfEnableToggle" id="8900b8258c961b32" memberName="hpfEnableToggle"
               virtualName="" explicitFocusOrder="0" pos="122 84 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="lpfEnableToggle" id="3b3db52817521d52" memberName="lpfEnableToggle"
               virtualName="" explicitFocusOrder="0" pos="122 132 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="centerAmpSlider" id="4fd9e3f8d71dc44e" memberName="centerAmpSlider"
          virtualName="" explicitFocusOrder="0" pos="191 96 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
