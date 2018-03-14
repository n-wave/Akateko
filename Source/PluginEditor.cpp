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

#include "PluginEditor.h"

using namespace std::placeholders;

//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
AkatekoAudioProcessorEditor::AkatekoAudioProcessorEditor (AkatekoAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    buttonColour = Colour(0x73707070);
    activeColour = Colour(0x7f007f7f);

    addAndMakeVisible(filter = new FilterComponent("Filter", processor, label));
    addAndMakeVisible(waveShaper = new WaveShapeComponent("WaveShape", processor, label));

    this->initialiseDisplayLabel();
    this->initialisePreAmpSection();
    this->initialiseModulationContainer();

    addAndMakeVisible (xyPad = new XYPad(label, xAxis, yAxis));
    xyPad->setName ("XY Pad : ");

    /*
     *
     *     extern const char*   hexagonpattern_png;
    const int            hexagonpattern_pngSize = 44063;
     */

    //[/Constructor_pre]

    //[UserPreSize]
    addAndMakeVisible (effectToggleButton = new ToggleButton ("effectToggle"));
    effectToggleButton->setButtonText (String());

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

    addAndMakeVisible (mainVolumeSlider = new Slider ("mainVolume"));
    mainVolumeSlider->setRange (-1, 1, 0.001);
    mainVolumeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    mainVolumeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    addAndMakeVisible (bypassToggle = new ToggleButton ("bypassToggle"));
    bypassToggle->setButtonText (String());
    bypassToggle->setToggleState (true, dontSendNotification);

    //[/UserPreSize]

    setSize (895, 600);


    //[Constructor] You can add your own custom stuff here..

    //this->bindAudioProcessorParameters();
    //[/Constructor]
}

AkatekoAudioProcessorEditor::~AkatekoAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    //save states to the processor should be the very first thing to be called
    //[/Destructor_pre]

    preDriveSlider = nullptr;
    preAmplitudeSlider = nullptr;
    preToggleButton = nullptr;
    lfoOneButton = nullptr;
    effectToggleButton = nullptr;
    lfoTwoButton = nullptr;
    envOneButton = nullptr;
    envTwoButton = nullptr;
    stepButton = nullptr;
    modulationButton = nullptr;
    loadPresetButton = nullptr;
    savePresetButton = nullptr;
    mainVolumeSlider = nullptr;
    bypassToggle = nullptr;
    waveShaper = nullptr;
    filter = nullptr;
    xyPad = nullptr;


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
    g.fillRoundedRectangle (10.0f, 65.0f, 80.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (10.0f, 65.0f, 80.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (100.0f, 65.0f, 255.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (100.0f, 65.0f, 255.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (365.0f, 65.0f, 255.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (365.0f, 65.0f, 255.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (630.0f, 65.0f, 255.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (630.0f, 65.0f, 255.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (10.0f, 335.0f, 80.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (10.0f, 335.0f, 80.0f, 255.0f, 2.000f, 2.000f);

    g.setColour (Colour (0xff323232));
    g.fillRoundedRectangle (100.0f, 335.0f, 520.0f, 255.0f, 2.000f);

    g.setColour (Colours::black);
    g.drawRoundedRectangle (100.0f, 335.0f, 520.0f, 255.0f, 2.000f, 2.000f);

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
                10, 16, 258, 30,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 16.00f, Font::plain));
    g.drawText (TRANS("PRE"),
                17, 67, 67, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 16.00f, Font::plain));
    g.drawText (TRANS("WAVESHAPER"),
                377, 67, 141, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 16.00f, Font::plain));
    g.drawText (TRANS("FX"),
                647, 67, 33, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("AMP"),
                13, 293, 57, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DRIVE"),
                21, 181, 57, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 16.00f, Font::plain));
    g.drawText (TRANS("FILTER"),
                113, 67, 73, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("FREQ"),
                103, 299, 57, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("Q"),
                167, 299, 19, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("FREQ"),
                231, 299, 57, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("Q"),
                303, 299, 19, 21,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AkatekoAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //waveShaper->setBounds (365, 65, 255, 255);
    //[/UserPreResize]

    preDriveSlider->setBounds (12, 112, 75, 75);
    preAmplitudeSlider->setBounds (12, 216, 75, 75);
    preToggleButton->setBounds (12, 64, 24, 24);
    lfoOneButton->setBounds (15, 340, 70, 30);
    effectToggleButton->setBounds (632, 64, 24, 24);
    lfoTwoButton->setBounds (16, 370, 70, 30);
    envOneButton->setBounds (16, 400, 70, 30);
    envTwoButton->setBounds (16, 430, 70, 30);
    stepButton->setBounds (16, 460, 70, 30);
    modulationButton->setBounds (16, 490, 70, 30);
    loadPresetButton->setBounds (768, 8, 40, 20);
    savePresetButton->setBounds (768, 32, 40, 20);
    mainVolumeSlider->setBounds (816, 8, 50, 50);
    bypassToggle->setBounds (864, 32, 24, 24);
    waveShaper->setBounds (365, 65, 255, 255);
    filter->setBounds (100, 65, 255, 255);
    xyPad->setBounds (635, 340, 245, 245);
    //[UserResized] Add your own custom resize handling here..

    lfoOne->setBounds(102, 338, 515, 250);
    lfoTwo->setBounds(102, 338, 515, 250);
    envOne->setBounds(102, 338, 515, 250);
    envTwo->setBounds(102, 338, 515, 250);
    stepSeq->setBounds(102, 338, 515, 250);
    modMatrix->setBounds(102, 338, 515, 250);

    label.setBounds (630, 10, 145, 40);

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
    else if (buttonThatWasClicked == loadPresetButton)
    {
        //[UserButtonCode_loadPresetButton] -- add your button handler code here..
        //[/UserButtonCode_loadPresetButton]
    }
    else if (buttonThatWasClicked == savePresetButton)
    {
        //[UserButtonCode_savePresetButton] -- add your button handler code here..
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


    //[/UserCode_handleCommandMessage]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AkatekoAudioProcessorEditor::initialiseDisplayLabel(){
    addAndMakeVisible (&label);
    label.setFont (Font (13.00f, Font::plain));
    label.setJustificationType (Justification::centredLeft);
    label.setEditable (false, false, false);
    label.setColour (Label::backgroundColourId, Colour (0xffb4b4b4));
    label.setColour (TextEditor::textColourId, Colours::black);
    label.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

/*
 * preAmpEnable : ParamID = "PREN"
 * preAmpColume : ParamID = "PRVL"
 * preAmpDrive  : ParamID = "PRDR"
 */

void AkatekoAudioProcessorEditor::initialisePreAmpSection(){
    vector<int> paramIndices = processor.getParameterIndices(AkatekoAudioProcessor::PreAmpId);
    StringArray paramName = processor.getParameterIds(AkatekoAudioProcessor::PreAmpId);
    int nrOfParameters = paramIndices.size();

    if(nrOfParameters == 3){
        addAndMakeVisible(preToggleButton = new ParamToggle (paramName[0],
                                                            *processor.getParameters().getUnchecked(paramIndices[0]),
                                                            label));

        addAndMakeVisible (preDriveSlider = new ParamSlider (paramName[1],
                                                             *processor.getParameters().getUnchecked(paramIndices[1]),
                                                             label));

        addAndMakeVisible (preAmplitudeSlider = new ParamSlider (paramName[2],
                                                                 *processor.getParameters().getUnchecked(paramIndices[2]),
                                                                 label));
    } else {
        addAndMakeVisible(preToggleButton = new ToggleButton("Pre"));
        addAndMakeVisible (preDriveSlider = new Slider ("preDrive"));
        addAndMakeVisible (preAmplitudeSlider = new Slider ("preAmplitude"));

        std::cerr << "AkatekoAudioProcessorEditor::initialisePreAmpSection" << std::endl;
        std::cerr << "/---------------Parameters not bound---------------/" << std::endl;
    }

    preDriveSlider->setRange (0, 1, 0.001);
    preDriveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    preDriveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    preAmplitudeSlider->setRange (0, 1, 0.001);
    preAmplitudeSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    preAmplitudeSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
}

void AkatekoAudioProcessorEditor::initialiseModulationContainer(){
    addAndMakeVisible (lfoOneButton = new TextButton ("lfoOne"));
    lfoOneButton->setButtonText (TRANS("LFO 1"));
    lfoOneButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    lfoOneButton->addListener (this);
    lfoOneButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    lfoOneButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    lfoOneButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (lfoTwoButton = new TextButton ("lfoTwo"));
    lfoTwoButton->setButtonText (TRANS("LFO 2"));
    lfoTwoButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    lfoTwoButton->addListener (this);
    lfoTwoButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    lfoTwoButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    lfoTwoButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (envOneButton = new TextButton ("envOneButton"));
    envOneButton->setButtonText (TRANS("ENV 1"));
    envOneButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    envOneButton->addListener (this);
    envOneButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    envOneButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    envOneButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (envTwoButton = new TextButton ("envTwo"));
    envTwoButton->setButtonText (TRANS("ENV 2"));
    envTwoButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    envTwoButton->addListener (this);
    envTwoButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    envTwoButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    envTwoButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (stepButton = new TextButton ("stepButton"));
    stepButton->setButtonText (TRANS("STEP"));
    stepButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    stepButton->addListener (this);
    stepButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    stepButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    stepButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (modulationButton = new TextButton ("modButton"));
    modulationButton->setButtonText (TRANS("MOD"));
    modulationButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    modulationButton->addListener (this);
    modulationButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    modulationButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    modulationButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    /* Gather Parameters and ParamIds */    

    addChildComponent(lfoOne = new LFOComponent("LFO 1", processor, 1, label));
    addChildComponent(lfoTwo = new LFOComponent("LFO 1", processor, 2, label));
    addChildComponent(envOne = new EnvelopeComponent("ENV 1", processor, 1, label));
    addChildComponent(envTwo = new EnvelopeComponent("ENV 2", processor, 2, label));
    addChildComponent(stepSeq = new StepSequencerComponent("SSEQ", processor, label));
    addChildComponent(modMatrix = new ModulationMatrixComponent(processor.getModMatrix()));

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
    }
}

void AkatekoAudioProcessorEditor::initialiseEffectContainer(){

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
    }
    this->repaint();
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
    <ROUNDRECT pos="10 65 80 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="100 65 255 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="365 65 255 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="630 65 255 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="10 335 80 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="100 335 520 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <ROUNDRECT pos="630 335 255 255" cornerSize="2" fill="solid: ff323232" hasStroke="1"
               stroke="2, mitered, butt" strokeColour="solid: ff000000"/>
    <TEXT pos="10 16 258 30" fill="linear: 88 16, 88 64, 0=ff6f6d6d, 1=ffffffff"
          hasStroke="0" text="AKATEKO" fontname="Good Times" fontsize="46"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="17 67 67 21" fill="solid: ffaaaaaa" hasStroke="0" text="PRE"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
    <TEXT pos="377 67 141 21" fill="solid: ffaaaaaa" hasStroke="0" text="WAVESHAPER"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
    <TEXT pos="647 67 33 21" fill="solid: ffaaaaaa" hasStroke="0" text="FX"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
    <TEXT pos="13 293 57 21" fill="solid: ffaaaaaa" hasStroke="0" text="AMP"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="21 181 57 21" fill="solid: ffaaaaaa" hasStroke="0" text="DRIVE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="113 67 73 21" fill="solid: ffaaaaaa" hasStroke="0" text="FILTER"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
    <TEXT pos="103 299 57 21" fill="solid: ffaaaaaa" hasStroke="0" text="FREQ"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="167 299 19 21" fill="solid: ffaaaaaa" hasStroke="0" text="Q"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="231 299 57 21" fill="solid: ffaaaaaa" hasStroke="0" text="FREQ"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="303 299 19 21" fill="solid: ffaaaaaa" hasStroke="0" text="Q"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="preDrive" id="c7d5b2ed0904b42b" memberName="preDriveSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="12 112 75 75"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <SLIDER name="preAmplitude" id="d05f9a7eb4a16aef" memberName="preAmplitudeSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="12 216 75 75"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <TOGGLEBUTTON name="pretoggle" id="af1a37888fa46c92" memberName="preToggleButton"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="12 64 24 24"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="lfoOne" id="3be90a7d038cacae" memberName="lfoOneButton"
              virtualName="" explicitFocusOrder="0" pos="15 340 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="LFO 1" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="effectToggle" id="f99ec011c2a826e1" memberName="effectToggleButton"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="632 64 24 24"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="lfoTwo" id="8a5cdd3b15902e28" memberName="lfoTwoButton"
              virtualName="" explicitFocusOrder="0" pos="16 370 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="LFO 2" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="envOneButton" id="bdaf7e11ea200503" memberName="envOneButton"
              virtualName="" explicitFocusOrder="0" pos="16 400 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="ENV 1" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="envTwo" id="18bd76a767b49aa8" memberName="envTwoButton"
              virtualName="" explicitFocusOrder="0" pos="16 430 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="ENV 2" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="stepButton" id="161d2f193b2d1e9c" memberName="stepButton"
              virtualName="" explicitFocusOrder="0" pos="16 460 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="STEP" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="modButton" id="411aa6c03fa5468d" memberName="modulationButton"
              virtualName="" explicitFocusOrder="0" pos="16 490 70 30" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="MOD" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="savePresetButton" id="2a6f9b9c2a0b2c96" memberName="loadPresetButton"
              virtualName="" explicitFocusOrder="0" pos="768 8 40 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="load" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="48fac67238ae71fe" memberName="savePresetButton"
              virtualName="" explicitFocusOrder="0" pos="768 32 40 20" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="mainVolume" id="6336a7102808882a" memberName="mainVolumeSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="816 8 50 50"
          min="-1" max="1" int="0.0010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <TOGGLEBUTTON name="bypassToggle" id="39a689422aa8782c" memberName="bypassToggle"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="864 32 24 24"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="1"/>
  <GENERICCOMPONENT name="waveShaper" id="97d7ede6e23b1658" memberName="waveShaper"
                    virtualName="WaveShapeComponent" explicitFocusOrder="0" pos="365 65 255 255"
                    class="Component" params="&quot;WSHP&quot;"/>
  <GENERICCOMPONENT name="filter" id="db18492e2bda189" memberName="filter" virtualName="FilterComponent"
                    explicitFocusOrder="0" pos="100 65 255 255" class="Component"
                    params="&quot;FILT&quot;"/>
  <GENERICCOMPONENT name="xyPad" id="fc769d97819c849c" memberName="XY Pad : " virtualName="XYPad"
                    explicitFocusOrder="0" pos="635 340 246 246" class="Component"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
