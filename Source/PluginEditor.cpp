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
    buttonColour = Colour(0x73707070);
    activeColour = Colour(0x7f007f7f);

    effectOneCommandId = 0;
    effectTwoCommandId = 0;

    addAndMakeVisible(filter = new FilterComponent("Filter", processor, paramLabel));
    addAndMakeVisible(waveShaper = new WaveShapeComponent("WaveShape", processor, paramLabel));

    addAndMakeVisible (xyPad = new XYPad(paramLabel,
                                         processor.getModMatrix().getWriteRegister(MODSRC::PADX),
                                         processor.getModMatrix().getWriteRegister(MODSRC::PADY)));

    String tmpXYPad = String("XYPAD");
    xyPadCommandId = tmpXYPad.hashCode(); //Create Identifier
    xyPad->setName(tmpXYPad);
    xyPad->setUpdateCommandId(xyPadCommandId);

    this->initialiseDisplayLabel();
    this->initialiseEffectContainer();
    this->initialiseModulationContainer();

    //[/Constructor_pre]
    //[UserPreSize]

    // Preset Buttons
    addAndMakeVisible (loadPresetButton = new TextButton ("savePresetButton"));
    loadPresetButton->setButtonText (TRANS("load"));
    loadPresetButton->addListener (this);
    loadPresetButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    loadPresetButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    loadPresetButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (savePresetButton = new TextButton ("new button"));
    savePresetButton->setButtonText (TRANS("save"));
    savePresetButton->addListener (this);
    savePresetButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    savePresetButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    savePresetButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    // Main Volume Sliders
    addAndMakeVisible (InputVolumeSlider = new Slider ("Input"));
    InputVolumeSlider->setRange (0.01, 2, 0);
    InputVolumeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    InputVolumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    addAndMakeVisible (mainVolumeSlider = new Slider ("mainVolume"));
    mainVolumeSlider->setRange (-1, 1, 0.001);
    mainVolumeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    mainVolumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    //[/UserPreSize]

    setSize (895, 600);


    //[Constructor] You can add your own custom stuff here..

    //Restore XY Pad Values
    if(processor.shapesInitialised()){
        xyPad->restoreFromString(processor.getUIState(AkatekoAudioProcessor::xyPadId));
    }

    // Set Labels
    presetLabel.setText("Akateko", sendNotificationSync);
    paramLabel.setText("Ready", sendNotificationSync);

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
    //[/UserVariables]


    //[/Destructor]
}

//==============================================================================
void AkatekoAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colours::black);
    g.fillRoundedRectangle (0.0f, 0.0f, 895.0f, 600.0f, 4.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (5.0f, 5.0f, 885.0f, 50.0f, 4.000f);

    g.setColour (Colour (0xff4e4e4e));
    g.fillRoundedRectangle (5.0f, 330.0f, 885.0f, 265.0f, 4.000f);

    g.setColour (Colour (0xff4e4e4e));
    g.fillRoundedRectangle (5.0f, 60.0f, 885.0f, 265.0f, 4.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (10.0f, 65.0f, 345.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (10.0f, 65.0f, 345.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (365.0f, 65.0f, 255.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (365.0f, 65.0f, 255.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (630.0f, 65.0f, 255.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (630.0f, 65.0f, 255.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (10.0f, 335.0f, 610.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (10.0f, 335.0f, 610.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (630.0f, 335.0f, 255.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (630.0f, 335.0f, 255.0f, 255.0f, 2.000f, 2.000f);

    g.setGradientFill (ColourGradient (Colour (0xff6f6d6d),
                                       88.0f, 16.0f,
                                       Colours::white,
                                       88.0f, 64.0f,
                                       false));
    g.setFont (Font ("Good Times", 46.00f, Font::plain));
    g.drawText (TRANS("AKATEKO"),
                50, 16, 258, 33,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 16.00f, Font::plain));
    g.drawText (TRANS("WAVESHAPER"),
                377, 67, 141, 21,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AkatekoAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //waveShaper->setBounds (365, 65, 255, 255);
    //[/UserPreResize]

    lfoOneButton->setBounds (24, 352, 70, 30);
    lfoTwoButton->setBounds (24, 384, 70, 30);
    envOneButton->setBounds (24, 416, 70, 30);
    envTwoButton->setBounds (24, 448, 70, 30);
    stepButton->setBounds (24, 480, 70, 30);
    modulationButton->setBounds (24, 512, 70, 30);
    loadPresetButton->setBounds (320, 8, 32, 20);
    savePresetButton->setBounds (320, 32, 32, 20);
    InputVolumeSlider->setBounds (10, 11, 40, 40);
    waveShaper->setBounds (365, 65, 255, 255);
    filter->setBounds (10, 65, 345, 255);
    xyPad->setBounds (635, 340, 246, 246);
    modMatrix->setBounds (108, 343, 505, 240);
    fxContainer->setBounds (630, 65, 255, 255);
    mainVolumeSlider->setBounds (844, 11, 40, 40);
    presetButton->setBounds (24, 544, 70, 30);
    //[UserResized] Add your own custom resize handling here..

    presetLabel.setBounds (368, 9, 248, 18);
    paramLabel.setBounds (368, 33, 248, 18);

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
        lfoOneButton->setColour(TextButton::buttonColourId, activeColour);
        lfoOne->setVisible(true);
        this->repaint();
        //[/UserButtonCode_lfoOneButton]
    }
    else if (buttonThatWasClicked == lfoTwoButton)
    {
        //[UserButtonCode_lfoTwoButton] -- add your button handler code here..

        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = LFO2;
        lfoTwoButton->setColour(TextButton::buttonColourId, activeColour);
        lfoTwo->setVisible(true);
        this->repaint();
        //[/UserButtonCode_lfoTwoButton]
    }
    else if (buttonThatWasClicked == envOneButton)
    {
        //[UserButtonCode_envOneButton] -- add your button handler code here..

        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = ENV1;
        envOneButton->setColour(TextButton::buttonColourId, activeColour);
        envOne->setVisible(true);
        this->repaint();
        //[/UserButtonCode_envOneButton]
    }
    else if (buttonThatWasClicked == envTwoButton)
    {
        //[UserButtonCode_envTwoButton] -- add your button handler code here..
        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = ENV2;
        envTwoButton->setColour(TextButton::buttonColourId, activeColour);
        envTwo->setVisible(true);
        this->repaint();
        //[/UserButtonCode_envTwoButton]
    }
    else if (buttonThatWasClicked == stepButton)
    {
        //[UserButtonCode_stepButton] -- add your button handler code here..
        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = SSEQ;
        stepButton->setColour(TextButton::buttonColourId, activeColour);
        stepSeq->setVisible(true);
        this->repaint();
        //[/UserButtonCode_stepButton]
    }
    else if (buttonThatWasClicked == modulationButton)
    {
        //[UserButtonCode_modulationButton] -- add your button handler code here..
        hideModulationComponent(activeModulation); //hide previous modulation component
        activeModulation = MODM;
        modulationButton->setColour(TextButton::buttonColourId, activeColour);
        modMatrix->setVisible(true);
        this->repaint();
        //[/UserButtonCode_modulationButton]
    }
    else if (buttonThatWasClicked == presetButton)
    {
        //[UserButtonCode_presetButton] -- add your button handler code here..
        hideModulationComponent(activeModulation);
        activeModulation = PRST;
        presetButton->setColour(TextButton::buttonColourId, activeColour);
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
    }
    //[/UserCode_handleCommandMessage]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AkatekoAudioProcessorEditor::initialiseDisplayLabel(){
    addAndMakeVisible(&presetLabel);
    presetLabel.setFont (Font (15.00f, Font::plain));
    presetLabel.setJustificationType (Justification::centredLeft);
    presetLabel.setEditable (false, false, false);
    presetLabel.setColour (Label::backgroundColourId, Colour (0x7f2d0000));
    presetLabel.setColour (Label::textColourId, Colour (0xffdfdfdf));
    presetLabel.setColour (TextEditor::textColourId, Colours::black);
    presetLabel.setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (&paramLabel);
    paramLabel.setFont (Font (15.00f, Font::plain));
    paramLabel.setJustificationType (Justification::centredLeft);
    paramLabel.setEditable (false, false, false);
    paramLabel.setColour (Label::backgroundColourId, Colour (0x7f2d0000));
    paramLabel.setColour (Label::textColourId, Colour (0xffdfdfdf));
    paramLabel.setColour (TextEditor::textColourId, Colours::black);
    paramLabel.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

/*
 * preAmpEnable : ParamID = "PREN"
 * preAmpColume : ParamID = "PRVL"
 * preAmpDrive  : ParamID = "PRDR"
 */

void AkatekoAudioProcessorEditor::initialiseModulationContainer(){
    // LFO One Button
    addAndMakeVisible (lfoOneButton = new TextButton ("lfoOne"));
    lfoOneButton->setButtonText (TRANS("LFO 1"));
    lfoOneButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    lfoOneButton->addListener (this);
    lfoOneButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    lfoOneButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    lfoOneButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));
    // LFO Two Button
    addAndMakeVisible (lfoTwoButton = new TextButton ("lfoTwo"));
    lfoTwoButton->setButtonText (TRANS("LFO 2"));
    lfoTwoButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    lfoTwoButton->addListener (this);
    lfoTwoButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    lfoTwoButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    lfoTwoButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));
    // ENV One Button
    addAndMakeVisible (envOneButton = new TextButton ("envOneButton"));
    envOneButton->setButtonText (TRANS("ENV 1"));
    envOneButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    envOneButton->addListener (this);
    envOneButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    envOneButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    envOneButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));
    // ENV Two Button
    addAndMakeVisible (envTwoButton = new TextButton ("envTwo"));
    envTwoButton->setButtonText (TRANS("ENV 2"));
    envTwoButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    envTwoButton->addListener (this);
    envTwoButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    envTwoButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    envTwoButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));
    // Step Button
    addAndMakeVisible (stepButton = new TextButton ("stepButton"));
    stepButton->setButtonText (TRANS("STEP"));
    stepButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    stepButton->addListener (this);
    stepButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    stepButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    stepButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));
    // Modulation Button
    addAndMakeVisible (modulationButton = new TextButton ("modButton"));
    modulationButton->setButtonText (TRANS("MOD"));
    modulationButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    modulationButton->addListener (this);
    modulationButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    modulationButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    modulationButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));
    // Preset Button
    addAndMakeVisible (presetButton = new TextButton ("preset"));
    presetButton->setButtonText (TRANS("PRESET"));
    presetButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    presetButton->addListener (this);
    presetButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    presetButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    presetButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    currentBPM = processor.getBPM();
    beatDivisor.calculateBeatDivisions(currentBPM);
    /* Gather Parameters and ParamIds */
    addChildComponent(lfoOne = new LFOComponent("LFO 1", processor, 1, paramLabel));
    lfoOne->setBeatDivisionStrings(beatDivisor.getStringArray());
    lfoOne->setBeatDivisionValues(beatDivisor.getValues());
    lfoOne->initFrequencySlider();

    addChildComponent(lfoTwo = new LFOComponent("LFO 2", processor, 2, paramLabel));
    lfoTwo->setBeatDivisionStrings(beatDivisor.getStringArray());
    lfoTwo->setBeatDivisionValues(beatDivisor.getValues());
    lfoTwo->initFrequencySlider();

    addChildComponent(envOne = new EnvelopeComponent("ENV 1", processor, 1, paramLabel));
    envOne->setBeatDivisionStrings(beatDivisor.getStringArray());
    envOne->setBeatDivisionValues(beatDivisor.getValues());
    envOne->initDurationSlider();

    addChildComponent(envTwo = new EnvelopeComponent("ENV 2", processor, 2, paramLabel));
    envTwo->setBeatDivisionStrings(beatDivisor.getStringArray());
    envTwo->setBeatDivisionValues(beatDivisor.getValues());
    envTwo->initDurationSlider();

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

    /* setCurrentShapes, only for testing when the preset
     * mechanism is properly implemented load up the correct
     * preset and everything should be set accordingly
     * by a default preset or the current selected preset
     * of the project.
     */
    if(!processor.shapesInitialised()){
       std::cout << "Entered start up" << std::endl;

       processor.setShapes(lfoOne->getCurrentShapes(),AkatekoAudioProcessor::LFO1Id);
       processor.setShapes(lfoTwo->getCurrentShapes(),AkatekoAudioProcessor::LFO2Id);
       processor.setShapes(waveShaper->getCurrentShapes(),AkatekoAudioProcessor::WaveShapeId);

       processor.setUIState(envOne->getUIState(), AkatekoAudioProcessor::Envelope1Id);
       processor.setUIState(envTwo->getUIState(), AkatekoAudioProcessor::Envelope2Id);
       processor.setUIState(stepSeq->getUIState(), AkatekoAudioProcessor::StepSequencerId);

       processor.setShapesInitialised(true);
    } else {
        /* If the editor is recreated retrieve stored shapes */
       lfoOne->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::LFO1Id));
       lfoTwo->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::LFO2Id));

       envOne->setUIState(processor.getUIState(AkatekoAudioProcessor::Envelope1Id));
       envTwo->setUIState(processor.getUIState(AkatekoAudioProcessor::Envelope2Id));

       stepSeq->setUIState(processor.getUIState(AkatekoAudioProcessor::StepSequencerId));
       waveShaper->setCurrentShapes(processor.getShapes(AkatekoAudioProcessor::WaveShapeId));
     //  xyPad->restoreFromString(processor.getUIState(AkatekoAudioProcessor::xyPadId));
    }
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
    addAndMakeVisible (loadPresetButton = new TextButton ("loadPresetButton"));
    loadPresetButton->setButtonText (TRANS("load"));
    loadPresetButton->addListener (this);
    loadPresetButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    loadPresetButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    loadPresetButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (savePresetButton = new TextButton ("savePresetButton"));
    savePresetButton->setButtonText (TRANS("save"));
    savePresetButton->addListener (this);
    savePresetButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    savePresetButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    savePresetButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));
}
/* If a current modulation Component is visible, hide it */
void AkatekoAudioProcessorEditor::hideModulationComponent(int modComponentId){
    switch(modComponentId){
        case LFO1:
            lfoOneButton->setColour(TextButton::buttonColourId, buttonColour);
            lfoOne->setVisible(false);
            break;
        case LFO2:
            lfoTwoButton->setColour(TextButton::buttonColourId, buttonColour);
            lfoTwo->setVisible(false);
            break;
        case ENV1:
            envOneButton->setColour(TextButton::buttonColourId, buttonColour);
            envOne->setVisible(false);
            break;
        case ENV2:
            envTwoButton->setColour(TextButton::buttonColourId, buttonColour);
            envTwo->setVisible(false);
            break;
        case SSEQ:
            stepButton->setColour(TextButton::buttonColourId, buttonColour);
            stepSeq->setVisible(false);
            break;
        case MODM:
            modulationButton->setColour(TextButton::buttonColourId, buttonColour);
            modMatrix->setVisible(false);
            break;
        case PRST:
            presetButton->setColour(TextButton::buttonColourId, buttonColour);
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
  <TEXTBUTTON name="lfoOne" id="3be90a7d038cacae" memberName="lfoOneButton"
              virtualName="" explicitFocusOrder="0" pos="24 352 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="LFO 1" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="lfoTwo" id="8a5cdd3b15902e28" memberName="lfoTwoButton"
              virtualName="" explicitFocusOrder="0" pos="24 384 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="LFO 2" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="envOneButton" id="bdaf7e11ea200503" memberName="envOneButton"
              virtualName="" explicitFocusOrder="0" pos="24 416 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="ENV 1" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="envTwo" id="18bd76a767b49aa8" memberName="envTwoButton"
              virtualName="" explicitFocusOrder="0" pos="24 448 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="ENV 2" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="stepButton" id="161d2f193b2d1e9c" memberName="stepButton"
              virtualName="" explicitFocusOrder="0" pos="24 480 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="STEP" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="modButton" id="411aa6c03fa5468d" memberName="modulationButton"
              virtualName="" explicitFocusOrder="0" pos="24 512 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="MOD" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="savePresetButton" id="2a6f9b9c2a0b2c96" memberName="loadPresetButton"
              virtualName="" explicitFocusOrder="0" pos="320 8 32 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="load" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="48fac67238ae71fe" memberName="savePresetButton"
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
  <TEXTBUTTON name="preset" id="6d9d037c6aab172b" memberName="presetButton"
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
