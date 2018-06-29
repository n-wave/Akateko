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
using akateko::MidiRow;
using akateko::initMidiRow;
using akateko::IgnoreRightClick;

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
    paramIndices = p.getParameterIndices(AkatekoAudioProcessor::WaveShapeId);
    StringArray paramNames = p.getParameterIds(AkatekoAudioProcessor::WaveShapeId);
    int nrOfParams = paramIndices.size();

    if(nrOfParams == 4){
        requestMenuIds[0] = paramNames[0].hash();
        requestMenuIds[1] = paramNames[1].hash();
        requestMenuIds[2] = paramNames[2].hash();
        requestMenuIds[3] = String("Shape One").hash();
        requestMenuIds[4] = String("Shape Two").hash();
        requestMenuIds[5] = String("Shape Three").hash();
        requestMenuIds[6] = String("Shape Four").hash();
        requestMenuIds[7] = String("Shape Five").hash();
        requestMenuIds[8] = String("Shape Six").hash();
        requestMenuIds[9] = String("Shape Seven").hash();
        requestMenuIds[10] = String("Shape Eight").hash();

        initialiseMidiStrings();

        addAndMakeVisible(waveshaperToggle = new ParamImageToggle (paramNames[0], *p.getParameters().getUnchecked(paramIndices[0]), label));
        addAndMakeVisible(waveShapeDriveSlider = new ParamSlider (paramNames[1], *p.getParameters().getUnchecked(paramIndices[1]), label, 0.5f, 1.0f));
        addAndMakeVisible(waveShapeMixSlider = new ParamSlider (paramNames[2], *p.getParameters().getUnchecked(paramIndices[2]), label));
        activeShape = p.getParameters().getUnchecked(paramIndices[3]);
    } else {
        addAndMakeVisible (waveshaperToggle = new ImageButton ("waveshaperToggle"));
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

    waveShaperDrawing->setColour(QTableDrawing::backgroundColourId, Colours::black);
    waveShaperDrawing->setColour(QTableDrawing::outlineColourId, Colour(0xAA407050));
    waveShaperDrawing->setColour(QTableDrawing::fillColourId, Colour(0xAA70C099));
    waveShaperDrawing->setColour(QTableDrawing::ellipseColourId, Colour(0xFF60B090));
    waveShaperDrawing->setColour(QTableDrawing::highLightColourId, Colour(0x9FA0C9B0));
    waveShaperDrawing->setColour(QTableDrawing::overlayGradientTwoId, Colour(0x4000FFDA));

    waveShapeDriveSlider->setRange (0, 1, 0.001);
    waveShapeDriveSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveShapeDriveSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    waveShapeMixSlider->setRange (0, 1, 0.001);
    waveShapeMixSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    waveShapeMixSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);

    waveshaperToggle->setButtonText (String());
    waveshaperToggle->setImages(false, true, false,
                                ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour(0x7F000000),
                                ImageCache::getFromMemory (BinaryData::ToggleOff_png, BinaryData::ToggleOff_pngSize), 1.0f, Colour (0x4F20BFCF),
                                ImageCache::getFromMemory (BinaryData::ToggleOn_png, BinaryData::ToggleOn_pngSize), 1.0f, Colour (0x3F20BFCF));

    waveshaperToggle->addListener(this);

    /* Buttons */

    // image Buttons use Owned Array

    addAndMakeVisible (loadShapeButton = new ImageButton ("loadShapeButton"));
    loadShapeButton->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    loadShapeButton->addListener (this);

    loadShapeButton->setImages (false, true, false,
                                ImageCache::getFromMemory (BinaryData::loadOff_png, BinaryData::loadOff_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (BinaryData::loadOn_png, BinaryData::loadOn_pngSize), 0.75f, Colour (0x7F709F9F),
                                ImageCache::getFromMemory (BinaryData::loadOn_png, BinaryData::loadOn_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (waveShapeSaveButton = new ImageButton ("waveShapeSave"));
    waveShapeSaveButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    waveShapeSaveButton->addListener (this);

    waveShapeSaveButton->setImages (false, true, false,
                                    ImageCache::getFromMemory (BinaryData::saveOff_png, BinaryData::saveOff_pngSize), 1.000f, Colour (0x00000000),
                                    ImageCache::getFromMemory (BinaryData::saveOn_png, BinaryData::saveOn_pngSize), 0.75f, Colour (0x7F709F9F),
                                    ImageCache::getFromMemory (BinaryData::saveOn_png, BinaryData::saveOn_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonOne = new IgnoreRightClick<ImageButton> ("Shape One"));
    shapeButtonOne->setButtonText (TRANS("1"));
    shapeButtonOne->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonOne->addListener (this);

    shapeButtonOne->setImages (false, true, false,
                               ImageCache::getFromMemory (BinaryData::b1off_png, BinaryData::b1off_pngSize), 1.000f, Colour (0x00000000),
                               ImageCache::getFromMemory (BinaryData::b1on_png, BinaryData::b1on_pngSize), 0.75f, Colour (0x7F709F9F),
                               ImageCache::getFromMemory (BinaryData::b1on_png, BinaryData::b1on_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonTwo = new IgnoreRightClick<ImageButton> ("Shape Two"));
    shapeButtonTwo->setButtonText (TRANS("2"));
    shapeButtonTwo->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonTwo->addListener (this);

    shapeButtonTwo->setImages (false, true, false,
                               ImageCache::getFromMemory (BinaryData::b2off_png, BinaryData::b2off_pngSize), 1.000f, Colour (0x00000000),
                               ImageCache::getFromMemory (BinaryData::b2on_png, BinaryData::b2on_pngSize), 0.75f, Colour (0x7F709F9F),
                               ImageCache::getFromMemory (BinaryData::b2on_png, BinaryData::b2on_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonThree = new IgnoreRightClick<ImageButton> ("Shape Three"));
    shapeButtonThree->setButtonText (TRANS("3"));
    shapeButtonThree->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonThree->addListener (this);

    shapeButtonThree->setImages (false, true, false,
                                 ImageCache::getFromMemory (BinaryData::b3off_png, BinaryData::b3off_pngSize), 1.000f, Colour (0x00000000),
                                 ImageCache::getFromMemory (BinaryData::b3on_png, BinaryData::b3on_pngSize), 0.75f, Colour (0x7F709F9F),
                                 ImageCache::getFromMemory (BinaryData::b3on_png, BinaryData::b3on_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonFour = new IgnoreRightClick<ImageButton>  ("Shape Four"));
    shapeButtonFour->setButtonText (TRANS("4"));
    shapeButtonFour->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonFour->addListener (this);

    shapeButtonFour->setImages (false, true, false,
                                ImageCache::getFromMemory (BinaryData::b4off_png, BinaryData::b4off_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (BinaryData::b4on_png, BinaryData::b4on_pngSize), 0.75f, Colour (0x7F709F9F),
                                ImageCache::getFromMemory (BinaryData::b4on_png, BinaryData::b4on_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonFive = new IgnoreRightClick<ImageButton>  ("Shape Five"));
    shapeButtonFive->setButtonText (TRANS("5"));
    shapeButtonFive->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonFive->addListener (this);

    shapeButtonFive->setImages (false, true, false,
                                ImageCache::getFromMemory (BinaryData::b5off_png, BinaryData::b5off_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (BinaryData::b5on_png, BinaryData::b5on_pngSize), 0.75f, Colour (0x7F709F9F),
                                ImageCache::getFromMemory (BinaryData::b5on_png, BinaryData::b5on_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonSix = new IgnoreRightClick<ImageButton>  ("Shape Six"));
    shapeButtonSix->setButtonText (TRANS("6"));
    shapeButtonSix->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonSix->addListener (this);

    shapeButtonSix->setImages (false, true, false,
                               ImageCache::getFromMemory (BinaryData::b6off_png, BinaryData::b6off_pngSize), 1.000f, Colour (0x00000000),
                               ImageCache::getFromMemory (BinaryData::b6on_png, BinaryData::b6on_pngSize), 0.75f, Colour (0x7F709F9F),
                               ImageCache::getFromMemory (BinaryData::b6on_png, BinaryData::b6on_pngSize), 1.000f, Colour (0x001f0000));

    addAndMakeVisible (clearShapeButton = new IgnoreRightClick<ImageButton>  ("Clear Shape"));
    clearShapeButton->setButtonText (TRANS("clear"));
    clearShapeButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    clearShapeButton->addListener (this);

    clearShapeButton->setImages (false, true, false,
                                 ImageCache::getFromMemory (BinaryData::clearOff_png, BinaryData::clearOff_pngSize), 1.000f, Colour (0x00000000),
                                 ImageCache::getFromMemory (BinaryData::clearOn_png, BinaryData::clearOn_pngSize), 0.75f, Colour (0x7F709F9F),
                                 ImageCache::getFromMemory (BinaryData::clearOn_png, BinaryData::clearOn_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonSeven = new IgnoreRightClick<ImageButton>  ("Shape Seven"));
    shapeButtonSeven->setButtonText (TRANS("7"));
    shapeButtonSeven->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonSeven->addListener (this);

    shapeButtonSeven->setImages (false, true, false,
                                 ImageCache::getFromMemory (BinaryData::b7off_png, BinaryData::b7off_pngSize), 1.000f, Colour (0x00000000),
                                 ImageCache::getFromMemory (BinaryData::b7on_png, BinaryData::b7on_pngSize), 0.75f, Colour (0x7F709F9F),
                                 ImageCache::getFromMemory (BinaryData::b7on_png, BinaryData::b7on_pngSize), 1.000f, Colour (0x1f000000));

    addAndMakeVisible (shapeButtonEight = new IgnoreRightClick<ImageButton> ("Shape Eight"));
    shapeButtonEight->setButtonText (TRANS("8"));
    shapeButtonEight->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    shapeButtonEight->addListener (this);

    shapeButtonEight->setImages (false, true, false,
                                 ImageCache::getFromMemory (BinaryData::b8off_png, BinaryData::b8off_pngSize), 1.000f, Colour (0x00000000),
                                 ImageCache::getFromMemory (BinaryData::b8on_png, BinaryData::b8on_pngSize), 0.75f, Colour (0x7F709F9F),
                                 ImageCache::getFromMemory (BinaryData::b8on_png, BinaryData::b8on_pngSize), 1.000f, Colour (0x1f000000));

    // image
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

    // Look And Feel

    //Initialise Waveshaper Shapes
    initialiseDefaultShapes();

    if(activeShape != nullptr){
        const int sIndex = activeShape->getValue();

        if(sIndex < currentShapes.size()){
            waveShaperDrawing->restoreFromString(currentShapes[sIndex]);
        }
        /* Set ActiveShape button Colour */
        switch(sIndex){
            case 0:
                shapeButtonOne->setState(ImageButton::buttonDown);
                break;
            case 1:
                shapeButtonTwo->setState(ImageButton::buttonDown);
                break;
            case 2:
                shapeButtonThree->setState(ImageButton::buttonDown);
                break;
            case 3:
                shapeButtonFour->setState(ImageButton::buttonDown);
                break;
            case 4:
                shapeButtonFive->setState(ImageButton::buttonDown);
                break;
            case 5:
                shapeButtonSix->setState(ImageButton::buttonDown);
                break;
            case 6:
                shapeButtonSeven->setState(ImageButton::buttonDown);
                break;
            case 7:
                shapeButtonSeven->setState(ImageButton::buttonDown);
                break;
            default:
                std::cerr << "Wrong Shape number supplied" << std::endl;
        }
    }


    /* Build Menu */
    menu.addItem(1, "learn", true, false, nullptr);
    menu.addSeparator();
    menu.addItem(0xFF, "clear");

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
    const int aShape = activeShape->getValue();

    switch(aShape){
        case 0: setShapeState(shapeButtonOne); break;
        case 1: setShapeState(shapeButtonTwo); break;
        case 2: setShapeState(shapeButtonThree); break;
        case 3: setShapeState(shapeButtonFour); break;
        case 4: setShapeState(shapeButtonFive); break;
        case 5: setShapeState(shapeButtonSix); break;
        case 6: setShapeState(shapeButtonSeven); break;
        case 7: setShapeState(shapeButtonEight); break;
    }

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
    waveShapeMixSlider->setBounds (182, 28, 68, 68);
    waveShapeDriveSlider->setBounds (182, 110, 68, 68);
    waveshaperToggle->setBounds (234, 3, 18, 18);

    shapeButtonOne->setBounds (9, 198, 38, 22);
    shapeButtonTwo->setBounds (49, 198, 38, 22);
    shapeButtonThree->setBounds (89, 198, 38, 22);
    shapeButtonFour->setBounds (129, 198, 38, 22);
    shapeButtonFive->setBounds (9, 222, 38, 22);
    shapeButtonSix->setBounds (49, 222, 38, 22);
    shapeButtonSeven->setBounds (89, 222, 38, 22);
    shapeButtonEight->setBounds (129, 222, 38, 22);

    clearShapeButton->setBounds (177, 222, 70, 22);
    waveShapeSaveButton->setBounds (213, 198, 34, 22);
    loadShapeButton->setBounds (177, 198, 34, 22);
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
            processor.setShape(currentShapes[sIndex], AkatekoAudioProcessor::WaveShapeId);
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
    defaultShapes.clear();
    defaultShapes.add(akateko::ws_default_shapes[0]);
    defaultShapes.add(akateko::ws_default_shapes[1]);
    defaultShapes.add(akateko::ws_default_shapes[2]);
    defaultShapes.add(akateko::ws_default_shapes[3]);
    defaultShapes.add(akateko::ws_default_shapes[4]);
    defaultShapes.add(akateko::ws_default_shapes[5]);
    defaultShapes.add(akateko::ws_default_shapes[6]);
    defaultShapes.add(akateko::ws_default_shapes[7]);

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
            shapeButtonOne->setState(ImageButton::buttonNormal);
            break;
        case 1:
            shapeButtonTwo->setState(ImageButton::buttonNormal);
            break;
        case 2:
            shapeButtonThree->setState(ImageButton::buttonNormal);
            break;
        case 3:
            shapeButtonFour->setState(ImageButton::buttonNormal);
            break;
        case 4:
            shapeButtonFive->setState(ImageButton::buttonNormal);
            break;
        case 5:
            shapeButtonSix->setState(ImageButton::buttonNormal);
            break;
        case 6:
            shapeButtonSeven->setState(ImageButton::buttonNormal);
            break;
        case 7:
            shapeButtonEight->setState(ImageButton::buttonNormal);
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

        buttonThatWasClicked->setState(ImageButton::buttonOver);

        processor.setWaveShaperBuffer(buffer);
        processor.setShape(currentShapes[shape], AkatekoAudioProcessor::WaveShapeId);
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

/* Call in paint if button is down, get the state and check for
   for hovered state, if true set hovered else button should be down
   based on the active shape value.

   On a later point refactor it differently by ditching the individual
   shape buttons and place them in an array
 */
void WaveShapeComponent::setShapeState(ImageButton *buttonClicked){
    const int tmpState = buttonClicked->getState();

    if(tmpState != ImageButton::buttonOver){
        buttonClicked->setState(ImageButton::buttonDown);
    }
}

void WaveShapeComponent::setLookAndFeel(LookAndFeel *cLaf, LookAndFeel *bLaf)
{
    waveShapeDriveSlider->setLookAndFeel(bLaf);
    waveShapeMixSlider->setLookAndFeel(bLaf);
    waveShaperDrawing->setLookAndFeel(cLaf);
    menu.setLookAndFeel(cLaf);
}

void WaveShapeComponent::initialiseMidiStrings(){
    midiStrings.clear();

    midiStrings.add(" WShaper: Enable");
    midiStrings.add(" WShaper: Drive");
    midiStrings.add(" WShaper: Mix");
    midiStrings.add(" WShaper: Shape 1");
    midiStrings.add(" WShaper: Shape 2");
    midiStrings.add(" WShaper: Shape 3");
    midiStrings.add(" WShaper: Shape 4");
    midiStrings.add(" WShaper: Shape 5");
    midiStrings.add(" WShaper: Shape 6");
    midiStrings.add(" WShaper: Shape 7");
    midiStrings.add(" WShaper: Shape 8");
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
    } else if(paramIndices.size() == 4 &&
               midiStrings.size() == 11)
    {
        int index = -1;
        int param = -1;
        int paramIndex = -1;
        int handling = -1;
        double minValue = 0.0;
        double maxValue = 1.0;

        if(commandId == requestMenuIds[0]){ // WaveShaper Enable
            index = menu.show();
            param = 0;
            paramIndex = 0;
            handling = akateko::MIDI_TO_INT_TOGGLE;
        } else if(commandId == requestMenuIds[1]){ // WaveShaper Drive
            index = menu.show();
            param = 1;
            paramIndex = 1;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[2]){ // WaveShaper Mix
            index = menu.show();
            param = 2;
            paramIndex = 2;
            handling = akateko::MIDI_TO_DOUBLE;
        } else if(commandId == requestMenuIds[3]){ // WaveShaper Shape 1
            resetShapButtonColour(0);
            index = menu.show();
            param = 3;
            paramIndex = 3;
            maxValue = 0;
            handling = akateko::MIDI_TO_CONSTANT;
        } else if(commandId == requestMenuIds[4]){ // WaveShaper Shape 2
            resetShapButtonColour(1);
            index = menu.show();
            param = 4;
            paramIndex = 3;
            minValue = 1;
            handling = akateko::MIDI_TO_CONSTANT;
        } else if(commandId == requestMenuIds[5]){ // WaveShaper Shape 3
            resetShapButtonColour(2);
            index = menu.show();
            param = 5;
            paramIndex = 3;
            minValue = 2;
            maxValue = 2;
            handling = akateko::MIDI_TO_CONSTANT;
        } else if(commandId == requestMenuIds[6]){ // WaveShaper Shape 4
            resetShapButtonColour(3);
            index = menu.show();
            param = 6;
            paramIndex = 3;
            minValue = 3;
            maxValue = 3;
            handling = akateko::MIDI_TO_CONSTANT;
        } else if(commandId == requestMenuIds[7]){ // WaveShaper Shape 5
            resetShapButtonColour(4);
            index = menu.show();
            param = 7;
            paramIndex = 3;
            minValue = 4;
            maxValue = 4;
            handling = akateko::MIDI_TO_CONSTANT;
        } else if(commandId == requestMenuIds[8]){ // WaveShaper Shape 6
            resetShapButtonColour(5);
            index = menu.show();
            param = 8;
            paramIndex = 3;
            minValue = 5;
            maxValue = 5;
            handling = akateko::MIDI_TO_CONSTANT;
        } else if(commandId == requestMenuIds[9]){ // WaveShaper Shape 7
            resetShapButtonColour(6);
            index = menu.show();
            param = 9;
            paramIndex = 3;
            minValue = 6;
            maxValue = 6;
            handling = akateko::MIDI_TO_CONSTANT;
        } else if(commandId == requestMenuIds[10]){ // WaveShaper Shape 8
            resetShapButtonColour(7);
            index = menu.show();
            param = 10;
            paramIndex = 3;
            minValue = 7;
            maxValue = 7;
            handling = akateko::MIDI_TO_CONSTANT;
        }

        if(index == 1){
            MidiRow tmpRow;

            initMidiRow(tmpRow, param, 0, 127, minValue, maxValue, paramIndices[paramIndex], handling, midiStrings[param], 2);
            processor.initiateMidiLearn(tmpRow);
        } else if(index == 0xFF){
            if(param >= 0 && param <= 3){
                processor.removeMidiRow(paramIndices[paramIndex]);
            } else {
                processor.removeMidiRow(param, paramIndices[paramIndex]);
            }

            String message = midiStrings[param] + String(" cleared");
            labelRef.setText(message, sendNotificationAsync);
        }
    }
    //[/UserCode_handleCommandMessage]
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
  <IMAGEBUTTON  name="waveshaperToggle" id="18447cc74b490f5" memberName="waveshaperToggle"
                virtualName="ToggleButton" explicitFocusOrder="0" pos="232 0 24 24"
                buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                state="0"/>
  <IMAGEBUTTON name="loadShapeButton" id="8d3ade691db684c4" memberName="loadShapeButton"
               virtualName="" explicitFocusOrder="0" pos="176 197 36 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="load" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="waveShapeSave" id="87daf60a80bf251d" memberName="waveShapeSaveButton"
               virtualName="" explicitFocusOrder="0" pos="212 197 36 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="save" connectedEdges="3"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="shapeButtonOne" id="76af27469265e466" memberName="shapeButtonOne"
               virtualName="" explicitFocusOrder="0" pos="8 197 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="1" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="shapeButtonTwo" id="dcd18eb4b7c3527a" memberName="shapeButtonTwo"
               virtualName="" explicitFocusOrder="0" pos="48 197 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="2" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="shapeButtonThree" id="4c4729d408f39c67" memberName="shapeButtonThree"
               virtualName="" explicitFocusOrder="0" pos="88 197 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="3" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="shapeButtonFour" id="4fe4dd2b8352ff7a" memberName="shapeButtonFour"
               virtualName="" explicitFocusOrder="0" pos="128 197 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="4" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="shapeButtonFive" id="e46d2cf6bb9f8ce2" memberName="shapeButtonFive"
               virtualName="" explicitFocusOrder="0" pos="8 221 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="5" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON  name="shapeButtonSix" id="383f2b3a0cf5ea4d" memberName="shapeButtonSix"
               virtualName="" explicitFocusOrder="0" pos="48 221 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="6" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="clearShapeButton" id="89fdc3342641232e" memberName="clearShapeButton"
               virtualName="" explicitFocusOrder="0" pos="176 221 72 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="clear" connectedEdges="3"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="shapeButtonSeven" id="da21b226956fb35d" memberName="shapeButtonSeven"
               virtualName="" explicitFocusOrder="0" pos="88 221 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="7" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
  <IMAGEBUTTON name="shapeButtonEight" id="bcfa7a8be7fbe81e" memberName="shapeButtonEight"
               virtualName="" explicitFocusOrder="0" pos="128 221 40 24" bgColOff="73707070"
               bgColOn="ff464646" textCol="fff0f0f0" buttonText="8" connectedEdges="12"
               needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
