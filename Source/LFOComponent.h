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

#ifndef __JUCE_HEADER_C67DDBB34A3E06D8__
#define __JUCE_HEADER_C67DDBB34A3E06D8__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "/work/programming-projects/msm/msm-dsp/Common/msm.h"
#include "/work/programming-projects/msm/msm-gui/BufferDisplay.h"
#include "/work/programming-projects/msm/msm-gui/QTableDrawing.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"

#include "Akateko.h"

#include "CustomLookAndFeel.h"
#include "SliderLookAndFeelBottomV1.h"
#include "SliderLookAndFeelBottomV2.h"
#include "PluginProcessor.h"
#include <vector>


//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LFOComponent  : public Component,
                      public SliderListener,
                      public ButtonListener
{
public:
    //==============================================================================
    LFOComponent (const String &name, AkatekoAudioProcessor &p, int lfo, Label &label);
    ~LFOComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    String getCurrentShape(); //when shape is updated call and store in the processor
    StringArray getCurrentShapes();
    void setCurrentShapes(StringArray shapes); //After first startup

    msmBuffer getBuffer();

//    void setBeatDivisionStrings(StringArray beatDivStr);
//    void setBeatDivisionValues(std::vector<double> beatDivVal);

    void updateFrequencySlider(double bpm);
    void initFrequencySlider();

    void handleCommandMessage(int commandId) override;
    void updateGui();
    void midiUpdate();

    void setLookAndFeel(LookAndFeel *cLaf,
                        LookAndFeel *r1Laf,
                        LookAndFeel *r2Laf);

    void calculateTimeDivision(double bpm);



    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void modifierKeysChanged (const ModifierKeys& modifiers) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AkatekoAudioProcessor &processor;
    void initialiseTimeDivisions();
    void updateParameters();
    void initialiseDefaultShapes();
    void resetShapButtonColour(int shapeButton);
    void setShape(Button *buttonThatWasClicked, int shape);
    void initialiseMIDIStrings();

    int findClosestTimeDivision(double freq);
    int getTimeDivisionIndex(vector<double> &values, double freq);

    msmBuffer currentBuffer;
    StringArray currentShapes;
    StringArray defaultShapes;

    StringArray beatDivision;
    std::vector<double> valueBeatDivision;    
    std::vector<int>paramIndices;
    int requestMenuIds[16];

    AudioProcessorParameter *frequency;
    AudioProcessorParameter *activeShape;
    AudioProcessorParameter *oneShot;
    AudioProcessorParameter *sync;

    Label &labelRef;

    double beatsPerMinute;
    bool freqMidiEnabled;
    bool defaultShape;
    int lfoNumber;

    PopupMenu menu;

    Colour buttonColour;
    Colour activeColour;

    StringArray lfoOneMIDI;
    StringArray lfoTwoMIDI;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<QTableDrawing> LFO;
    ScopedPointer<Slider> lfoPhaseSlider;
    ScopedPointer<Slider> lfoFrequencySlider;
    ScopedPointer<Slider> lfoPWMSlider;
    ScopedPointer<Slider> lfoSmoothSlider;

    ScopedPointer<BufferDisplay> bufferDisplay;
    ScopedPointer<ToggleButton> lfoEnableToggle;
    ScopedPointer<Label> nameLabel;
    ScopedPointer<TextButton> shapeButtonOne;
    ScopedPointer<TextButton> shapeButtonTwo;
    ScopedPointer<TextButton> shapeButtonThree;
    ScopedPointer<TextButton> shapeButtonFour;
    ScopedPointer<TextButton> shapeButtonFive;
    ScopedPointer<TextButton> shapeButtonSix;
    ScopedPointer<TextButton> shapeButtonSeven;
    ScopedPointer<TextButton> shapeButtonEight;
    ScopedPointer<TextButton> loadButton;
    ScopedPointer<TextButton> clearButton;
    ScopedPointer<TextButton> saveButton;
    ScopedPointer<TextButton> oneShotButton;
    ScopedPointer<TextButton> freeRunningButton;
    ScopedPointer<TextButton> syncButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_C67DDBB34A3E06D8__
