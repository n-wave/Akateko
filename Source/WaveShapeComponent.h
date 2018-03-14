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

#ifndef __JUCE_HEADER_458886B41B1B6C46__
#define __JUCE_HEADER_458886B41B1B6C46__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-gui/QTableDrawing.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"

#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class WaveShapeComponent  : public Component,
                            public ButtonListener
{
public:
    //==============================================================================
    WaveShapeComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~WaveShapeComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setUpdateCommandId(int hashCode);

    String getCurrentShape(); //when shape is updated call and store in the processor
    StringArray getCurrentShapes();
    void setCurrentShapes(StringArray shapes); //After first startup

    msmBuffer getBuffer();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void handleCommandMessage (int commandId) override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void modifierKeysChanged (const ModifierKeys& modifiers) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseDefaultShapes();
    void resetShapButtonColour(int shapeButton);
    void setShape(Button *buttonThatWasClicked, int shape);

    AkatekoAudioProcessor &processor;

    StringArray defaultShapes;
    StringArray currentShapes;
    AudioProcessorParameter *activeShape;
    Label &labelRef;

    int commandUpdate;
    bool defaultShape;

    float overlayWidth;
    float overlayHeight;
    float overlayXPos;
    float overlayYPos;

    Colour buttonColour;
    Colour activeColour;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<QTableDrawing> waveShaperDrawing;
    ScopedPointer<Slider> waveShapeDriveSlider;
    ScopedPointer<Slider> waveShapeMixSlider;
    ScopedPointer<Slider> waveShapeAmpSlider;
    ScopedPointer<ToggleButton> waveshaperToggle;
    ScopedPointer<TextButton> loadShapeButton;
    ScopedPointer<TextButton> waveShapeSaveButton;
    ScopedPointer<TextButton> shapeButtonOne;
    ScopedPointer<TextButton> shapeButtonTwo;
    ScopedPointer<TextButton> shapeButtonThree;
    ScopedPointer<TextButton> shapeButtonFour;
    ScopedPointer<TextButton> shapeButtonFive;
    ScopedPointer<TextButton> shapeButtonSix;
    ScopedPointer<TextButton> clearShapeButton;
    ScopedPointer<Slider> waveShapeRoutingSlider;
    ScopedPointer<TextButton> printShape;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveShapeComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_458886B41B1B6C46__
