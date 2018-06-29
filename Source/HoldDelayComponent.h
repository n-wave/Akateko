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

#include "/work/programming-projects/msm/msm-gui/ParamImageToggle.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include <vector>

#include "CustomLookAndFeel.h"
#include "SliderLookAndFeel.h"
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
    void handleCommandMessage(int commandId) override;

    void setBPM(double bpm);

    void buttonStateChanged(Button *button) override;

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initialiseTimeDivision();
    int findClosestTimeDivision(vector<double> &values, double time);
    int getTimeDivisionIndex(vector<double> &values, double time);

    void initLoopSlider();
    void initLatchSlider();

    void calculateTimeDivision(double bpm);
    void setGlideSliderRange(double bpm);
    void initialiseMidiStrings();

    AkatekoAudioProcessor &processor;
    Label &labelRef;

    AudioProcessorParameter *trigger = nullptr;

    AudioProcessorParameter *sync = nullptr;
    AudioProcessorParameter *loop = nullptr;
    AudioProcessorParameter *speed = nullptr;

    AudioProcessorParameter *gap = nullptr;
    AudioProcessorParameter *smooth = nullptr;
    AudioProcessorParameter *glide = nullptr;

    AudioProcessorParameter *fade = nullptr;
    AudioProcessorParameter *direction = nullptr;
    AudioProcessorParameter *length = nullptr;

    AudioProcessorParameter *latchEnable = nullptr;
    AudioProcessorParameter *latch = nullptr;


    StringArray division;
    std::vector<double> timeDivision;

    StringArray latchDivision;
    std::vector<double> latchValues;

    StringArray midiStrings;
    std::vector<int> paramIndices;
    int requestMenuIds[15];
    PopupMenu menu;


    String triggerId;
    bool triggerClicked;
    double beatsPerMinute;
    double glideMaxRange; // 1 whole note

    Colour activeColour;

    ScopedPointer<CustomLookAndFeel> claf;
    ScopedPointer<SliderLookAndFeel> blaf;
    ScopedPointer<SliderLookAndFeel> pblaf;
    ScopedPointer<SliderLookAndFeel> dlaf;

    //==============================================================================
    ScopedPointer<Slider> loopSlider;
    ScopedPointer<Slider> speedSlider;
    ScopedPointer<Slider> directionSlider;
    ScopedPointer<ImageButton> enableToggle;
    ScopedPointer<ImageButton> triggerButton;
    ScopedPointer<Slider> mixSlider;
    ScopedPointer<ImageButton> syncToggle;
    ScopedPointer<Slider> panSlider;
    ScopedPointer<Slider> lengthSlider;
    ScopedPointer<Slider> glideSlider;
    ScopedPointer<Slider> fadeSlider;
    ScopedPointer<ImageButton> glideToggle;
    ScopedPointer<ImageButton> gapPositionToggle;

    ScopedPointer<ImageButton> latchToggle; //Latch enabled
    ScopedPointer<Slider> latchSlider; // Create image for the slider

    //[/UserVariables]

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoldDelayComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_27D93873FE920892__
