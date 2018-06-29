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
#include "Akateko.h"

using namespace akateko;
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
AkatekoAudioProcessorEditor::AkatekoAudioProcessorEditor (AkatekoAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    modInitialized = false;
    buttonColour = Colour(0x73707070);
    activeColour = Colour(0x7f007f7f);

    effectOneCommandId = 0;
    effectTwoCommandId = 0;

    addAndMakeVisible(filter = new FilterComponent("Filter", processor, paramLabel));
    addAndMakeVisible(waveShaper = new WaveShapeComponent("WaveShape", processor, paramLabel));

    /* Prepare XYPAD */
    addAndMakeVisible (xyPad = new XYPad(processor,
                                         paramLabel));


    String tmpXYPad = String("XYPAD");
    xyPadCommandId = tmpXYPad.hashCode(); //Create Identifier
    xyPad->setName(tmpXYPad);
    xyPad->setUpdateCommandId(xyPadCommandId);
    xyPad->setColour(XYPad::outlineColourId, Colour(0xAA407050));
    xyPad->setColour(XYPad::overlayGradientTwoId, Colour(0x3F00FFDA));
    xyPad->setColour(XYPad::fillColourId, Colour(0xFF60B090));

    initialiseDisplayLabel();
    initialiseEffectContainer();
    initialiseModulationContainer();
    initialiseToolBarSection();

    addChildComponent(backPanel = new BackPanel(p), -1);


    //[/Constructor_pre]
    //[UserPreSize]
    // Preset Buttons

    // Main Volume Sliders

    //[/UserPreSize]

    setSize (895, 600);


    //[Constructor] You can add your own custom stuff here..

    //Set Look and Feel Methods for the child components
    claf = new CustomLookAndFeel();
    r1laf = new SliderLookAndFeelBottomV1();
    r2laf = new SliderLookAndFeelBottomV2();

    slaf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::AkatekoSmallV2_png, BinaryData::AkatekoSmallV2_pngSize));
    blaf = new SliderLookAndFeel(ImageCache::getFromMemory(BinaryData::AkatekoBigV3_png, BinaryData::AkatekoBigV3_pngSize));

    waveShaper->setLookAndFeel(claf, blaf);
    filter->setLookAndFeel(claf, blaf, slaf);

    lfoOne->setLookAndFeel(claf, r1laf, r2laf);
    lfoTwo->setLookAndFeel(claf, r1laf, r2laf);
    envOne->setLookAndFeel(claf);
    envTwo->setLookAndFeel(claf);
    stepSeq->setLookAndFeel(claf);
    preset->setLookAndFeel(claf);
    backPanel->setLookAndFeel(claf, r1laf);
    xyPad->setLookAndFeel(claf);

    //Main Volume Sliders
    InputVolumeSlider->setLookAndFeel(slaf);
    mainVolumeSlider->setLookAndFeel(slaf);

    // Set Labels
    presetLabel.setText("Akateko", sendNotificationSync);
    paramLabel.setText("Ready", sendNotificationSync);
    bpmLabel.setText(String(currentBPM, 2), sendNotificationAsync);

    // Set background Image
    backGroundImage = ImageCache::getFromMemory(BinaryData::AkatekoMetal_png, BinaryData::AkatekoMetal_pngSize);


    // Timer functionality
    startTimer(50); //Time in Milliseconds
    midiLearnFlash = 0;

    //Popup Menu
    menu.addItem(1, "learn");
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
    //[/Constructor]
}

AkatekoAudioProcessorEditor::~AkatekoAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    //save states to the processor should be the very first thing to be called
    //[/Destructor_pre]

    lfoOneButton = nullptr;
    lfoTwoButton = nullptr;
    envOneButton = nullptr;
    envTwoButton = nullptr;
    stepButton = nullptr;
    modulationButton = nullptr;
    loadPresetButton = nullptr;
    savePresetButton = nullptr;
    InputVolumeSlider = nullptr;
    waveShaper = nullptr;
    filter = nullptr;
    xyPad = nullptr;
    modMatrix = nullptr;
    fxContainer = nullptr;
    mainVolumeSlider = nullptr;
    presetButton = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    lfoOne = nullptr;
    lfoTwo = nullptr;
    envOne = nullptr;
    envTwo = nullptr;
    stepSeq = nullptr;
    modMatrix = nullptr;
    slaf = nullptr;
    claf = nullptr;
    r1laf = nullptr;
    r2laf = nullptr;

    inputVolume = nullptr;
    outputVolume = nullptr;
    backPanel = nullptr;
    //[/Destructor]
}

//==============================================================================
void AkatekoAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

    // Set Button state thus changing the necessary image accordingly

    switch(activeModulation){
        case LFO1: setButtonState(lfoOneButton); break;
        case LFO2: setButtonState(lfoTwoButton); break;
        case ENV1: setButtonState(envOneButton); break;
        case ENV2: setButtonState(envTwoButton); break;
        case SSEQ: setButtonState(stepButton); break;
        case MODM: setButtonState(modulationButton); break;
        case PRST: setButtonState(presetButton); break;
    }


    g.setTiledImageFill(backGroundImage, getWidth(), getHeight(), 1.0);
    g.fillRect(0, 0, getWidth(), getHeight());
    //[/UserPrePaint]

    g.setGradientFill (ColourGradient (Colour (0xff6f6d6d),
                                       88.0f, 16.0f,
                                       Colours::white,
                                       88.0f, 64.0f,
                                       false));

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 16.00f, Font::plain));
    g.drawText (TRANS("WAVESHAPER"),
                377, 67, 141, 21,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..

    g.setColour(Colours::black);
    g.fillRoundedRectangle(726, 14, 105, 32, 2.f);

    g.setColour(Colour (0xFF001D1D));
    g.fillRect(731, 18, 95, 24);

    g.setColour(Colour(0xFF4A997A));
    g.setFont (Font ("Arial Black", 16.00f, Font::plain));
    g.drawText("BPM", 769, 21, 75, 24,
                Justification::centred, true);

    //[/UserPaint]
}

void AkatekoAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    lfoOneButton->setBounds (24, 351, 66, 31);
    lfoTwoButton->setBounds (24, 383, 66, 31);
    envOneButton->setBounds (24, 415, 66, 31);
    envTwoButton->setBounds (24, 447, 66, 31);

    stepButton->setBounds (24, 479, 66, 31);
    modulationButton->setBounds (24, 511, 66, 31);
    presetButton->setBounds (24, 543, 66, 31);

    loadPresetButton->setBounds (320, 9, 31, 19);
    savePresetButton->setBounds (320, 30, 31, 19);

    waveShaper->setBounds (365, 65, 255, 255);
    filter->setBounds (10, 65, 345, 255);
    xyPad->setBounds (635, 340, 246, 246);
    modMatrix->setBounds (108, 343, 505, 240);
    fxContainer->setBounds (630, 65, 255, 255);

    InputVolumeSlider->setBounds (7, 10, 42, 42);
    mainVolumeSlider->setBounds (844, 10, 42, 42);
    //[/UserPreResize]


    //[UserResized] Add your own custom resize handling here..
    presetLabel.setBounds (366, 9, 252, 19);
    paramLabel.setBounds (366, 30, 252, 19);
    bpmLabel.setBounds(730, 18, 60, 24);

    modMatrix->initialiseHeader(505, 240);
    lfoOne->setBounds(102, 338, 515, 250);
    lfoTwo->setBounds(102, 338, 515, 250);
    envOne->setBounds(102, 338, 515, 250);
    envTwo->setBounds(102, 338, 515, 250);
    stepSeq->setBounds(102, 338, 515, 250);
    preset->setBounds(102, 338, 515, 250);

    /* Set Image after resize for proper, gradient positioning */
    const Image tmpBackGround = ImageCache::getFromMemory(BinaryData::hexagonpattern_png, BinaryData::hexagonpattern_pngSize);
    xyPad->setBackgroundImage(tmpBackGround, true);


    // 102, 338, 515, 250
    overlay = ColourGradient(Colours::transparentBlack,
                             359.5,
                             463,
                             Colour(0x2000DFBA),
                             525,
                             525,
                             true);

    backPanel->setBounds(0, 0, 895, 600);
    //[/UserResized]
}

void AkatekoAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == lfoOneButton)
    {
        //[UserButtonCode_lfoOneButton] -- add your button handler code here..

        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = LFO1;
        lfoOneButton->setState(ImageButton::buttonOver);
        lfoOne->setVisible(true);
        this->repaint();
        //[/UserButtonCode_lfoOneButton]
    }
    else if (buttonThatWasClicked == lfoTwoButton)
    {
        //[UserButtonCode_lfoTwoButton] -- add your button handler code here..

        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = LFO2;
        lfoTwoButton->setState(ImageButton::buttonOver);
        lfoTwo->setVisible(true);
        this->repaint();
        //[/UserButtonCode_lfoTwoButton]
    }
    else if (buttonThatWasClicked == envOneButton)
    {
        //[UserButtonCode_envOneButton] -- add your button handler code here..

        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = ENV1;
        envOneButton->setState(ImageButton::buttonOver);
        envOne->setVisible(true);
        this->repaint();
        //[/UserButtonCode_envOneButton]
    }
    else if (buttonThatWasClicked == envTwoButton)
    {
        //[UserButtonCode_envTwoButton] -- add your button handler code here..
        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = ENV2;
        envTwoButton->setState(ImageButton::buttonOver);
        envTwo->setVisible(true);
        this->repaint();
        //[/UserButtonCode_envTwoButton]
    }
    else if (buttonThatWasClicked == stepButton)
    {
        //[UserButtonCode_stepButton] -- add your button handler code here..
        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = SSEQ;
        stepButton->setState(ImageButton::buttonOver);
        stepSeq->setVisible(true);
        this->repaint();
        //[/UserButtonCode_stepButton]
    }
    else if (buttonThatWasClicked == modulationButton)
    {
        //[UserButtonCode_modulationButton] -- add your button handler code here..
        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = MODM;
        modulationButton->setState(ImageButton::buttonOver);
        modMatrix->setVisible(true);
        this->repaint();
        //[/UserButtonCode_modulationButton]
    }
    else if (buttonThatWasClicked == presetButton)
    {
        //[UserButtonCode_presetButton] -- add your button handler code here..
        hideModulationComponent(activeModulation);
        activeModulation = PRST;
        presetButton->setState(ImageButton::buttonOver);
        preset->setVisible(true);
        this->repaint();
        //[/UserButtonCode_presetButton]
    }
    else if (buttonThatWasClicked == loadPresetButton)
    {
        //[UserButtonCode_loadPresetButton] -- add your button handler code here..
        processor.load();
        updateGui();
        //[/UserButtonCode_loadPresetButton]
    }
    else if (buttonThatWasClicked == savePresetButton)
    {
        //[UserButtonCode_savePresetButton] -- add your button handler code here..
        processor.setShapes(waveShaper->getCurrentShapes(),AkatekoAudioProcessor::WaveShapeId);
        processor.setShapes(lfoOne->getCurrentShapes(),AkatekoAudioProcessor::LFO1Id);
        processor.setShapes(lfoTwo->getCurrentShapes(),AkatekoAudioProcessor::LFO2Id);
        processor.setUIState(envOne->getUIState(), AkatekoAudioProcessor::Envelope1Id);
        processor.setUIState(envTwo->getUIState(), AkatekoAudioProcessor::Envelope2Id);
        processor.setUIState(stepSeq->getUIState(), AkatekoAudioProcessor::StepSequencerId);
        processor.setUIState(xyPad->toString(), AkatekoAudioProcessor::xyPadId);
        processor.setUIState(fxContainer->getUIState(), AkatekoAudioProcessor::FxContainerId);
       // processor.setUIState(stepSeq->getUIState(), AkatekoAudioProcessor::StepSequencerId);


        processor.save();
        //[/UserButtonCode_savePresetButton]
    }
    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void AkatekoAudioProcessorEditor::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    //[/UserCode_visibilityChanged]
}

void AkatekoAudioProcessorEditor::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...

    if(commandId == effectOneCommandId){
        int effectOne = fxContainer->getActiveEffectProcessor(1);
        modMatrix->setEffectsDestination(effectOne, 1);
    } else if(commandId == effectTwoCommandId){
        int effectTwo = fxContainer->getActiveEffectProcessor(2);
        modMatrix->setEffectsDestination(effectTwo, 2);
    } else if(commandId == xyPadCommandId){
        processor.setUIState(xyPad->toString(), AkatekoAudioProcessor::xyPadId);
    } else if(commandId == presetCommandId){
        updateGui();
        const String tmpPresetName = processor.getCurrentPresetName();
        presetLabel.setText(tmpPresetName, sendNotificationSync);
    } else if(commandId == updateTimeId){
        double bpm = processor.getBeatsPerMinute();

        if(modInitialized && bpm >= 20.0 && bpm != currentBPM ){
            currentBPM = bpm;

            stepSeq->calculateBeatDivision(currentBPM); //Update Beats Per Minute

            lfoOne->calculateTimeDivision(currentBPM);
            lfoTwo->calculateTimeDivision(currentBPM);

            envOne->calculateTimeDivision(currentBPM);
            envTwo->calculateTimeDivision(currentBPM);

            fxContainer->updateBeatsPerMinute();
            bpmLabel.setText(String(currentBPM, 2), sendNotificationAsync);

        }
    } else if(paramIndices.size() == 2 &&
              midiStrings.size() == 2)
    {
        int index = -1;
        int param = -1;

        if(commandId == requestMenuId[0]){
            index = menu.show();
            param = 0;
        } else if(commandId == requestMenuId[1]){
            index = menu.show();
            param = 1;
        }

        if(index == 1){
            MidiRow tmpRow;
            initMidiRow(tmpRow, param, 0, 127, 0.0, 2.0, paramIndices[param], akateko::MIDI_TO_DOUBLE, midiStrings[param], 0);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            processor.removeMidiRow(paramIndices[param]);
        }
    }

    //[/UserCode_handleCommandMessage]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AkatekoAudioProcessorEditor::initialiseDisplayLabel(){
    addAndMakeVisible(&presetLabel);
    presetLabel.setFont (Font (15.00f, Font::plain));
    presetLabel.setJustificationType (Justification::centredLeft);
    presetLabel.setEditable (false, false, false);
    presetLabel.setColour (Label::backgroundColourId, Colour(0xEF002020));
    presetLabel.setColour (Label::textColourId,  Colour(0xFF4A997A));

    addAndMakeVisible (&paramLabel);
    paramLabel.setFont (Font (15.00f, Font::plain));
    paramLabel.setJustificationType (Justification::centredLeft);
    paramLabel.setEditable (false, false, false);
    paramLabel.setColour (Label::backgroundColourId, Colour (0xEF002020));
    paramLabel.setColour (Label::textColourId, Colour(0xFF4A997A));
    paramLabel.setColour (TextEditor::textColourId, Colours::black);

    addAndMakeVisible(&bpmLabel);
    bpmLabel.setFont (Font (22.00f, Font::bold));
    bpmLabel.setJustificationType (Justification::centredRight);
    bpmLabel.setEditable (false, false, false);
    bpmLabel.setColour (Label::backgroundColourId, Colours::transparentBlack);
    bpmLabel.setColour (Label::textColourId, Colour(0xFF4A997A));
    bpmLabel.setColour (TextEditor::textColourId, Colours::black);
}

/*
 * preAmpEnable : ParamID = "PREN"
 * preAmpColume : ParamID = "PRVL"
 * preAmpDrive  : ParamID = "PRDR"
 */

void AkatekoAudioProcessorEditor::initialiseModulationContainer(){
    // LFO One Button

    addAndMakeVisible (lfoOneButton = new ImageButton ("lfoOne"));
    lfoOneButton->setButtonText (TRANS("LFO 1"));
    lfoOneButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    lfoOneButton->addListener (this);

    lfoOneButton->setImages (false, true, false,
                             ImageCache::getFromMemory (BinaryData::lfo1Off_png, BinaryData::lfo1Off_pngSize), 1.000f, Colour (0x00000000),
                             ImageCache::getFromMemory (BinaryData::lfo1On_png, BinaryData::lfo1On_pngSize), 0.75f, Colour (0x7F709F9F),
                             ImageCache::getFromMemory (BinaryData::lfo1On_png, BinaryData::lfo1On_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (lfoTwoButton = new ImageButton ("lfoTwo"));
    lfoTwoButton->setButtonText (TRANS("LFO 2"));
    lfoTwoButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    lfoTwoButton->addListener (this);

    lfoTwoButton->setImages (false, true, false,
                             ImageCache::getFromMemory (BinaryData::lfo2Off_png, BinaryData::lfo2Off_pngSize), 1.000f, Colour (0x00000000),
                             ImageCache::getFromMemory (BinaryData::lfo2On_png, BinaryData::lfo2On_pngSize), 0.75f, Colour (0x7F709F9F),
                             ImageCache::getFromMemory (BinaryData::lfo2On_png, BinaryData::lfo2On_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (envOneButton = new ImageButton ("envOneButton"));
    envOneButton->setButtonText (TRANS("ENV 1"));
    envOneButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    envOneButton->addListener (this);

    envOneButton->setImages (false, true, false,
                             ImageCache::getFromMemory (BinaryData::env1Off_png, BinaryData::env1Off_pngSize), 1.000f, Colour (0x00000000),
                             ImageCache::getFromMemory (BinaryData::env1On_png, BinaryData::env1On_pngSize), 0.75f, Colour (0x7F709F9F),
                             ImageCache::getFromMemory (BinaryData::env1On_png, BinaryData::env1On_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (envTwoButton = new ImageButton ("envTwo"));
    envTwoButton->setButtonText (TRANS("ENV 2"));
    envTwoButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    envTwoButton->addListener (this);

    envTwoButton->setImages (false, true, false,
                             ImageCache::getFromMemory (BinaryData::env2Off_png, BinaryData::env2Off_pngSize), 1.000f, Colour (0x00000000),
                             ImageCache::getFromMemory (BinaryData::env2on_png, BinaryData::env2on_pngSize), 0.75f, Colour (0x7F709F9F),
                             ImageCache::getFromMemory (BinaryData::env2on_png, BinaryData::env2on_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (stepButton = new ImageButton ("stepButton"));
    stepButton->setButtonText (TRANS("STEP"));
    stepButton->setConnectedEdges (Button::ConnectedOnBottom);
    stepButton->addListener (this);

    stepButton->setImages (false, true, false,
                           ImageCache::getFromMemory (BinaryData::stepOff_png, BinaryData::stepOff_pngSize), 1.000f, Colour (0x00000000),
                           ImageCache::getFromMemory (BinaryData::stepOn_png, BinaryData::stepOn_pngSize), 0.75f, Colour (0x7F709F9F),
                           ImageCache::getFromMemory (BinaryData::stepOn_png, BinaryData::stepOn_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (modulationButton = new ImageButton ("modButton"));
    modulationButton->setButtonText (TRANS("MOD"));
    modulationButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    modulationButton->addListener (this);

    modulationButton->setImages (false, true, false,
                                 ImageCache::getFromMemory (BinaryData::modOff_png, BinaryData::modOff_pngSize), 1.000f, Colour (0x00000000),
                                 ImageCache::getFromMemory (BinaryData::modOn_png, BinaryData::modOn_pngSize), 0.75f, Colour (0x7F709F9F),
                                 ImageCache::getFromMemory (BinaryData::modOn_png, BinaryData::modOn_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (presetButton = new ImageButton ("preset"));
    presetButton->setButtonText (TRANS("PRESET"));
    presetButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    presetButton->addListener (this);

    presetButton->setImages (false, true, false,
                             ImageCache::getFromMemory (BinaryData::presetOff_png, BinaryData::presetOff_pngSize), 1.000f, Colour (0x00000000),
                             ImageCache::getFromMemory (BinaryData::presetOn_png, BinaryData::presetOn_pngSize), 0.75f, Colour (0x7F709F9F),
                             ImageCache::getFromMemory (BinaryData::presetOn_png, BinaryData::presetOn_pngSize), 1.000f, Colour (0x1f000000));


    currentBPM = processor.getBeatsPerMinute();

    /* Gather Parameters and ParamIds */
    addChildComponent(lfoOne = new LFOComponent("LFO 1", processor, 1, paramLabel));
    lfoOne->calculateTimeDivision(currentBPM);

    addChildComponent(lfoTwo = new LFOComponent("LFO 2", processor, 2, paramLabel));
    lfoTwo->calculateTimeDivision(currentBPM);

    addChildComponent(envOne = new EnvelopeComponent("ENV 1", processor, 1, paramLabel));
    envOne->calculateTimeDivision(currentBPM);

    addChildComponent(envTwo = new EnvelopeComponent("ENV 2", processor, 2, paramLabel));
    envTwo->calculateTimeDivision(currentBPM);

    addChildComponent(stepSeq = new StepSequencerComponent("SSEQ", processor, paramLabel));

    addChildComponent(modMatrix = new MatrixTable(processor.getModMatrix()));
    modMatrix->setColour(TableListBox::backgroundColourId, Colours::black);

    // Preset Component
    addChildComponent(preset = new PresetComponent("Presets", processor, presetLabel, paramLabel));
    // Generate Hash Code
    const String tmpPresetId = String(preset->getName()) + String(":gui");
    presetCommandId = tmpPresetId.hashCode();
    preset->setUpdateCommandId(presetCommandId);


    lfoOne->setVisible(true);
    activeModulation = LFO1;
    lfoOneButton->setColour(TextButton::buttonColourId, activeColour);

    // Retrieve Shapes
    lfoOne->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::LFO1Id));
    lfoTwo->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::LFO2Id));
    envOne->setUIState(processor.getUIState(AkatekoAudioProcessor::Envelope1Id));
    envTwo->setUIState(processor.getUIState(AkatekoAudioProcessor::Envelope2Id));
    stepSeq->setUIState(processor.getUIState(AkatekoAudioProcessor::StepSequencerId));
    waveShaper->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::WaveShapeId));
     //  xyPad->restoreFromString(processor.getUIState(AkatekoAudioProcessor::xyPadId));

    modInitialized = true;
}

void AkatekoAudioProcessorEditor::initialiseEffectContainer(){
    String name = String("FX : ");
    String fxOneUpdateId = name + String("Update 1");
    String fxTwoUpdateId = name + String("Update 2");

    effectOneCommandId = fxOneUpdateId.hash();
    effectTwoCommandId = fxTwoUpdateId.hash();

    addAndMakeVisible (fxContainer = new FXContainer (name, processor, paramLabel));


    fxContainer->setEffectOneUpdateId(effectOneCommandId);
    fxContainer->setEffectTwoUpdateId(effectTwoCommandId);
}

void AkatekoAudioProcessorEditor::initialiseToolBarSection(){
    addAndMakeVisible (loadPresetButton = new ImageButton ("loadPresetButton"));
    loadPresetButton->setImages (false, true, false,
                                 ImageCache::getFromMemory (BinaryData::loadOnV2_png, BinaryData::loadOnV2_pngSize), 1.000f, Colour (0x5F000000),
                                 ImageCache::getFromMemory (BinaryData::loadOnV2_png, BinaryData::loadOnV2_pngSize), 0.75f, Colour (0x7F709F9F),
                                 ImageCache::getFromMemory (BinaryData::loadOnV2_png, BinaryData::loadOnV2_pngSize), 1.000f, Colour (0x4f000000));
    loadPresetButton->addListener (this);

    addAndMakeVisible (savePresetButton = new ImageButton ("savePresetButton"));
    savePresetButton->setImages (false, true, false,
                                 ImageCache::getFromMemory (BinaryData::saveOnV2_png, BinaryData::saveOnV2_pngSize), 1.000f, Colour (0x5F000000),
                                 ImageCache::getFromMemory (BinaryData::saveOnV2_png, BinaryData::saveOffV2_pngSize), 0.75f, Colour (0x7F709F9F),
                                 ImageCache::getFromMemory (BinaryData::saveOnV2_png, BinaryData::saveOffV2_pngSize), 1.000f, Colour (0x4f000000));

    savePresetButton->addListener (this);


    const OwnedArray<AudioProcessorParameter> &params = processor.getParameters();
    paramIndices = processor.getParameterIndices(AkatekoAudioProcessor::GlobalId);
    StringArray paramIds = processor.getParameterIds(AkatekoAudioProcessor::GlobalId);

    midiStrings.clear();

    if(paramIndices.size() == 2 &&
       paramIds.size() == 2 &&
       params.size() >= paramIndices[1])
    {
       requestMenuId[0] = paramIds[0].hash();
       requestMenuId[1] = paramIds[1].hash();

       midiStrings.add("Global: In Volume");
       midiStrings.add(" Global: Out Volume");

       addAndMakeVisible (InputVolumeSlider = new IgnoreRightClick<Slider> (paramIds[0]));
       inputVolume = params.getUnchecked(paramIndices[0]);

       addAndMakeVisible (mainVolumeSlider = new IgnoreRightClick<Slider> (paramIds[1]));
       outputVolume = params.getUnchecked(paramIndices[1]);

    } else {
        std::cerr << "AkatekoAudioProcessor::initialiseToolBarSection" << std::endl;
        std::cerr << "Parameters are not bound properly" <<  std::endl;

        addAndMakeVisible (InputVolumeSlider = new Slider ("Input"));
        addAndMakeVisible (mainVolumeSlider = new Slider ("mainVolume"));
    }

    InputVolumeSlider->setRange (0, 2, 0);
    InputVolumeSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    InputVolumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    InputVolumeSlider->setSkewFactor(0.31f);
    InputVolumeSlider->setDoubleClickReturnValue(true, 1.0f);
    InputVolumeSlider->addListener(this);

    mainVolumeSlider->setRange (0, 2, 0);
    mainVolumeSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    mainVolumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mainVolumeSlider->setSkewFactor(0.31f);
    mainVolumeSlider->setDoubleClickReturnValue(true, 1.0f);
    mainVolumeSlider->addListener(this);

    // Load previous Values
    if(inputVolume != nullptr){
        InputVolumeSlider->setValue(inputVolume->getValue());
    }

    if(outputVolume != nullptr){
        mainVolumeSlider->setValue(outputVolume->getValue());
    }
}
/* If a current modulation Component is visible, hide it */
void AkatekoAudioProcessorEditor::hideModulationComponent(int modComponentId){
    switch(modComponentId){
        case LFO1:
            lfoOneButton->setState(ImageButton::buttonNormal);
            lfoOne->setVisible(false);
            break;
        case LFO2:
            lfoTwoButton->setState(ImageButton::buttonNormal);
            lfoTwo->setVisible(false);
            break;
        case ENV1:
            envOneButton->setState(ImageButton::buttonNormal);
            envOne->setVisible(false);
            break;
        case ENV2:
            envTwoButton->setState(ImageButton::buttonNormal);
            envTwo->setVisible(false);
            break;
        case SSEQ:
            stepButton->setState(ImageButton::buttonNormal);
            stepSeq->setVisible(false);
            break;
        case MODM:
            modulationButton->setState(ImageButton::buttonNormal);
            modMatrix->setVisible(false);
            break;
        case PRST:
            presetButton->setState(ImageButton::buttonNormal);
            preset->setVisible(false);
            break;
    }
    this->repaint();
}

/* When a preset is loaded update the whole
 * Graphical User interface.
 */

void AkatekoAudioProcessorEditor::updateGui(){
    //Retrieve Shapes Stored as String Arrays
    waveShaper->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::WaveShapeId));
    lfoOne->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::LFO1Id));
    lfoTwo->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::LFO2Id));
    envOne->setUIState(processor.getUIState(AkatekoAudioProcessor::Envelope1Id));
    envTwo->setUIState(processor.getUIState(AkatekoAudioProcessor::Envelope2Id));
    stepSeq->setUIState(processor.getUIState(AkatekoAudioProcessor::StepSequencerId));
    xyPad->restoreFromString(processor.getUIState(AkatekoAudioProcessor::xyPadId));
    fxContainer->setUITState(processor.getUIState(AkatekoAudioProcessor::FxContainerId));

    //After loading update all gui components
    filter->updateGui();
    waveShaper->updateGui();
    lfoOne->updateGui();
    lfoTwo->updateGui();

    envOne->updateGui();
    envTwo->updateGui();
    stepSeq->updateGui();
    fxContainer->updateGui();

    modMatrix->updateGui();

}

void AkatekoAudioProcessorEditor::setButtonState(ImageButton *button){
   const int tmpState = button->getState();

   if(tmpState != ImageButton::buttonOver){
       button->setState(ImageButton::buttonDown);
   }
}

void AkatekoAudioProcessorEditor::paintOverChildren(Graphics &g){
    if(!backPanel->isVisible()){
        g.setGradientFill(overlay);
        g.fillRoundedRectangle(102, 338, 515, 250, 2.f);
    }
}

void AkatekoAudioProcessorEditor::sliderValueChanged(Slider *slider){
    if(slider == InputVolumeSlider){
        const double tmpValue = InputVolumeSlider->getValue();
        double dbValue = msm::dbConversion(tmpValue);

        if(dbValue > 6.00){
            dbValue = 6.00;
        }

        const String tmpLabel = InputVolumeSlider->getName() +
                                String(dbValue, 2) +
                                String("dB");


        if(inputVolume != nullptr){
            inputVolume->setValue(tmpValue);
        }

        paramLabel.setText(tmpLabel, dontSendNotification);
    } else if(slider == mainVolumeSlider){
        const double tmpValue = mainVolumeSlider->getValue();
        double dbValue = msm::dbConversion(tmpValue);

        if(dbValue > 6.00){
            dbValue = 6.00;
        }
        const String tmpLabel = mainVolumeSlider->getName() +
                                String(dbValue, 2) +
                                String("dB");

        if(outputVolume != nullptr){
            outputVolume->setValue(tmpValue);
        }

        paramLabel.setText(tmpLabel, dontSendNotification);
    }
}

bool AkatekoAudioProcessorEditor::keyPressed(const KeyPress &key){
    if(key == KeyPress::spaceKey){
        processor.printPositionInfo();
    } else if(key == KeyPress::tabKey || key == KeyPress::F1Key){
        if(backPanel->isVisible()){
            backPanel->setVisible(false);
        } else {
            backPanel->setVisible(true);
        }
    }

    return false;
}

//Timer Callback
void AkatekoAudioProcessorEditor::timerCallback(){
    if(processor.getMidiLearnStatus()){
        midiLearnFlash = midiLearnFlash & 0B1111;

        if(midiLearnFlash < 12){
            paramLabel.setText("MIDI Learn : Waiting for input", sendNotificationAsync);
        } else {
            paramLabel.setText("MIDI Learn : ", sendNotificationAsync);
        }

        midiLearnFlash ++;
    } else {
        if(processor.getMidiMsgBoundStatus()){
            String message = processor.getMidiMessageBounded();
            message += "bounded";

            paramLabel.setText(message, sendNotificationAsync);
        }
        midiLearnFlash = 0;
    }

    // Make it possible to gather the arrayIndex that needs to be
    // updated i.e. the individual widget instead of brute force
    // updating the whole gui class.
    if(processor.requestGuiUpdate()){
        vector<bool> updateSection = processor.getUpdatedSections();

        for(int i=0; i<updateSection.size(); i++){
            if(updateSection[i]){
                updateGuiSection(i);
            }
        }
    }
}



void AkatekoAudioProcessorEditor::updateGuiSection(int section){
    switch(section){
        case AkatekoAudioProcessor::GlobalId:
            if(inputVolume != nullptr){
                InputVolumeSlider->setValue(inputVolume->getValue());
            }

            if(outputVolume != nullptr){
                mainVolumeSlider->setValue(outputVolume->getValue());
            }

            break;
        case AkatekoAudioProcessor::FilterId:
            filter->updateGui();
            break;
        case AkatekoAudioProcessor::WaveShapeId:
            waveShaper->updateGui();
            break;
        case AkatekoAudioProcessor::LFO1Id:
            if(lfoOne->isVisible()){
                lfoOne->updateGui();
            }
            break;
        case AkatekoAudioProcessor::LFO2Id:
            if(lfoTwo->isVisible()){
                lfoTwo->updateGui();
            }
            break;
        case AkatekoAudioProcessor::Envelope1Id:
            if(envOne->isVisible()){
                envOne->updateGui();
            }
            break;
        case AkatekoAudioProcessor::Envelope2Id:
            if(envTwo->isVisible()){
                envTwo->updateGui();
            }
            break;
        case AkatekoAudioProcessor::StepSequencerId:
            if(stepSeq->isVisible()){
                stepSeq->updateGui();
            }
            break;
        case AkatekoAudioProcessor::xyPadId: /*Implement*/
            std::cout << "Received update XY Pad" << std::endl;
            xyPad->updateGui();
            break;
    }

    if(section >= AkatekoAudioProcessor::FxContainerId &&
       section <= AkatekoAudioProcessor::pReverbId)
    {
         fxContainer->updateGui();
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AkatekoAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor"
                 constructorParams="AkatekoAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor (&amp;p), processor (p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="895" initialHeight="600">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="0">
    <ROUNDRECT pos="0 0 895 600" cornerSize="4" fill="solid: ff000000" hasStroke="0"/>
    <ROUNDRECT pos="5 5 885 50" cornerSize="4" fill="solid: ff323232" hasStroke="0"/>
    <ROUNDRECT pos="5 330 885 265" cornerSize="4" fill="solid: ff4e4e4e" hasStroke="0"/>
    <ROUNDRECT pos="5 60 885 265" cornerSize="4" fill="solid: ff4e4e4e" hasStroke="0"/>
    <ROUNDRECT pos="10 65 345 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="365 65 255 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="630 65 255 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="10 335 610 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="630 335 255 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <TEXT pos="50 16 258 33" fill="linear: 88 16, 88 64, 0=ff6f6d6d, 1=ffffffff"
          hasStroke="0" text="AKATEKO" fontname="Good Times" fontsize="46"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="377 67 141 21" fill="solid: ffaaaaaa" hasStroke="0" text="WAVESHAPER"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <IMAGEBUTTON name="lfoOne" id="3be90a7d038cacae" memberName="lfoOneButton"
               virtualName="" explicitFocusOrder="0" pos="24 352 70 30" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="LFO 1" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="lfoTwo" id="8a5cdd3b15902e28" memberName="lfoTwoButton"
               virtualName="" explicitFocusOrder="0" pos="24 384 70 30" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="LFO 2" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="envOneButton" id="bdaf7e11ea200503" memberName="envOneButton"
               virtualName="" explicitFocusOrder="0" pos="24 416 70 30" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="ENV 1" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="envTwo" id="18bd76a767b49aa8" memberName="envTwoButton"
               virtualName="" explicitFocusOrder="0" pos="24 448 70 30" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="ENV 2" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="stepButton" id="161d2f193b2d1e9c" memberName="stepButton"
               virtualName="" explicitFocusOrder="0" pos="24 480 70 30" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="STEP" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="modButton" id="411aa6c03fa5468d" memberName="modulationButton"
               virtualName="" explicitFocusOrder="0" pos="24 512 70 30" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="MOD" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <ImageButton name="savePresetButton" id="2a6f9b9c2a0b2c96" memberName="loadPresetButton"
               virtualName="" explicitFocusOrder="0" pos="320 8 32 20" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="load" connectedEdges="3"
               needsCallback="1" radioGroupId="0"/>
  <ImageButton name="new button" id="48fac67238ae71fe" memberName="savePresetButton"
               virtualName="" explicitFocusOrder="0" pos="320 32 32 20" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="save" connectedEdges="3"
               needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Input" id="6336a7102808882a" memberName="InputVolumeSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="10 11 40 40"
          min="0.010000000000000000208" max="2" int="0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <GENERICCOMPONENT name="waveShaper" id="97d7ede6e23b1658" memberName="waveShaper"
                    virtualName="WaveShapeComponent" explicitFocusOrder="0" pos="365 65 255 255"
                    class="Component" params="&quot;WSHP&quot;"/>
  <GENERICCOMPONENT name="filter" id="db18492e2bda189" memberName="filter" virtualName="FilterComponent"
                    explicitFocusOrder="0" pos="10 65 345 255" class="Component"
                    params="&quot;FILT&quot;"/>
  <GENERICCOMPONENT name="xyPad" id="fc769d97819c849c" memberName="xyPad" virtualName="XYPad"
                    explicitFocusOrder="0" pos="635 340 246 246" class="Component"
                    params=""/>
  <GENERICCOMPONENT name="Modulation Matrix" id="4d30054a30547bdc" memberName="modMatrix"
                    virtualName="MatrixTable" explicitFocusOrder="0" pos="108 343 505 240"
                    class="Component" params="ModulationMatrix &amp;modm"/>
  <GENERICCOMPONENT name="fxContainer" id="741f651e1a422402" memberName="fxContainer"
                    virtualName="FXContainer" explicitFocusOrder="0" pos="630 65 255 255"
                    class="Component" params="&quot;FX Container&quot;"/>
  <SLIDER name="mainVolume" id="7b235f0e952fd646" memberName="mainVolumeSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="844 11 40 40"
          min="0.010000000000000000208" max="2" int="0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <IMAGEBUTTON name="preset" id="6d9d037c6aab172b" memberName="presetButton"
               virtualName="" explicitFocusOrder="0" pos="24 544 70 30" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="PRESET" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <LABEL name="presetLabel" id="ef58931134b9d2d8" memberName="presetLabel"
         virtualName="" explicitFocusOrder="0" pos="368 9 248 18" bkgCol="7f2d0000"
         textCol="ffdfdfdf" edTextCol="ff000000" edBkgCol="0" labelText="Preset Name &#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="paramLabel" id="7331442217f9d85d" memberName="paramLabel"
         virtualName="" explicitFocusOrder="0" pos="368 33 248 18" bkgCol="7f2d0000"
         textCol="ffdfdfdf" edTextCol="ff000000" edBkgCol="0" labelText="Params&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
