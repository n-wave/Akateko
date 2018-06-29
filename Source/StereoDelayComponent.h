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

#ifndef __JUCE_HEADER_7BE8747CBCC1D0__
#define __JUCE_HEADER_7BE8747CBCC1D0__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "SliderLookAndFeel.h"
#include "PluginProcessor.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamImageToggle.h"

#include <vector>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.
    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class StereoDelayComponent  : public Component,
                              public SliderListener,
                              public ButtonListener
{
public:
    //==============================================================================
    StereoDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~StereoDelayComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void handleCommandMessage(int commandId) override;
    void calculateTimeDivision(double bpm);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseTimeDivisions();

    int getTimeDivisionIndex(std::vector<double> values, double time);
    int findClosestTimeDivision(double time);
    void updateSliderRange(Button *toggle, Slider *slider, AudioProcessorParameter *param);
    void updateSliderValue(Button *toggle, Slider *slider, AudioProcessorParameter *param);
    void initialiseMidiStrings();

    AudioProcessorParameter *leftDelay;
    AudioProcessorParameter *rightDelay;

    StringArray division;
    std::vector<double> timeDivision;
    std::vector<int> paramIndices;
    int requestMenuIds[10];
    StringArray midiStrings;

    PopupMenu menu;

    AkatekoAudioProcessor &processor;
    Label &labelRef;

    ScopedPointer<CustomLookAndFeel>claf;
    ScopedPointer<SliderLookAndFeel>blaf;
    ScopedPointer<SliderLookAndFeel> fblaf;


    double beatsPerMinute;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> leftDelaySlider;
    ScopedPointer<Slider> rightDelaySlider;
    ScopedPointer<Slider> rightFBSlider;
    ScopedPointer<Slider> leftFBSlider;
    ScopedPointer<Slider> crossSlider;
    ScopedPointer<Slider> mixSlider;
    ScopedPointer<ImageButton> enableToggle;
    ScopedPointer<ImageButton> leftSyncToggle;
    ScopedPointer<ImageButton> rightSyncToggle;
    ScopedPointer<ImageButton> crossEnabledToggle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_7BE8747CBCC1D0__
