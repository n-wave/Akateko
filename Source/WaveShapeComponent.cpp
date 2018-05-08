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
#include "WaveShapeComponent.h"
#include <vector>

using std::vector;
//[/Headers]

#include "WaveShapeComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

/*
    waveschaper labels:

    1: paramId : enable
    2: paramId : drive
    3: paramId : mix
    4: paramId : amp
    5: paramId : routing


    overlayWidth;
    overlayHeight;
    overlayXPos;
    overlayYPos;


    Colour buttonColour;
    Colour activeColour;
*/


//[/MiscUserDefs]

//==============================================================================
WaveShapeComponent::WaveShapeComponent (const String &name, AkatekoAudioProcessor &p, Label &label)
    : Component::Component(name),
      overlayWidth(0.f),
      overlayHeight(0.f),
      overlayXPos(0.f),
      overlayYPos(0.f),
      processor(p),
      labelRef(label),
      buttonColour(Colour(0x73707070)),
      activeColour(Colour(0x7f007f7f))
{
    //[Constructor_pre] You can add your own custom stuff here..
    vector<int> paramIndices = p.getParameterIndices(AkatekoAudioProcessor::WaveShapeId);
    StringArray paramNames = p.getParameterIds(AkatekoAudioProcessor::WaveShapeId);
    int nrOfParams = paramIndices.size();

    if(nrOfParams == 4){
        addAndMakeVisible(waveshaperToggle = new ParamToggle (paramNames[0], *p.getParameters().getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(waveShapeDriveSlider = new ParamSlider (paramNames[1], *p.getParameters().getUnchecked(paramIndices[1]), label, 0.5f, 1.0f));
        addAndMakeVisible(waveShapeMixSlider = new ParamSlider (paramNames[2], *p.getParameters().getUnchecked(paramIndices[2]), label));
        activeShape = p.getParameters().getUnchecked(paramIndices[5]);
    } else {
        addAndMakeVisible (waveshaperToggle = new ToggleButton ("waveshaperToggle"));
        addAndMakeVisible (waveShapeDriveSlider = new Slider ("waveShapeDrive"));
        addAndMakeVisible (waveShapeMixSlider = new Slider ("waveShapeMix"));
        activeShape = nullptr;

        std::cerr << "WaveShaperComponent::WaveShaperComponent" << std::endl;
        std::cerr << "/-------Parameters are not bound-------/" << std::endl;
    }
    //[/Constructor_pre]

    //[UserPreSize]
    addAndMakeVisible (waveShaperDrawing = new QTableDrawing ("WS"));
    waveShaperDrawing->setBufferSize(16385);
    waveShaperDrawing->setName ("waveShaper");
    waveShaperDrawing->setFileExtension("shape");

    waveShapeDriveSlider->setRange (0, 1, 0.001);
    waveShapeDriveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveShapeDriveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    waveShapeMixSlider->setRange (0, 1, 0.001);
    waveShapeMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveShapeMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    waveshaperToggle->setButtonText (String());
    waveshaperToggle->addListener(this);

    /* Buttons */
    addAndMakeVisible (loadShapeButton = new TextButton ("loadShapeButton"));
    loadShapeButton->setButtonText (TRANS("load"));
    loadShapeButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    loadShapeButton->addListener (this);
    loadShapeButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    loadShapeButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    loadShapeButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (waveShapeSaveButton = new TextButton ("waveShapeSave"));
    waveShapeSaveButton->setButtonText (TRANS("save"));
    waveShapeSaveButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    waveShapeSaveButton->addListener (this);
    waveShapeSaveButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    waveShapeSaveButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    waveShapeSaveButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonOne = new TextButton ("Shape One"));
    shapeButtonOne->setButtonText (TRANS("1"));
    shapeButtonOne->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonOne->addListener (this);
    shapeButtonOne->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonOne->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonOne->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonTwo = new TextButton ("Shape Two"));
    shapeButtonTwo->setButtonText (TRANS("2"));
    shapeButtonTwo->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonTwo->addListener (this);
    shapeButtonTwo->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonTwo->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonTwo->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonThree = new TextButton ("Shape Three"));
    shapeButtonThree->setButtonText (TRANS("3"));
    shapeButtonThree->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonThree->addListener (this);
    shapeButtonThree->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonThree->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonThree->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonFour = new TextButton ("Shape Four"));
    shapeButtonFour->setButtonText (TRANS("4"));
    shapeButtonFour->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonFour->addListener (this);
    shapeButtonFour->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonFour->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonFour->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonFive = new TextButton ("Shape Five"));
    shapeButtonFive->setButtonText (TRANS("5"));
    shapeButtonFive->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonFive->addListener (this);
    shapeButtonFive->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonFive->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonFive->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonSix = new TextButton ("Shape Six"));
    shapeButtonSix->setButtonText (TRANS("6"));
    shapeButtonSix->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonSix->addListener (this);
    shapeButtonSix->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonSix->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonSix->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonSeven = new TextButton ("shapeButtonSeven"));
    shapeButtonSeven->setButtonText (TRANS("7"));
    shapeButtonSeven->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonSeven->addListener (this);
    shapeButtonSeven->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonSeven->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonSeven->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (shapeButtonEight = new TextButton ("shapeButtonEight"));
    shapeButtonEight->setButtonText (TRANS("8"));
    shapeButtonEight->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonEight->addListener (this);
    shapeButtonEight->setColour (TextButton::buttonColourId, Colour (0x73707070));
    shapeButtonEight->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    shapeButtonEight->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    addAndMakeVisible (clearShapeButton = new TextButton ("clearShapeButton"));
    clearShapeButton->setButtonText (TRANS("clear"));
    clearShapeButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    clearShapeButton->addListener (this);
    clearShapeButton->setColour (TextButton::buttonColourId, Colour (0x73707070));
    clearShapeButton->setColour (TextButton::buttonOnColourId, Colour (0xff464646));
    clearShapeButton->setColour (TextButton::textColourOffId, Colour (0xfff0f0f0));

    shapeButtonOne->setMouseClickGrabsKeyboardFocus(false);
    shapeButtonTwo->setMouseClickGrabsKeyboardFocus(false);
    shapeButtonThree->setMouseClickGrabsKeyboardFocus(false);
    shapeButtonFour->setMouseClickGrabsKeyboardFocus(false);
    shapeButtonFive->setMouseClickGrabsKeyboardFocus(false);
    shapeButtonSix->setMouseClickGrabsKeyboardFocus(false);
    shapeButtonSeven->setMouseClickGrabsKeyboardFocus(false);
    shapeButtonEight->setMouseClickGrabsKeyboardFocus(false);

    //[/UserPreSize]

    setSize (255, 255);


    //[Constructor] You can add your own custom stuff here..
    this->initialiseDefaultShapes();

    if(activeShape != nullptr){
        const int sIndex = activeShape->getValue();

        if(sIndex < currentShapes.size()){
            waveShaperDrawing->restoreFromString(currentShapes[sIndex]);
        }
        /* Set ActiveShape button Colour */
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
                shapeButtonSeven->setColour(TextButton::buttonColourId, activeColour);
                break;
            default:
                std::cerr << "Wrong Shape number supplied" << std::endl;
        }
    }

    //[/Constructor]
}

WaveShapeComponent::~WaveShapeComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    waveShaperDrawing = nullptr;
    waveShapeDriveSlider = nullptr;
    waveShapeMixSlider = nullptr;
    waveshaperToggle = nullptr;
    loadShapeButton = nullptr;
    waveShapeSaveButton = nullptr;
    shapeButtonOne = nullptr;
    shapeButtonTwo = nullptr;
    shapeButtonThree = nullptr;
    shapeButtonFour = nullptr;
    shapeButtonFive = nullptr;
    shapeButtonSix = nullptr;
    clearShapeButton = nullptr;
    shapeButtonSeven = nullptr;
    shapeButtonEight = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    activeShape = nullptr;
    //[/Destructor]
}

//==============================================================================
void WaveShapeComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

    //[/UserPrePaint]

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("DRIVE"),
                184, 168, 57, 21,
                Justification::centred, true);

    g.setColour (Colour (0xffaaaaaa));
    g.setFont (Font ("Good Times", 12.00f, Font::plain));
    g.drawText (TRANS("MIX"),
                196, 88, 33, 21,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..


    //[/UserPaint]
}

void WaveShapeComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    waveShaperDrawing->setBounds (8, 24, 160, 160);
    waveShapeDriveSlider->setBounds (183, 112, 60, 60);
    waveShapeMixSlider->setBounds (183, 32, 60, 60);
    waveshaperToggle->setBounds (232, 0, 24, 24);
    loadShapeButton->setBounds (176, 197, 36, 24);
    waveShapeSaveButton->setBounds (212, 197, 36, 24);
    shapeButtonOne->setBounds (8, 197, 40, 24);
    shapeButtonTwo->setBounds (48, 197, 40, 24);
    shapeButtonThree->setBounds (88, 197, 40, 24);
    shapeButtonFour->setBounds (128, 197, 40, 24);
    shapeButtonFive->setBounds (8, 221, 40, 24);
    shapeButtonSix->setBounds (48, 221, 40, 24);
    clearShapeButton->setBounds (176, 221, 72, 24);
    shapeButtonSeven->setBounds (88, 221, 40, 24);
    shapeButtonEight->setBounds (128, 221, 40, 24);
    //[UserResized] Add your own custom resize handling here..


    //[/UserResized]
}

void WaveShapeComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    if(buttonThatWasClicked == waveshaperToggle){
        processor.enableWaveShaper();
    }


    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadShapeButton)
    {
        //[UserButtonCode_loadShapeButton] -- add your button handler code here..
        waveShaperDrawing->load();
        waveShaperDrawing->fillBuffer();

        if(activeShape != nullptr){
            const int sIndex = activeShape->getValue();
            const String tmpShape = waveShaperDrawing->toString();

            currentShapes.set(sIndex, tmpShape);
            getParentComponent()->postCommandMessage(commandUpdate);
        }
        //[/UserButtonCode_loadShapeButton]
    }
    else if (buttonThatWasClicked == waveShapeSaveButton)
    {
        //[UserButtonCode_waveShapeSaveButton] -- add your button handler code here..
        waveShaperDrawing->save();
        //[/UserButtonCode_waveShapeSaveButton]
    }
    else if (buttonThatWasClicked == shapeButtonOne)
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
    else if (buttonThatWasClicked == clearShapeButton)
    {
        //[UserButtonCode_clearShapeButton] -- add your button handler code here..
        waveShaperDrawing->clear();
        waveShaperDrawing->fillBuffer();
        msmBuffer buffer = waveShaperDrawing->getBuffer();

        if(activeShape != nullptr){
            const int sIndex = activeShape->getValue();
            const String tmpShape = waveShaperDrawing->toString();

            currentShapes.set(sIndex, tmpShape);
            processor.setWaveShaperBuffer(buffer);
            processor.updateShape(currentShapes[sIndex], AkatekoAudioProcessor::WaveShapeId);
        }

        this->repaint();
        //[/UserButtonCode_clearShapeButton]
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

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void WaveShapeComponent::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
    if(commandId == QTableDrawing::qtableUpdated){
        waveShaperDrawing->fillBuffer();
        msmBuffer buffer = waveShaperDrawing->getBuffer();

        if(activeShape != nullptr){
            const int tmpShape = activeShape->getValue();
            const String newShape = waveShaperDrawing->toString();

            currentShapes.set(tmpShape, newShape);
            processor.setWaveShaperBuffer(buffer);
        }
    }
    //[/UserCode_handleCommandMessage]
}

void WaveShapeComponent::mouseEnter (const MouseEvent& e)
{
    //[UserCode_mouseEnter] -- Add your code here...
    this->setWantsKeyboardFocus(true);
    //[/UserCode_mouseEnter]
}

void WaveShapeComponent::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    this->setWantsKeyboardFocus(false);
    //[/UserCode_mouseExit]
}

void WaveShapeComponent::modifierKeysChanged (const ModifierKeys& modifiers)
{
    //[UserCode_modifierKeysChanged] -- Add your code here...
    if(modifiers.isCtrlDown()){
        defaultShape = true;
    }
    //[/UserCode_modifierKeysChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void WaveShapeComponent::initialiseDefaultShapes(){
    defaultShapes.add(String("m 8 152 q 80 80 152 8"));
    defaultShapes.add(String("m 8 152 q 41 152 80 80 115 10 152 8"));
    defaultShapes.add(String("m 8 152 q 80 152 80 80 80 8 152 8"));
    defaultShapes.add(String("m 8 152 q 31 152 44 152 80 152 80 80 80 8 116 8 126 8 152 8"));
    defaultShapes.add(String("m 8 152 q 33 152 44 137 62 108.5 80 80 98 53 116 26 123 8 152 8"));
    defaultShapes.add(String("m 8 152 q 26 144.5 44 137 62 108.5 80 80 98 53 116 26 134 17 152 8"));
    defaultShapes.add(String("m 8 152 q 26 152 44 152 62 116 80 80 98 44 116 8 134 8 152 8"));
    defaultShapes.add(String("m 8 152 q 77 147 80 80 88 8 116 8 134 8 152 8"));

    currentShapes = StringArray(defaultShapes);
}

msmBuffer WaveShapeComponent::getBuffer(){
    return waveShaperDrawing->getBuffer();
}

String WaveShapeComponent::getCurrentShape(){
    String tmpShape = String("m 8.5 161.5 q 85 161.5 85 85 85 8.5 161.5 8.5");

    if(activeShape != nullptr){
        int sIndex = activeShape->getValue();
        tmpShape = currentShapes[sIndex];
    }

    return tmpShape;
}

StringArray WaveShapeComponent::getCurrentShapes(){
    return currentShapes;
}

void WaveShapeComponent::setCurrentShapes(StringArray shapes){
    if(shapes.size() == currentShapes.size()){
        currentShapes = StringArray(shapes);

        if(activeShape != nullptr){
            const int sIndex= activeShape->getValue();
            const String tmpShape = currentShapes[sIndex];

            waveShaperDrawing->restoreFromString(tmpShape);
            waveShaperDrawing->fillBuffer();
        }
    }
}

void WaveShapeComponent::setUpdateCommandId(int hashCode){
    commandUpdate = hashCode;
}

void WaveShapeComponent::resetShapButtonColour(int shapeButton){
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

void WaveShapeComponent::setShape(Button *buttonThatWasClicked, int shape){
    if(shape >= 0 && defaultShapes.size()){
        if(defaultShape){
            currentShapes.set(shape, String(defaultShapes[shape]));
            defaultShape = false;
        }

        waveShaperDrawing->restoreFromString(currentShapes[shape]);
        waveShaperDrawing->fillBuffer();
        waveShaperDrawing->repaint();
        msmBuffer buffer = waveShaperDrawing->getBuffer();

        if(activeShape != nullptr){
            resetShapButtonColour(activeShape->getValue());
            activeShape->setValue(shape);

            String tmpString = getName() + " : " + buttonThatWasClicked->getName();
            labelRef.setText(tmpString, dontSendNotification);
        }

        buttonThatWasClicked->setColour(TextButton::buttonColourId, activeColour);

        processor.setWaveShaperBuffer(buffer);
        processor.updateShape(currentShapes[shape], AkatekoAudioProcessor::WaveShapeId);
    }
}

void WaveShapeComponent::initialiseActiveShape(){
    if(activeShape != nullptr){
        int sIndex = activeShape->getValue();

        //Reset all buttons

        for(int i=0; i<8; i++){
            resetShapButtonColour(i);
        }

        if(sIndex < currentShapes.size()){
            waveShaperDrawing->restoreFromString(currentShapes[sIndex]);
        }
        /* Set ActiveShape button Colour */
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
                shapeButtonSeven->setColour(TextButton::buttonColourId, activeColour);
                break;
            default:
                std::cerr << "Wrong Shape number supplied" << std::endl;
        }

        /* Restore Buffer in processor */

        if(sIndex < currentShapes.size()){
            waveShaperDrawing->restoreFromString(currentShapes[sIndex]);
            waveShaperDrawing->fillBuffer();
            waveShaperDrawing->repaint();
            msmBuffer buffer = waveShaperDrawing->getBuffer();
            processor.setWaveShaperBuffer(buffer);
        }
    }
}

void WaveShapeComponent::updateGui(){
    waveshaperToggle->postCommandMessage(ParamToggle::update);
    waveShapeDriveSlider->postCommandMessage(ParamSlider::update);
    waveShapeMixSlider->postCommandMessage(ParamSlider::update);
    initialiseActiveShape();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WaveShapeComponent" componentName=""
                 parentClasses="public Component" constructorParams="const String &amp;name, AkatekoAudioProcessor &amp;p, Label &amp;label"
                 variableInitialisers="Component::Component(name)&#10;overlayWidth(0.f)&#10;overlayHeight(0.f)&#10;overlayXPos(0.f)&#10;overlayYPos(0.f)&#10;processor(p)&#10;labelRef(label)  buttonColour(Colour(0x73707070)) activeColour(Colour(0x7f007f7f))"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="255" initialHeight="255">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
    <METHOD name="mouseEnter (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff">
    <TEXT pos="184 168 57 21" fill="solid: ffaaaaaa" hasStroke="0" text="DRIVE"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
    <TEXT pos="196 88 33 21" fill="solid: ffaaaaaa" hasStroke="0" text="MIX"
          fontname="Good Times" fontsize="12" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="waveShaper" id="d58816a706dcce60" memberName="waveShaperDrawing"
                    virtualName="QTableDrawing" explicitFocusOrder="0" pos="8 24 160 160"
                    class="Component" params="&quot;WS&quot;"/>
  <SLIDER name="waveShapeDrive" id="a9617dddd545fe10" memberName="waveShapeDriveSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="183 112 60 60"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryHorizontalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="0"/>
  <SLIDER name="waveShapeMix" id="3bbb09deda8e3492" memberName="waveShapeMixSlider"
          virtualName="Slider" explicitFocusOrder="0" pos="183 32 60 60"
          min="0" max="1" int="0" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <TOGGLEBUTTON name="waveshaperToggle" id="18447cc74b490f5" memberName="waveshaperToggle"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="232 0 24 24"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="loadShapeButton" id="8d3ade691db684c4" memberName="loadShapeButton"
              virtualName="" explicitFocusOrder="0" pos="176 197 36 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="load" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="waveShapeSave" id="87daf60a80bf251d" memberName="waveShapeSaveButton"
              virtualName="" explicitFocusOrder="0" pos="212 197 36 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="save" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonOne" id="76af27469265e466" memberName="shapeButtonOne"
              virtualName="" explicitFocusOrder="0" pos="8 197 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="1" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonTwo" id="dcd18eb4b7c3527a" memberName="shapeButtonTwo"
              virtualName="" explicitFocusOrder="0" pos="48 197 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="2" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonThree" id="4c4729d408f39c67" memberName="shapeButtonThree"
              virtualName="" explicitFocusOrder="0" pos="88 197 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="3" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonFour" id="4fe4dd2b8352ff7a" memberName="shapeButtonFour"
              virtualName="" explicitFocusOrder="0" pos="128 197 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="4" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonFive" id="e46d2cf6bb9f8ce2" memberName="shapeButtonFive"
              virtualName="" explicitFocusOrder="0" pos="8 221 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="5" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonSix" id="383f2b3a0cf5ea4d" memberName="shapeButtonSix"
              virtualName="" explicitFocusOrder="0" pos="48 221 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="6" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="clearShapeButton" id="89fdc3342641232e" memberName="clearShapeButton"
              virtualName="" explicitFocusOrder="0" pos="176 221 72 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="clear" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonSeven" id="da21b226956fb35d" memberName="shapeButtonSeven"
              virtualName="" explicitFocusOrder="0" pos="88 221 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="7" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="shapeButtonEight" id="bcfa7a8be7fbe81e" memberName="shapeButtonEight"
              virtualName="" explicitFocusOrder="0" pos="128 221 40 24" bgColOff="73707070"
              bgColOn="ff464646" textCol="fff0f0f0" buttonText="8" connectedEdges="12"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
