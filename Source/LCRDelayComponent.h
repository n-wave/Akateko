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

#ifndef __JUCE_HEADER_44CFD595D0467C62__
#define __JUCE_HEADER_44CFD595D0467C62__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "SliderLookAndFeel.h"
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamImageToggle.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LCRDelayComponent  : public Component,
                           public SliderListener,
                           public ButtonListener
{
public:
    //==============================================================================
    LCRDelayComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~LCRDelayComponent();

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
    void initialiseTimeDivision();
    int getTimeDivisionIndex(std::vector<double> values, double time);
    int findClosestTimeDivision(double time);
    void updateSliderRange(Button *toggle, Slider *slider, AudioProcessorParameter *param);
    void updateSliderValue(Button *toggle, Slider *slider, AudioProcessorParameter *param);
    void initialiseMidiStrings();

    AudioProcessorParameter *leftDelay;
    AudioProcessorParameter *rightDelay;
    AudioProcessorParameter *centerDelay;
    AudioProcessorParameter *pan;

    StringArray division;
    std::vector<double> timeDivision;
    std::vector<int> paramIndices;
    int requestMenuId[18] = {0};
    StringArray midiStrings;



    PopupMenu menu;

    AkatekoAudioProcessor &processor;
    Label &labelRef;

    ScopedPointer<CustomLookAndFeel> claf;
    ScopedPointer<SliderLookAndFeel> blaf;
    ScopedPointer<SliderLookAndFeel> slaf;
    ScopedPointer<SliderLookAndFeel> plaf;
    ScopedPointer<SliderLookAndFeel> fblaf;

    double beatsPerMinute;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> leftDelaySlider;
    ScopedPointer<Slider> rightDelaySlider;
    ScopedPointer<Slider> rightFBSlider;
    ScopedPointer<Slider> leftFBSlider;
    ScopedPointer<Slider> leftMixSlider;
    ScopedPointer<ImageButton> enableToggle;
    ScopedPointer<ImageButton> leftSyncToggle;
    ScopedPointer<ImageButton> rightSyncToggle;
    ScopedPointer<Slider> centerDelaySlider;
    ScopedPointer<ImageButton> centerSyncToggle;
    ScopedPointer<Slider> centerFBSlider;
    ScopedPointer<Slider> rightMixSlider;
    ScopedPointer<Slider> panSlider;
    ScopedPointer<Slider> hpfSlider;
    ScopedPointer<Slider> lpfSlider;
    ScopedPointer<ImageButton> hpfEnableToggle;
    ScopedPointer<ImageButton> lpfEnableToggle;
    ScopedPointer<Slider> centerAmpSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LCRDelayComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_44CFD595D0467C62__
