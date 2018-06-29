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
#include "Akateko.h"


using akateko::IgnoreRightClick;
using akateko::MidiRow;
using akateko::updateFxGUI;
using akateko::updateFxBPM;
using akateko::initMidiRow;
using akateko::frequency_ratios;

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
      beatsPerMinute(120.0),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..

    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::StereoflangerId);
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
        requestMenuIds[0] = paramIds[0].hash();
        requestMenuIds[1] = paramIds[1].hash();
        requestMenuIds[2] = paramIds[2].hash();
        requestMenuIds[3] = paramIds[3].hash();
        requestMenuIds[4] = paramIds[4].hash();
        requestMenuIds[5] = paramIds[5].hash();
        requestMenuIds[6] = paramIds[6].hash();
        requestMenuIds[7] = paramIds[7].hash();

        addAndMakeVisible(enableToggle = new ParamImageToggle(paramIds[0], *params.getUnchecked(paramIndices[0]), labelRef));
        addAndMakeVisible(crossFB = new ParamImageToggle(paramIds[1], *params.getUnchecked(paramIndices[1]), labelRef));

        addAndMakeVisible(SyncToggle = new IgnoreRightClick<ImageButton>(paramIds[2]));
        sync = params.getUnchecked(paramIndices[2]);

        addAndMakeVisible(rateSlider = new IgnoreRightClick<Slider>(paramIds[3]));
        rate = params.getUnchecked(paramIndices[3]);

        addAndMakeVisible(phaseSlider = new IgnoreRightClick<Slider>(paramIds[4]));
        phase = params.getUnchecked(paramIndices[4]);

        addAndMakeVisible(depthSlider = new ParamSlider(paramIds[5], *params.getUnchecked(paramIndices[5]), labelRef));
        addAndMakeVisible(feedBackSlider = new ParamSlider(paramIds[6], *params.getUnchecked(paramIndices[6]), labelRef, -1.0, 1.0));

        addAndMakeVisible(waveSlider = new IgnoreRightClick<Slider>(paramIds[7]));
        wave = params.getUnchecked(paramIndices[7]);

        //std::cout << paramIndices[3] << std::endl;

        initialiseMidiStrings();
    } else {
        std::cerr << "StereoFlangerComponent::StereoFlangerComponents" << std::endl;
        std::cerr << "Parameters not bound properly" << std::endl;

        addAndMakeVisible (enableToggle = new ImageButton ("enableToggle"));
        addAndMakeVisible (crossFB = new ImageButton ("crossFB"));
        addAndMakeVisible (SyncToggle = new ImageButton ("SyncToggle"));

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
    const Image bigRotary = ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize);
    const Image waveRotary = ImageCache::getFromMemory(BinaryData::WaveStripV1_png, BinaryData::WaveStripV1_pngSize);
    const Image fbRotary = ImageCache::getFromMemory(BinaryData::PanBig_png, BinaryData::PanBig_pngSize);

    claf = new CustomLookAndFeel();
    blaf = new SliderLookAndFeel(bigRotary);
    wlaf = new SliderLookAndFeel(waveRotary);
    fblaf = new SliderLookAndFeel(fbRotary);

    menu.setLookAndFeel(claf);

    feedBackSlider->setRange (0, 1, 0);
    feedBackSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    feedBackSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    feedBackSlider->setLookAndFeel(fblaf);
    feedBackSlider->setDoubleClickReturnValue(true, 0.5);

    phaseSlider->setRange (0, 1, 0);
    phaseSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    phaseSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    phaseSlider->setDoubleClickReturnValue(true, 0.5f);
    phaseSlider->setLookAndFeel(blaf);
    phaseSlider->addListener (this);

    depthSlider->setRange (0, 1, 0);
    depthSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    depthSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    depthSlider->setLookAndFeel(blaf);

    rateSlider->setRange (0, 1, 0);
    rateSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    rateSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    rateSlider->setLookAndFeel(blaf);
    rateSlider->addListener (this);

    waveSlider->setRange (0, 1, 0.25);
    waveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    waveSlider->setLookAndFeel(wlaf);
    waveSlider->addListener (this);

    crossFB->setButtonText (String());
    crossFB->setImages(false, true, false,
                       ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                       ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                       ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    enableToggle->setButtonText (String());
    enableToggle->setImages(false, true, false,
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                            ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                            ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    SyncToggle->setButtonText (String());
    SyncToggle->setClickingTogglesState(true);
    SyncToggle->setImages(false, true, false,
                          ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                          ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                          ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    SyncToggle->addListener (this);
    //[/UserPreSize]

    setSize (255, 232);


    //[Constructor] You can add your own custom stuff here..

    // Get Current BPM from processor
    initialiseTimeDivisions();
    double bpm = processor.getBeatsPerMinute();

    if(bpm <= 20.0){
        beatsPerMinute = 20.0;
    } else {
        beatsPerMinute = bpm;
    }

    calculateBeatDivision(beatsPerMinute);

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

    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
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
    blaf = nullptr;
    wlaf = nullptr;
    claf = nullptr;
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
    //[/UserPreResize]d

    feedBackSlider->setBounds (169, 24, 64, 64);
    phaseSlider->setBounds (24, 144, 64, 64);
    depthSlider->setBounds (169, 144, 64, 64);
    rateSlider->setBounds (24, 24, 64, 64);
    waveSlider->setBounds (95, 88, 64, 64);

    SyncToggle->setBounds (82, 70, 16, 16);
    crossFB->setBounds (155, 70, 16, 16);
    enableToggle->setBounds (238, 0, 18, 18);
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
        if(sync != nullptr &&
           rate != nullptr &&
           valueBeatDivision.size() == 24)
        {
            const bool tmpState = SyncToggle->getToggleState();
            double tmpRate = rate->getValue();
            sync->setValue(tmpState);

            if(tmpState){
                int tmpIndex = findClosestTimeDivision(tmpRate);
                rateSlider->setRange(0, valueBeatDivision.size()-1, 1);
                rateSlider->setValue(tmpIndex);

                // Convert Period to Frequency
                tmpRate = valueBeatDivision[tmpIndex];

                if(processor.getRegisteredMidi(paramIndices[3])){
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[3]);
                }

                rate->setValue(tmpRate);
            } else {
                const double maxFreq = valueBeatDivision[valueBeatDivision.size()-1];
                const double minFreq = valueBeatDivision[0];

                if(processor.getRegisteredMidi(paramIndices[3])){
                    processor.changeMidiRowMinMax(minFreq, maxFreq, akateko::MIDI_TO_DOUBLE, paramIndices[3]);
                }

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
        const bool tmpSync = SyncToggle->getToggleState();

        if(tmpSync){
            int tmpVal = rateSlider->getValue();

            if(tmpVal < beatDivision.size() &&
               tmpVal < valueBeatDivision.size())
            {
                float tmpRate = valueBeatDivision[tmpVal];
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
void StereoFlangerComponent::initRateSlider(){
    if(sync != nullptr &&
       rate != nullptr &&
       beatDivision.size() != 0 &&
       valueBeatDivision.size() != 0)
    {
        const bool tmpSync = sync->getValue();
        double tmpRate = rate->getValue();

        if(tmpSync){
            int tmpIndex = getTimeDivisionIndex(valueBeatDivision, tmpRate);

            rateSlider->setRange(0, 23, 1);
            rateSlider->setValue(tmpIndex, sendNotificationAsync);
        } else {        
            const double maxFreq = valueBeatDivision[valueBeatDivision.size()-1];
            const double minFreq = valueBeatDivision[0];

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

void StereoFlangerComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" S-Flanger: Enable");
    midiStrings.add(" S-Flanger: Cross");
    midiStrings.add(" S-Flanger: Sync");
    midiStrings.add(" S-Flanger: Rate");
    midiStrings.add(" S-Flanger: Phase");
    midiStrings.add(" S-Flanger: Depth");
    midiStrings.add(" S-Flanger: FB");
    midiStrings.add(" S-Flanger: Wave");
}

void StereoFlangerComponent::handleCommandMessage(int commandId){
    if(commandId == updateFxGUI){
        enableToggle->postCommandMessage(ParamImageToggle::update);
        SyncToggle->postCommandMessage(ParamImageToggle::update);
        crossFB->postCommandMessage(ParamImageToggle::update);
        depthSlider->postCommandMessage(ParamSlider::update);
        feedBackSlider->postCommandMessage(ParamSlider::update);

        initRateSlider();

        if(sync != nullptr){
            const bool tmpSync = sync->getValue();
            SyncToggle->setToggleState(tmpSync, dontSendNotification);
        }

        if(wave != nullptr){
            const double tmpWave = wave->getValue()*0.25;
            waveSlider->setValue(tmpWave, dontSendNotification);
        }

        if(phase != nullptr){
            phaseSlider->setValue(phase->getValue());
        }

        if(paramIndices.size() == 8 &&
           processor.getRegisteredMidi(paramIndices[3])){
            if(SyncToggle->getToggleState()){
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[3]);
            } else {
                processor.changeMidiRowMinMax(valueBeatDivision[0], valueBeatDivision[23], akateko::MIDI_TO_DOUBLE, paramIndices[3]);
            }
        }

    } else if(commandId == updateFxBPM){
        double bpm = processor.getBeatsPerMinute();

        if(bpm < 20.0){
            bpm = 20.0;
        }

        calculateBeatDivision(bpm);
    }

    if(paramIndices.size() == 8 &&
       midiStrings.size() == 8 &&
       valueBeatDivision.size() == 24)
    {
        int index = -1;
        int param = -1;
        int handling = -1;
        int minValue = 0.0;
        int maxValue = 1.0;

        if(commandId == requestMenuIds[0]){         // Enable
            index = menu.show();
            param = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[1]){  // Cross Enabled
            index = menu.show();
            param = 1;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[2]){  // Sync
            index = menu.show();
            param = 2;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[3]){  // Rate
            index = menu.show();
            param = 3;

            if(SyncToggle->getToggleState()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                maxValue = valueBeatDivision[23];
                handling = akateko::MIDI_TO_DOUBLE;
            }
        } else if(commandId == requestMenuIds[4]){  // Phase
            index = menu.show();
            param = 4;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[5]){  // Depth
            index = menu.show();
            param = 5;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[6]){  // Feedback
            index = menu.show();
            param = 6;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[7]){  // Wave
            index = menu.show();
            param = 7;
            maxValue = 4;
            handling = akateko::MIDI_TO_INT;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[param], handling, midiStrings[param], 13);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }
}

int StereoFlangerComponent::findClosestTimeDivision(double freq){
    int result = 0;

    if(freq != 0){
        int nrOfSteps = valueBeatDivision.size();

        if(nrOfSteps != 0){
            bool run = true;
            int index = 1;

            std::cout << "Frequency" << freq << std::endl;

            while(run && index < nrOfSteps-1){
                double prev = valueBeatDivision[index-1];
                double val = valueBeatDivision[index];
                double next = valueBeatDivision[index+1];

                std::cout << "Previous" << prev << std::endl;
                std::cout << "Middle" << val << std::endl;
                std::cout << "Next" << next << std::endl;

                if(val == freq){
                    result = index;
                    run = false;
                }

                if(freq >= prev && freq < val){
                    result = index-1;
                    run = false;
                }

                if(freq > val && freq <= next){
                    result = index+1;
                    run = false;
                }

                index +=3;
            }
        }
    }
    return result;
}

int StereoFlangerComponent::getTimeDivisionIndex(vector<double> &values, double freq){
    int index = 0;

    while(index < values.size()){
        double tmpValue = values[index];
        double error = tmpValue*0.05;

        if(freq >= tmpValue-error &&
           freq <= tmpValue+error)
        {
            return index;
        }
        index++;
    }
    return 0;
}

void StereoFlangerComponent::initialiseTimeDivisions(){
    StringArray noteVals;
    beatDivision.clear();

    noteVals.add("4N");
    noteVals.add("3N");
    noteVals.add("2N");
    noteVals.add("1N");
    noteVals.add("1/2N");
    noteVals.add("1/4N");
    noteVals.add("1/8N");
    noteVals.add("1/16N");

    for(int i=0; i<noteVals.size(); i++){
        String dot = noteVals[i] + String("D");
        String triplet = noteVals[i] + String("T");

        beatDivision.add(dot);
        beatDivision.add(noteVals[i]);
        beatDivision.add(triplet);
    }
}

void StereoFlangerComponent::calculateBeatDivision(double bpm){
    if(bpm > 0.0){
        // Calculate 1/128 Note, multiply with 2
        valueBeatDivision.clear();

        for(int i=0; i<beatDivision.size(); i++){
            valueBeatDivision.push_back(bpm*frequency_ratios[i]);

        }
    }
    initRateSlider();
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
  <IMAGEBUTTON name="SyncToggle" id="b22b43d4900d0f35" memberName="SyncToggle"
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
  <IMAGEBUTTON name="crossFB" id="ba0dd7e095193965" memberName="crossFB" virtualName=""
               explicitFocusOrder="0" pos="157 64 24 24" buttonText="" connectedEdges="0"
               needsCallback="0" radioGroupId="0" state="0"/>
  <IMAGEBUTTON name="enableToggle" id="dfef00aeb55135fd" memberName="enableToggle"
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
