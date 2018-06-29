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

#ifndef __JUCE_HEADER_815DCE08E2C6363E__
#define __JUCE_HEADER_815DCE08E2C6363E__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "/work/programming-projects/msm/msm-gui/ParamSlider.h"
#include "/work/programming-projects/msm/msm-gui/ParamImageToggle.h"
#include "PluginProcessor.h"
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
class PlateReverbComponent  : public Component
{
public:
    //==============================================================================
    PlateReverbComponent (const String &name, AkatekoAudioProcessor &p, Label &label);
    ~PlateReverbComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void handleCommandMessage(int commandId) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void initiateMidiStrings();

    AkatekoAudioProcessor &processor;
    Label &labelRef;
    std::vector<int> paramIndices;
    int requestMenuId[5];
    StringArray midiStrings;

    PopupMenu menu;

    ScopedPointer<CustomLookAndFeel> claf;
    ScopedPointer<SliderLookAndFeel> blaf;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ImageButton> enableToggle;
    ScopedPointer<Slider> decaySlider;
    ScopedPointer<Slider> dampingSlider;
    ScopedPointer<Slider> bandWidthSlider;
    ScopedPointer<Slider> mixSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlateReverbComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_815DCE08E2C6363E__
