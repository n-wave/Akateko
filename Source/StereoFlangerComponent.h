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

#ifndef __JUCE_HEADER_F3BDDF681E2CA4F8__
#define __JUCE_HEADER_F3BDDF681E2CA4F8__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-dsp/Common/BeatDivisor.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamImageToggle.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/QuadratureOscillator.h"
#include "CustomLookAndFeel.h"
#include "SliderLookAndFeel.h"
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
class StereoFlangerComponent  : public Component,
                                public ButtonListener,
                                public SliderListener
{
public:
    //==============================================================================
    StereoFlangerComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~StereoFlangerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void handleCommandMessage(int commandId) override;

    void updateGui();
    void calculateBeatDivision(double bpm);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void resetShapeButtonColour(int ShapeButton);

    void initRateSlider();
    void initialiseMidiStrings();

    void initialiseTimeDivisions();
    int findClosestTimeDivision(double freq);
    int getTimeDivisionIndex(vector<double> &values, double freq);

    double beatsPerMinute;

    AkatekoAudioProcessor &processor;
    Label &labelRef;

    AudioProcessorParameter *wave;
    AudioProcessorParameter *sync;
    AudioProcessorParameter *rate;
    AudioProcessorParameter *phase;

    StringArray beatDivision;
    std::vector<double> valueBeatDivision;

    std::vector<int> paramIndices;
    int requestMenuIds[8];
    StringArray midiStrings;
    PopupMenu menu;

    ScopedPointer<CustomLookAndFeel> claf;
    ScopedPointer<SliderLookAndFeel> blaf;
    ScopedPointer<SliderLookAndFeel> wlaf;
    ScopedPointer<SliderLookAndFeel> fblaf;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> feedBackSlider;
    ScopedPointer<ImageButton> SyncToggle;
    ScopedPointer<Slider> phaseSlider;
    ScopedPointer<Slider> depthSlider;
    ScopedPointer<Slider> rateSlider;
    ScopedPointer<ImageButton> crossFB;
    ScopedPointer<ImageButton> enableToggle;
    ScopedPointer<Slider> waveSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoFlangerComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_F3BDDF681E2CA4F8__
