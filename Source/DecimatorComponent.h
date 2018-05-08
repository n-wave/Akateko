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

#ifndef __JUCE_HEADER_30E3BB3F047E521E__
#define __JUCE_HEADER_30E3BB3F047E521E__

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
class DecimatorComponent  : public Component,
                            public SliderListener
{
public:
    //==============================================================================
    DecimatorComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~DecimatorComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    enum commandIds{
        update
    };

    void handleCommandMessage(int commandId) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AkatekoAudioProcessor &processor;
    Label &labelRef;

    AudioProcessorParameter *bitReduction;
    AudioProcessorParameter *srateReduction;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> rateSlider;
    ScopedPointer<Slider> bitSlider;
    ScopedPointer<Slider> mixSlider;
    ScopedPointer<ToggleButton> enableToggle;
    ScopedPointer<ToggleButton> filterToggle;
    ScopedPointer<ToggleButton> reduceEnable;
    ScopedPointer<ToggleButton> srateEnable;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecimatorComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_30E3BB3F047E521E__
