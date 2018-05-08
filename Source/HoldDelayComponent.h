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

#ifndef __JUCE_HEADER_27D93873FE920892__
#define __JUCE_HEADER_27D93873FE920892__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include <vector>

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class HoldDelayComponent  : public Component,
                            public SliderListener,
                            public ButtonListener
{
public:
    //==============================================================================
    HoldDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~HoldDelayComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    enum commandIds{
        update
    };

    void handleCommandMessage(int commandId) override;

    void setBPM(double bpm);
    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    bool keyPressed (const KeyPress& key) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseTimeDivision();
    int findClosestTimeDivision(double time);
    void initLoopSlider();

    void calculateTimeDivision(double bpm);
    void setGlideSliderRange(double bpm);

    AkatekoAudioProcessor &processor;
    Label &labelRef;

    AudioProcessorParameter *sync;
    AudioProcessorParameter *trigger;
    AudioProcessorParameter *loop;
    AudioProcessorParameter *speed;

    AudioProcessorParameter *gap;
    AudioProcessorParameter *smooth;
    AudioProcessorParameter *glide;

    AudioProcessorParameter *fade;
    AudioProcessorParameter *direction;
    AudioProcessorParameter *length;


    StringArray division;
    std::vector<double> timeDivision;

    String triggerId;
    bool triggerClicked;
    double beatsPerMinute;

    Colour activeColour;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> loopSlider;
    ScopedPointer<Slider> speedSlider;
    ScopedPointer<Slider> directionSlider;
    ScopedPointer<ToggleButton> enableToggle;
    ScopedPointer<Slider> mixSlider;
    ScopedPointer<ToggleButton> syncToggle;
    ScopedPointer<Slider> panSlider;
    ScopedPointer<Slider> lengthSlider;
    ScopedPointer<Slider> glideSlider;
    ScopedPointer<Slider> fadeSlider;
    ScopedPointer<ToggleButton> glideToggle;
    ScopedPointer<ToggleButton> gapPositionToggle;
    Path internalPath1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoldDelayComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_27D93873FE920892__
