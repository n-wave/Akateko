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

#ifndef __JUCE_HEADER_E2DCC5129BDB47A2__
#define __JUCE_HEADER_E2DCC5129BDB47A2__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamToggle.h"
#include "/work/programming-projects/msm/msm-dsp/Generators/QuadratureOscillator.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DimensionChorusComponent  : public Component,
                                  public SliderListener
{
public:
    //==============================================================================
    DimensionChorusComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~DimensionChorusComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    enum commandIds{
        update
    };

    void handleCommandMessage(int commandId) override;
    void updateGui();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AkatekoAudioProcessor &processor;
    Label &labelRef;

    AudioProcessorParameter *sync;
    AudioProcessorParameter *wave;
    AudioProcessorParameter *spread;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> rateSlider;
    ScopedPointer<Slider> depthSlider;
    ScopedPointer<Slider> spreadSlider;
    ScopedPointer<Slider> centerAmpSlider;
    ScopedPointer<Slider> mixSlider;
    ScopedPointer<Slider> centerPanSlider;
    ScopedPointer<Slider> frequencySlider;
    ScopedPointer<Slider> waveSlider;
    ScopedPointer<ToggleButton> enableToggle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DimensionChorusComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_E2DCC5129BDB47A2__
