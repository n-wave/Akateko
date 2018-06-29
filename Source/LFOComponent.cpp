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
#include <iostream>
//[/Headers]

#include "LFOComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using akateko::IgnoreRightClick;
using akateko::frequency_ratios;
using akateko::initMidiRow;

/*
 *   int commandUpdate;
 *   int commandReset;
 */

//[/MiscUserDefs]

//==============================================================================
LFOComponent::LFOComponent (const String &name, AkatekoAudioProcessor &p, int lfo, Label &label)
    : Component::Component(name),
      processor(p),
      defaultShape(false),
      activeShape(nullptr),
      oneShot(nullptr),
      sync(nullptr),
      lfoNumber(0),
      freqMidiEnabled(false),
      beatsPerMinute(120.0),
      buttonColour(Colour(0x3F10A0A0)),
      activeColour(Colour(0x9F20BFaF)),
      labelRef(label)
{
    //[Constructor_pre] You can add your own custom stuff here..
    StringArray paramNames;
    bool proceed = false;

    if(lfo == 1){
        paramIndices = p.getParameterIndices(AkatekoAudioProcessor::LFO1Id);
        paramNames = p.getParameterIds(AkatekoAudioProcessor::LFO1Id);
        lfoNumber = lfo;
        proceed = true;
    } else if(lfo == 2) {
        paramIndices = p.getParameterIndices(AkatekoAudioProcessor::LFO2Id);
        paramNames = p.getParameterIds(AkatekoAudioProcessor::LFO2Id);
        lfoNumber = lfo;
        proceed = true;
    }

    int nrOfParams = paramIndices.size();

    if(nrOfParams == 8 && proceed){
        /*
         * Collect Audio Processor Parameters based on the needed
         * Indices located in the parent Processor object
         * The paramIds are used for supplying the right text to
         * the label. This circumvent the upcasts and running
         * through the whole Parameter Array, searching for
         * the right Parameters.
         *
         * LFO Parameters
         *
         * paramIndices[0] : LFO Enable
         * paramIndices[1] : LFO One Shot
         * paramIndices[2] : LFO Sync
         * paramIndices[3] : LFO Frequency
         * paramIndices[4] : LFO Phase
         * paramIndices[5] : LFO PWM
         * paramIndices[6] : LFO Shape
         * paramIndices[7] : LFO Smooth
         *
         */

        // Follow Midi Table Layout
        requestMenuIds[0] = paramNames[0].hash();
        requestMenuIds[1] = String("One Shot").hash();
        requestMenuIds[2] = String("Free").hash();
        requestMenuIds[3] = String("Sync").hash();
        requestMenuIds[4] = paramNames[3].hash();
        requestMenuIds[5] = paramNames[4].hash();
        requestMenuIds[6] = paramNames[5].hash();
        requestMenuIds[7] = String("Shape One").hash();
        requestMenuIds[8] = String("Shape Two").hash();
        requestMenuIds[9] = String("Shape Three").hash();
        requestMenuIds[10] = String("Shape Four").hash();
        requestMenuIds[11] = String("Shape Five").hash();
        requestMenuIds[12] = String("Shape Six").hash();
        requestMenuIds[13] = String("Shape Seven").hash();
        requestMenuIds[14] = String("Shape Eight").hash();
        requestMenuIds[15] = paramNames[7].hash();

        addAndMakeVisible (lfoEnableToggle = new ParamToggle (paramNames[0], *p.getParameters().getUnchecked(paramIndices[0]), label)); //LFO Enable
        oneShot = p.getParameters().getUnchecked(paramIndices[1]);  //LFO OneShot
        sync = p.getParameters().getUnchecked(paramIndices[2]); //LFO Sync
        frequency = p.getParameters().getUnchecked(paramIndices[3]); //Frequency

        addAndMakeVisible (lfoFrequencySlider = new IgnoreRightClick<Slider>(paramNames[3])); //LFO Frequency
        addAndMakeVisible (lfoPhaseSlider = new ParamSlider (paramNames[4], *p.getParameters().getUnchecked(paramIndices[4]), label)); //LFO Phase
        addAndMakeVisible (lfoPWMSlider = new ParamSlider (paramNames[5], *p.getParameters().getUnchecked(paramIndices[5]), label, -1.f, 1.f)); //LFO PWM                
        activeShape = p.getParameters().getUnchecked(paramIndices[6]);
        addAndMakeVisible (lfoSmoothSlider = new ParamSlider (paramNames[7], *p.getParameters().getUnchecked(paramIndices[7]), label, 0.f, 1.0f)); // LFO Smooth


    } else {
        addAndMakeVisible (lfoEnableToggle = new ToggleButton ("lfoEnableToggle"));
        oneShot = nullptr;
        sync = nullptr;
        addAndMakeVisible (lfoFrequencySlider = new IgnoreRightClick<Slider> ("lfoFreq"));
        addAndMakeVisible (lfoPhaseSlider = new Slider ("lfoPhase"));
        addAndMakeVisible (lfoPWMSlider = new Slider ("PWM"));
        addAndMakeVisible (lfoSmoothSlider = new Slider("Smooth"));
        activeShape = nullptr;

        std::cerr << "LFOComponent::LFOComponent : " << lfo << std::endl;
        std::cerr << "/-----Parameters not bound-----/" << std::endl;
    }

    initialiseMIDIStrings();

    // Build Menu
    menu.addItem(1, "learn", true, false, nullptr);
    menu.addSeparator();
    menu.addItem(0xFF, "clear");
    //[/Constructor_pre]

    //[UserPreSize]
    addAndMakeVisible (LFO = new QTableDrawing ("lfo"));
    LFO->setName ("new component");

    LFO->setColour(QTableDrawing::backgroundColourId, Colours::black);
    LFO->setColour(QTableDrawing::outlineColourId, Colour(0xAA70C099));
    LFO->setColour(QTableDrawing::fillColourId, Colour(0xBB70C099));
    LFO->setColour(QTableDrawing::ellipseColourId, Colour(0xFF60B090));
    LFO->setColour(QTableDrawing::highLightColourId, Colour(0x9FA0C9B0));
    LFO->setColour(QTableDrawing::overlayGradientTwoId, Colours::transparentBlack);

    lfoFrequencySlider->setRange (0, 1, 0);
    lfoFrequencySlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lfoFrequencySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoFrequencySlider->addListener (this);

    lfoPhaseSlider->setRange (0, 1, 0);
    lfoPhaseSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lfoPhaseSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoPhaseSlider->addListener (this);

    lfoPWMSlider->setRange (0, 1, 0);
    lfoPWMSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lfoPWMSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoPWMSlider->setDoubleClickReturnValue(true, 0.5);
    lfoPWMSlider->addListener (this);

    lfoSmoothSlider->setRange (0, 1, 0);
    lfoSmoothSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    lfoSmoothSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoSmoothSlider->setDoubleClickReturnValue(true, 0.5);
    lfoSmoothSlider->addListener (this);

    addAndMakeVisible (bufferDisplay = new BufferDisplay(BufferDisplay::DRAW128));
    bufferDisplay->setName ("bufferDisplay");

    lfoEnableToggle->setButtonText (String());
    lfoEnableToggle->addListener (this);

    addAndMakeVisible (nameLabel = new Label ("nameLabel",
                                              TRANS("LFOx")));
    nameLabel->setText(name, dontSendNotification);
    nameLabel->setFont (Font ("Good Times", 15.00f, Font::plain));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (Label::textColourId, Colour(0xFF4A997A));
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    LFO->setFileExtension("lfo");

    /* Buttons*/
    addAndMakeVisible (shapeButtonOne = new IgnoreRightClick<TextButton> ("Shape One"));
    shapeButtonOne->setButtonText (TRANS("1"));
    shapeButtonOne->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonOne->addListener (this);

    addAndMakeVisible (shapeButtonTwo = new IgnoreRightClick<TextButton>  ("Shape Two"));
    shapeButtonTwo->setButtonText (TRANS("2"));
    shapeButtonTwo->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonTwo->addListener (this);

    addAndMakeVisible (shapeButtonThree = new IgnoreRightClick<TextButton>  ("Shape Three"));
    shapeButtonThree->setButtonText (TRANS("3"));
    shapeButtonThree->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonThree->addListener (this);

    addAndMakeVisible (shapeButtonFour = new IgnoreRightClick<TextButton>  ("Shape Four"));
    shapeButtonFour->setButtonText (TRANS("4"));
    shapeButtonFour->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonFour->addListener (this);

    addAndMakeVisible (shapeButtonFive = new IgnoreRightClick<TextButton>  ("Shape Five"));
    shapeButtonFive->setButtonText (TRANS("5"));
    shapeButtonFive->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonFive->addListener (this);

    addAndMakeVisible (shapeButtonSix = new IgnoreRightClick<TextButton>  ("Shape Six"));
    shapeButtonSix->setButtonText (TRANS("6"));
    shapeButtonSix->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonSix->addListener (this);

    addAndMakeVisible (shapeButtonSeven = new IgnoreRightClick<TextButton>  ("Shape Seven"));
    shapeButtonSeven->setButtonText (TRANS("7"));
    shapeButtonSeven->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonSeven->addListener (this);

    addAndMakeVisible (shapeButtonEight = new IgnoreRightClick<TextButton>  ("Shape Eight"));
    shapeButtonEight->setButtonText (TRANS("8"));
    shapeButtonEight->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonEight->addListener (this);

    addAndMakeVisible (loadButton = new TextButton ("loadButton"));
    loadButton->setButtonText (TRANS("load"));
    loadButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    loadButton->addListener (this);

    addAndMakeVisible (clearButton = new TextButton ("clearButton"));
    clearButton->setButtonText (TRANS("clear"));
    clearButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    clearButton->addListener (this);

    addAndMakeVisible (saveButton = new TextButton ("saveButton"));
    saveButton->setButtonText (TRANS("save"));
    saveButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    saveButton->addListener (this);

    addAndMakeVisible (oneShotButton = new IgnoreRightClick<TextButton> ("One Shot"));
    oneShotButton->setTooltip (TRANS("One Shot"));
    oneShotButton->setButtonText (TRANS("shot"));
    oneShotButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    oneShotButton->addListener (this);

    addAndMakeVisible (freeRunningButton = new IgnoreRightClick<TextButton> ("Free"));
    freeRunningButton->setTooltip (TRANS("Free Running\n"));
    freeRunningButton->setButtonText (TRANS("free"));
    freeRunningButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    freeRunningButton->addListener (this);

    addAndMakeVisible (syncButton = new IgnoreRightClick<TextButton> ("Sync"));
    syncButton->setTooltip (TRANS("Sync"));
    syncButton->setButtonText(TRANS("sync"));
    syncButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    syncButton->addListener (this);

    //[/UserPreSize]
    setSize (515, 250);
    //[Constructor] You can add your own custom stuff here..
    bufferDisplay->setSignalColour(Colour(0xBB70C099));
    bufferDisplay->setBackgroundColour(Colour(0xFF000000));
    bufferDisplay->setOutlineColour(Colour(0xAA70C099));
    bufferDisplay->setOverlayColour(Colours::transparentBlack,
                                    Colours::transparentBlack);

    bufferDisplay->drawSingleCycleWaveform(true); // connect the last sample with the first
    bufferDisplay->drawAxis(true);

    /* fill and gather buffer */
    this->initialiseDefaultShapes();

    if(activeShape != nullptr){
        int sIndex = activeShape->getValue();

        if(sIndex < currentShapes.size()){
            LFO->restoreFromString(currentShapes[sIndex]);
            LFO->fillBuffer();
            msmBuffer tmpBuffer = LFO->getBuffer();
            bufferDisplay->setBuffer(tmpBuffer);
        }

        switch(sIndex){
            case 0:
                shapeButtonOne->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 1:
                shapeButtonTwo->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 2:
                shapeButtonThree->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 3:
                shapeButtonFour->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 4:
                shapeButtonFive->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 5:
                shapeButtonSix->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 6:
                shapeButtonSeven->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 7:
                shapeButtonEight->setColour(TextButton::buttonColourId, activeColour);
                break;
        }
    }

    if(oneShot != nullptr){
        const bool tmpOneShot = oneShot->getValue();

        if(tmpOneShot){
            oneShotButton->setColour(TextButton::buttonColourId, activeColour);
        } else {
            oneShotButton->setColour(TextButton::buttonColourId, buttonColour);
        }
    }

    if(sync != nullptr && frequency != nullptr){
        const bool tmpSync = sync->getValue();

        if(!tmpSync && frequency){
            lfoFrequencySlider->setRange(0.0001, 20.0);
            syncButton->setColour(TextButton::buttonColourId, buttonColour);
            freeRunningButton->setColour(TextButton::buttonColourId, activeColour);
        } else {
            lfoFrequencySlider->setRange(0, 23, 1);
            syncButton->setColour(TextButton::buttonColourId, activeColour);
            freeRunningButton->setColour(TextButton::buttonColourId, buttonColour);
        }
    }

    double bpm = processor.getBeatsPerMinute();

    if(bpm < 20.0){
        bpm = 20.0;
    }

    beatsPerMinute = bpm;
    initialiseTimeDivisions();
    calculateTimeDivision(beatsPerMinute);
    //[/Constructor]
}

LFOComponent::~LFOComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    LFO = nullptr;
    lfoPhaseSlider = nullptr;
    lfoFrequencySlider = nullptr;
    lfoPWMSlider = nullptr;
    bufferDisplay = nullptr;
    lfoEnableToggle = nullptr;
    nameLabel = nullptr;
    shapeButtonOne = nullptr;
    shapeButtonTwo = nullptr;
    shapeButtonThree = nullptr;
    shapeButtonFour = nullptr;
    shapeButtonFive = nullptr;
    shapeButtonSix = nullptr;
    shapeButtonSeven = nullptr;
    shapeButtonEight = nullptr;
    loadButton = nullptr;
    clearButton = nullptr;
    saveButton = nullptr;
    oneShotButton = nullptr;
    freeRunningButton = nullptr;
    syncButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    lfoSmoothSlider = nullptr;

    frequency = nullptr;
    activeShape = nullptr;
    oneShot = nullptr;
    sync = nullptr;
    //[/Destructor]
}

//==============================================================================
void LFOComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colour ( Colour(0xFF4A997A)));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("FREQ"),
                262, 210, 51, 30,
                Justification::centred, true);

    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PHASE"),
                316, 210, 75, 30,
                Justification::centred, true);

    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("PWM"),
                376, 210, 78, 30,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    g.setFont (Font ("Good Times", 12.f, Font::plain));
    g.drawText (TRANS("SMOOTH"),
                438, 210, 78, 30,
                Justification::centred, true);

    //[/UserPaint]
}

void LFOComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    LFO->setBounds (5, 5, 240, 240);

    lfoFrequencySlider->setBounds (260, 160, 56, 56);
    lfoPhaseSlider->setBounds (325, 160, 56, 56);

    lfoPWMSlider->setBounds (387, 160, 56, 56);
    lfoSmoothSlider->setBounds(449, 160, 56, 56);

    bufferDisplay->setBounds (392, 24, 114, 114);
    lfoEnableToggle->setBounds (492, 1, 24, 24);
    nameLabel->setBounds (423, 2, 66, 24);
    shapeButtonOne->setBounds (256, 48, 32, 32);
    shapeButtonTwo->setBounds (288, 48, 32, 32);
    shapeButtonThree->setBounds (320, 48, 32, 32);
    shapeButtonFour->setBounds (352, 48, 32, 32);
    shapeButtonFive->setBounds (256, 80, 32, 32);
    shapeButtonSix->setBounds (288, 80, 32, 32);
    shapeButtonSeven->setBounds (320, 80, 32, 32);
    shapeButtonEight->setBounds (352, 80, 32, 32);
    loadButton->setBounds (256, 112, 43, 24);
    clearButton->setBounds (342, 112, 43, 24);
    saveButton->setBounds (299, 112, 43, 24);
    oneShotButton->setBounds (256, 24, 43, 24);
    freeRunningButton->setBounds (299, 24, 43, 24);
    syncButton->setBounds (342, 24, 43, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LFOComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == lfoPhaseSlider)
    {
        //[UserSliderCode_lfoPhaseSlider] -- add your slider handling code here..
        bufferDisplay->setBufferOffset(sliderThatWasMoved->getValue());
        //[/UserSliderCode_lfoPhaseSlider]
    }
    else if (sliderThatWasMoved == lfoFrequencySlider)
    {
        //[UserSliderCode_lfoFrequencySlider] -- add your slider handling code here..
        String tmpName = lfoFrequencySlider->getName();

        if(sync != nullptr){
            const bool tmpSync = sync->getValue();

            if(tmpSync){
                int tmpVal = lfoFrequencySlider->getValue();

                if(tmpVal < beatDivision.size() &&
                   tmpVal < valueBeatDivision.size())
                {
                    float tmpFreq = valueBeatDivision[tmpVal];
                    tmpName += beatDivision[tmpVal];

                    //std::cout << "LFOComponent::lfoFrequency, slider Value : " << tmpFreq << std::endl;
                    frequency->setValue(tmpFreq);
                    //std::cout << "LFOComponent::lfoFrequency, Parameter value : " << tmpFreq << std::endl;
                }
            } else {
                double tmpVal = lfoFrequencySlider->getValue();
                frequency->setValue(tmpVal);
                tmpName += String(tmpVal, 2);
            }
        }

        labelRef.setText(tmpName, dontSendNotification);
        //[/UserSliderCode_lfoFrequencySlider]
    }
    else if (sliderThatWasMoved == lfoPWMSlider)
    {
        //[UserSliderCode_lfoPWMSlider] -- add your slider handling code here..
        const float tmpValue = (lfoPWMSlider->getValue()*1.90)-0.95;

        bufferDisplay->setZeroFilling(tmpValue);
        bufferDisplay->repaint();
        //[/UserSliderCode_lfoPWMSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void LFOComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == lfoEnableToggle)
    {
        //[UserButtonCode_lfoEnableToggle] -- add your button handler code here..
        //[/UserButtonCode_lfoEnableToggle]
    } else if (buttonThatWasClicked == shapeButtonOne)
    {
        //[UserButtonCode_shapeButtonOne] -- add your button handler code here..
        setShape(shapeButtonOne, 0);
        this->repaint();
        //[/UserButtonCode_shapeButtonOne]
    }
    else if (buttonThatWasClicked == shapeButtonTwo)
    {
        //[UserButtonCode_shapeButtonTwo] -- add your button handler code here..
        setShape(shapeButtonTwo, 1);
        this->repaint();
        //[/UserButtonCode_shapeButtonTwo]
    }
    else if (buttonThatWasClicked == shapeButtonThree)
    {
        //[UserButtonCode_shapeButtonThree] -- add your button handler code here..
        setShape(shapeButtonThree, 2);
        this->repaint();
        //[/UserButtonCode_shapeButtonThree]
    }
    else if (buttonThatWasClicked == shapeButtonFour)
    {
        //[UserButtonCode_shapeButtonFour] -- add your button handler code here..
        setShape(shapeButtonFour, 3);
        this->repaint();
        //[/UserButtonCode_shapeButtonFour]
    }
    else if (buttonThatWasClicked == shapeButtonFive)
    {
        //[UserButtonCode_shapeButtonFive] -- add your button handler code here..
        setShape(shapeButtonFive, 4);
        this->repaint();
        //[/UserButtonCode_shapeButtonFive]
    }
    else if (buttonThatWasClicked == shapeButtonSix)
    {
        //[UserButtonCode_shapeButtonSix] -- add your button handler code here..
        setShape(shapeButtonSix, 5);
        this->repaint();
        //[/UserButtonCode_shapeButtonSix]
    }
    else if (buttonThatWasClicked == shapeButtonSeven)
    {
        //[UserButtonCode_shapeButtonSeven] -- add your button handler code here..
        setShape(shapeButtonSeven, 6);
        this->repaint();
        //[/UserButtonCode_shapeButtonSeven]
    }
    else if (buttonThatWasClicked == shapeButtonEight)
    {
        //[UserButtonCode_shapeButtonEight] -- add your button handler code here..
        setShape(shapeButtonEight, 7);
        this->repaint();
        //[/UserButtonCode_shapeButtonEight]
    }
    else if (buttonThatWasClicked == loadButton)
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        LFO->load();
        LFO->fillBuffer();
        LFO->repaint();

        currentBuffer = LFO->getBuffer();
        bufferDisplay->setBuffer(currentBuffer);

        if(activeShape != nullptr){
            const int sIndex = activeShape->getValue();
            const String tmpShape = LFO->toString();

            currentShapes.set(sIndex, tmpShape);

            if(lfoNumber == 1){
                processor.setLowFreqOscOneBuffer(currentBuffer);
                processor.setShape(currentShapes[sIndex], AkatekoAudioProcessor::LFO1Id);
            } else if(lfoNumber == 2){
                processor.setLowFreqOscTwoBuffer(currentBuffer);
                processor.setShape(currentShapes[sIndex], AkatekoAudioProcessor::LFO2Id);
            }
        }

        this->repaint();
        //[/UserButtonCode_loadButton]
    }
    else if (buttonThatWasClicked == clearButton)
    {
        //[UserButtonCode_clearButton] -- add your button handler code here..
        LFO->clear();
        LFO->fillBuffer();
        LFO->repaint();

        currentBuffer = LFO->getBuffer();
        bufferDisplay->setBuffer(currentBuffer);

        if(activeShape != nullptr){
            const int sIndex = activeShape->getValue();
            const String tmpShape = LFO->toString();

            currentShapes.set(sIndex, tmpShape);

            if(lfoNumber == 1){
                processor.setLowFreqOscOneBuffer(currentBuffer);
                processor.setShape(currentShapes[sIndex], AkatekoAudioProcessor::LFO1Id);
            } else if(lfoNumber == 2){
                processor.setLowFreqOscTwoBuffer(currentBuffer);
                processor.setShape(currentShapes[sIndex], AkatekoAudioProcessor::LFO2Id);
            }
        }

        /* update String Array */
        //getParentComponent()->postCommandMessage(commandUpdate);
        this->repaint();
        //[/UserButtonCode_clearButton]
    }
    else if (buttonThatWasClicked == saveButton)
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
        LFO->save();
        //[/UserButtonCode_saveButton]
    }
    else if (buttonThatWasClicked == oneShotButton)
    {
        //[UserButtonCode_oneShotButton] -- add your button handler code here..

        if(oneShot != nullptr){
            const bool tmpOneShot = !oneShot->getValue();
            oneShot->setValue(tmpOneShot);

            if(tmpOneShot){
                oneShotButton->setColour(TextButton::buttonColourId, activeColour);
            } else {
                oneShotButton->setColour(TextButton::buttonColourId, buttonColour);
            }
        }

        //[/UserButtonCode_oneShotButton]
    }
    else if (buttonThatWasClicked == freeRunningButton)
    {
        //[UserButtonCode_freeRunningButton] -- add your button handler code here..
        if(sync != nullptr){
            sync->setValue(false);
            syncButton->setColour(TextButton::buttonColourId, buttonColour);
            freeRunningButton->setColour(TextButton::buttonColourId, activeColour);
        } else {
            std::cerr << "Sync parameter not bound properly" << std::endl;
        }

        lfoFrequencySlider->setRange(0.0001, 20.0);

        if(!paramIndices.empty()){
            processor.setLowFreqOscOneBounds(0.0001, 20.0);

            if(processor.getRegisteredMidi(paramIndices[3]))
            {
                processor.changeMidiRowMinMax(0.0001, 20.0, akateko::MIDI_TO_DOUBLE,  paramIndices[3]);
            }
        }

        if(frequency != nullptr){
            double tmpFreq = frequency->getValue();
            lfoFrequencySlider->setValue(tmpFreq, sendNotificationAsync);
        } else {
            std::cerr << "Frequency parameter not bound properly" << std::endl;
        }

        //[/UserButtonCode_freeRunningButton]
    }
    else if (buttonThatWasClicked == syncButton)
    {
        //[UserButtonCode_syncButton] -- add your button handler code here..
        if(sync != nullptr && frequency != nullptr){
            sync->setValue(true);
            syncButton->setColour(TextButton::buttonColourId, activeColour);
            freeRunningButton->setColour(TextButton::buttonColourId, buttonColour);

            double tmpFreq = frequency->getValue();
            int tmpIndex =  findClosestTimeDivision(tmpFreq);

            tmpFreq = valueBeatDivision[tmpIndex];
            frequency->setValue(tmpFreq);

            lfoFrequencySlider->setRange(0, 23, 1);
            lfoFrequencySlider->setValue(tmpIndex);

            const double minFreq = beatsPerMinute*valueBeatDivision[0];
            const double maxFreq = beatsPerMinute*valueBeatDivision[valueBeatDivision.size()-1];

            if(!paramIndices.empty()){
                processor.setLowFreqOscOneBounds(minFreq, maxFreq);

                if(processor.getRegisteredMidi(paramIndices[3]))
                {
                    processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[3]);
                }
            }
        } else {
            std::cerr << "Parameters are not bounded properly" << std::endl;
        }

        // Change sliderSetting
        //[/UserButtonCode_syncButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void LFOComponent::mouseEnter (const MouseEvent& e)
{
    //[UserCode_mouseEnter] -- Add your code here...
    this->setWantsKeyboardFocus(true);
    //[/UserCode_mouseEnter]
}

void LFOComponent::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    this->setWantsKeyboardFocus(false);
    //[/UserCode_mouseExit]
}

void LFOComponent::modifierKeysChanged (const ModifierKeys& modifiers)
{
    //[UserCode_modifierKeysChanged] -- Add your code here...
    if(modifiers.isCtrlDown()){
        defaultShape = true;
    }
    //[/UserCode_modifierKeysChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


msmBuffer LFOComponent::getBuffer(){
    return currentBuffer;
}

void LFOComponent::initialiseDefaultShapes(){
    defaultShapes.clear();

    defaultShapes.add(akateko::lfo_default_shapes[0]);
    defaultShapes.add(akateko::lfo_default_shapes[1]);
    defaultShapes.add(akateko::lfo_default_shapes[2]);
    defaultShapes.add(akateko::lfo_default_shapes[3]);
    defaultShapes.add(akateko::lfo_default_shapes[4]);
    defaultShapes.add(akateko::lfo_default_shapes[5]);
    defaultShapes.add(akateko::lfo_default_shapes[6]);
    defaultShapes.add(akateko::lfo_default_shapes[7]);

    currentShapes = StringArray(defaultShapes);
}

String LFOComponent::getCurrentShape(){
    String tmpShape = String("m 12 228 q 120 120 228 12");

    if(activeShape != nullptr){
        int sIndex = activeShape->getValue();
        tmpShape = currentShapes[sIndex];
    }
    return tmpShape;
}

void LFOComponent::setCurrentShapes(StringArray shapes){
    if(shapes.size() == currentShapes.size()){
        currentShapes = StringArray(shapes);

        if(activeShape != nullptr){
            const int sIndex = activeShape->getValue();
            const String tmpShape = currentShapes[sIndex];

            LFO->restoreFromString(tmpShape);
            LFO->fillBuffer();
        }
    }
}

StringArray LFOComponent::getCurrentShapes(){
    return currentShapes;
}

void LFOComponent::resetShapButtonColour(int shapeButton){
    switch(shapeButton){
        case 0:
            shapeButtonOne->setColour(TextButton::buttonColourId, buttonColour);
            break;
        case 1:
            shapeButtonTwo->setColour(TextButton::buttonColourId, buttonColour);
            break;
        case 2:
            shapeButtonThree->setColour(TextButton::buttonColourId, buttonColour);
            break;
        case 3:
            shapeButtonFour->setColour(TextButton::buttonColourId, buttonColour);
            break;
        case 4:
            shapeButtonFive->setColour(TextButton::buttonColourId, buttonColour);
            break;
        case 5:
            shapeButtonSix->setColour(TextButton::buttonColourId, buttonColour);
            break;
        case 6:
            shapeButtonSeven->setColour(TextButton::buttonColourId, buttonColour);
            break;
        case 7:
            shapeButtonEight->setColour(TextButton::buttonColourId, buttonColour);
            break;
        default:
            std::cerr << "Wrong button number supplied" << std::endl;
    }
}

void LFOComponent::setShape(Button *buttonThatWasClicked, int shape){
    if(shape >= 0 && shape < 8){
        if(defaultShape){
            currentShapes.set(shape, String(defaultShapes[shape]));
            defaultShape = false;
        }

        LFO->restoreFromString(currentShapes[shape]);
        LFO->fillBuffer();
        LFO->repaint();

        currentBuffer = LFO->getBuffer();
        bufferDisplay->setBuffer(currentBuffer);

        if(activeShape != nullptr){
            resetShapButtonColour(activeShape->getValue());
            buttonThatWasClicked->setColour(TextButton::buttonColourId, activeColour);

            activeShape->setValue(shape);

            String tmpString = getName() + " : " +buttonThatWasClicked->getName();
            labelRef.setText(tmpString, dontSendNotification);
        }

        if(lfoNumber == 1){
            processor.setLowFreqOscOneBuffer(currentBuffer);
            processor.setShape(currentShapes[shape], AkatekoAudioProcessor::LFO1Id);
        } else if(lfoNumber == 2){
            processor.setLowFreqOscTwoBuffer(currentBuffer);
            processor.setShape(currentShapes[shape], AkatekoAudioProcessor::LFO2Id);
        }
    }
}

/*
void LFOComponent::setBeatDivisionStrings(StringArray beatDivStr){
    beatDivision.swapWith(beatDivStr);
}

void LFOComponent::setBeatDivisionValues(std::vector<double> beatDivVal){  
    valueBeatDivision.swap(beatDivVal);
}
*/

/* Call for setting the Beat divisions */
void LFOComponent::initFrequencySlider(){
    if(sync != nullptr &&
       frequency != nullptr &&
       beatDivision.size() != 0 &&
       valueBeatDivision.size() != 0)
    {
        const bool tmpSync = sync->getValue();
        double freq = frequency->getValue();

        if(tmpSync){         
            int tmpIndex = getTimeDivisionIndex(valueBeatDivision, freq);
            lfoFrequencySlider->setValue(tmpIndex, sendNotificationAsync);
        } else {
            lfoFrequencySlider->setValue(freq, sendNotificationAsync);
        }
    } else {
        if(sync == nullptr || frequency == nullptr){
            std::cerr << "Parameters bound not properly : " << getName() << std::endl;
        }

        if(beatDivision.size() == 0 || valueBeatDivision.size() == 0){
            std::cerr << "Beat Divisor tables not set properly" << std::endl;
        }
    }
}

// Call When A preset is loaded

void LFOComponent::updateGui(){
    lfoEnableToggle->postCommandMessage(ParamToggle::update);
    lfoPhaseSlider->postCommandMessage(ParamSlider::update);
    lfoPWMSlider->postCommandMessage(ParamSlider::update);
    lfoSmoothSlider->postCommandMessage(ParamSlider::update);

    // Shape Buttons
    if(activeShape != nullptr){
        int sIndex = activeShape->getValue();

        // Reset All Shapes
        for(int i=0; i<8; i++){
            resetShapButtonColour(i);
        }

        switch(sIndex){
            case 0:
                shapeButtonOne->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 1:
                shapeButtonTwo->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 2:
                shapeButtonThree->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 3:
                shapeButtonFour->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 4:
                shapeButtonFive->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 5:
                shapeButtonSix->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 6:
                shapeButtonSeven->setColour(TextButton::buttonColourId, activeColour);
                break;
            case 7:
                shapeButtonEight->setColour(TextButton::buttonColourId, activeColour);
                break;
            default:
                std::cerr << "LFOComponent::updateGui" << std::endl;
                std::cerr << "Wrong Shape Number supplied" << std::endl;
        }

        if(sIndex < currentShapes.size()){
            LFO->restoreFromString(currentShapes[sIndex]);
            LFO->fillBuffer();
            currentBuffer = LFO->getBuffer();
            bufferDisplay->setBuffer(currentBuffer);

            if(lfoNumber == 1){
                processor.setLowFreqOscOneBuffer(currentBuffer);
            } else if(lfoNumber == 2){
                processor.setLowFreqOscTwoBuffer(currentBuffer);
            }
        }
    }

    // Set One Shot Button
    if(oneShot != nullptr){
        const bool tmpOneShot = oneShot->getValue();

        if(tmpOneShot){
            oneShotButton->setColour(TextButton::buttonColourId, activeColour);
        } else {
            oneShotButton->setColour(TextButton::buttonColourId, buttonColour);
        }
    }

    // Set Sync and Frequency Slider
    if(sync != nullptr && frequency != nullptr){
        const bool tmpSync = sync->getValue();

        if(!tmpSync){
            lfoFrequencySlider->setRange(0.0001, 20.0);
            syncButton->setColour(TextButton::buttonColourId, buttonColour);
            freeRunningButton->setColour(TextButton::buttonColourId, activeColour);


            if( paramIndices.size() == 8 &&
                processor.getRegisteredMidi(paramIndices[3]))
            {
                processor.changeMidiRowMinMax(0.001, 20.0, akateko::MIDI_TO_DOUBLE, paramIndices[3]);
            }

        } else {
            lfoFrequencySlider->setRange(0, 23, 1);
            syncButton->setColour(TextButton::buttonColourId, activeColour);
            freeRunningButton->setColour(TextButton::buttonColourId, buttonColour);

            if( paramIndices.size() == 8 &&
                processor.getRegisteredMidi(paramIndices[3]))
            {
                processor.changeMidiRowMinMax(0, 23, akateko::MIDI_TO_INT, paramIndices[3]);
            }
        }
    }

    initFrequencySlider();
}

void LFOComponent::setLookAndFeel(LookAndFeel *cLaf, LookAndFeel *r1Laf, LookAndFeel *r2Laf)
{
    lfoEnableToggle->setLookAndFeel(cLaf);

    shapeButtonOne->setLookAndFeel(cLaf);
    shapeButtonTwo->setLookAndFeel(cLaf);
    shapeButtonThree->setLookAndFeel(cLaf);
    shapeButtonFour->setLookAndFeel(cLaf);
    shapeButtonFive->setLookAndFeel(cLaf);
    shapeButtonSix->setLookAndFeel(cLaf);
    shapeButtonSeven->setLookAndFeel(cLaf);
    shapeButtonEight->setLookAndFeel(cLaf);
    oneShotButton->setLookAndFeel(cLaf);
    syncButton->setLookAndFeel(cLaf);
    freeRunningButton->setLookAndFeel(cLaf);
    clearButton->setLookAndFeel(cLaf);
    saveButton->setLookAndFeel(cLaf);
    loadButton->setLookAndFeel(cLaf);

    lfoFrequencySlider->setLookAndFeel(r1Laf);
    lfoPhaseSlider->setLookAndFeel(r1Laf);
    lfoPWMSlider->setLookAndFeel(r2Laf);
    lfoSmoothSlider->setLookAndFeel(r1Laf);

    LFO->setLookAndFeel(cLaf);
    menu.setLookAndFeel(cLaf);
}

void LFOComponent::updateFrequencySlider(double bpm){
    if(beatsPerMinute != bpm){
        double mult = beatsPerMinute/bpm;
        beatsPerMinute = bpm;

        initFrequencySlider();
    }
}

void LFOComponent::initialiseMIDIStrings(){
    // LFO One
    lfoOneMIDI.clear();

    lfoOneMIDI.add(" LFO One: Enable");
    lfoOneMIDI.add(" LFO One: OneShot");
    lfoOneMIDI.add(" LFO One: Free");
    lfoOneMIDI.add(" LFO One: Sync");
    lfoOneMIDI.add(" LFO One: Freq");
    lfoOneMIDI.add(" LFO One: Phase");
    lfoOneMIDI.add(" LFO One: PWM");
    lfoOneMIDI.add(" LFO One: Shape 1");
    lfoOneMIDI.add(" LFO One: Shape 2");
    lfoOneMIDI.add(" LFO One: Shape 3");
    lfoOneMIDI.add(" LFO One: Shape 4");
    lfoOneMIDI.add(" LFO One: Shape 5");
    lfoOneMIDI.add(" LFO One: Shape 6");
    lfoOneMIDI.add(" LFO One: Shape 7");
    lfoOneMIDI.add(" LFO One: Shape 8");
    lfoOneMIDI.add(" LFO One: Smooth");

    // LFO Two
    lfoTwoMIDI.clear();

    lfoTwoMIDI.add(" LFO Two: Enable");
    lfoTwoMIDI.add(" LFO Two: OneShot");
    lfoTwoMIDI.add(" LFO Two: Free");
    lfoTwoMIDI.add(" LFO Two: Sync");
    lfoTwoMIDI.add(" LFO Two: Freq");
    lfoTwoMIDI.add(" LFO Two: Phase");
    lfoTwoMIDI.add(" LFO Two: PWM");
    lfoTwoMIDI.add(" LFO Two: Shape 1");
    lfoTwoMIDI.add(" LFO Two: Shape 2");
    lfoTwoMIDI.add(" LFO Two: Shape 3");
    lfoTwoMIDI.add(" LFO Two: Shape 4");
    lfoTwoMIDI.add(" LFO Two: Shape 5");
    lfoTwoMIDI.add(" LFO Two: Shape 6");
    lfoTwoMIDI.add(" LFO Two: Shape 7");
    lfoTwoMIDI.add(" LFO Two: Shape 8");
    lfoTwoMIDI.add(" LFO Two: Smooth");
}

void LFOComponent::handleCommandMessage(int commandId)
{
    if(commandId == QTableDrawing::qtableUpdated){
       // std::cout << LFO->toString() << std::endl;

        LFO->fillBuffer();
        currentBuffer = LFO->getBuffer();
        bufferDisplay->setBuffer(currentBuffer);

        int tmpShape = activeShape->getValue();
        String newShape = LFO->toString();

       // std::cout << newShape << std::endl;

        currentShapes.set(tmpShape, newShape);

        if(lfoNumber == 1){
            processor.setLowFreqOscOneBuffer(currentBuffer);
            processor.setShape(currentShapes[tmpShape], AkatekoAudioProcessor::LFO1Id);
        } else if(lfoNumber == 2){
            processor.setLowFreqOscTwoBuffer(currentBuffer);
            processor.setShape(currentShapes[tmpShape], AkatekoAudioProcessor::LFO2Id);
        }
    } else if(paramIndices.size() == 8 &&
              lfoOneMIDI.size() == 16 &&
              lfoTwoMIDI.size() == 16)
    {
        int index = -1;
        int param = -1;
        int paramIndex = -1;
        int handling = -1;
        double minValue = 0.0;
        double maxValue = 1.0;

        if(commandId == requestMenuIds[0]){ //Enable
            index = menu.show();
            param = 0;
            paramIndex = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[1]){ //One Shot
            index = menu.show();
            param = 1;
            paramIndex = 1;
            handling = akateko::MIDI_TO_INT_TOGGLE;

            if(oneShot->getValue()){
                oneShotButton->setColour(TextButton::buttonColourId, activeColour);
            } else {
                oneShotButton->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[2]){ //*Free
            index = menu.show();
            param = 2;
            paramIndex = 2;
            maxValue = 0.0;
            handling = akateko::MIDI_TO_CONSTANT;

            if(!sync->getValue()){
                freeRunningButton->setColour(TextButton::buttonColourId, activeColour);
            } else {
                freeRunningButton->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[3]){ //*Sync
            index = menu.show();
            param = 3;
            paramIndex = 2;
            minValue = 1;
            handling = akateko::MIDI_TO_CONSTANT;

            if(sync->getValue()){
                syncButton->setColour(TextButton::buttonColourId, activeColour);
            } else {
                syncButton->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[4]){ //Frequency
            index = menu.show();
            param = 4;
            paramIndex = 3;

            //std::cout << "LFO Param Index : " << paramIndices[paramIndex]<< std::endl;

            if(sync->getValue()){
                maxValue = 23;
                handling = akateko::MIDI_TO_INT;
            } else {
                minValue = 0.0001;
                maxValue = 20.0;
                handling = akateko::MIDI_TO_DOUBLE;
            }
        } else if(commandId == requestMenuIds[5]){ //Phase
            index = menu.show();
            param = 5;
            paramIndex = 4;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[6]){ //PWM
            index = menu.show();
            param = 6;
            paramIndex = 5;
            minValue = 0.0;
            maxValue = 1.0;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[7]){ //Shape One
            index = menu.show();
            paramIndex = 6;
            param = 7;
            maxValue = 0;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 0){
                shapeButtonOne->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonOne->setColour(TextButton::buttonColourId, buttonColour);
            }         
        } else if(commandId == requestMenuIds[8]){ //Shape Two
            index = menu.show();
            paramIndex = 6;
            param = 8;
            minValue = 1;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 1){
                shapeButtonTwo->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonTwo->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[9]){ //Shape Three
            index = menu.show();
            paramIndex = 6;
            param = 9;
            minValue = 2;
            maxValue = 2;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 2){
                shapeButtonThree->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonThree->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[10]){ //Shape Four
            index = menu.show();
            paramIndex = 6;
            param = 10;
            minValue = 3;
            maxValue = 3;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 3){
                shapeButtonFour->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonFour->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[11]){ //Shape Five
            index = menu.show();
            paramIndex = 6;
            param = 11;
            minValue = 4;
            maxValue = 4;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 4){
                shapeButtonFive->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonFive->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[12]){ //Shape Six
            index = menu.show();
            paramIndex = 6;
            param = 12;
            minValue = 5;
            maxValue = 5;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 5){
                shapeButtonSix->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonSix->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[13]){ //Shape Seven
            index = menu.show();
            paramIndex = 6;
            param = 13;
            minValue = 6;
            maxValue = 6;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 6){
                shapeButtonSeven->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonSeven->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[14]){ //Shape Eight
            index = menu.show();
            paramIndex = 6;
            param = 14;
            minValue = 7;
            maxValue = 7;
            handling = akateko::MIDI_TO_CONSTANT;

            if(activeShape->getValue() == 7){
                shapeButtonEight->setColour(TextButton::buttonColourId, activeColour);
            } else {
                shapeButtonEight->setColour(TextButton::buttonColourId, buttonColour);
            }
        } else if(commandId == requestMenuIds[15]){ // Smooth
            index = menu.show();
            paramIndex = 7;
            param = 15;
            handling = akateko::MIDI_TO_DOUBLE;
        }

        int section = 1;
        StringArray &tmpArray = lfoOneMIDI;

        if(lfoNumber == 1){
            section = 3;
        } else {
            tmpArray = lfoTwoMIDI;
            section = 4;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[paramIndex], handling,tmpArray[param], section);
            processor.initiateMidiLearn(tmpRow);

        } else if(index == 0xFF){
            if(param == 2 || param == 3 ||
               (param >= 7 && param <= 14))
            {
                processor.removeMidiRow(param, paramIndices[paramIndex]);
            } else {
                processor.removeMidiRow(paramIndices[paramIndex]);
            }

            String message = tmpArray[param] + String(" cleared");
            labelRef.setText(message, sendNotificationAsync);
        }
    }
}

int LFOComponent::findClosestTimeDivision(double freq){
    int result = 0;

    if(freq != 0){
        int nrOfSteps = valueBeatDivision.size();

        if(nrOfSteps != 0){
            bool run = true;
            int index = 1;

            while(run && index < nrOfSteps-1){
                double prev = valueBeatDivision[index-1];
                double val = valueBeatDivision[index];
                double next = valueBeatDivision[index+1];

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

int LFOComponent::getTimeDivisionIndex(vector<double> &values, double freq){
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

void LFOComponent::calculateTimeDivision(double bpm){
    if(bpm > 0.0){
        // Calculate 1/128 Note, multiply with 2
        valueBeatDivision.clear();

        for(int i=0; i<beatDivision.size(); i++){
            valueBeatDivision.push_back(bpm*frequency_ratios[i]);
        }
    }
    initFrequencySlider();
}


void LFOComponent::initialiseTimeDivisions(){
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


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LFOComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, int lfo, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;processor(p)&#10;defaultShape(false)&#10;activeShape(nullptr)&#10;oneShot(nullptr)&#10;sync(nullptr)&#10;lfoNumber(0) buttonColour(Colour(0x73707070)) activeColour(Colour(0x7f007f7f))&#10;labelRef(label)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="515" initialHeight="250">
  <METHODS>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
    <METHOD name="mouseEnter (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323232">
    <TEXT pos="258 216 51 30" fill="solid: ffaaaaaa" hasStroke="0" text="FREQ"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
    <TEXT pos="320 216 75 30" fill="solid: ffaaaaaa" hasStroke="0" text="PHASE"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
    <TEXT pos="412 216 78 30" fill="solid: ffaaaaaa" hasStroke="0" text="PWM"
          fontname="Good Times" fontsize="16" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="86fad89d553cc7f2" memberName="LFO" virtualName="QTableDrawing"
                    explicitFocusOrder="0" pos="5 5 240 240" class="Component" params="&quot;lfo&quot;"/>
  <SLIDER name="lfoPhase" id="1ecbcdd2c1f246b" memberName="lfoPhaseSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="326 154 64 64"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="lfoFreq" id="2ca728dbadfca9e5" memberName="lfoFrequencySlider"
          virtualName="Slider" explicitFocusOrder="0" pos="254 154 64 64"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="PWM" id="51883d74dbc51579" memberName="lfoPWMSlider" virtualName="Slider"
          explicitFocusOrder="0" pos="418 154 64 64" min="0" max="1" int="0.0010000000000000000208"
          style="RotaryHorizontalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <GENERICCOMPONENT name="bufferDisplay" id="cf424d516c92028b" memberName="bufferDisplay"
                    virtualName="BufferDisplay" explicitFocusOrder="0" pos="392 24 114 114"
                    class="Component" params=""/>
  <TOGGLEBUTTON name="lfoEnableToggle" id="20a8ad00b5e9c4e" memberName="lfoEnableToggle"
                virtualName="" explicitFocusOrder="0" pos="494 0 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="nameLabel" id="a3d123be4d25c5a3" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="421 1 66 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="LFO x" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Good Times"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="resetButton" id="75e324d8b104ab06" memberName="resetButton"
              virtualName="" explicitFocusOrder="0" pos="256 4 16 16" buttonText="r"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonOne" id="76af27469265e466" memberName="shapeButtonOne"
              virtualName="" explicitFocusOrder="0" pos="256 48 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="1" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonTwo" id="3bb13a0dcde29488" memberName="shapeButtonTwo"
              virtualName="" explicitFocusOrder="0" pos="288 48 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="2" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonThree" id="617b5ecc2eca1159" memberName="shapeButtonThree"
              virtualName="" explicitFocusOrder="0" pos="320 48 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="3" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonFour" id="4a30dae598d98762" memberName="shapeButtonFour"
              virtualName="" explicitFocusOrder="0" pos="352 48 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="4" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonFive" id="7642b87a2ed9d859" memberName="shapeButtonFive"
              virtualName="" explicitFocusOrder="0" pos="256 80 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="5" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonSix" id="dcd15fc0c7475cce" memberName="shapeButtonSix"
              virtualName="" explicitFocusOrder="0" pos="288 80 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="6" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonSeven" id="372ea42ed6c269a3" memberName="shapeButtonSeven"
              virtualName="" explicitFocusOrder="0" pos="320 80 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="7" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonEight" id="d80a706269f3c754" memberName="shapeButtonEight"
              virtualName="" explicitFocusOrder="0" pos="352 80 32 32" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="8" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="loadButton" id="cea12f6ee86b14b8" memberName="loadButton"
              virtualName="" explicitFocusOrder="0" pos="256 112 43 24" tooltip="load shape"
              bgColOff="73707070" bgColOn="ff464646" textCol="fff0f0f0" buttonText="load"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="clearButton" id="63d1602c605c9ca3" memberName="clearButton"
              virtualName="" explicitFocusOrder="0" pos="342 112 43 24" tooltip="clear"
              bgColOff="73707070" bgColOn="ff464646" textCol="fff0f0f0" buttonText="clear"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="saveButton" id="99d0d24ae5c2e637" memberName="saveButton"
              virtualName="" explicitFocusOrder="0" pos="299 112 43 24" tooltip="save shape"
              bgColOff="73707070" bgColOn="ff464646" textCol="fff0f0f0" buttonText="save"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="printButton" id="c59897883d2d9f7d" memberName="printButton"
              virtualName="" explicitFocusOrder="0" pos="280 4 16 16" buttonText="p"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="oneShot" id="2988f89eb7a2a076" memberName="oneShotButton"
              virtualName="" explicitFocusOrder="0" pos="256 24 43 24" tooltip="One Shot"
              bgColOff="73707070" bgColOn="ff464646" textCol="fff0f0f0" buttonText="shot"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="freeRunning" id="57fa67a2c45a2e78" memberName="freeRunningButton"
              virtualName="" explicitFocusOrder="0" pos="299 24 43 24" tooltip="Free Running&#10;"
              bgColOff="73707070" bgColOn="ff464646" textCol="fff0f0f0" buttonText="free"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="sync" id="553f4881a3440059" memberName="syncButton" virtualName=""
              explicitFocusOrder="0" pos="342 24 43 24" tooltip="Sync&#10;"
              bgColOff="73707070" bgColOn="ff464646" textCol="fff0f0f0" buttonText="sync"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
