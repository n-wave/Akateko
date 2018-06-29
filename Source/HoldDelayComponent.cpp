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

#include "HoldDelayComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <iostream>
#include "Akateko.h"

using std::vector;
using akateko::updateFxGUI;
using akateko::updateFxBPM;

using akateko::IgnoreRightClick;
using akateko::MidiRow;
using akateko::initMidiRow;

using akateko::h_delay_timing_ratios;
using akateko::h_delay_latch_ratios;
/*
AudioParameterBool *hDelayEnable;
AudioParameterBool *hDelayTrigger;
AudioParameterBool *hDelaySync;
AudioParameterFloat *hDelayLoop;
AudioParameterFloat *hDelaySpeed;
AudioParameterFloat *hDelayDirection;
AudioParameterFloat *hDelayPan;
AudioParameterFloat *hDelayMix;
*/
//[/MiscUserDefs]

//==============================================================================
HoldDelayComponent::HoldDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      processor(p),
      labelRef(label),
      triggerClicked(false), activeColour(Colour(0x7f007f7f))
{
    //[Constructor_pre] You can add your own custom stuff here..
    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::hDelayId);
    StringArray paramIds = p.getParameterIds(AkatekoAudioProcessor::hDelayId);
    const OwnedArray<AudioProcessorParameter> &params = p.getParameters();

    const int paramSize = params.size();

    if(paramIndices.size() ==  15 &&
       paramIds.size() == 15 &&
       paramSize >= paramIndices[14])
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
        requestMenuIds[9] = paramIds[9].hash();
        requestMenuIds[10] = paramIds[10].hash();
        requestMenuIds[11] = paramIds[11].hash();
        requestMenuIds[12] = paramIds[12].hash();
        requestMenuIds[13] = paramIds[13].hash();
        requestMenuIds[14] = paramIds[14].hash();

        initialiseMidiStrings();

        addAndMakeVisible(enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), label));

        addAndMakeVisible(triggerButton = new IgnoreRightClick<ImageButton>(paramIds[1]));
        trigger = params.getUnchecked(paramIndices[1]);

        addAndMakeVisible(syncToggle = new IgnoreRightClick<ImageButton>(paramIds[2]));
        sync = params.getUnchecked(paramIndices[2]);

        addAndMakeVisible(loopSlider = new IgnoreRightClick<Slider>(paramIds[3]));
        loop = params.getUnchecked(paramIndices[3]);

        addAndMakeVisible(speedSlider = new IgnoreRightClick<Slider>(paramIds[4]));
        speed = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible(glideToggle = new IgnoreRightClick<ImageButton>(paramIds[5]));
        smooth = params.getUnchecked(paramIndices[5]);

        addAndMakeVisible(glideSlider = new IgnoreRightClick<Slider>(paramIds[6]));
        glide = params.getUnchecked(paramIndices[6]);

        addAndMakeVisible(directionSlider = new IgnoreRightClick<Slider>(paramIds[7]));
        direction = params.getUnchecked(paramIndices[7]);

        addAndMakeVisible(gapPositionToggle = new IgnoreRightClick<ImageButton>(paramIds[8]));
        gap = params.getUnchecked(paramIndices[8]);

        addAndMakeVisible(lengthSlider = new IgnoreRightClick<Slider>(paramIds[9]));
        length = params.getUnchecked(paramIndices[9]);

        addAndMakeVisible(fadeSlider = new IgnoreRightClick<Slider>(paramIds[10]));
        fade = params.getUnchecked(paramIndices[10]);

        addAndMakeVisible(panSlider = new ParamSlider(paramIds[11], *params.getUnchecked(paramIndices[11]), label));
        addAndMakeVisible(mixSlider = new ParamSlider(paramIds[12], *params.getUnchecked(paramIndices[12]), label));

        addAndMakeVisible(latchToggle = new IgnoreRightClick<ImageButton>(paramIds[13]));
        latchEnable = params.getUnchecked(paramIndices[13]);

        addAndMakeVisible(latchSlider = new IgnoreRightClick<Slider>(paramIds[14]));
        latch = params.getUnchecked(paramIndices[14]);

    } else {
        std::cerr << "HoldDelayComponent::HoldDelayComponent" << std::endl;
        std::cerr << "Parameters are not bound properly" << std::endl;

        addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));
        addAndMakeVisible (triggerButton = new ImageButton("trigger"));
        addAndMakeVisible (syncToggle = new ImageButton ("syncToggle"));
        addAndMakeVisible (loopSlider = new Slider ("loopSlider"));
        addAndMakeVisible (speedSlider = new Slider ("speedSlider"));
        addAndMakeVisible (glideToggle = new ImageButton ("glideToggle"));
        addAndMakeVisible (glideSlider = new Slider ("glideSlider"));
        addAndMakeVisible (directionSlider = new Slider ("directionSlider"));
        addAndMakeVisible (gapPositionToggle = new ImageButton ("gapPositionToggle"));
        addAndMakeVisible (lengthSlider = new Slider ("lengthSlider"));
        addAndMakeVisible (fadeSlider = new Slider ("fadeSlider"));
        addAndMakeVisible (panSlider = new Slider ("panSlider"));
        addAndMakeVisible (mixSlider = new Slider ("mixSlider"));

        addAndMakeVisible (latchToggle = new ImageButton ("latchEnable"));
        addAndMakeVisible (latchSlider = new Slider("latchSlider"));

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

    //[UserPreSize]maxPeriod

    loopSlider->setRange (0, 1, 0);
    loopSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    loopSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    loopSlider->addListener (this);


    speedSlider->setRange (0.25, 4.0, 0);
    speedSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    speedSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    speedSlider->setSkewFactorFromMidPoint(1.0);
    speedSlider->setDoubleClickReturnValue(true, 1.0f);
    speedSlider->addListener(this);

    directionSlider->setRange (0, 1, 0.5);
    directionSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    directionSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    directionSlider->setMouseDragSensitivity(50);
    directionSlider->addListener (this);

    mixSlider->setRange (0, 1, 0);
    mixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);


    panSlider->setRange (0, 1, 0);
    panSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    panSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    panSlider->setDoubleClickReturnValue(true, 0.5f);

    lengthSlider->setRange (0.10, 1, 0);
    lengthSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lengthSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    lengthSlider->addListener (this);

    glideSlider->setRange (1, 32, 1);
    glideSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    glideSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    glideSlider->addListener (this);

    fadeSlider->setRange (0, 1, 0);
    fadeSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    fadeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fadeSlider->addListener (this);

    latchSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    latchSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    latchSlider->setRange(0, 14, 1);
    latchSlider->addListener (this);

    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    triggerButton->setImages(false, true, false,
                       ImageCache::getFromMemory (BinaryData::HoldOff_png, BinaryData::HoldOff_pngSize), 1.0f, Colour(0x00000000),
                       ImageCache::getFromMemory (BinaryData::HoldOff_png, BinaryData::HoldOff_pngSize), 1.0f, Colour (0x2F20DFBF),
                       ImageCache::getFromMemory (BinaryData::HoldOn_png, BinaryData::HoldOn_pngSize), 1.0f, Colour (0x3F40DFBF));
    triggerButton->addListener(this);

    syncToggle->setClickingTogglesState(true);
    syncToggle->setImages(false, true, false,
                          ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                          ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                          ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    syncToggle->addListener (this);


    glideToggle->setClickingTogglesState(true);
    glideToggle->setImages(false, true, false,
                           ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                           ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                           ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    glideToggle->addListener (this);

    gapPositionToggle->setClickingTogglesState(true);
    gapPositionToggle->setImages(false, true, false,
                                 ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                                 ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                 ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    gapPositionToggle->addListener (this);

    latchToggle->setClickingTogglesState(true);
    latchToggle->setImages(false, true, false,
                                 ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x6F000000),
                                 ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                 ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));
    latchToggle->addListener (this);



    triggerClicked = false;
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
    setGlideSliderRange(beatsPerMinute);

    if(sync != nullptr){
        const bool tmpSync = sync->getValue();
        syncToggle->setToggleState(tmpSync, dontSendNotification);
    }

    if(speed != nullptr){
        const double tmpSpeed = speed->getValue();
        speedSlider->setValue(tmpSpeed, dontSendNotification);
    }

    if(direction != nullptr){
        const double tmpDir = direction->getValue()*0.5;
        directionSlider->setValue(tmpDir, dontSendNotification);
    }

    if(fade != nullptr){
        const double tmpFade = fade->getValue();
        fadeSlider->setValue(tmpFade, dontSendNotification);
    }

    if(gap != nullptr){
        const bool tmpGap = gap->getValue();
        gapPositionToggle->setToggleState(tmpGap, dontSendNotification);
    }

    if(smooth != nullptr){
        const bool tmpSmooth = smooth->getValue();
        glideToggle->setToggleState(tmpSmooth, dontSendNotification);
    }

    if(glide != nullptr){
        const double tmpGlide = glide->getValue();

        glideSlider->setValue(tmpGlide);
    }

    if(fade != nullptr){
        const double tmpFade = fade->getValue();
        fadeSlider->setValue(tmpFade, dontSendNotification);
    }

    if(length != nullptr){
        const double tmpLength = length->getValue();
        lengthSlider->setValue(tmpLength, dontSendNotification);
    }

    if(latchEnable != nullptr){
        const bool tmpLatch = latchEnable->getValue();
        latchToggle->setToggleState(tmpLatch, dontSendNotification);
    }

    claf = new CustomLookAndFeel();
    blaf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize));
    dlaf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::Direction_png, BinaryData::Direction_pngSize));
    pblaf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::PanBig_png, BinaryData::PanBig_pngSize));

    menu.setLookAndFeel(claf);

    loopSlider->setLookAndFeel(blaf);
    speedSlider->setLookAndFeel(blaf);
    directionSlider->setLookAndFeel(dlaf);
    mixSlider->setLookAndFeel(blaf);
    panSlider->setLookAndFeel(pblaf);
    lengthSlider->setLookAndFeel(blaf);
    glideSlider->setLookAndFeel(blaf);
    fadeSlider->setLookAndFeel(blaf);
    latchSlider->setLookAndFeel(blaf);

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
    //[/Constructor]
}

HoldDelayComponent::~HoldDelayComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    loopSlider = nullptr;
    speedSlider = nullptr;
    directionSlider = nullptr;
    enableToggle = nullptr;
    mixSlider = nullptr;
    syncToggle = nullptr;
    panSlider = nullptr;
    lengthSlider = nullptr;
    glideSlider = nullptr;
    fadeSlider = nullptr;
    glideToggle = nullptr;
    gapPositionToggle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    triggerButton = nullptr;

    sync = nullptr;
    trigger = nullptr;
    loop = nullptr;
    speed = nullptr;
    direction = nullptr;

    gap = nullptr;
    smooth = nullptr;
    fade = nullptr;
    glide = nullptr;
    length = nullptr;

    claf = nullptr;
    blaf = nullptr;
    pblaf = nullptr;
    dlaf = nullptr;

    latchEnable = nullptr;
    latch = nullptr;

    latchSlider = nullptr;
    latchToggle = nullptr;
    //[/Destructor]
}

//==============================================================================
void HoldDelayComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("MIX"),
                199, 80, 32, 12,
                Justification::centred, true);

    g.drawText (TRANS("PAN"),
                199, 147, 32, 12,
                Justification::centred, true);

    g.drawText (TRANS("LATCH"),
                192, 210, 48, 12,
                Justification::centred, true);

    g.drawText (TRANS("Loop"),
                21, 112, 42, 10,
                Justification::centred, true);

    g.drawText (TRANS("DIR"),
                72, 80, 46, 10,
                Justification::centred, true);

    g.drawText (TRANS("SPEED"),
                14, 180, 46, 12,
                Justification::centred, true);

    g.drawText (TRANS("LENGTH"),
                68, 210, 56, 12,
                Justification::centred, true);

    g.drawText (TRANS("FADE"),
                130, 174, 43, 12,
                Justification::centred, true);

    g.drawText (TRANS("GLIDE"),
                128, 110, 46, 10,
                Justification::centred, true);

    g.setGradientFill (ColourGradient (Colour (0xff9a9696),
                                       96.0f, 96.0f,
                                       Colour (0xf01f1f1f),
                                       96.0f, 112.0f,
                                       true));



    //[UserPaint] Add your own custom painting code here..

    //[/UserPaint]
}

void HoldDelayComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    loopSlider->setBounds (16, 66, 48, 48);
    speedSlider->setBounds (16, 130, 48, 48);

    directionSlider->setBounds (75, 36, 42, 42);

    mixSlider->setBounds (192, 34, 48, 48);
    panSlider->setBounds (192, 99, 48, 48);
    latchSlider->setBounds(192, 162, 48, 48);

    lengthSlider->setBounds (72, 162, 48, 48);
    glideSlider->setBounds (129, 66, 48, 48);
    fadeSlider->setBounds (129, 130, 48, 48);

    enableToggle->setBounds (238, 0, 18, 18);
    syncToggle->setBounds (61, 96, 16, 16);
    glideToggle->setBounds (113, 96, 16, 16);
    gapPositionToggle->setBounds (87, 144, 16, 16);

    latchToggle->setBounds(239, 178, 16, 16);


//136

    //[UserResized] Add your own custom resize handling here..
    triggerButton->setBounds(73, 100, 44, 44);
    //[/UserResized]
}

void HoldDelayComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == loopSlider)
    {
        //[UserSliderCode_loopSlider] -- add your slider handling code here..
        String tmpName = loopSlider->getName();
        const bool tmpSync = syncToggle->getToggleState();

        if(tmpSync){
            int tmpVal = loopSlider->getValue();

            if(tmpVal < timeDivision.size() &&
               tmpVal < division.size())
            {
                double tmpRate = timeDivision[tmpVal];
                tmpName += division[tmpVal];

                loop->setValue(tmpRate);
            }
        } else {
            double tmpVal = loopSlider->getValue();

            loop->setValue(tmpVal);
            tmpName += String(tmpVal, 2);
        }

        labelRef.setText(tmpName, dontSendNotification);

        //[/UserSliderCode_loopSlider]
    }
    else if (sliderThatWasMoved == directionSlider)
    {
        //[UserSliderCode_directionSlider] -- add your slider handling code here..
        int tmpDirection = directionSlider->getValue()*2.0;
        String tmpName = directionSlider->getName();

        switch(tmpDirection){
            case 0:
                tmpName += String("forward");
                break;
            case 1:
                tmpName += String("reverse");
                break;
            case 2:
                tmpName += String("back&forth");
                break;
        }

        if(direction != nullptr){
            direction->setValue(tmpDirection);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_directionSlider]
    }
    else if (sliderThatWasMoved == lengthSlider)
    {
        //[UserSliderCode_lengthSlider] -- add your slider handling code here..
        const double tmpValue = lengthSlider->getValue();
        const String tmpName = lengthSlider->getName() +
                               String(tmpValue*100.0, 2) +
                               String("%");

        if(length != nullptr){
            length->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_lengthSlider]
    }
    else if (sliderThatWasMoved == glideSlider)
    {
        //[UserSliderCode_glideSlider] -- add your slider handling code here..
        const int tmpValue = glideSlider->getValue();
        const bool tmpState = glideToggle->getToggleState();

        String tmpName = glideSlider->getName() + String(tmpValue);

        if(tmpState){
            tmpName += String(" ms");
        } else{
            tmpName += String(" reps");
        }

        if(glide != nullptr){
            glide->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_glideSlider]
    }
    else if (sliderThatWasMoved == fadeSlider)
    {
        //[UserSliderCode_fadeSlider] -- add your slider handling code here..
        const double tmpValue = fadeSlider->getValue();
        const String tmpName = fadeSlider->getName() +
                               String(tmpValue*50.0, 2) +
                               String("%");

        if(fade != nullptr){
            fade->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);

        //[/UserSliderCode_fadeSlider]
    }

    //[UsersliderValueChanged_Post]
    else if (sliderThatWasMoved == speedSlider){
        double tmpValue = speedSlider->getValue();
        String tmpName = speedSlider->getName() + String(tmpValue, 2);

        if(speed != nullptr){
            speed->setValue(tmpValue);
        }

        labelRef.setText(tmpName, dontSendNotification);
    } else if(sliderThatWasMoved == latchSlider){
        if(latch != nullptr &&
           latchEnable != nullptr)
        {
            const double tmpIndex = latchSlider->getValue();

            if(tmpIndex <= latchDivision.size() &&
               tmpIndex <= latchValues.size())
            {
                const double tmpValue = latchValues[tmpIndex];

                latch->setValue(tmpValue);
                const String tmpMessage = latchSlider->getName() +
                                          String(latchDivision[tmpIndex]);

                labelRef.setText(tmpMessage, dontSendNotification);
            }
        }
    }
    //[/UsersliderValueChanged_Post]
}

void HoldDelayComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == syncToggle)
    {
        //[UserButtonCode_syncToggle] -- add your button handler code here..
        if(sync != nullptr && loop != nullptr){
            const int tmpState = syncToggle->getToggleState();
            double tmpLoop = loop->getValue();

            String tmpName = loopSlider->getName();
            sync->setValue(tmpState);

            if(tmpState){
                int tmpIndex = findClosestTimeDivision(timeDivision, tmpLoop);
                loopSlider->setRange(0, timeDivision.size()-1);
                loopSlider->setValue(tmpIndex, sendNotificationAsync);

                if(paramIndices.size() == 15 &&
                   timeDivision.size() == 18 &&
                   processor.getRegisteredMidi(paramIndices[3]))
                {
                    processor.changeMidiRowMinMax(0, 17, akateko::MIDI_TO_INT, paramIndices[3]);
                }

                // Convert Period to Frequency
                loop->setValue(timeDivision[tmpIndex]);
                tmpName += division[tmpIndex];
            } else {
                const double minPeriod = 1.0; //1ms 480 samples should be close enought
                const double maxPeriod = timeDivision[timeDivision.size()-1];

                if(tmpLoop > maxPeriod){
                    tmpLoop = maxPeriod;
                } else if(tmpLoop < minPeriod){
                    tmpLoop = minPeriod;
                }

                if(paramIndices.size() == 15 &&
                   timeDivision.size() == 18 &&
                   processor.getRegisteredMidi(paramIndices[3]))
                {
                    processor.changeMidiRowMinMax(minPeriod, maxPeriod, akateko::MIDI_TO_DOUBLE, paramIndices[3]);
                }

                loopSlider->setRange(minPeriod, maxPeriod, 0);
                loopSlider->setValue(tmpLoop, sendNotificationAsync);
                loop->setValue(tmpLoop);

                tmpName += String(tmpLoop, 2);
            }

            labelRef.setText(tmpName, sendNotificationAsync);
        } else {
            std::cerr << "Parameters are not bound properly" << std::endl;
        }
        //[/UserButtonCode_syncToggle]
    }
    else if (buttonThatWasClicked == glideToggle)
    {
        //[UserButtonCode_glideToggle] -- add your button handler code here..
        const bool tmpGlide = glideToggle->getToggleState();
        String tmpName = glideToggle->getName();

        if(tmpGlide){
            tmpName += String("Smooth");
        } else {
            tmpName += String("Incremental");
        }

        if(smooth != nullptr){
            smooth->setValue(tmpGlide);
        }

        setGlideSliderRange(beatsPerMinute);

        if(processor.getRegisteredMidi(paramIndices[6])){
            if(!glideToggle->getToggleState()){
                processor.changeMidiRowMinMax(1, 32, akateko::MIDI_TO_INT, paramIndices[6]);
            } else {
                processor.changeMidiRowMinMax(1.0, glideMaxRange, akateko::MIDI_TO_DOUBLE, paramIndices[6]);
            }
        }

        labelRef.setText(tmpName, sendNotificationAsync);
        //[/UserButtonCode_glideToggle]
    }
    else if (buttonThatWasClicked == gapPositionToggle)
    {
        //[UserButtonCode_gapPositionToggle] -- add your button handler code here..
        const bool tmpGap = gapPositionToggle->getToggleState();
        String tmpName = gapPositionToggle->getName();

        if(tmpGap){
            tmpName += String("Front");
        } else {
            tmpName += String("Back");
        }

        if(gap != nullptr){
            gap->setValue(tmpGap);
        }

        labelRef.setText(tmpName, sendNotificationAsync);
        //[/UserButtonCode_gapPositionToggle]
    }

    //[UserbuttonClicked_Post]

    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void HoldDelayComponent::setBPM(double bpm){
    beatsPerMinute = bpm;

    calculateTimeDivision(beatsPerMinute);
    setGlideSliderRange(beatsPerMinute);
}


void HoldDelayComponent::initialiseTimeDivision(){
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

    for(int i=0; i<6; i++){
        String dot = noteVals[i] + String("D");
        String triplet = noteVals[i] + String("T");

        division.add(triplet);
        division.add(noteVals[i]);
        division.add(dot);
    }

    //Build latch Time Division
    latchDivision.clear();

    for(int i=3; i<noteVals.size(); i++){
        String dot = noteVals[i] + String("D");
        String triplet = noteVals[i] + String("T");

        latchDivision.add(triplet);
        latchDivision.add(noteVals[i]);
        latchDivision.add(dot);
    }
}

void HoldDelayComponent::calculateTimeDivision(double bpm){
    if(bpm >= 20.0){
        beatsPerMinute = bpm;
        double crotchet  = 60000.0/bpm;
        glideMaxRange = crotchet*4.0;

        //Calculate 2dot note
        timeDivision.clear();

        for(int i=0; i<division.size(); i++){
            timeDivision.push_back(crotchet*h_delay_timing_ratios[i]);
        }

        //Calculate Latch Division
        latchValues.clear();

        for(int i=0; i<latchDivision.size(); i++){
            latchValues.push_back(h_delay_latch_ratios[i]);
        }
    }

    initLoopSlider();
    initLatchSlider();
}


/* Use this function while switching from
 * "Not quantised delay times" to quantised
 * delay times. On startup use the getTimeIndex.
 */

int HoldDelayComponent::findClosestTimeDivision(vector<double> &values, double time){
    int result = 0;
    int nrOfSteps = values.size();

    if(time != 0 && nrOfSteps != 0){
        bool run = true;
        int index = 1;

        while(run && index < nrOfSteps-4){
            double prev = values[index-1]; // Nt
            double val = values[index];    // N
            double next = values[index+1]; // ND

            if(time == val){
                result = index;
                run = false;
            }else if(time >= prev && time < val){
                result = index-1;
                run = false;
            }else if(time > val && time <= next){
                result = index+1;
                run = false;
            }
            index += 3;
        }
    }
    return result;
}

int HoldDelayComponent::getTimeDivisionIndex(vector<double> &values, double time){
    int index = 0;

    while(index < values.size()){
        double tmpValue = values[index];
        double error = tmpValue*0.02;

        if(time >= tmpValue-error &&
           time <= tmpValue+error)
        {
            return index;
        }
        index++;
    }
    return 0;
}

void HoldDelayComponent::initLoopSlider(){
    if(syncToggle != nullptr &&
       loop != nullptr &&
       timeDivision.size() != 0 &&
       division.size() != 0)
    {
        const bool tmpSync = sync->getValue();
        double tmpRate = loop->getValue();

        if(tmpSync){
            int tmpIndex = getTimeDivisionIndex(timeDivision, tmpRate);

            loopSlider->setRange(0, timeDivision.size()-1, 1);
            loopSlider->setValue(tmpIndex, sendNotificationAsync);
        } else {
            loopSlider->setRange(1.0,  timeDivision[timeDivision.size()-1], 0);
            loopSlider->setValue(tmpRate, sendNotificationAsync);
        }
    } else {
        if(sync == nullptr || loop == nullptr){
            std::cerr << "Parameter not bounded properly : " << getName() << std::endl;
        }

        if(division.size() == 0 || timeDivision.size() == 0){
            std::cerr << "Time division tables not set properly" << std::endl;
        }
    }
}

void HoldDelayComponent::initLatchSlider(){
    if(latch != nullptr &&
       latchDivision.size() != 0 &&
       latchValues.size() != 0)
    {
        const double tmpLatch = latch->getValue();
        const int tmpIndex = getTimeDivisionIndex(latchValues, tmpLatch);
        latchSlider->setValue(tmpIndex, sendNotificationAsync);
        //Find closes indeex for latch division
    } else {
        if(latch == nullptr){
            std::cerr << "Parameter not bounded properly : " << latchSlider->getName() << std::endl;
        } else if(latchDivision.size() == 0 || latchValues.size() == 0){
            std::cerr << "Latch Division not set properly" << std::endl;
        }
    }
}

void HoldDelayComponent::setGlideSliderRange(double bpm){
    if(smooth != nullptr &&
       glide != nullptr)
    {
        const bool tmpSmooth = smooth->getValue();
        double tmpGlide = glide->getValue();

        if(tmpSmooth){
            glideSlider->setRange(10.0, glideMaxRange);

            if(tmpGlide < 10.0){
                tmpGlide = 10.0;
            } else if(tmpGlide > glideMaxRange){
                tmpGlide = glideMaxRange;
            }

            glide->setValue(tmpGlide);
        } else {
            glideSlider->setRange(1, 32, 1);

            if(tmpGlide < 1){
                tmpGlide = 1;
            } else if(tmpGlide > 32){
                tmpGlide = 32;
            }

            glide->setValue(tmpGlide);
        }
    }
}

void HoldDelayComponent::handleCommandMessage(int commandId){

    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        panSlider->postCommandMessage(ParamSlider::update);
        mixSlider->postCommandMessage(ParamSlider::update);

        initLoopSlider();
        initLatchSlider();

        if(trigger != nullptr){
            const bool tmpTrigger = trigger->getValue();

            if(tmpTrigger){
                triggerButton->setState(ImageButton::buttonDown);
            } else {
                triggerButton->setState(ImageButton::buttonNormal);
            }
        }

        if(sync != nullptr){
            const bool tmpSync = sync->getValue();
            syncToggle->setToggleState(tmpSync, dontSendNotification);
        }

        setGlideSliderRange(beatsPerMinute);


        if(speed != nullptr){
            const double tmpSpeed = speed->getValue();
            speedSlider->setValue(tmpSpeed, dontSendNotification);
        }

        if(direction != nullptr){
            const double tmpDir = direction->getValue()*0.5;
            directionSlider->setValue(tmpDir, dontSendNotification);
        }

        if(fade != nullptr){
            const double tmpFade = fade->getValue();
            fadeSlider->setValue(tmpFade, dontSendNotification);
        }

        if(gap != nullptr){
            const bool tmpGap = gap->getValue();
            gapPositionToggle->setToggleState(tmpGap, dontSendNotification);
        }

        if(smooth != nullptr){
            const bool tmpSmooth = smooth->getValue();
            glideToggle->setToggleState(tmpSmooth, dontSendNotification);
        }

        if(glide != nullptr){
            const double tmpGlide = glide->getValue();

            glideSlider->setValue(tmpGlide);
        }

        if(fade != nullptr){
            const double tmpFade = fade->getValue();
            fadeSlider->setValue(tmpFade, dontSendNotification);
        }

        if(length != nullptr){
            const double tmpLength = length->getValue();
            lengthSlider->setValue(tmpLength, dontSendNotification);
        }

        if(latchEnable != nullptr){
            const bool tmpLatchEnabled = latchEnable->getValue();
            latchToggle->setToggleState(tmpLatchEnabled, dontSendNotification);
        }

        if(paramIndices.size() == 15){
            if(processor.getRegisteredMidi(paramIndices[3])){
                if(syncToggle->getToggleState()){
                    processor.changeMidiRowMinMax(0, 17, akateko::MIDI_TO_INT, paramIndices[3]);
                } else {
                    processor.changeMidiRowMinMax(1.0, timeDivision[17], akateko::MIDI_TO_DOUBLE, paramIndices[3]);
                }
            }

            if(processor.getRegisteredMidi(paramIndices[6])){
                if(!glideToggle->getToggleState()){
                    processor.changeMidiRowMinMax(1, 32, akateko::MIDI_TO_INT, paramIndices[6]);
                } else {
                    processor.changeMidiRowMinMax(1.0, glideMaxRange, akateko::MIDI_TO_DOUBLE, paramIndices[6]);
                }
            }

        }

    } else if(commandId == updateFxBPM){
        double bpm = processor.getBeatsPerMinute();

        if(bpm < 20.0){
            bpm = 20.0;
        }

        calculateTimeDivision(bpm);
    }

    if(paramIndices.size() == 15 &&
       midiStrings.size() == 15 &&
       timeDivision.size() == 18)
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
        } else if(commandId == requestMenuIds[1]){  // Trigger
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_INT_BUTTON;
        } else if(commandId == requestMenuIds[2]){  // Quantise
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[3]){  // Loop flexible
            index = menu.show();
            param = 3;

            if(syncToggle->getToggleState()){
                maxValue = 17;
                handling = akateko::MIDI_TO_INT;
            } else {
                minValue = 1.0;
                maxValue = timeDivision[17];
                handling = akateko::MIDI_TO_DOUBLE;
            }

        } else if(commandId == requestMenuIds[4]){  // Speed
            index = menu.show();
            param = 4;
            minValue = 0.25;
            maxValue = 4.0;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[5]){ // Glide Option
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[6]){  // Glide Flexible
            index = menu.show();
            param = 6;

            if(glideToggle->getToggleState()){
                minValue = 10.0;
                maxValue = glideMaxRange;
                handling = akateko::MIDI_TO_DOUBLE;
            } else {
                minValue = 1;
                maxValue = 32;
                handling = akateko::MIDI_TO_INT;
            }
        } else if(commandId == requestMenuIds[7]){  // Direction
            index = menu.show();
            param = 7;
            maxValue = 2;
            handling = akateko::MIDI_TO_INT;
        } else if(commandId == requestMenuIds[8]){  // Gap Direction
            index = menu.show();
            param = 8;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[9]){  // Length Gap
            index = menu.show();
            param = 9;
            minValue = 0.10;
            maxValue = 1.0;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[10]){ // Fade
            index = menu.show();
            param = 10;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[11]){ // Pan
            index = menu.show();
            param = 11;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[12]){ // Mix
            index = menu.show();
            param = 12;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[13]){ // Latch Enabled
            index = menu.show();
            param = 13;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[14]){ //latch
            index = menu.show();
            param = 14;
            maxValue = 14;
            handling = akateko::MIDI_TO_INT;
        }

        if(index == 1){
            MidiRow tmpRow;
            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling, midiStrings[param], 15);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }
}

void HoldDelayComponent::buttonStateChanged(Button *button){
    if(button == triggerButton){
        const int state = button->getState();
        String tmpName = button->getName();

        if(state == ImageButton::buttonDown){
            triggerClicked = true;
            trigger->setValue(1);
            tmpName += String("On");
            labelRef.setText(tmpName, sendNotificationAsync);
        }else if(triggerClicked && state != ImageButton::buttonDown){
            triggerClicked = false;
            trigger->setValue(0);
            tmpName += String("Off");
            labelRef.setText(tmpName, sendNotificationAsync);
        }
    }
}

void HoldDelayComponent::initialiseMidiStrings(){
    std::cout << "initialiseMidiStrings()" << std::endl;

    midiStrings.clear();

    midiStrings.add(" H-Delay: Enable");
    midiStrings.add(" H-Delay: Trigger");
    midiStrings.add(" H-Delay: Quantise");
    midiStrings.add(" H-Delay: Loop");
    midiStrings.add(" H-Delay: Speed");
    midiStrings.add(" H-Delay: Glide opt");
    midiStrings.add(" H-Delay: Glide");
    midiStrings.add(" H-Delay: Dir");
    midiStrings.add(" H-Delay: Gap Dir");
    midiStrings.add(" H-Delay: Length");
    midiStrings.add(" H-Delay: Fade");
    midiStrings.add(" H-Delay: Pan");
    midiStrings.add(" H-Delay: Mix");
    midiStrings.add(" H-Delay: Latch En");
    midiStrings.add( " H-Delay: Latch");
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="HoldDelayComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;labelRef(label)&#10;trigger(nullptr)&#10;loop(nullptr)&#10;direction(nullptr)&#10;triggerClicked(false), activeColour(Colour(0x7f007f7f))"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="232">
  <METHODS>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="200 181 32 12" fill="solid: ffaaaaaa" hasStroke="0" text="PAN"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="200 85 32 12" fill="solid: ffaaaaaa" hasStroke="0" text="MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="21 102 42 10" fill="solid: ffaaaaaa" hasStroke="0" text="Loop"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="72 68 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="speed"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="24 164 27 12" fill="solid: ffaaaaaa" hasStroke="0" text="DIR"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <PATH pos="0 0 100 100" fill=" radial: 96 96, 96 112, 0=ff9a9696, 1=f01f1f1f"
          hasStroke="0" nonZeroWinding="1">s 96.5 100 c 103.93 100 110 106.07  110 113.5 c 110 120.93 103.93 127  96.5 127 c 89.07 127 83 120.93  83 113.5 c 83 106.07 89.07 100  96.5 100 x</PATH>
    <TEXT pos="68 200 56 12" fill="solid: ffaaaaaa" hasStroke="0" text="Length"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="130 164 43 12" fill="solid: ffaaaaaa" hasStroke="0" text="FADE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="128 100 46 10" fill="solid: ffaaaaaa" hasStroke="0" text="GLIDE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="loopSlider" id="f4769a4741fb690a" memberName="loopSlider"
          virtualName="" explicitFocusOrder="0" pos="16 56 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="speedSlider" id="d630945e9da5e83d" memberName="speedSlider"
          virtualName="" explicitFocusOrder="0" pos="72 24 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="directionSlider" id="2f7b6f88ae19bb1e" memberName="directionSlider"
          virtualName="" explicitFocusOrder="0" pos="16 120 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <IMAGEBUTTON name="enableToggle" id="88f90ae5f3542048" memberName="enableToggle"
               virtualName="" explicitFocusOrder="0" pos="235 -3 24 24" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="mixSlider" id="d2598f216032723b" memberName="mixSlider"
          virtualName="" explicitFocusOrder="0" pos="192 40 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <IMAGEBUTTON name="syncToggle" id="599c91ee9ff402cf" memberName="syncToggle"
               virtualName="" explicitFocusOrder="0" pos="63 86 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="panSlider" id="c7e7aee6d5c7fa6f" memberName="panSlider"
          virtualName="" explicitFocusOrder="0" pos="192 136 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <SLIDER name="lengthSlider" id="71e0ffafea1e4031" memberName="lengthSlider"
          virtualName="" explicitFocusOrder="0" pos="72 152 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="glideSlider" id="bfcc73334e2ab0b1" memberName="glideSlider"
          virtualName="" explicitFocusOrder="0" pos="128 56 48 48" min="4"
          max="64" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="fadeSlider" id="cc9942dd41f988fe" memberName="fadeSlider"
          virtualName="" explicitFocusOrder="0" pos="128 120 48 48" min="0"
          max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <IMAGEBUTTON name="glideToggle" id="b51864a83bd92dc9" memberName="glideToggle"
               virtualName="" explicitFocusOrder="0" pos="111 86 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="gapPositionToggle" id="8422f566c6bc1cb" memberName="gapPositionToggle"
               virtualName="" explicitFocusOrder="0" pos="87 126 24 24" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
